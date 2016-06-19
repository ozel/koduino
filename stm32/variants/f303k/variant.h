#ifndef variant_h
#define variant_h

#include "chip.h"

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

// >>> Autogenerated by variant_codegen.py >>>

extern const uint8_t PF0;
extern const uint8_t PF1;
extern const uint8_t PA0;
extern const uint8_t PA1;
extern const uint8_t PA2;
extern const uint8_t PA3;
extern const uint8_t PA4;
extern const uint8_t PA5;
extern const uint8_t PA6;
extern const uint8_t PA7;
extern const uint8_t PB0;
extern const uint8_t PB1;
extern const uint8_t PA8;
extern const uint8_t PA9;
extern const uint8_t PA10;
extern const uint8_t PA11;
extern const uint8_t PA12;
extern const uint8_t PA13;
extern const uint8_t PA14;
extern const uint8_t PA15;
extern const uint8_t PB3;
extern const uint8_t PB4;
extern const uint8_t PB5;
extern const uint8_t PB6;
extern const uint8_t PB7;

extern const uint8_t TIMER1;
extern const uint8_t TIMER2;
extern const uint8_t TIMER3;
extern const uint8_t TIMER15;
extern const uint8_t TIMER16;
extern const uint8_t TIMER17;
extern const uint8_t TIMER6;
extern const uint8_t TIMER7;

// <<< Autogenerated by variant_codegen.py <<<

// Mandated by the core library
#define LED_BUILTIN PB3

#ifdef __cplusplus
} // extern "C"

#include "USARTClass.h"

extern USARTClass Serial1; // default PA9, PA10
extern USARTClass Serial2; // default PA2, PA15



bool isTimer32Bit(uint8_t tim);
uint8_t numChannelsInTimer(uint8_t tim);
bool isAnalogPin(uint8_t pin);
void variantInit();

#endif // __cplusplus

#endif