/* SSD1331 Driver by AZO */

#include "ssd1331.h"

static void Lock(SSD1331_t* ptSSD1331) {
  ptSSD1331->tMemoryBarrier();
  while(*ptSSD1331->ppLock != ptSSD1331) {
    if(!*ptSSD1331->ppLock) {
      *ptSSD1331->ppLock = ptSSD1331;
      ptSSD1331->tMemoryBarrier();
    }
  }
}

static void Unlock(SSD1331_t* ptSSD1331) {
  *ptSSD1331->ppLock = NULL;
  ptSSD1331->tMemoryBarrier();
}

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
) {
  bool bValid = false;

  if(ptSSD1331 && pInstance && tWrite && tMemoryBarrier && ppLock && u8MaxX > 0 && u8MaxY > 0) {
    bValid = true;
    ptSSD1331->pInstance      = pInstance;
    ptSSD1331->tRead          = tRead;
    ptSSD1331->tWrite         = tWrite;
    ptSSD1331->tMemoryBarrier = tMemoryBarrier;
    ptSSD1331->ppLock         = ppLock;
    ptSSD1331->u8MaxX         = u8MaxX;
    ptSSD1331->u8MaxY         = u8MaxY;
    ptSSD1331->u8Contrast     = u8Contrast;
    ptSSD1331->bSleep         = false;
    ptSSD1331->bScroll        = false;
    ptSSD1331->bInvert        = false;
  }

  return bValid;
}

bool SSD1331_InitDevice(SSD1331_t* ptSSD1331) {
  bool bValid = false;
  uint8_t u8X, u8Y, u8Count;

  if(ptSSD1331) {
    if(ptSSD1331->tWrite) {
      bValid = true;
      ptSSD1331->tWriteData.u8Count = 30;
      ptSSD1331->tWriteData.bData = false;
      ptSSD1331->tWriteData.au8Data[ 0] = 0xAE;        /* Set Display Off */
      ptSSD1331->tWriteData.au8Data[ 1] = 0xA0;        /* Remap & Color Depth setting */
      ptSSD1331->tWriteData.au8Data[ 2] = 0b01110010;  /* A[7:6] = 00; 256 color. A[7:6] = 01; 65k color format */
      ptSSD1331->tWriteData.au8Data[ 3] = 0xA1;        /* Set Display Start Line */
      ptSSD1331->tWriteData.au8Data[ 4] = 0;
      ptSSD1331->tWriteData.au8Data[ 5] = 0xA2;        /* Set Display Offset */
      ptSSD1331->tWriteData.au8Data[ 6] = 0;
      ptSSD1331->tWriteData.au8Data[ 7] = 0xA4;        /* Set Display Mode (Normal) */
      ptSSD1331->tWriteData.au8Data[ 8] = 0xA8;        /* Set Multiplex Ratio */
      ptSSD1331->tWriteData.au8Data[ 9] = 0b00111111;  /* 15-63 */
      ptSSD1331->tWriteData.au8Data[10] = 0xAD;        /* Set Master Configration */
      ptSSD1331->tWriteData.au8Data[11] = 0b00111111;  /* a[0]=0 Select external Vcc supply, a[0]=1 Reserved(reset) */
      ptSSD1331->tWriteData.au8Data[12] = 0xB0;        /* Power Save Mode */
      ptSSD1331->tWriteData.au8Data[13] = 0x0B;        /* 0x1A Enable power save mode */
      ptSSD1331->tWriteData.au8Data[14] = 0xB1;        /* Phase 1 and 2 period adjustment */
      ptSSD1331->tWriteData.au8Data[15] = 0x74;
      ptSSD1331->tWriteData.au8Data[16] = 0xB3;        /* Display Clock DIV */
      ptSSD1331->tWriteData.au8Data[17] = 0xF0;
      ptSSD1331->tWriteData.au8Data[18] = 0x8A;        /* Pre Charge A */
      ptSSD1331->tWriteData.au8Data[19] = 0x81;
      ptSSD1331->tWriteData.au8Data[20] = 0x8B;        /* Pre Charge B */
      ptSSD1331->tWriteData.au8Data[21] = 0x82;
      ptSSD1331->tWriteData.au8Data[22] = 0x8C;        /* Pre Charge C */
      ptSSD1331->tWriteData.au8Data[23] = 0x83;
      ptSSD1331->tWriteData.au8Data[24] = 0xBB;        /* Set Pre-charge level */
      ptSSD1331->tWriteData.au8Data[25] = 0x3A;
      ptSSD1331->tWriteData.au8Data[26] = 0xBE;        /* Set VcomH */
      ptSSD1331->tWriteData.au8Data[27] = 0x3E;
      ptSSD1331->tWriteData.au8Data[28] = 0x87;        /* Set Master Current Control */
      ptSSD1331->tWriteData.au8Data[29] = 0x06;
      Lock(ptSSD1331);
      ptSSD1331->tWrite(ptSSD1331->pInstance, &ptSSD1331->tWriteData);
      Unlock(ptSSD1331);

      ptSSD1331->tWriteData.u8Count = 8;
      ptSSD1331->tWriteData.bData = false;
      ptSSD1331->tWriteData.au8Data[0] = 0x21;  /* Dummy Drawing Line */
      ptSSD1331->tWriteData.au8Data[1] = 0;
      ptSSD1331->tWriteData.au8Data[2] = 0;
      ptSSD1331->tWriteData.au8Data[3] = ptSSD1331->u8MaxX - 1;
      ptSSD1331->tWriteData.au8Data[4] = ptSSD1331->u8MaxY - 1;
      ptSSD1331->tWriteData.au8Data[5] = 0;
      ptSSD1331->tWriteData.au8Data[6] = 0;
      ptSSD1331->tWriteData.au8Data[7] = 0;
      Lock(ptSSD1331);
      ptSSD1331->tWrite(ptSSD1331->pInstance, &ptSSD1331->tWriteData);
      Unlock(ptSSD1331);

      ptSSD1331->tWriteData.u8Count = 12;
      ptSSD1331->tWriteData.bData = false;
      ptSSD1331->tWriteData.au8Data[ 0] = 0x15;        /* Set Column Address */
      ptSSD1331->tWriteData.au8Data[ 1] = 0;
      ptSSD1331->tWriteData.au8Data[ 2] = ptSSD1331->u8MaxX - 1;
      ptSSD1331->tWriteData.au8Data[ 3] = 0x75;        /* Set Row Address */
      ptSSD1331->tWriteData.au8Data[ 4] = 0;
      ptSSD1331->tWriteData.au8Data[ 5] = ptSSD1331->u8MaxY - 1;
      ptSSD1331->tWriteData.au8Data[ 6] = 0x81;        /* Set Contrast for Color A */
      ptSSD1331->tWriteData.au8Data[ 7] = ptSSD1331->u8Contrast;
      ptSSD1331->tWriteData.au8Data[ 8] = 0x82;        /* Set Contrast for Color B */
      ptSSD1331->tWriteData.au8Data[ 9] = ptSSD1331->u8Contrast;
      ptSSD1331->tWriteData.au8Data[10] = 0x83;        /* Set Contrast for Color C */
      ptSSD1331->tWriteData.au8Data[11] = ptSSD1331->u8Contrast;
      Lock(ptSSD1331);
      ptSSD1331->tWrite(ptSSD1331->pInstance, &ptSSD1331->tWriteData);
      Unlock(ptSSD1331);

      ptSSD1331->tWriteData.u8Count = 16;
      ptSSD1331->tWriteData.bData = true;
      for(u8Y = 0; u8Y < ptSSD1331->u8MaxY; u8Y++) {  /* Display Black OUT */
        for(u8X = 0; u8X < ptSSD1331->u8MaxX; u8X += 8) {
          for(u8Count = 0; u8Count < 8; u8Count++) {
            ptSSD1331->tWriteData.au8Data[2 * u8Count    ] = 0;
            ptSSD1331->tWriteData.au8Data[2 * u8Count + 1] = 0;
          }
          Lock(ptSSD1331);
          ptSSD1331->tWrite(ptSSD1331->pInstance, &ptSSD1331->tWriteData);
          Unlock(ptSSD1331);
        }
      }

      ptSSD1331->tWriteData.u8Count = 1;
      ptSSD1331->tWriteData.bData = false;
      ptSSD1331->tWriteData.au8Data[0] = 0xAF;        /* Set Display On */
      Lock(ptSSD1331);
      ptSSD1331->tWrite(ptSSD1331->pInstance, &ptSSD1331->tWriteData);
      Unlock(ptSSD1331);
    }
  }

  return bValid;
}

bool SSD1331_SetContrast(SSD1331_t* ptSSD1331, const uint8_t u8Contrast) {
  bool bValid = false;

  if(ptSSD1331) {
    if(ptSSD1331->tWrite) {
      bValid = true;
      ptSSD1331->u8Contrast = u8Contrast;
      ptSSD1331->tWriteData.u8Count = 6;
      ptSSD1331->tWriteData.bData = false;
      ptSSD1331->tWriteData.au8Data[0] = 0x81;  /* Set Contrast for Color A */
      ptSSD1331->tWriteData.au8Data[1] = u8Contrast;
      ptSSD1331->tWriteData.au8Data[2] = 0x82;  /* Set Contrast for Color B */
      ptSSD1331->tWriteData.au8Data[3] = u8Contrast;
      ptSSD1331->tWriteData.au8Data[4] = 0x83;  /* Set Contrast for Color C */
      ptSSD1331->tWriteData.au8Data[5] = u8Contrast;
      Lock(ptSSD1331);
      ptSSD1331->tWrite(ptSSD1331->pInstance, &ptSSD1331->tWriteData);
      Unlock(ptSSD1331);
    }
  }

  return bValid;
}

bool SSD1331_Clear(SSD1331_t* ptSSD1331) {
  bool bValid = false;

  if(ptSSD1331) {
    if(ptSSD1331->tWrite) {
      bValid = true;
      ptSSD1331->tWriteData.u8Count = 5;
      ptSSD1331->tWriteData.bData = false;
      ptSSD1331->tWriteData.au8Data[0] = 0x25;                   /* Clear Window */
      ptSSD1331->tWriteData.au8Data[1] = 0;                      /* Column Address of Start */
      ptSSD1331->tWriteData.au8Data[2] = 0;                      /* Row Address of Start */
      ptSSD1331->tWriteData.au8Data[3] = ptSSD1331->u8MaxX - 1;  /* Column Address of End */
      ptSSD1331->tWriteData.au8Data[4] = ptSSD1331->u8MaxY - 1;  /* Row Address of End */
      Lock(ptSSD1331);
      ptSSD1331->tWrite(ptSSD1331->pInstance, &ptSSD1331->tWriteData);
      Unlock(ptSSD1331);
    }
  }

  return bValid;
}

bool SSD1331_DrawPixel(SSD1331_t* ptSSD1331, const int16_t i16X, const uint16_t i16Y, const uint16_t u16Color) {
  bool bValid = false;
  uint16_t u16UColor;

printf("DrawPixel: %d, %d\n", i16X,i16Y);
  if(ptSSD1331) {
    if(ptSSD1331->tWrite) {
      bValid = true;
      if(i16X >= 0 && i16X < ptSSD1331->u8MaxX && i16Y >= 0 && i16Y < ptSSD1331->u8MaxY) {
        ptSSD1331->tWriteData.u8Count = 6;
        ptSSD1331->tWriteData.bData = false;
        ptSSD1331->tWriteData.au8Data[0] = 0x15;  /* Set Column Address */
        ptSSD1331->tWriteData.au8Data[1] = (uint8_t)i16X;
        ptSSD1331->tWriteData.au8Data[2] = (uint8_t)i16X;
        ptSSD1331->tWriteData.au8Data[3] = 0x75;  /* Set Row Address */
        ptSSD1331->tWriteData.au8Data[4] = (uint8_t)i16Y;
        ptSSD1331->tWriteData.au8Data[5] = (uint8_t)i16Y;
        Lock(ptSSD1331);
        ptSSD1331->tWrite(ptSSD1331->pInstance, &ptSSD1331->tWriteData);
        Unlock(ptSSD1331);

        ptSSD1331->tWriteData.u8Count = 2;
        ptSSD1331->tWriteData.bData = true;
        u16UColor = ptSSD1331->bInvert ? ~u16Color : u16Color;
        ptSSD1331->tWriteData.au8Data[0] = ((u16UColor >> 8) & 0xFF);
        ptSSD1331->tWriteData.au8Data[1] = ( u16UColor       & 0xFF);
        Lock(ptSSD1331);
        ptSSD1331->tWrite(ptSSD1331->pInstance, &ptSSD1331->tWriteData);
        Unlock(ptSSD1331);
      }
    }
  }

  return bValid;
}

bool SSD1331_DrawLine(
  SSD1331_t* ptSSD1331,
  const int16_t i16X1,
  const int16_t i16Y1,
  const int16_t i16X2,
  const int16_t i16Y2,
  const uint16_t u16Color
) {
  bool bValid = false;
  int16_t i16UX1, i16UX2, i16UY1, i16UY2;
  uint8_t u8TX1, u8TX2, u8TY1, u8TY2;
  double dA;
  int16_t i16B;
  int16_t i16Temp;
  uint16_t u16UColor;
  uint8_t u8Touch = 0;

  if(ptSSD1331) {
    if(ptSSD1331->tWrite) {
      bValid = true;
      if(
        (i16X1 >= 0 && i16X1 < ptSSD1331->u8MaxX) &&
        (i16X2 >= 0 && i16X2 < ptSSD1331->u8MaxX) &&
        (i16Y1 >= 0 && i16Y1 < ptSSD1331->u8MaxY) &&
        (i16Y2 >= 0 && i16Y2 < ptSSD1331->u8MaxY)
      ) {
        u8TX1 = (uint8_t)i16X1;
        u8TX2 = (uint8_t)i16X2;
        u8TY1 = (uint8_t)i16Y1;
        u8TY2 = (uint8_t)i16Y2;
        u8Touch = 1;
      } else {
        if(i16X1 <= i16X2) {
          i16UX1 = i16X1;
          i16UX2 = i16X2;
          i16UY1 = i16Y1;
          i16UY2 = i16Y2;
        } else {
          i16UX1 = i16X2;
          i16UX2 = i16X1;
          i16UY1 = i16Y2;
          i16UY2 = i16Y1;
        }
        if(i16UX1 == i16UX2) {
          if(i16UX1 >= 0 && i16UX1 < ptSSD1331->u8MaxX) {
            if(i16UY1 < 0) {
              i16UY1 = 0;
            }
            if(i16UY1 >= ptSSD1331->u8MaxY) {
              i16UY1 = ptSSD1331->u8MaxY - 1;
            }
            if(i16UY2 < 0) {
              i16UY2 = 0;
            }
            if(i16UY2 >= ptSSD1331->u8MaxY) {
              i16UY2 = ptSSD1331->u8MaxY - 1;
            }
            u8TX1 = (uint8_t)i16UX1;
            u8TX2 = (uint8_t)i16UX2;
            u8TY1 = (uint8_t)i16UY1;
            u8TY2 = (uint8_t)i16UY2;
            u8Touch = 1;
          }
        } else if(i16UY1 == i16UY2) {
          if(i16UY1 >= 0 && i16UY1 < ptSSD1331->u8MaxY) {
            if(i16UX1 < 0) {
              i16UX1 = 0;
            }
            if(i16UX1 >= ptSSD1331->u8MaxX) {
              i16UX1 = ptSSD1331->u8MaxX - 1;
            }
            if(i16UX2 < 0) {
              i16UX2 = 0;
            }
            if(i16UX2 >= ptSSD1331->u8MaxX) {
              i16UX2 = ptSSD1331->u8MaxX - 1;
            }
            u8TX1 = (uint8_t)i16UX1;
            u8TX2 = (uint8_t)i16UX2;
            u8TY1 = (uint8_t)i16UY1;
            u8TY2 = (uint8_t)i16UY2;
            u8Touch = 1;
          }
        } else {
          dA = ((double)i16UY2 - i16UY1) / (i16UX2 - i16UX1);
          i16B = ((i16UY1 * 2) + 1) - (int16_t)(dA * ((i16UX1 * 2) + 1));

          i16Temp = dA + i16B;
          if(i16Temp >= 0 && i16Temp < (ptSSD1331->u8MaxY * 2)) {
            if(!u8Touch) {
              u8TX1 = 0;
              u8TY1 = (uint8_t)(i16Temp / 2);
            } else {
              u8TX2 = 0;
              u8TY2 = (uint8_t)(i16Temp / 2);
            }
            u8Touch |= 1 << 0;
          }
          i16Temp = (int16_t)(dA * ((ptSSD1331->u8MaxX * 2) - 1)) + i16B;
          if(i16Temp >= 0 && i16Temp < (ptSSD1331->u8MaxY * 2)) {
            if(!u8Touch) {
              u8TX1 = ptSSD1331->u8MaxX - 1;
              u8TY1 = (uint8_t)(i16Temp / 2);
            } else {
              u8TX2 = ptSSD1331->u8MaxX - 1;
              u8TY2 = (uint8_t)(i16Temp / 2);
            }
            u8Touch |= 1 << 1;
          }
          i16Temp = (int16_t)((1 - i16B) / dA);
          if(i16Temp >= 0 && i16Temp < (ptSSD1331->u8MaxX * 2)) {
            if(!u8Touch) {
              u8TX1 = (uint8_t)(i16Temp / 2);
              u8TY1 = 0;
            } else {
              u8TX2 = (uint8_t)(i16Temp / 2);
              u8TY2 = 0;
            }
            u8Touch |= 1 << 2;
          }
          i16Temp = (int16_t)((((ptSSD1331->u8MaxY * 2) - 1) - i16B) / dA);
          if(i16Temp >= 0 && i16Temp < (ptSSD1331->u8MaxX * 2)) {
            if(!u8Touch) {
              u8TX1 = (uint8_t)(i16Temp / 2);
              u8TY1 = ptSSD1331->u8MaxY - 1;
            } else {
              u8TX2 = (uint8_t)(i16Temp / 2);
              u8TY2 = ptSSD1331->u8MaxY - 1;
            }
            u8Touch |= 1 << 3;
          }
          if(u8Touch) {
            if(u8TX1 > u8TX2) {
              i16Temp = u8TX2;
              u8TX2 = u8TX1;
              u8TX1 = i16Temp;
              i16Temp = u8TY2;
              u8TY2 = u8TY1;
              u8TY1 = i16Temp;
            }
            if(
              (i16UX1 >= 0 && i16UX1 < ptSSD1331->u8MaxX) &&
              (i16UY1 >= 0 && i16UY1 < ptSSD1331->u8MaxY)
            ) {
              u8TX1 = (uint8_t)i16UX1;
              u8TY1 = (uint8_t)i16UY1;
            } else if(
              (i16UX2 >= 0 && i16UX2 < ptSSD1331->u8MaxX) &&
              (i16UY2 >= 0 && i16UY2 < ptSSD1331->u8MaxY)
            ) {
              u8TX2 = (uint8_t)i16X2;
              u8TY2 = (uint8_t)i16Y2;
            }
          }
        }
      }

      if(u8Touch) {
        ptSSD1331->tWriteData.u8Count = 8;
        ptSSD1331->tWriteData.bData = false;
        ptSSD1331->tWriteData.au8Data[0] = 0x21;  /* Drawing Line */
        ptSSD1331->tWriteData.au8Data[1] = u8TX1;
        ptSSD1331->tWriteData.au8Data[2] = u8TY1;
        ptSSD1331->tWriteData.au8Data[3] = u8TX2;
        ptSSD1331->tWriteData.au8Data[4] = u8TY2;
        u16UColor = ptSSD1331->bInvert ? ~u16Color : u16Color;
        ptSSD1331->tWriteData.au8Data[5] = ((u16UColor >> 10) & 0x3E);
        ptSSD1331->tWriteData.au8Data[6] = ((u16UColor >>  5) & 0x3F);
        ptSSD1331->tWriteData.au8Data[7] = ( u16UColor <<  1  & 0x3E);
        Lock(ptSSD1331);
        ptSSD1331->tWrite(ptSSD1331->pInstance, &ptSSD1331->tWriteData);
        Unlock(ptSSD1331);
      }
    }
  }

  return bValid;
}

bool SSD1331_DrawRectangle(
  SSD1331_t* ptSSD1331,
  const int16_t i16X1,
  const int16_t i16Y1,
  const int16_t i16X2,
  const int16_t i16Y2,
  const uint16_t u16Color,
  const bool bFill,
  const uint16_t u16FillColor
) {
  bool bValid = false;
  int16_t i16UX1, i16UX2, i16UY1, i16UY2;
  uint8_t u8TX1, u8TX2, u8TY1, u8TY2;
  uint16_t u16UColor;
  bool bDraw = false;

  if(ptSSD1331) {
    if(ptSSD1331->tWrite) {
      bValid = true;
      if(
        (i16X1 >= 0 && i16X1 < ptSSD1331->u8MaxX) &&
        (i16Y1 >= 0 && i16Y1 < ptSSD1331->u8MaxY) &&
        (i16X2 >= 0 && i16X2 < ptSSD1331->u8MaxX) &&
        (i16Y2 >= 0 && i16Y2 < ptSSD1331->u8MaxY)
      ) {
        u8TX1 = (uint8_t)i16X1;
        u8TY1 = (uint8_t)i16Y1;
        u8TX2 = (uint8_t)i16X2;
        u8TY2 = (uint8_t)i16Y2;
        bDraw = true;
      } else {
        if(i16X1 <= i16X2) {
          i16UX1 = i16X1;
          i16UX2 = i16X2;
          i16UY1 = i16Y1;
          i16UY2 = i16Y2;
        } else {
          i16UX1 = i16X2;
          i16UX2 = i16X1;
          i16UY1 = i16Y2;
          i16UY2 = i16Y1;
        }
        if(i16UX1 < 0) {
          if(i16UY1 < 0) {
            if(i16UX2 >= 0) {
              if(i16UY2 >= 0) {
                bDraw = true;
                u8TX1 = 0;
                u8TY1 = 0;
                if(i16UX2 < ptSSD1331->u8MaxX) {
                  u8TX2 = (uint8_t)i16X2;
                } else {
                  u8TX2 = ptSSD1331->u8MaxX - 1;
                }
                if(i16UY2 < ptSSD1331->u8MaxY) {
                  u8TY2 = (uint8_t)i16UY2;
                } else {
                  u8TY2 = ptSSD1331->u8MaxY - 1;
                }
              }
            }
          } else {
            if(i16UY1 < ptSSD1331->u8MaxY) {
              if(i16UX2 >= 0) {
                bDraw = true;
                u8TX1 = 0;
                u8TY1 = (uint8_t)i16UY1;
                if(i16UX2 < ptSSD1331->u8MaxX) {
                  u8TX2 = (uint8_t)i16UX2;
                } else {
                  u8TX2 = ptSSD1331->u8MaxX - 1;
                }
                if(i16UY2 >= ptSSD1331->u8MaxY) {
                  u8TY2 = ptSSD1331->u8MaxY - 1;
                } else {
                  if(i16Y2 >= 0) {
                    u8TY2 = (uint8_t)i16UX2;
                  } else {
                    u8TY2 = 0;
                  }
                }
              }
            } else {
              if(i16UX2 >= 0) {
                if(i16UY2 >= ptSSD1331->u8MaxY) {
                  bDraw = true;
                  u8TX1 = 0;
                  u8TY1 = ptSSD1331->u8MaxY - 1;
                  if(i16UX2 < ptSSD1331->u8MaxX) {
                    u8TX2 = (uint8_t)i16UX2;
                  } else {
                    u8TX2 = ptSSD1331->u8MaxX - 1;
                  }
                  if(i16UY2 >= 0) {
                    u8TY2 = (uint8_t)i16UX2;
                  } else {
                    u8TY2 = 0;
                  }
                }
              }
            }
          }
        } else {
          if(i16UX1 < ptSSD1331->u8MaxX) {
            if(i16UY1 < 0) {
              if(i16UY2 >= 0) {
                bDraw = true;
                u8TX1 = (uint8_t)i16UX1;
                u8TY1 = 0;
                if(i16UX2 < ptSSD1331->u8MaxX) {
                  u8TX2 = (uint8_t)i16UY2;
                } else {
                  u8TX2 = ptSSD1331->u8MaxX - 1;
                }
                if(i16UY2 < ptSSD1331->u8MaxY) {
                  u8TY2 = (uint8_t)i16UY2;
                } else {
                  u8TY2 = ptSSD1331->u8MaxY - 1;
                }
              }
            } else {
              if(i16Y1 < ptSSD1331->u8MaxY) {
                bDraw = true;
                u8TX1 = (uint8_t)i16UX1;
                u8TY1 = (uint8_t)i16UY1;
                if(i16UX2 < ptSSD1331->u8MaxX) {
                  u8TX2 = (uint8_t)i16UY2;
                } else {
                  u8TX2 = ptSSD1331->u8MaxX - 1;
                }
                if(i16UY2 < ptSSD1331->u8MaxY) {
                  if(i16UY2 < 0) {
                    u8TY2 = 0;
                  } else {
                    u8TY2 = (uint8_t)i16UY2;
                  }
                } else {
                  u8TY2 = ptSSD1331->u8MaxY - 1;
                }
              } else {
                if(i16UY2 < ptSSD1331->u8MaxY) {
                  bDraw = true;
                  u8TX1 = (uint8_t)i16UX1;
                  u8TY1 = ptSSD1331->u8MaxY - 1;
                  if(i16UX2 < ptSSD1331->u8MaxX) {
                    u8TX2 = (uint8_t)i16UY2;
                  } else {
                    u8TX2 = ptSSD1331->u8MaxX - 1;
                  }
                  if(i16UY2 < 0) {
                    u8TY2 = 0;
                  } else {
                    u8TY2 = (uint8_t)i16UY2;
                  }
                }
              }
            }
          }
        }
      }

      if(bDraw) {
        if(bFill) {
          ptSSD1331->tWriteData.u8Count = 13;
          ptSSD1331->tWriteData.bData = false;
          ptSSD1331->tWriteData.au8Data[ 0] = 0x26;        /* Fill Enable or Disable */
          ptSSD1331->tWriteData.au8Data[ 1] = 0b00000001;  /* A0=1 Fill Enable */
          ptSSD1331->tWriteData.au8Data[ 2] = 0x22;        /* Drawing Rectangle */
          ptSSD1331->tWriteData.au8Data[ 3] = u8TX1;
          ptSSD1331->tWriteData.au8Data[ 4] = u8TY1;
          ptSSD1331->tWriteData.au8Data[ 5] = u8TX2;
          ptSSD1331->tWriteData.au8Data[ 6] = u8TY2;
          u16UColor = ptSSD1331->bInvert ? ~u16Color : u16Color;
          ptSSD1331->tWriteData.au8Data[ 7] = ((u16UColor >> 10) & 0x3E);
          ptSSD1331->tWriteData.au8Data[ 8] = ((u16UColor >>  5) & 0x3F);
          ptSSD1331->tWriteData.au8Data[ 9] = ( u16UColor <<  1  & 0x3E);
          u16UColor = ptSSD1331->bInvert ? ~u16FillColor : u16FillColor;
          ptSSD1331->tWriteData.au8Data[10] = ((u16UColor >> 10) & 0x3E);
          ptSSD1331->tWriteData.au8Data[11] = ((u16UColor >>  5) & 0x3F);
          ptSSD1331->tWriteData.au8Data[12] = ( u16UColor <<  1  & 0x3E);
          Lock(ptSSD1331);
          ptSSD1331->tWrite(ptSSD1331->pInstance, &ptSSD1331->tWriteData);
          Unlock(ptSSD1331);
        } else {
          ptSSD1331->tWriteData.u8Count = 13;
          ptSSD1331->tWriteData.bData = false;
          ptSSD1331->tWriteData.au8Data[ 0] = 0x26;        /* Fill Enable or Disable */
          ptSSD1331->tWriteData.au8Data[ 1] = 0b00000000;  /* A0=0 Fill Disable */
          ptSSD1331->tWriteData.au8Data[ 2] = 0x22;        /* Drawing Rectangle */
          ptSSD1331->tWriteData.au8Data[ 3] = u8TX1;
          ptSSD1331->tWriteData.au8Data[ 4] = u8TY1;
          ptSSD1331->tWriteData.au8Data[ 5] = u8TX2;
          ptSSD1331->tWriteData.au8Data[ 6] = u8TY2;
          u16UColor = ptSSD1331->bInvert ? ~u16Color : u16Color;
          ptSSD1331->tWriteData.au8Data[ 7] = ((u16UColor >> 10) & 0x3E);
          ptSSD1331->tWriteData.au8Data[ 8] = ((u16UColor >>  5) & 0x3F);
          ptSSD1331->tWriteData.au8Data[ 9] = ( u16UColor <<  1  & 0x3E);
          ptSSD1331->tWriteData.au8Data[10] = ((u16UColor >> 10) & 0x3E);
          ptSSD1331->tWriteData.au8Data[11] = ((u16UColor >>  5) & 0x3F);
          ptSSD1331->tWriteData.au8Data[12] = ( u16UColor <<  1  & 0x3E);
          Lock(ptSSD1331);
          ptSSD1331->tWrite(ptSSD1331->pInstance, &ptSSD1331->tWriteData);
          Unlock(ptSSD1331);
        }
      }
    }
  }

  return bValid;
}

bool SSD1331_DrawCircle(
  SSD1331_t* ptSSD1331,
  const int16_t i16X,
  const int16_t i16Y,
  const int16_t i16R,
  const uint16_t u16Color,
  const bool bFill,
  const uint16_t u16FillColor
) {
  bool bValid = false;
  int16_t i16UX, i16UY, i16F;

  if(ptSSD1331 && i16R > 0) {
    if(ptSSD1331->tWrite) {
      bValid = true;
      if(
        (i16X >= -1 * i16R && i16X < ptSSD1331->u8MaxX + i16R) &&
        (i16Y >= -1 * i16R && i16Y < ptSSD1331->u8MaxY + i16R)
      ) {
        if(bFill) {
          i16UX = i16R;
          i16UY = 0;
          i16F = -2 * i16R + 3;
          while(i16UX >= i16UY) {
            SSD1331_DrawLine(ptSSD1331, i16X + i16UX, i16Y + i16UY, i16X - i16UX, i16Y + i16UY, u16FillColor);
            SSD1331_DrawLine(ptSSD1331, i16X + i16UX, i16Y - i16UY, i16X - i16UX, i16Y - i16UY, u16FillColor);
            SSD1331_DrawLine(ptSSD1331, i16X + i16UY, i16Y + i16UX, i16X - i16UY, i16Y + i16UX, u16FillColor);
            SSD1331_DrawLine(ptSSD1331, i16X + i16UY, i16Y - i16UX, i16X - i16UY, i16Y - i16UX, u16FillColor);
            if(i16F >= 0) {
              i16UX--;
              i16F -= 4 * i16UX;
            }
            i16UY++;
            i16F += 4 * i16UY + 2;
          }
        }

        i16UX = i16R;
        i16UY = 0;
        i16F = -2 * i16R + 3;
        while(i16UX >= i16UY) {
          SSD1331_DrawPixel(ptSSD1331, i16X + i16UX, i16Y + i16UY, u16Color);
          SSD1331_DrawPixel(ptSSD1331, i16X - i16UX, i16Y + i16UY, u16Color);
          SSD1331_DrawPixel(ptSSD1331, i16X + i16UX, i16Y - i16UY, u16Color);
          SSD1331_DrawPixel(ptSSD1331, i16X - i16UX, i16Y - i16UY, u16Color);
          SSD1331_DrawPixel(ptSSD1331, i16X + i16UY, i16Y + i16UX, u16Color);
          SSD1331_DrawPixel(ptSSD1331, i16X - i16UY, i16Y + i16UX, u16Color);
          SSD1331_DrawPixel(ptSSD1331, i16X + i16UY, i16Y - i16UX, u16Color);
          SSD1331_DrawPixel(ptSSD1331, i16X - i16UY, i16Y - i16UX, u16Color);
          if(i16F >= 0) {
            i16UX--;
            i16F -= 4 * i16UX;
          }
          i16UY++;
          i16F += 4 * i16UY + 2;
        }
      }
    }
  }

  return bValid;
}

bool SSD1331_Scroll(SSD1331_t* ptSSD1331, const uint8_t u8X, const uint8_t u8Y, const uint8_t u8Interval) {
  bool bValid = false;

  if(ptSSD1331 && (u8X > 0 || u8Y > 0)) {
    if(ptSSD1331->tWrite) {
      bValid = true;
      ptSSD1331->tWriteData.u8Count = 7;
      ptSSD1331->tWriteData.bData = false;
      ptSSD1331->tWriteData.au8Data[0] = 0x27;               /* Continuous scroll */
      ptSSD1331->tWriteData.au8Data[1] = u8X;                /* Horizontal scroll */
      ptSSD1331->tWriteData.au8Data[2] = 0;                  /* Define row as start row address */
      ptSSD1331->tWriteData.au8Data[3] = ptSSD1331->u8MaxY;  /* Scrolling rows */
      ptSSD1331->tWriteData.au8Data[4] = u8Y;                /* vertical scrolling offset */
      ptSSD1331->tWriteData.au8Data[5] = u8Interval;         /* time interval */
      ptSSD1331->tWriteData.au8Data[6] = 0x2F;               /* Activate scrolling */
      Lock(ptSSD1331);
      ptSSD1331->tWrite(ptSSD1331->pInstance, &ptSSD1331->tWriteData);
      Unlock(ptSSD1331);
    }
  }

  return bValid;
}

bool SSD1331_DisableScroll(SSD1331_t* ptSSD1331) {
  bool bValid = false;

  if(ptSSD1331) {
    if(ptSSD1331->tWrite) {
      bValid = true;
      if(ptSSD1331->bScroll) {
        ptSSD1331->tWriteData.u8Count = 1;
        ptSSD1331->tWriteData.bData = false;
        ptSSD1331->tWriteData.au8Data[0] = 0x2E;
        Lock(ptSSD1331);
        ptSSD1331->tWrite(ptSSD1331->pInstance, &ptSSD1331->tWriteData);
        Unlock(ptSSD1331);
        ptSSD1331->bScroll = false;
      }
    }
  }

  return bValid;
}

bool SSD1331_Sleep(SSD1331_t* ptSSD1331) {
  bool bValid = false;

  if(ptSSD1331) {
    if(ptSSD1331->tWrite) {
      bValid = true;
      if(!ptSSD1331->bSleep) {
        ptSSD1331->tWriteData.u8Count = 1;
        ptSSD1331->tWriteData.bData = false;
        ptSSD1331->tWriteData.au8Data[0] = 0xAE;
        Lock(ptSSD1331);
        ptSSD1331->tWrite(ptSSD1331->pInstance, &ptSSD1331->tWriteData);
        Unlock(ptSSD1331);
        ptSSD1331->bScroll = true;
      }
    }
  }

  return bValid;
}

bool SSD1331_Wakeup(SSD1331_t* ptSSD1331) {
  bool bValid = false;

  if(ptSSD1331) {
    if(ptSSD1331->tWrite) {
      bValid = true;
      if(ptSSD1331->bSleep) {
        ptSSD1331->tWriteData.u8Count = 1;
        ptSSD1331->tWriteData.bData = false;
        ptSSD1331->tWriteData.au8Data[0] = 0xAF;
        Lock(ptSSD1331);
        ptSSD1331->tWrite(ptSSD1331->pInstance, &ptSSD1331->tWriteData);
        Unlock(ptSSD1331);
        ptSSD1331->bScroll = false;
      }
    }
  }

  return bValid;
}
