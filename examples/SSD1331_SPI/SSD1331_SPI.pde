#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <SPI.h>

#include <ssd1331_spi.h>

void* pLock = NULL;

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
  switch(0) {
  case 0:
    oMyDisplay.drawPixel(
      random(96), random(64),
      SSD1331_SPI_RGBTOCOLOR(random(32), random(64), random(32))
    );
    break;
  case 1:
    oMyDisplay.drawLine(
      (int16_t)random(192) - 48, (int16_t)random(128) - 32,
      (int16_t)random(192) - 48, (int16_t)random(128) - 32,
      SSD1331_SPI_RGBTOCOLOR(random(32), random(64), random(32))
    );
    break;
  case 2:
    oMyDisplay.drawRectangle(
      (int16_t)random(192) - 48, (int16_t)random(128) - 32,
      (int16_t)random(192) - 48, (int16_t)random(128) - 32,
      SSD1331_SPI_RGBTOCOLOR(random(32), random(64), random(32)),
      false,
      0
    );
    break;
  case 3:
    oMyDisplay.drawRectangle(
      (int16_t)random(192) - 48, (int16_t)random(128) - 32,
      (int16_t)random(192) - 48, (int16_t)random(128) - 32,
      SSD1331_SPI_RGBTOCOLOR(random(32), random(64), random(32)),
      true,
      SSD1331_SPI_RGBTOCOLOR(random(32), random(64), random(32))
    );
    break;
  case 4:
    oMyDisplay.drawCircle(
      (int16_t)random(192) - 48, (int16_t)random(128) - 32,
      random(64),
      SSD1331_SPI_RGBTOCOLOR(random(32), random(64), random(32)),
      false,
      0
    );
    break;
  case 5:
    oMyDisplay.drawCircle(
      (int16_t)random(192) - 48, (int16_t)random(128) - 32,
      random(64),
      SSD1331_SPI_RGBTOCOLOR(random(32), random(64), random(32)),
      true,
      SSD1331_SPI_RGBTOCOLOR(random(32), random(64), random(32))
    );
    break;
  }
}

