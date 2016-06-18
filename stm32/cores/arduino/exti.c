/**
 * @authors Particle Industries, Avik De <avikde@gmail.com>

  This file is part of koduino <https://github.com/avikde/koduino>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
  
#include "exti.h"
#include "nvic.h"
#include "system_clock.h"
#include "gpio.h"
#include "timebase.h"
#include "variant.h"

// default is expected period 1000 us, set by systemClockInit()
int PWM_IN_EXTI_MAXPERIOD = 0;
int PWM_IN_EXTI_MINPERIOD = 0;
int PWM_IN_MAXPERIOD = 0;
int PWM_IN_MINPERIOD = 0;

// Some code from https://github.com/spark/core-firmware/blob/master/src/spark_wiring_interrupts.cpp

// Indexed by the pinSource of the pin
const uint32_t extiLines[] = {
  EXTI_Line0, 
  EXTI_Line1, 
  EXTI_Line2, 
  EXTI_Line3, 
  EXTI_Line4, 
  EXTI_Line5, 
  EXTI_Line6, 
  EXTI_Line7, 
  EXTI_Line8, 
  EXTI_Line9, 
  EXTI_Line10, 
  EXTI_Line11, 
  EXTI_Line12, 
  EXTI_Line13, 
  EXTI_Line14, 
  EXTI_Line15
};

const uint8_t extiIRQn[] = {
  EXTI0_IRQn,     //0
  EXTI1_IRQn,     //1
#if defined(SERIES_STM32F37x) || defined(SERIES_STM32F30x)
  EXTI2_TSC_IRQn,  //2 was named EXTI2_TS_IRQn in older CMSIS verions
#else
  EXTI2_IRQn,     //2
#endif
  EXTI3_IRQn,     //3
  EXTI4_IRQn,     //4
  EXTI9_5_IRQn,   //5
  EXTI9_5_IRQn,   //6
  EXTI9_5_IRQn,   //7
  EXTI9_5_IRQn,   //8
  EXTI9_5_IRQn,   //9
  EXTI15_10_IRQn, //10
  EXTI15_10_IRQn, //11
  EXTI15_10_IRQn, //12
  EXTI15_10_IRQn, //13
  EXTI15_10_IRQn, //14
  EXTI15_10_IRQn  //15
};

//Array to hold user ISR function pointers
EXTIChannel EXTI_MAP[] = {
  { .handler = (ISRType)0, .bPwmIn = 0, .pinName = 0, .risingEdgeMs = 0, .risingEdgeSubMs = 0, .pulsewidth = 0, .period = 0 },  // EXTI0
  { .handler = (ISRType)0, .bPwmIn = 0, .pinName = 0, .risingEdgeMs = 0, .risingEdgeSubMs = 0, .pulsewidth = 0, .period = 0 },  // EXTI1
  { .handler = (ISRType)0, .bPwmIn = 0, .pinName = 0, .risingEdgeMs = 0, .risingEdgeSubMs = 0, .pulsewidth = 0, .period = 0 },  // EXTI2
  { .handler = (ISRType)0, .bPwmIn = 0, .pinName = 0, .risingEdgeMs = 0, .risingEdgeSubMs = 0, .pulsewidth = 0, .period = 0 },  // EXTI3
  { .handler = (ISRType)0, .bPwmIn = 0, .pinName = 0, .risingEdgeMs = 0, .risingEdgeSubMs = 0, .pulsewidth = 0, .period = 0 },  // EXTI4
  { .handler = (ISRType)0, .bPwmIn = 0, .pinName = 0, .risingEdgeMs = 0, .risingEdgeSubMs = 0, .pulsewidth = 0, .period = 0 },  // EXTI5
  { .handler = (ISRType)0, .bPwmIn = 0, .pinName = 0, .risingEdgeMs = 0, .risingEdgeSubMs = 0, .pulsewidth = 0, .period = 0 },  // EXTI6
  { .handler = (ISRType)0, .bPwmIn = 0, .pinName = 0, .risingEdgeMs = 0, .risingEdgeSubMs = 0, .pulsewidth = 0, .period = 0 },  // EXTI7
  { .handler = (ISRType)0, .bPwmIn = 0, .pinName = 0, .risingEdgeMs = 0, .risingEdgeSubMs = 0, .pulsewidth = 0, .period = 0 },  // EXTI8
  { .handler = (ISRType)0, .bPwmIn = 0, .pinName = 0, .risingEdgeMs = 0, .risingEdgeSubMs = 0, .pulsewidth = 0, .period = 0 },  // EXTI9
  { .handler = (ISRType)0, .bPwmIn = 0, .pinName = 0, .risingEdgeMs = 0, .risingEdgeSubMs = 0, .pulsewidth = 0, .period = 0 },  // EXTI10
  { .handler = (ISRType)0, .bPwmIn = 0, .pinName = 0, .risingEdgeMs = 0, .risingEdgeSubMs = 0, .pulsewidth = 0, .period = 0 },  // EXTI11
  { .handler = (ISRType)0, .bPwmIn = 0, .pinName = 0, .risingEdgeMs = 0, .risingEdgeSubMs = 0, .pulsewidth = 0, .period = 0 },  // EXTI12
  { .handler = (ISRType)0, .bPwmIn = 0, .pinName = 0, .risingEdgeMs = 0, .risingEdgeSubMs = 0, .pulsewidth = 0, .period = 0 },  // EXTI13
  { .handler = (ISRType)0, .bPwmIn = 0, .pinName = 0, .risingEdgeMs = 0, .risingEdgeSubMs = 0, .pulsewidth = 0, .period = 0 },  // EXTI14
  { .handler = (ISRType)0, .bPwmIn = 0, .pinName = 0, .risingEdgeMs = 0, .risingEdgeSubMs = 0, .pulsewidth = 0, .period = 0 },  // EXTI15
};

void attachInterruptWithPriority(uint8_t pinName, ISRType ISR, InterruptTrigger mode, uint8_t priority) {
  // Connect EXTIi to this pin
  const PinInfo *pinfo = &PIN_MAP[pinName];
#if defined(SERIES_STM32F10x)
  if (pinfo->port == GPIOA)
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, pinfo->pin);
  else if (pinfo->port == GPIOB)
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, pinfo->pin);
  else if (pinfo->port == GPIOC)
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, pinfo->pin);
  else if (pinfo->port == GPIOD)
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, pinfo->pin);
  else if (pinfo->port == GPIOE)
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, pinfo->pin);
  else if (pinfo->port == GPIOF)
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOF, pinfo->pin);
#else
  if (pinfo->port == GPIOA)
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, pinfo->pin);
  else if (pinfo->port == GPIOB)
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, pinfo->pin);
  else if (pinfo->port == GPIOC)
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, pinfo->pin);
  else if (pinfo->port == GPIOD)
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, pinfo->pin);
#if !defined(STM32F303x8) //FIXME: needs probably more exceptions
  else if (pinfo->port == GPIOE)
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, pinfo->pin);
  else if (pinfo->port == GPIOF)
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, pinfo->pin);
#endif
#endif

  // Configure EXTIi line
  EXTI_InitTypeDef EXTI_InitStructure;

  EXTI_InitStructure.EXTI_Line = extiLines[pinfo->pin];
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  if (mode == RISING)
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  else if (mode == FALLING)
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  else
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  // Local code
  EXTI_MAP[pinfo->pin].handler = ISR;

  // Enable the interrupt with low priority.
  // TODO: check https://github.com/spark/core-firmware/blob/master/src/spark_wiring_interrupts.cpp does something peculiar with special cases??
  nvicEnable(extiIRQn[pinfo->pin], priority);
}

void attachInterrupt(uint8_t pinName, ISRType ISR, InterruptTrigger mode) {
  attachInterruptWithPriority(pinName, ISR, mode, 0x0f);
}

void detachInterrupt(uint8_t pinName) {
  const PinInfo *pinfo = &PIN_MAP[pinName];
  //Clear the pending interrupt flag for that interrupt pin
  EXTI_ClearITPendingBit(extiLines[pinfo->pin]);

  //EXTI structure to init EXT
  EXTI_InitTypeDef EXTI_InitStructure;
  //Select the appropriate EXTI line
  EXTI_InitStructure.EXTI_Line = extiLines[pinfo->pin];
  //disable that EXTI line
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  //send values to registers
  EXTI_Init(&EXTI_InitStructure);

  //unregister the user's handler
  EXTI_MAP[pinfo->pin].handler = (ISRType)0;
}

void noInterrupts() {
  //Only disable the interrupts that are exposed to the user
  for (uint8_t i=0; i<16; ++i)
    NVIC_DisableIRQ(extiIRQn[i]);
}

void interrupts() {
  //Only disable the interrupts that are exposed to the user
  for (uint8_t i=0; i<16; ++i)
    NVIC_EnableIRQ(extiIRQn[i]);
}

// EXTI interrupts: these seem to be the same for every series, so goes here instead of in variant

void EXTI0_IRQHandler()
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    wirishExternalInterruptHandler(0);
}
void EXTI1_IRQHandler()
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    wirishExternalInterruptHandler(1);
}
#if defined(SERIES_STM32F37x) || defined(SERIES_STM32F30x)
void EXTI2_TS_IRQHandler()
{
  if(EXTI_GetITStatus(EXTI_Line2) != RESET)
    wirishExternalInterruptHandler(2);
}
#else
void EXTI2_IRQHandler()
{
  if(EXTI_GetITStatus(EXTI_Line2) != RESET)
    wirishExternalInterruptHandler(2);
}
#endif
void EXTI3_IRQHandler()
{
  if(EXTI_GetITStatus(EXTI_Line3) != RESET)
    wirishExternalInterruptHandler(3);
}
void EXTI4_IRQHandler()
{
  if(EXTI_GetITStatus(EXTI_Line4) != RESET)
    wirishExternalInterruptHandler(4);
}
void EXTI9_5_IRQHandler()
{
  if(EXTI_GetITStatus(EXTI_Line5) != RESET)
    wirishExternalInterruptHandler(5);
  if(EXTI_GetITStatus(EXTI_Line6) != RESET)
    wirishExternalInterruptHandler(6);
  if(EXTI_GetITStatus(EXTI_Line7) != RESET)
    wirishExternalInterruptHandler(7);
  if(EXTI_GetITStatus(EXTI_Line8) != RESET)
    wirishExternalInterruptHandler(8);
  if(EXTI_GetITStatus(EXTI_Line9) != RESET)
    wirishExternalInterruptHandler(9);
}
void EXTI15_10_IRQHandler()
{
  if(EXTI_GetITStatus(EXTI_Line10) != RESET)
    wirishExternalInterruptHandler(10);
  if(EXTI_GetITStatus(EXTI_Line11) != RESET)
    wirishExternalInterruptHandler(11);
  if(EXTI_GetITStatus(EXTI_Line12) != RESET)
    wirishExternalInterruptHandler(12);
  if(EXTI_GetITStatus(EXTI_Line13) != RESET)
    wirishExternalInterruptHandler(13);
  if(EXTI_GetITStatus(EXTI_Line14) != RESET)
    wirishExternalInterruptHandler(14);
  if(EXTI_GetITStatus(EXTI_Line15) != RESET)
    wirishExternalInterruptHandler(15);
}

