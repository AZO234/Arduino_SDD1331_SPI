#ifndef SSD1331_SPI_H
#define SSD1331_SPI_H

/* SSD1331 SPI Driver for Arduino IDE by AZO */

#define SSD1331_SPI_RGBTOCOLOR(ucR, ucG, ucB)	((((unsigned int)(ucR) & 0x1F) << 11) | (((unsigned int)(ucG) & 0x3F) << 5) | (unsigned int)(ucB) & 0x1F))

class SSD1331_SPI {
	public:
		SSD1331_SPI(const unsigned char ucMaxX, const unsigned char ucMaxY, const unsigned char ucNSS, const unsigned char ucDC, const unsigned char ucRES);
		unsigned char GetMaxX(void);
		unsigned char GetMaxY(void);

		void Initialize(const unsigned char ucBeginSPI);
		void SetBrightness(const unsigned char ucBrightness);
		void Clear(void);
		void DrawPixel(const unsigned int uiX, const unsigned int uiY, const unsigned int uiColor);
		void DrawLine(const unsigned int uiX1, const unsigned int uiY1, const unsigned int uiX2, const unsigned int uiY2, const unsigned int uiColor);
		void DrawRectangle(const unsigned int uiX1, const unsigned int uiY1, const unsigned int uiX2, const unsigned int uiY2, const unsigned int uiColor);
		void DrawRectangleFill(const unsigned int uiX1, const unsigned int uiY1, const unsigned int uiX2, const unsigned int uiY2, const unsigned int uiLineColor, const unsigned int uiFillColor);
		void ActivateScroll(const unsigned int uiX, const unsigned int uiY, const unsigned int uiInterval);
		void DeactivateScroll(void);
	
		unsigned char IsSleep(void);
		void Sleep(void);
		void Wakeup(void);

	private:
		unsigned char ucMaxX;
		unsigned char ucMaxY;
		unsigned char ucNSS;
		unsigned char ucDC;
		unsigned char ucRES;
		unsigned char ucSleep;

		void CommandWrite(const unsigned char ucData);
		void DataWrite(const unsigned char ucData);
		void CommandWriteBytes(const unsigned char* pucData, const unsigned int uiCount);
		void DataWriteBytes(const unsigned char* pucData, const unsigned int uiCount);
};

#endif	/* SSD1331_SPI_H */

