
#include "variant.h"
#include "types.h"
#include "USARTClass.h"
#include "adc.h"


// >>> Autogenerated by /Applications/Arduino.app/Contents/Resources/Java/hardware/koduino/stm32/variants/variant_codegen.py >>>

const uint8_t PA0 = 0;
const uint8_t PA1 = 1;
const uint8_t PA2 = 2;
const uint8_t PA3 = 3;
const uint8_t PA4 = 4;
const uint8_t PA5 = 5;
const uint8_t PA6 = 6;
const uint8_t PA7 = 7;
const uint8_t PA8 = 8;
const uint8_t PA9 = 9;
const uint8_t PA10 = 10;
const uint8_t PA11 = 11;
const uint8_t PA12 = 12;
const uint8_t PA13 = 13;
const uint8_t PA14 = 14;
const uint8_t PA15 = 15;
const uint8_t PB0 = 16;
const uint8_t PB3 = 17;
const uint8_t PB4 = 18;
const uint8_t PB5 = 19;
const uint8_t PB6 = 20;
const uint8_t PB7 = 21;
const uint8_t PF0 = 22;
const uint8_t PF1 = 23;

PinInfo PIN_MAP[] = {
  {GPIOA, 0, 1, 1, TIMER2, 1, NOT_SET, NOT_SET},
  {GPIOA, 1, 2, 1, TIMER2, 2, NOT_SET, NOT_SET},
  {GPIOA, 2, 3, 1, TIMER2, 3, NOT_SET, NOT_SET},
  {GPIOA, 3, 4, 1, TIMER2, 4, NOT_SET, NOT_SET},
  {GPIOA, 4, 5, NOT_SET, NOT_SET, NOT_SET, NOT_SET, NOT_SET},
  {GPIOA, 5, NOT_SET, 1, TIMER2, 1, NOT_SET, NOT_SET},
  {GPIOA, 6, 10, 1, TIMER16, 1, NOT_SET, NOT_SET},
  {GPIOA, 7, 15, 1, TIMER17, 1, NOT_SET, NOT_SET},
  {GPIOA, 8, NOT_SET, 6, TIMER1, 1, NOT_SET, NOT_SET},
  {GPIOA, 9, NOT_SET, 6, TIMER1, 2, NOT_SET, NOT_SET},
  {GPIOA, 10, NOT_SET, 6, TIMER1, 3, NOT_SET, NOT_SET},
  {GPIOA, 11, NOT_SET, 11, TIMER1, 4, NOT_SET, NOT_SET},
  {GPIOA, 12, NOT_SET, 1, TIMER16, 1, NOT_SET, NOT_SET},
  {GPIOA, 13, NOT_SET, NOT_SET, NOT_SET, NOT_SET, NOT_SET, NOT_SET},
  {GPIOA, 14, NOT_SET, NOT_SET, NOT_SET, NOT_SET, NOT_SET, NOT_SET},
  {GPIOA, 15, NOT_SET, NOT_SET, NOT_SET, NOT_SET, NOT_SET, NOT_SET},
  {GPIOB, 0, 11, NOT_SET, NOT_SET, NOT_SET, NOT_SET, NOT_SET},
  {GPIOB, 3, NOT_SET, 1, TIMER2, 2, NOT_SET, NOT_SET},
  {GPIOB, 4, NOT_SET, 1, TIMER16, 1, NOT_SET, NOT_SET},
  {GPIOB, 5, NOT_SET, 10, TIMER17, 1, NOT_SET, NOT_SET},
  {GPIOB, 6, NOT_SET, NOT_SET, NOT_SET, NOT_SET, NOT_SET, NOT_SET},
  {GPIOB, 7, NOT_SET, NOT_SET, NOT_SET, NOT_SET, NOT_SET, NOT_SET},
  {GPIOF, 0, NOT_SET, NOT_SET, NOT_SET, NOT_SET, NOT_SET, NOT_SET},
  {GPIOF, 1, NOT_SET, NOT_SET, NOT_SET, NOT_SET, NOT_SET, NOT_SET},
};

const uint8_t TIMER1 = 0;
const uint8_t TIMER2 = 1;
const uint8_t TIMER15 = 2;
const uint8_t TIMER16 = 3;
const uint8_t TIMER17 = 4;
const uint8_t TIMER6 = 5;

TimerChannelData timer1ch[4] = {
  {0, false, 0, 0, 0, 0},
  {0, false, 0, 0, 0, 0},
  {0, false, 0, 0, 0, 0},
  {0, false, 0, 0, 0, 0}
};
TimerChannelData timer2ch[4] = {
  {0, false, 0, 0, 0, 0},
  {0, false, 0, 0, 0, 0},
  {0, false, 0, 0, 0, 0},
  {0, false, 0, 0, 0, 0}
};
TimerChannelData timer15ch[2] = {
  {0, false, 0, 0, 0, 0},
  {0, false, 0, 0, 0, 0}
};

TimerInfo TIMER_MAP[] = {
  {TIM1, TIM1_CC_IRQn, 1000, 0, timer1ch},
  {TIM2, TIM2_IRQn, 1000, 0, timer2ch},
  {TIM15, TIM1_BRK_TIM15_IRQn, 1000, 0, timer15ch},
  {TIM16, TIM1_UP_TIM16_IRQn, 1000, 0, (TimerChannelData *)0},
  {TIM17, TIM1_TRG_COM_TIM17_IRQn, 1000, 0, (TimerChannelData *)0},
  {TIM6, TIM6_DAC_IRQn, 1000, 0, (TimerChannelData *)0},
  // ^^^ Look up IRQn names from CMSIS device header ^^^
};

// <<< Autogenerated by /Applications/Arduino.app/Contents/Resources/Java/hardware/koduino/stm32/variants/variant_codegen.py <<<

// Use the 3 basic timers and two others
TimebaseChannel TIMEBASE_MAP[] = {
  {.timer = NOT_SET, .isr = 0}, // 0
  {.timer = NOT_SET, .isr = 0} // 1
};
// Serial
USARTInfo USART_MAP[2] = {
  { USART1, USART1_IRQn, PA9, PA10 },
  { USART2, USART2_IRQn, PA2, PA3 }
};
USARTClass Serial1(&USART_MAP[0]);
USARTClass Serial2(&USART_MAP[1]);


bool isTimer32Bit(uint8_t tim) {
  return false;
}

uint8_t numChannelsInTimer(uint8_t tim) {
  if (tim < TIMER15)
    return 4;
  else if (tim < TIMER16)
    return 2;
  else if (tim < TIMER6)
    return 1;
  else
    return 0;
}

bool isAnalogPin(uint8_t pin) {
  return (PIN_MAP[pin].adcChannel != NOT_SET);
}

void variantInit() {
  TIMEBASE_MAP[0].timer = TIMER6;
  TIMEBASE_MAP[1].timer = TIMER15;

  // GPIO
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
  // SYSCFG - needed for EXTI
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  // Timer clocks
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);

  // USART
  RCC_USARTCLKConfig(RCC_USART1CLK_SYSCLK);
  RCC_USARTCLKConfig(RCC_USART2CLK_SYSCLK);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  // ADC(s)
  RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div2);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);
  adcInit(ADC1);
}
