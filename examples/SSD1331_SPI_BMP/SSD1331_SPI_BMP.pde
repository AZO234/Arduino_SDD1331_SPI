#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#if defined(__AVR__)
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#else
#define PROGMEM
#endif

#include <SPI.h>

#include <ssd1331_spi.h>
#include "bmp.h"

void* pLock = NULL;

extern BMP_t tLala;
extern BMP_t tXevious;

SSD1331_SPI oMyDisplay;

static void SSD1331_SPI_SetPinIO(const uint8_t u8Pin, const uint8_t u8IOMode) {
  switch(u8IOMode) {
  case 0:
    pinMode(u8Pin, OUTPUT);
    break;
  case 1:
    pinMode(u8Pin, INPUT);
    break;
  case 2:
    pinMode(u8Pin, INPUT_PULLUP);
    break;
  }
}

static void SSD1331_SPI_PinOutput(const uint8_t u8Pin, const bool bHigh) {
  digitalWrite(u8Pin, bHigh ? HIGH : LOW);
}

static void SSD1331_SPI_Transfer(const uint8_t u8Data) {
  SPI.transfer(u8Data);
}

static void SSD1331_SPI_DelayMS(const uint16_t u16MS) {
  delay(u16MS);
}

static void SSD1331_SPI_MemoryBarrier(void) {
}

void DisplayImage(SSD1331_SPI* poDisplay, const int16_t i16X, const int8_t i16Y, const BMP_t* ptImage) {
  uint16_t u16X, u16Y;
  uint16_t u16Color, u16LineSize;

  if(poDisplay && ptImage) {
    u16LineSize = ptImage->u32Width * 2;

    for(u16Y = 0; u16Y < ptImage->u32Height; u16Y++) {
      for(u16X = 0; u16X < ptImage->u32Width; u16X++) {
#if defined(__AVR__)
        u16Color = pgm_read_byte_near(ptImage->pu8ImageData + u16Y * u16LineSize + u16X * 2);
        u16Color |= (uint16_t)pgm_read_byte_near(ptImage->pu8ImageData + u16Y * u16LineSize + u16X * 2 + 1) << 8;
#elif defined(ESP8266)
        memcpy_P(&u16Color, &ptImage->pu8ImageData[u16Y * u16LineSize + u16X * 2], 2);
#else
        u16Color = ptImage->pu8ImageData[u16Y * u16LineSize + u16X * 2];
        u16Color |= (uint16_t)ptImage->pu8ImageData[u16Y * u16LineSize + u16X * 2 + 1] << 8;
#endif
        poDisplay->drawPixel(i16X + u16X, i16Y + u16Y, u16Color);
      }
    }
  }
}

void setup() {
  oMyDisplay.initialize(
    SSD1331_SPI_SetPinIO,
    SSD1331_SPI_PinOutput,
    SSD1331_SPI_Transfer,
    SSD1331_SPI_DelayMS,
    SSD1331_SPI_MemoryBarrier,
    &pLock,
    10, 9, 8, 96, 64, 0xFF
  );

  SPI.begin();
  SPI.setDataMode(SPI_MODE3);

  oMyDisplay.initDevice();
  oMyDisplay.clear();
}

void loop() {
  DisplayImage(&oMyDisplay, 0, 0, &tLala);
  delay(5000);
  DisplayImage(&oMyDisplay, 0, 0, &tXevious);
  delay(5000);
}
