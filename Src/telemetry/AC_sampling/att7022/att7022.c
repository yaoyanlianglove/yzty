/**
  ******************************************************************************
  * File Name          : ATT7022.c
  * Description        : This file provides code for how to use the ATT7022 chip.
  ******************************************************************************
  * @attention
  * The ATT7022 chip adopts SPI communication interface.
  *
  ******************************************************************************
  */


#include "att7022.h"
#include "main.h"
#include "gpio.h"
#include "debug.h"
#include "crc.h"
#include "delay.h"

/*****************************************************************************
 Function    : ATT7022_Read_REG
 Description : Read REG from the chip ATT7022.
 Input       : regAddr-Register address
 Output      : None
 Return      : Register data
 *****************************************************************************/
static uint32_t ATT7022_Read_REG(uint8_t regAddr)
{
    uint8_t  pData;
    uint8_t  bufData[3];
    uint32_t readData = 0;
    HAL_StatusTypeDef res;
    pData = regAddr;
    ATT7022_CS_LOW;
    res = HAL_SPI_Transmit(&HSPI, &pData, 1, ATT7022_SPI_TIMEOUT_VALUE);
    if(res != HAL_OK)
    {
        dprintf("ATT7022 send error: %d/r/n", res);
    }
    delay_us(3);
    HAL_SPI_Receive(&HSPI, bufData, 3, ATT7022_SPI_TIMEOUT_VALUE);
    if(res != HAL_OK)
    {
        dprintf("ATT7022 receive error: %d/r/n", res);
    }
    ATT7022_CS_HIGH;
    readData = (uint32_t)((bufData[0] << 16) + (bufData[1] << 8) + bufData[2]);
    return readData;
}

/*****************************************************************************
 Function    : ATT7022_Write_REG
 Description : Write data to the register of the chip ATT702.
 Input       : regAddr-Register address, writeData-Write data.
 Output      : None
 Return      : None 
 *****************************************************************************/
static void ATT7022_Write_REG(uint8_t regAddr, uint32_t writeData)
{
    uint8_t  pData;
    uint8_t  bufData[3];
    HAL_StatusTypeDef res;
    pData = regAddr | 0x80;
    ATT7022_CS_LOW;
    bufData[0] = (writeData >> 16) & 0xff;
    bufData[1] = (writeData >>  8) & 0xff;
    bufData[2] =  writeData        & 0xff;
    res = HAL_SPI_Transmit(&HSPI, &pData, 1, ATT7022_SPI_TIMEOUT_VALUE);
    if(res != HAL_OK)
    {
        dprintf("ATT7022 send error: %d/r/n", res);
    }
    HAL_SPI_Transmit(&HSPI, bufData, 3, ATT7022_SPI_TIMEOUT_VALUE);
    if(res != HAL_OK)
    {
        dprintf("ATT7022 send error: %d/r/n", res);
    }
    ATT7022_CS_HIGH;
}

/*****************************************************************************
 Function    : ATT7022_Hard_Reset
 Description : The ATT7022 chip hardware reset.
 Input       : None
 Output      : None
 Return      : None 
 *****************************************************************************/
static void ATT7022_Hard_Reset(void)
{
    ATT7022_RESET_LOW;
    delay_us(22);
    ATT7022_RESET_HIGH;
}
/*****************************************************************************
 Function    : ATT7022_Reset_Ago_Wait_Work
 Description : Wait for the operation request signal of ATT7022 chip.
 Input       : timeout ms,Timeout must be greater than 30ms. 
 Output      : None
 Return      : The status of ATT7022. 
 *****************************************************************************/
static ATT7022StatusTypeDef ATT7022_Reset_Ago_Wait_Work(uint32_t timeout)
{
    uint16_t waitTime = 0;
    if(timeout <= 30)
        timeout = 30;
    while(ATT7022_IRQ_STATE)//输出低电平，为复位完成
    {
        delay_us(1000);
        if(waitTime < timeout)
            waitTime++;
        else
            return ATT7022_RESET_TIMEOUT;
    }
    return ATT7022_OK;
}
/*****************************************************************************
 Function    : ATT7022_Write_Ago_Wait_Work
 Description : Wait for the operation request signal of ATT7022 chip.
 Input       : timeout ms. 
 Output      : None
 Return      : The status of ATT7022. 
 *****************************************************************************/
static ATT7022StatusTypeDef ATT7022_Write_Ago_Wait_Work(uint32_t timeout)
{
    uint16_t waitTime = 0;
    while(!ATT7022_IRQ_STATE)//输出高电平，写校表数据完成
    {
        delay_us(1000);
        if(waitTime < timeout)
            waitTime++;
        else
            return ATT7022_RESET_TIMEOUT;     
    }
    return ATT7022_OK;
}
/*****************************************************************************
 Function    : ATT7022_Init
 Description : The ATT7022 chip init.
 Input       : calPara-Calibration parameter, input-AC input during calibration.
 Output      : None
 Return      : The status of ATT7022.  
 *****************************************************************************/
ATT7022StatusTypeDef ATT7022_Init(ACSampleCalibrationParaTypeDef *calPara, 
                                  ACSampleCalibrationInputTypeDef *input, 
                                  uint32_t timeout)
{
    ATT7022StatusTypeDef res = ATT7022_OK;
    ATT7022_Hard_Reset();
    res = ATT7022_Reset_Ago_Wait_Work(30);
    if(res != ATT7022_OK)
        return res;

    ATT7022_Write_REG(0xc9, 0x005A);    //打开写使能

    ATT7022_Write_REG(0x01, 0xB97E);    //设置模式配置寄存器
    ATT7022_Write_REG(0x03, 0xF804);    //设置EMU单元,推荐是0XD804
    ATT7022_Write_REG(0x1E, (uint32_t)(HFconst *((input->u)*(input->i)/220)));   //高频脉冲输出参数 3200
    ATT7022_Write_REG(0x31, 0x3437);    //模拟模块使能
    ATT7022_Write_REG(0x02, 0x0000);    //设置AD增益

                                        //温度补偿 
    ATT7022_Write_REG(0x6D, 0xFF11);
    ATT7022_Write_REG(0x6E, 0x2B53);
    ATT7022_Write_REG(0x6F, 0xD483);

    ATT7022_Write_REG(0x17, calPara->ua);
    ATT7022_Write_REG(0x18, calPara->ub);
    ATT7022_Write_REG(0x19, calPara->uc);
                                        //电流增益
    ATT7022_Write_REG(0x1A, calPara->ia);
    ATT7022_Write_REG(0x1B, calPara->ib);
    ATT7022_Write_REG(0x1C, calPara->ic);
                                       //有功功率增益 有功、无功、视在的增益是一样的
    ATT7022_Write_REG(0x04, calPara->pa);
    ATT7022_Write_REG(0x05, calPara->pb);
    ATT7022_Write_REG(0x06, calPara->pc);
                                       //无功功率增益
    ATT7022_Write_REG(0x07, calPara->pa);
    ATT7022_Write_REG(0x08, calPara->pb);
    ATT7022_Write_REG(0x09, calPara->pc);
                                       //视在功率增益
    ATT7022_Write_REG(0x0A, calPara->pa);
    ATT7022_Write_REG(0x0B, calPara->pb);
    ATT7022_Write_REG(0x0C, calPara->pc);

    ATT7022_Write_REG(0xc9, 0X00000A);      //关闭写使能，数据不等于0X00005A关闭

    res = ATT7022_Write_Ago_Wait_Work(timeout);

    if(res != ATT7022_OK)
        return res;

    //ATT7022 needs to wait for 2 seconds to read data after initialization
    delay_ms(2000);
    return ATT7022_OK;
}
/*****************************************************************************
 Function    : Get_Ugain
 Description : Calculate the voltage gain when calibrating the meter.
 Input       : addr,input-AC input during calibration.
 Output      : None
 Return      : The voltage gain.  
 *****************************************************************************/
static uint32_t Get_Ugain(uint8_t addr, ACSampleCalibrationInputTypeDef *input)
{  
    float urms, ur, ugain;
    ur = input->u;
    urms = (ATT7022_Read_REG(addr) & 0x00FFFFFF)/8192.0;
    ugain = ur/urms - 1;
    if(ugain >= 0)
        return (uint32_t)(ugain*32768.0);
    else
        return (uint32_t)(65536.0 + ugain*32768.0);
}
/*******************************************************************************
 Function    : Get_Igain
 Description : Calculate the current gain when calibrating the meter.
 Input       : addr,input-AC input during calibration.
 Output      : None
 Return      : The current gain.  
*******************************************************************************/
static uint32_t Get_Igain(uint8_t addr, ACSampleCalibrationInputTypeDef *input)
{
    float irms, ir, igain;
    ir = input->i;
    irms = (ATT7022_Read_REG(addr) & 0x00FFFFFF)/4096.0/ATT7022_N;
    igain = ir/irms -1;
    if(igain >= 0)
        return (uint32_t)(igain*32768.0);
    else
        return (uint32_t)(65536.0 + igain*32768.0);
}
/*******************************************************************************
 Function    : Get_Pgain
 Description : Calculate the power gain when calibrating the meter.
 Input       : addr,input-AC input during calibration.
 Output      : None
 Return      : The power gain.  
*******************************************************************************/
static uint32_t Get_Pgain(uint8_t addr, ACSampleCalibrationInputTypeDef *input)
{ 
    float perr, pgain;
    uint32_t power;
    power = ATT7022_Read_REG(addr) & 0x00FFFFFF;
    if(power > 1024*1024*8)
        power = power - 1024*1024*16;
    perr = power * P_K * 220/input->u/input->i;
    pgain = (input->u*input->i - perr)/perr;
    if(pgain < 0)
        pgain = 1024*64 + pgain*1024*32;
    else
        pgain = pgain*1024*32;
    return (uint32_t)pgain;
}
/*******************************************************************************
 Function    : ATT7022_Calibration
 Description : ATT7022 chip calibration.
 Input       : calPara-Calibration parameter, input-AC input during calibration.
 Output      : None
 Return      : The results of ATT7022 Calibration.  
*******************************************************************************/
ATT7022StatusTypeDef ATT7022_Calibration(ACSampleCalibrationParaTypeDef *calPara, 
                                         ACSampleCalibrationInputTypeDef *input)
{
    ATT7022StatusTypeDef res = ATT7022_OK;
    uint8_t temp[CALIBRATION_DATA_LEN] = {0};
    ATT7022_Hard_Reset();
    res = ATT7022_Reset_Ago_Wait_Work(30);
    if(res != ATT7022_OK)
        return res;

    ATT7022_Write_REG(0xc9, 0x005A);    //打开写使能

    ATT7022_Write_REG(0x01, 0xB97E);    //设置模式配置寄存器
    ATT7022_Write_REG(0x03, 0xF804);    //设置EMU单元,推荐是0XD804
    ATT7022_Write_REG(0x1E, (uint32_t)(HFconst *((input->u)*(input->i)/220)));   //高频脉冲输出参数 3200
    ATT7022_Write_REG(0x31, 0x3437);    //模拟模块使能
    ATT7022_Write_REG(0x02, 0x0000);    //设置AD增益

                                        //温度补偿 
    ATT7022_Write_REG(0x6D, 0xFF11);
    ATT7022_Write_REG(0x6E, 0x2B53);
    ATT7022_Write_REG(0x6F, 0xD483);

    res = ATT7022_Write_Ago_Wait_Work(10000);

    if(res != ATT7022_OK)
        return res;

    delay_ms(2000);             //配置完成后要等待一段时间才能读数

    calPara->ua = Get_Ugain(0x0D, input);
    calPara->ub = Get_Ugain(0x0E, input);
    calPara->uc = Get_Ugain(0x0F, input);

    calPara->ia = Get_Igain(0x10, input);
    calPara->ib = Get_Igain(0x11, input);
    calPara->ic = Get_Igain(0x12, input);

    calPara->pa = Get_Pgain(0x01, input);
    calPara->pb = Get_Pgain(0x02, input);
    calPara->pc = Get_Pgain(0x03, input);
                                            //赋给数据计算校验和
    temp[0 ] = (calPara->ua >> 24) & 0xFF;
    temp[1 ] = (calPara->ua >> 16) & 0xFF;
    temp[2 ] = (calPara->ua >> 8 ) & 0xFF;
    temp[3 ] =  calPara->ua & 0xFF;

    temp[4 ] = (calPara->ub >> 24) & 0xFF;
    temp[5 ] = (calPara->ub >> 16) & 0xFF;
    temp[6 ] = (calPara->ub >> 8 ) & 0xFF;
    temp[7 ] =  calPara->ub & 0xFF;

    temp[8 ] = (calPara->uc >> 24) & 0xFF;
    temp[9 ] = (calPara->uc >> 16) & 0xFF;
    temp[10] = (calPara->uc >> 8 ) & 0xFF;
    temp[11] =  calPara->uc & 0xFF;

    temp[12] = (calPara->ia >> 24) & 0xFF;
    temp[13] = (calPara->ia >> 16) & 0xFF;
    temp[14] = (calPara->ia >> 8 ) & 0xFF;
    temp[15] =  calPara->ia & 0xFF;

    temp[16] = (calPara->ib >> 24) & 0xFF;
    temp[17] = (calPara->ib >> 16) & 0xFF;
    temp[18] = (calPara->ib >> 8 ) & 0xFF;
    temp[19] =  calPara->ib & 0xFF;

    temp[20] = (calPara->ic >> 24) & 0xFF;
    temp[21] = (calPara->ic >> 16) & 0xFF;
    temp[22] = (calPara->ic >> 8 ) & 0xFF;
    temp[23] =  calPara->ic & 0xFF;

    temp[24] = (calPara->pa >> 24) & 0xFF;
    temp[25] = (calPara->pa >> 16) & 0xFF;
    temp[26] = (calPara->pa >> 8 ) & 0xFF;
    temp[27] =  calPara->pa & 0xFF;

    temp[28] = (calPara->pb >> 24) & 0xFF;
    temp[29] = (calPara->pb >> 16) & 0xFF;
    temp[30] = (calPara->pb >> 8 ) & 0xFF;
    temp[31] =  calPara->pb & 0xFF;

    temp[32] = (calPara->pc >> 24) & 0xFF;
    temp[33] = (calPara->pc >> 16) & 0xFF;
    temp[34] = (calPara->pc >> 8 ) & 0xFF;
    temp[35] =  calPara->pc & 0xFF;

    calPara->crc = CRC_16(temp, CALIBRATION_DATA_LEN - 2);

    return ATT7022_OK;
}
/*******************************************************************************
 Function    : Read_U_Data
 Description : Read voltage data.
 Input       : addr
 Output      : none
 Return      : The voltage data.  
*******************************************************************************/
float Read_U_Data(uint8_t addr)
{
    return ((ATT7022_Read_REG(addr) & 0x00FFFFFF)/8192.0);
}
/*******************************************************************************
 Function    : Read_I_Data
 Description : Read current data.
 Input       : addr
 Output      : none
 Return      : The current data. 
*******************************************************************************/
float Read_I_Data(uint8_t addr)
{
    return ((ATT7022_Read_REG(addr) & 0x00FFFFFF)/4096.0/ATT7022_N);
}
/*******************************************************************************
 Function    : Read_P_Data
 Description : Read power data.
 Input       : addr,input-AC input during calibration.
 Output      : none
 Return      : The power data.
*******************************************************************************/    
float Read_P_Data(uint8_t addr, ACSampleCalibrationInputTypeDef *input)
{
    int32_t power = 0;
    power = (ATT7022_Read_REG(addr) & 0x00FFFFFF);
    if(power > 0x800000)
        power = power - 0x1000000;
    return power * P_K *220/input->u/input->i;
}
/*******************************************************************************
 Function    : Read_PT_Data
 Description : Read total power data.
 Input       : addr,input-AC input during calibration.
 Output      : none
 Return      : The total power data.
*******************************************************************************/    
float Read_PT_Data(uint8_t addr, ACSampleCalibrationInputTypeDef *input)
{
    int32_t power = 0;
    power = (ATT7022_Read_REG(addr) & 0x00FFFFFF);
    if(power > 0x800000)
        power = power - 0x1000000;
    return power * P_K *2*220/input->u/input->i;
}
/*******************************************************************************
 Function    : Read_PF_Data
 Description : Read power factor data.
 Input       : addr
 Output      : none
 Return      : The power factor data.
*******************************************************************************/
float Read_PF_Data(uint8_t addr)
{
    int32_t pf = 0;
    pf = (ATT7022_Read_REG(addr) & 0x00FFFFFF);
    if(pf > 0x800000)
        pf = pf - 0x1000000;
    return (pf*1.0/0x800000);
}
/*******************************************************************************
 Function    : Read_TMP_Data
 Description : Read tempreture data.
 Input       : addr
 Output      : none
 Return      : The tempreture data.
*******************************************************************************/
float Read_TMP_Data(uint8_t addr)
{
    int32_t temp = 0;
    temp = ATT7022_Read_REG(addr) & 0x00FFFFFF;
    if(temp > 128)
        temp = temp - 256;
    return (25 - temp*0.726);
}
/*******************************************************************************
 Function    : Read_Check
 Description : none
 Input       : none
 Output      : none
 Return      : none
*******************************************************************************/
ATT7022StatusTypeDef Read_Check(void)
{
    int32_t id = 0;
    id = ATT7022_Read_REG(00) & 0x00FFFFFF;
    if(id != 0x7122A0)
        return ATT7022_ERROR;
    return ATT7022_OK;
}
/************************ZXDQ *****END OF FILE****/
