#include <intrinsics.h>
#include "stm32f10x.h"
#include "main.h"
#include "stm32f10x_adc.h"

void Sleep(int msec);

/*************************************************************************
 * Function Name: InitADC1
 * Parameters: none
 * Return: none
 *
 * Description: ADC Init subroutine
 *
 *************************************************************************/
void InitADC1(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
ADC_InitTypeDef   ADC_InitStructure;

  // ADC init
  // ADC Deinit
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  ADC_DeInit(ADC1);

  // RC0 - analog input
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = (GPIOSpeed_TypeDef)0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init (GPIOC, &GPIO_InitStructure);

  // ADC Structure Initialization
  ADC_StructInit(&ADC_InitStructure);

  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  // Enable the ADC
  ADC_Cmd(ADC1, ENABLE);

  // ADC calibration
  // Enable ADC1 reset calibaration register
  ADC_ResetCalibration(ADC1);
  // Check the end of ADC1 reset calibration register
  while(ADC_GetResetCalibrationStatus(ADC1) == SET);

  // Start ADC1 calibaration
  ADC_StartCalibration(ADC1);
  // Check the end of ADC1 calibration
  while(ADC_GetCalibrationStatus(ADC1) == SET);

}

/*************************************************************************
 * Function Name: GetADC1Channel
 * Parameters: Int8U channel
 * Return: Int16U
 *
 * Description: ADC Convert
 *
 *************************************************************************/
uint16_t GetADC1Channel(uint8_t chanel)
{
  // Configure channel
  ADC_RegularChannelConfig(ADC1, chanel, 1, ADC_SampleTime_55Cycles5);

  // Start the conversion
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);

  // Wait until conversion completion
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

  // Get the conversion value
  return ADC_GetConversionValue(ADC1);
}


  volatile uint32_t Pos;
void main(void)
{

  
  __disable_interrupt();
  
  // PA0 is connected VRes
  
//  /*RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);*/
//  RCC->APB2ENR |= 0x00000004;
//  /*RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, DISABLE);*/
//  RCC->APB2RSTR &= ~0x4;
//  
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  // ADC1 Init
  InitADC1();
  
  __enable_interrupt();
  
  while(1)
  {
    // Calculate position of the potentiometer in percents
    Pos = (GetADC1Channel(ADC_Channel_0)*100)/4095;
    Sleep(1);
  }
}

// Sleep cpu
void Sleep(int msec){
  int i, j;
  for(i = 0; i < msec; i++)
  {
    for(j = 0; j < 72000/4; j++)
    {
    }
  }
}

