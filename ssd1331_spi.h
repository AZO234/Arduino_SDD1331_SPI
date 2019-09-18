#ifndef _SSD1331_SPI_H_
#define _SSD1331_SPI_H_

/* SSD1331 SPI Driver by AZO */

#include "ssd1331.h"

typedef void (*SSD1331_SPI_SetPinIO_t)(const uint8_t u8Pin, const uint8_t u8IOMode);
typedef void (*SSD1331_SPI_PinOutput_t)(const uint8_t u8Pin, const bool bHigh);
typedef void (*SSD1331_SPI_Transfer_t)(const uint8_t u8Data);
typedef void (*SSD1331_SPI_DelayMS_t)(const uint16_t u16MS);

class SSD1331_SPI {
public:
  SSD1331_SPI(void);

  /* MaxX and MaxY must be multiples of 8 */
  bool initialize(
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
  );
  bool initDevice(void);

  uint8_t getCS(void);
  uint8_t getRST(void);
  uint8_t getDC(void);

  bool clear(void);
  bool drawPixel(const uint8_t u8X, const uint8_t u8Y, const uint16_t u16Color);
  bool drawLine(
    const int16_t i16X1,
    const int16_t i16Y1,
    const int16_t i16X2,
    const int16_t i16Y2,
    const uint16_t u16Color
  );
  bool drawRectangle(
    const int16_t i16X1,
    const int16_t i16Y1,
    const int16_t i16X2,
    const int16_t i16Y2,
    const uint16_t u16Color,
    const bool bFill,
    const uint16_t u16FillColor
  );
  bool drawCircle(
    const int16_t i16X,
    const int16_t i16Y,
    const int16_t i16R,
    const uint16_t u16Color,
    const bool bFill,
    const uint16_t u16FillColor
  );
  bool isInvert(void);
  void setInvert(const bool bInvert);

  bool isScroll(void);
  bool scroll(const uint8_t u8X, const uint8_t u8Y, const uint8_t u8Interval);
  bool disableScroll(void);

  bool isSleep(void);
  bool sleep(void);
  bool wakeup(void);

  /* Access (Don't use) */
  static void gen_write(void* pSSD1331_SPI, const SSD1331_Write_Data_t* ptWriteData);

private:
  SSD1331_t tSSD1331;
  uint8_t u8CS;
  uint8_t u8RST;
  uint8_t u8DC;

  SSD1331_SPI_SetPinIO_t tSetPinIO;
  SSD1331_SPI_PinOutput_t tPinOutput;
  SSD1331_SPI_Transfer_t tTransfer;
  SSD1331_SPI_DelayMS_t tDelayMS;
};

#endif  /* _SSD1331_SPI_H_ */

