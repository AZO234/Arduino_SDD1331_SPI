/* SSD1331 SPI Driver by AZO */

#include <stdio.h>
#include "ssd1331_spi.h"

SSD1331_SPI::SSD1331_SPI(void) {
}

bool SSD1331_SPI::initialize(
  const SSD1331_SPI_SetPinIO_t  tSetPinIO,
  const SSD1331_SPI_PinOutput_t tPinOutput,
  const SSD1331_SPI_Transfer_t  tTransfer,
  const SSD1331_SPI_DelayMS_t   tDelayMS,
  const SSD1331_MemoryBarrier_t tMemoryBarrier,
  void** ppLock,
  const uint8_t u8CS,
  const uint8_t u8RST,
  const uint8_t u8DC,
  const uint8_t u8MaxX,
  const uint8_t u8MaxY,
  const uint8_t u8Contrast
) {
  bool bValid = false;

  this->tSetPinIO = NULL;
  this->tPinOutput = NULL;
  this->tTransfer = NULL;
  this->tDelayMS = NULL;

  if(tSetPinIO && tPinOutput && tTransfer && tDelayMS && tMemoryBarrier && ppLock) {
    this->tSetPinIO = tSetPinIO;
    this->tPinOutput = tPinOutput;
    this->tTransfer = tTransfer;
    this->tDelayMS = tDelayMS;

    this->u8CS = u8CS;
    this->u8RST = u8RST;
    this->u8DC = u8DC;

    this->tSetPinIO(this->u8CS, 0);
    this->tPinOutput(this->u8CS, true);
    while(*ppLock != this) {
      tMemoryBarrier();
      if(!*ppLock) {
        *ppLock = this;
      }
      tMemoryBarrier();
    }
    this->tSetPinIO(this->u8DC, 0);
    this->tPinOutput(this->u8DC, true);
    *ppLock = NULL;
    tMemoryBarrier();
    this->tSetPinIO(this->u8RST, 0);
    this->tPinOutput(this->u8RST, true);

    this->tPinOutput(this->u8RST, false);
    this->tDelayMS(10);
    this->tPinOutput(this->u8RST, true);
    this->tDelayMS(10);

    bValid = SSD1331_Initialize(&this->tSSD1331, this, NULL, SSD1331_SPI::gen_write, tMemoryBarrier, ppLock, u8MaxX, u8MaxY, u8Contrast);
    this->tDelayMS(100);
  }

  return bValid;
}

/* Access (Don't use) */
void SSD1331_SPI::gen_write(void* pSSD1331_SPI, const SSD1331_Write_Data_t* ptWriteData) {
  uint8_t u8Count;
  SSD1331_SPI* poSSD1331_SPI = (SSD1331_SPI*)pSSD1331_SPI;

  if(pSSD1331_SPI && ptWriteData) {
    if(ptWriteData->u8Count > 0) {
      poSSD1331_SPI->tPinOutput(poSSD1331_SPI->u8CS, false);
      poSSD1331_SPI->tPinOutput(poSSD1331_SPI->u8DC, ptWriteData->bData);
      for(u8Count = 0; u8Count < ptWriteData->u8Count; u8Count++) {
        poSSD1331_SPI->tTransfer(ptWriteData->au8Data[u8Count]);
      }
      poSSD1331_SPI->tPinOutput(poSSD1331_SPI->u8CS, true);
    }
  }
}

bool SSD1331_SPI::initDevice(void) {
  return SSD1331_InitDevice(&this->tSSD1331);
}

uint8_t SSD1331_SPI::getCS(void) {
  return u8CS;
}

uint8_t SSD1331_SPI::getRST(void) {
  return u8RST;
}

uint8_t SSD1331_SPI::getDC(void) {
  return u8DC;
}

bool SSD1331_SPI::clear(void) {
  return SSD1331_Clear(&this->tSSD1331);
}

bool SSD1331_SPI::drawPixel(const uint8_t u8X, const uint8_t u8Y, const uint16_t u16Color) {
  return SSD1331_DrawPixel(&this->tSSD1331, u8X, u8Y, u16Color);
}

bool SSD1331_SPI::drawLine(
  const int16_t i16X1,
  const int16_t i16Y1,
  const int16_t i16X2,
  const int16_t i16Y2,
  const uint16_t u16Color
) {
  return SSD1331_DrawLine(&this->tSSD1331, i16X1, i16Y1, i16X2, i16Y2, u16Color);
}

bool SSD1331_SPI::drawRectangle(
  const int16_t i16X1,
  const int16_t i16Y1,
  const int16_t i16X2,
  const int16_t i16Y2,
  const uint16_t u16Color,
  const bool bFill,
  const uint16_t u16FillColor
) {
  return SSD1331_DrawRectangle(&this->tSSD1331, i16X1, i16Y1, i16X2, i16Y2, u16Color, bFill, u16FillColor);
}

bool SSD1331_SPI::drawCircle(
  const int16_t i16X,
  const int16_t i16Y,
  const int16_t i16R,
  const uint16_t u16Color,
  const bool bFill,
  const uint16_t u16FillColor
) {
  return SSD1331_DrawCircle(&this->tSSD1331, i16X, i16Y, i16R, u16Color, bFill, u16FillColor);
}

bool SSD1331_SPI::isInvert(void) {
  return this->tSSD1331.bInvert;
}

void SSD1331_SPI::setInvert(const bool bInvert) {
  this->tSSD1331.bInvert = bInvert;
}

bool SSD1331_SPI::isScroll(void) {
  return this->tSSD1331.bScroll;
}

bool SSD1331_SPI::scroll(const uint8_t u8X, const uint8_t u8Y, const uint8_t u8Interval) {
  return SSD1331_Scroll(&this->tSSD1331, u8X, u8Y, u8Interval);
}

bool SSD1331_SPI::disableScroll(void) {
  return SSD1331_DisableScroll(&this->tSSD1331);
}

bool SSD1331_SPI::isSleep(void) {
  return this->tSSD1331.bSleep;
}

bool SSD1331_SPI::sleep(void) {
  return SSD1331_Sleep(&this->tSSD1331);
}

bool SSD1331_SPI::wakeup(void) {
  return SSD1331_Wakeup(&this->tSSD1331);
}

