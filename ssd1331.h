#ifndef _SSD1331_H_
#define _SSD1331_H_

/* SSD1331 Driver by AZO */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define SSD1331_SPI_RGBTOCOLOR(u8R, u8G, u8B)	((((uint16_t)u8R & 0x1F) << 11) | (((uint16_t)u8G & 0x3F) << 5) | (u8B & 0x1F))

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct SSD1331_Write_Data_t_ {
  bool bData;
  uint8_t u8Count;
  uint8_t au8Data[30];
} SSD1331_Write_Data_t;

/* Access */
typedef uint8_t (*SSD1331_Read_t)(void* pInstance, const uint8_t u8Reg);
typedef void (*SSD1331_Write_t)(void* pInstance, const SSD1331_Write_Data_t* ptWriteData);
typedef void (*SSD1331_MemoryBarrier_t)(void);

typedef struct SSD1331_t_ {
  void* pInstance;
  SSD1331_Read_t tRead;
  SSD1331_Write_t tWrite;
  SSD1331_MemoryBarrier_t tMemoryBarrier;
  void** ppLock;
  uint8_t u8MaxX;
  uint8_t u8MaxY;
  uint8_t u8Contrast;
  bool bSleep;
  bool bScroll;
  bool bInvert;
  SSD1331_Write_Data_t tWriteData;
} SSD1331_t;

bool SSD1331_Initialize(
  SSD1331_t* ptSSD1331,
  void* pInstance,
  const SSD1331_Read_t tRead,
  const SSD1331_Write_t tWrite,
  const SSD1331_MemoryBarrier_t tMemoryBarrier,
  void** ppLock,
  const uint8_t u8MaxX,
  const uint8_t u8MaxY,
  const uint8_t u8Contrast
);
bool SSD1331_InitDevice(SSD1331_t* ptSSD1331);
bool SSD1331_SetContrast(SSD1331_t* ptSSD1331, const uint8_t u8Contrast);

bool SSD1331_Clear(SSD1331_t* ptSSD1331);
bool SSD1331_DrawPixel(SSD1331_t* ptSSD1331, const int16_t i16X, const uint16_t i16Y, const uint16_t u16Color);
bool SSD1331_DrawLine(
  SSD1331_t* ptSSD1331,
  const int16_t i16X1,
  const int16_t i16Y1,
  const int16_t i16X2,
  const int16_t i16Y2,
  const uint16_t u16Color
);
bool SSD1331_DrawRectangle(
  SSD1331_t* ptSSD1331,
  const int16_t i16X1,
  const int16_t i16Y1,
  const int16_t i16X2,
  const int16_t i16Y2,
  const uint16_t u16Color,
  const bool bFill,
  const uint16_t u16FillColor
);
bool SSD1331_DrawCircle(
  SSD1331_t* ptSSD1331,
  const int16_t i16X,
  const int16_t i16Y,
  const int16_t i16R,
  const uint16_t u16Color,
  const bool bFill,
  const uint16_t u16FillColor
);

bool SSD1331_Scroll(SSD1331_t* ptSSD1331, const uint8_t u8X, const uint8_t u8Y, const uint8_t u8Interval);
bool SSD1331_DisableScroll(SSD1331_t* ptSSD1331);

bool SSD1331_Sleep(SSD1331_t* ptSSD1331);
bool SSD1331_Wakeup(SSD1331_t* ptSSD1331);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SSD1331_H_ */

