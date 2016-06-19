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
#include "USARTClass.h"
#include "nvic.h"
#include "pins.h"
#include "system_clock.h"

// Initialize Class Variables //////////////////////////////////////////////////
USART_InitTypeDef USARTClass::USART_InitStructure;
// bool USARTClass::USARTSerial_Enabled = false;

// Constructors ////////////////////////////////////////////////////////////////

USARTClass::USARTClass(USARTInfo *usartMapPtr) {
  usartMap = usartMapPtr;

  usartMap->rxBuf = &_rxBuf;
  usartMap->txBuf = &_txBuf;

  memset(&_rxBuf, 0, sizeof(RingBuffer));
  memset(&_txBuf, 0, sizeof(RingBuffer));

  // transmitting = false;
  usartMap->rxCallback = NULL;

  irqnPriority = 3;
}

void USARTClass::init(uint32_t baud, uint32_t wordLength, uint32_t parity, uint32_t stopBits) {
  // Enable interrupt (for RXNE)
  nvicEnable(usartMap->irqChannel, irqnPriority);
  
  // Init USART
  USART_InitStructure.USART_BaudRate = baud;
  USART_InitStructure.USART_WordLength = wordLength;
  USART_InitStructure.USART_Parity = parity;
  USART_InitStructure.USART_StopBits = stopBits;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(usartMap->USARTx, &USART_InitStructure);

  USART_Cmd(usartMap->USARTx, ENABLE);
}

// Public Methods //////////////////////////////////////////////////////////////

void USARTClass::begin(uint32_t baud, uint8_t config) {
  // Need to check datasheet
  uint8_t af = 7;
#if defined(SERIES_STM32F4xx)
  // FIXME: this will need to be expanded
  if (usartMap->USARTx == USART6) {
    af = 8;
  }
#endif
#if defined(SERIES_STM32F30x) and !defined(STM32F303x8)
  // FIXME: this will need to be expanded
  if (usartMap->USARTx == UART4 || usartMap->USARTx == UART5) {
    af = 5;
  }
#endif
  pinModeAlt(usartMap->txPin, GPIO_OType_PP, GPIO_PuPd_UP, af);
  pinModeAlt(usartMap->rxPin, GPIO_OType_PP, GPIO_PuPd_UP, af);

  switch (config) {
    case SERIAL_8N1: init(baud, USART_WordLength_8b, USART_Parity_No, USART_StopBits_1);
    break;
    case SERIAL_8N2: init(baud, USART_WordLength_8b, USART_Parity_No, USART_StopBits_2);
    break;
    case SERIAL_7E1: init(baud, USART_WordLength_8b, USART_Parity_Even, USART_StopBits_1);
    break;
    case SERIAL_8E1: init(baud, USART_WordLength_9b, USART_Parity_Even, USART_StopBits_1);
    break;
    case SERIAL_7E2: init(baud, USART_WordLength_8b, USART_Parity_Even, USART_StopBits_2);
    break;
    case SERIAL_8E2: init(baud, USART_WordLength_9b, USART_Parity_Even, USART_StopBits_2);
    break;
    case SERIAL_7O1: init(baud, USART_WordLength_8b, USART_Parity_Odd, USART_StopBits_1);
    break;
    case SERIAL_8O1: init(baud, USART_WordLength_9b, USART_Parity_Odd, USART_StopBits_1);
    break;
    case SERIAL_7O2: init(baud, USART_WordLength_8b, USART_Parity_Odd, USART_StopBits_2);
    break;
    case SERIAL_8O2: init(baud, USART_WordLength_9b, USART_Parity_Odd, USART_StopBits_2);
    break;
  }

  // USART interrupts
  USART_ClearFlag(usartMap->USARTx, USART_FLAG_RXNE);
  USART_ClearITPendingBit(usartMap->USARTx, USART_IT_RXNE);
  USART_ITConfig(usartMap->USARTx, USART_IT_RXNE, ENABLE);
}

void USARTClass::end() {
  // wait for transmission to end
  flush();
  USART_ITConfig(usartMap->USARTx, USART_IT_RXNE, DISABLE);
  USART_ITConfig(usartMap->USARTx, USART_IT_TXE, DISABLE);
	USART_Cmd(usartMap->USARTx, DISABLE);
  NVIC_DisableIRQ(usartMap->irqChannel);
	// clear any received data
	_rxBuf.head = _rxBuf.tail;
}

void USARTClass::setPins(uint8_t tx, uint8_t rx) {
  usartMap->txPin = tx;
  usartMap->rxPin = rx;
}

int USARTClass::available(void) {
	return (int)(SERIAL_BUFFER_SIZE + _rxBuf.head - _rxBuf.tail) % SERIAL_BUFFER_SIZE;
}

int USARTClass::peek(void) {
	if (_rxBuf.head == _rxBuf.tail)
    // RX is empty
		return -1;
	else
		return _rxBuf.buffer[_rxBuf.tail];
}

int USARTClass::read(void) {
	// if the head isn't ahead of the tail, we don't have any characters
	if (_rxBuf.head == _rxBuf.tail)
		return -1;
	else {
    // initially, head=tail. first new character goes in head+1, so read() should return tail+1
    _rxBuf.tail = (unsigned int)(_rxBuf.tail + 1) % SERIAL_BUFFER_SIZE;
		unsigned char c = _rxBuf.buffer[_rxBuf.tail];
		return c;
	}
}

bool USARTClass::writeComplete() {
  // return (_txBuf.head == _txBuf.tail);
  return (USART_GetITStatus(usartMap->USARTx, USART_IT_TXE) == RESET);
}

void USARTClass::flush() {
  // wait for transmission of outgoing data
  while (_txBuf.head != _txBuf.tail);
	// Loop until last frame transmission complete
	while (USART_GetFlagStatus(usartMap->USARTx, USART_FLAG_TC) == RESET);
}

void USARTClass::stopTX() {
  // don't wait for transmission of outgoing data
  USART_ITConfig(usartMap->USARTx, USART_IT_TXE, DISABLE);
  _txBuf.head = _txBuf.tail = 0;
}

void USARTClass::flushInput() {
  _rxBuf.tail = _rxBuf.head;
}

size_t USARTClass::write(uint8_t c) {
  // HACK: to stop bootloading problems, don't output to Serial1 for the first second
  if (usartMap->USARTx == USART1 && millis() < 1000)
    return 0;

  uint8_t next = (_txBuf.head + 1) % SERIAL_BUFFER_SIZE;

  // Buffer overrun protection?
  while (next == _txBuf.tail || ((__get_PRIMASK() & 1) && _txBuf.head != _txBuf.tail)) {
    // Interrupts are on but they are not being serviced because this was called from a higher priority interrupt
    if (USART_GetITStatus(usartMap->USARTx, USART_IT_TXE) && USART_GetFlagStatus(usartMap->USARTx, USART_FLAG_TXE)) {
      // protect for good measure
      USART_ITConfig(usartMap->USARTx, USART_IT_TXE, DISABLE);
      // Write out a byte
      USART_SendData(usartMap->USARTx, _txBuf.buffer[_txBuf.tail++]);
      _txBuf.tail %= SERIAL_BUFFER_SIZE;
      // unprotect
      USART_ITConfig(usartMap->USARTx, USART_IT_TXE, ENABLE);
    }
  }

  _txBuf.buffer[_txBuf.head] = c;
  _txBuf.head = next;

  USART_ITConfig(usartMap->USARTx, USART_IT_TXE, ENABLE);
  // bus uses its own write command
	return 1;
}

// size_t USARTClass::write(const uint8_t *buffer, size_t size) {
//   // HACK: to stop bootloading problems, don't output to Serial1 for the first second
//   // if (usartMap->USARTx == USART1 && millis() < 1000)
//   //   return 0;

//   for (uint8_t i=0; i<size; ++i) {
//     uint8_t next = (_txBuf.head+1) % SERIAL_BUFFER_SIZE;
//     if (next == _txBuf.tail) {
//       return i;
//     }
//     _txBuf.buffer[next] = buffer[i];
//     _txBuf.head = next;
//   }

//   USART_ITConfig(usartMap->USARTx, USART_IT_TXE, ENABLE);
//   // bus uses its own write command
//   return size;
// }

// void USARTClass::attachInterrupt(ByteFunc f) {
//   usartMap->rxCallback = f;
// }

// void USARTClass::detachInterrupt() {
//   usartMap->rxCallback = NULL;
// }

// CUSTOM FUNCTIONS

uint8_t USARTClass::peekAt(uint8_t pos) {
  // first new element received goes at head+1
  // initially tail=head. so peekAt(0) should return tail+1
  return _rxBuf.buffer[(_rxBuf.tail + pos + 1) % SERIAL_BUFFER_SIZE];
}

extern "C" {
// weak definitions
void busHandlerTC(void *busObject) {}
void busHandlerRX(void *busObject, uint8_t byte) {}
}
