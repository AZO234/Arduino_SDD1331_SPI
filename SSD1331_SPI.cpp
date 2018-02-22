/* SSD1331 SPI Driver for Arduino IDE by AZO */

#include "SSD1331_SPI.h"
#include <SPI.h>

SSD1331_SPI::SSD1331_SPI(const unsigned char ucMaxX, const unsigned char ucMaxY, const unsigned char ucNSS, const unsigned char ucDC, const unsigned char ucRES) {
	this->ucMaxX = ucMaxX;
	this->ucMaxY = ucMaxY;
	this->ucNSS = ucNSS;
	this->ucDC = ucDC;
	this->ucRES = ucRES;
	this->ucSleep = 0;
}

void SSD1331_SPI::CommandWrite(const unsigned char ucData) {
	digitalWrite(this->ucNSS, LOW);
	digitalWrite(this->ucDC, LOW);
	SPI.transfer(ucData);
	digitalWrite(this->ucNSS, HIGH);
}

void SSD1331_SPI::DataWrite(const unsigned char ucData) {
	digitalWrite(this->ucNSS, LOW);
	digitalWrite(this->ucDC, HIGH);
	SPI.transfer(ucData);
	digitalWrite(this->ucNSS, HIGH);
}

void SSD1331_SPI::CommandWriteBytes(const unsigned char* pucData, const unsigned int uiCount) {
	unsigned int i;

	digitalWrite(this->ucNSS, LOW);
	digitalWrite(this->ucDC, LOW);
	for(i = 0; i < uiCount; i++) {
		SPI.transfer(pucData[i]);
	}
	digitalWrite(this->ucNSS, HIGH);
}

void SSD1331_SPI::DataWriteBytes(const unsigned char *pucData, const unsigned int uiCount) {
	unsigned int i;

	digitalWrite(this->ucNSS, LOW);
	digitalWrite(this->ucDC, HIGH);
	for(i = 0; i < uiCount; i++) {
		SPI.transfer(pucData[i]);
	}
	digitalWrite(this->ucNSS, HIGH);
}

void SSD1331_SPI::Initialize(const unsigned char ucBeginSPI) {
	unsigned int i, j;

	pinMode(this->ucNSS, OUTPUT);
	digitalWrite(this->ucNSS, HIGH);
	pinMode(this->ucDC, OUTPUT);

	pinMode(this->ucRES, OUTPUT);
	digitalWrite(this->ucRES, HIGH);
	digitalWrite(this->ucRES, LOW);
	delay(1);
	digitalWrite(this->ucRES, HIGH);

	if(ucBeginSPI){
		SPI.begin();
		SPI.setDataMode(SPI_MODE3);
	}

	this->CommandWrite(0xAE); //Set Display Off
	this->CommandWrite(0xA0); //Remap & Color Depth setting　
		this->CommandWrite(0b01110010); //A[7:6] = 00; 256 color. A[7:6] = 01; 65k color format
	this->CommandWrite(0xA1); //Set Display Start Line
		this->CommandWrite(0);
	this->CommandWrite(0xA2); //Set Display Offset
		this->CommandWrite(0);
	this->CommandWrite(0xA4); //Set Display Mode (Normal)
	this->CommandWrite(0xA8); //Set Multiplex Ratio
		this->CommandWrite(0b00111111); //15-63
	this->CommandWrite(0xAD); //Set Master Configration
		this->CommandWrite(0b10001110); //a[0]=0 Select external Vcc supply, a[0]=1 Reserved(reset)
	this->CommandWrite(0xB0); //Power Save Mode
		this->CommandWrite(0x0B); //0x1A Enable power save mode
	this->CommandWrite(0xB1); //Phase 1 and 2 period adjustment
		this->CommandWrite(0x74);
	this->CommandWrite(0xB3); //Display Clock DIV
		this->CommandWrite(0xF0);
	this->CommandWrite(0x8A); //Pre Charge A
		this->CommandWrite(0x81);
	this->CommandWrite(0x8B); //Pre Charge B
		this->CommandWrite(0x82);
	this->CommandWrite(0x8C); //Pre Charge C
		this->CommandWrite(0x83);
	this->CommandWrite(0xBB); //Set Pre-charge level
		this->CommandWrite(0x3A);
	this->CommandWrite(0xBE); //Set VcomH
		this->CommandWrite(0x3E);
	this->CommandWrite(0x87); //Set Master Current Control
		this->CommandWrite(0x06);
	this->CommandWrite(0x15); //Set Column Address
		this->CommandWrite(0);
		this->CommandWrite(this->ucMaxX - 1);
	this->CommandWrite(0x75); //Set Row Address
		this->CommandWrite(0);
		this->CommandWrite(this->ucMaxY - 1);
	this->CommandWrite(0x81); //Set Contrast for Color A
		this->CommandWrite(255);
	this->CommandWrite(0x82); //Set Contrast for Color B
		this->CommandWrite(255);
	this->CommandWrite(0x83); //Set Contrast for Color C
		this->CommandWrite(255);

	for(j = 0;  j < this->ucMaxY; j++) { //Display Black OUT
		for(i = 0; i < this->ucMaxX; i++) {
			this->DataWrite(0x00);
			this->DataWrite(0x00);
		}
	}

	this->CommandWrite(0xAF); //Set Display On

	delay(100);
}

void SSD1331_SPI::SetBrightness(const unsigned char ucBrightness) {
	this->CommandWrite(0x81); //Set Contrast for Color A
		this->CommandWrite(ucBrightness);
	this->CommandWrite(0x82); //Set Contrast for Color B
		this->CommandWrite(ucBrightness);
	this->CommandWrite(0x83); //Set Contrast for Color C
		this->CommandWrite(ucBrightness);
}

void SSD1331_SPI::Clear(void) {
	this->CommandWrite(0x25); //Clear Window
		this->CommandWrite(0); //Column Address of Start
		this->CommandWrite(0); //Row Address of Start
		this->CommandWrite(this->ucMaxX - 1); //Column Address of End
		this->CommandWrite(this->ucMaxY - 1); //Row Address of End
}

void SSD1331_SPI::DrawPixel(const unsigned int uiX, const unsigned int uiY, const unsigned int uiColor) {
	unsigned char aucCommands[6];
	unsigned char aucDatas[2];

	if(uiX >= this->ucMaxX || uiY >= this->ucMaxY) {
		return;
	}

	aucCommands[0] = 0x15; //Set Column Address
		aucCommands[1] = uiX;
		aucCommands[2] = uiX;
	aucCommands[3] = 0x75; //Set Row Address
		aucCommands[4] = uiY;
		aucCommands[5] = uiY;

	this->CommandWriteBytes(aucCommands, 6);

	aucDatas[0] = (uiColor >> 8) & 0xFF;
	aucDatas[1] =  uiColor       & 0xFF;

	DataWriteBytes(aucDatas, 2);
}

void SSD1331_SPI::DrawLine(const int iX1, const int iY1, const int iX2, const int iY2, const unsigned int uiColor) {
	unsigned char aucCommands[8];
	int iUX1, iUX2, iUY1, iUY2;

	iUX1 = iX1;
	iUX2 = iX2;
	iUY1 = iY1;
	iUY2 = iY2;
	if(iX1 > iX2) {
		iUX1 = iX2;
		iUX2 = iX1;
	}
	if(iY1 > iY2) {
		iUY1 = iY2;
		iUY2 = iY1;
	}

	if(iUX1 < 0) {
		iUX1 = 0;
	}
	if(iUY2 < 0) {
		iUY2 = 0;
	}
	if(iUX2 >= this->ucMaxX) {
		iUX2 = this->ucMaxX - 1;
	}
	if(iUY2 >= this->ucMaxY) {
		iUY2 = this->ucMaxY - 1;
	}

	aucCommands[0] = 0x21; //Drawing Line
		aucCommands[1] = iUX1;
		aucCommands[2] = iUY1;
		aucCommands[3] = iUX2;
		aucCommands[4] = iUY2;
		aucCommands[5] = (uiColor >> 11) & 0x1F;
		aucCommands[6] = (uiColor >>  5) & 0x3F;
		aucCommands[7] =  uiColor        & 0x1F;

	this->CommandWriteBytes(aucCommands, 8);
}

void SSD1331_SPI::DrawRectangle(const unsigned int uiX1, const unsigned int uiY1, const unsigned int uiX2, const unsigned int uiY2, const unsigned int uiColor) {
 	unsigned char aucCommands[13];
	unsigned int uiUX1, uiUX2, uiUY1, uiUY2;

	uiUX1 = uiX1;
	uiUX2 = uiX2;
	uiUY1 = uiY1;
	uiUY2 = uiY2;
	if(uiX1 > uiX2) {
		uiUX1 = uiX2;
		uiUX2 = uiX1;
	}
	if(uiY1 > uiY2) {
		uiUY1 = uiY2;
		uiUY2 = uiY1;
	}

	if(uiUX1 >= this->ucMaxX || uiUY1 >= this->ucMaxY) {
		return;
	}

	if(uiUX2 >= this->ucMaxX) {
		uiUX2 = this->ucMaxX - 1;
	}
	if(uiUY2 >= this->ucMaxY) {
		uiUY2 = this->ucMaxY - 1;
	}

	aucCommands[0] = 0x26; //Fill Enable or Disable
		aucCommands[1] = 0b00000000; //A0=0 Fill Enable
	aucCommands[2] = 0x22; //Drawing Rectangle
		aucCommands[3] = uiUX1;
		aucCommands[4] = uiUY1;
		aucCommands[5] = uiUX2;
		aucCommands[6] = uiUY2;
		aucCommands[7] = (uiColor >> 11) & 0x1F;
		aucCommands[8] = (uiColor >>  5) & 0x3F;
		aucCommands[9] =  uiColor        & 0x1F;
		aucCommands[10] = 0;
		aucCommands[11] = 0;
		aucCommands[12] = 0;

	this->CommandWriteBytes(aucCommands, 13);
}

void SSD1331_SPI::DrawRectangleFill(const unsigned int uiX1, const unsigned int uiY1, const unsigned int uiX2, const unsigned int uiY2, const unsigned int uiLineColor, const unsigned int uiFillColor) {
	unsigned char aucCommands[13];
	unsigned int uiUX1, uiUX2, uiUY1, uiUY2;

	uiUX1 = uiX1;
	uiUX2 = uiX2;
	uiUY1 = uiY1;
	uiUY2 = uiY2;
	if(uiX1 > uiX2) {
		uiUX1 = uiX2;
		uiUX2 = uiX1;
	}
	if(uiY1 > uiY2) {
		uiUY1 = uiY2;
		uiUY2 = uiY1;
	}

	if(uiUX1 >= this->ucMaxX || uiUY1 >= this->ucMaxY) {
		return;
	}

	if(uiUX2 >= this->ucMaxX) {
		uiUX2 = this->ucMaxX - 1;
	}
	if(uiUY2 >= this->ucMaxY) {
		uiUY2 = this->ucMaxY - 1;
	}

	aucCommands[0] = 0x26; //Fill Enable or Disable
		aucCommands[1] = 0b00000001; //A0=0 Fill Enable
	aucCommands[2] = 0x22; //Drawing Rectangle
		aucCommands[3] = uiUX1;
		aucCommands[4] = uiUY1;
		aucCommands[5] = uiUX2;
		aucCommands[6] = uiUY2;
		aucCommands[7] = (uiLineColor >> 11) & 0x1F;
		aucCommands[8] = (uiLineColor >>  5) & 0x3F;
		aucCommands[9] =  uiLineColor        & 0x1F;
		aucCommands[10] = (uiFillColor >> 11) & 0x1F;
		aucCommands[11] = (uiFillColor >>  5) & 0x3F;
		aucCommands[12] =  uiFillColor        & 0x1F;

	this->CommandWriteBytes(aucCommands, 13);
}

void SSD1331_SPI::DrawCircle(const unsigned int uiX, const unsigned int uiY, const unsigned int uiR, const unsigned int uiColor) {
	int iX = uiR;
	int iY = 0;
	int iF = -2 * uiR + 3;

	while(iX >= iY) {
		this->DrawPixel(uiX + iX, uiY + iY, uiColor);
		this->DrawPixel(uiX - iX, uiY + iY, uiColor);
		this->DrawPixel(uiX + iX, uiY - iY, uiColor);
		this->DrawPixel(uiX - iX, uiY - iY, uiColor);
		this->DrawPixel(uiX + iY, uiY + iX, uiColor);
		this->DrawPixel(uiX - iY, uiY + iX, uiColor);
		this->DrawPixel(uiX + iY, uiY - iX, uiColor);
		this->DrawPixel(uiX - iY, uiY - iX, uiColor);
		if(iF >= 0) {
			iX--;
			iF -= 4 * iX;
		}
		iY++;
		iF += 4 * iY + 2;
	}
}

void SSD1331_SPI::DrawCircleFill(const unsigned int uiX, const unsigned int uiY, const unsigned int uiR, const unsigned int uiLineColor, const unsigned int uiFillColor) {
	int iX = uiR;
	int iY = 0;
	int iF = -2 * uiR + 3;

	while(iX >= iY) {
		this->DrawPixel(uiX + iX, uiY + iY, uiLineColor);
		this->DrawPixel(uiX - iX, uiY + iY, uiLineColor);
		this->DrawLine(uiX - iX + 1, uiY + iY, uiX + iX - 1, uiY + iY, uiFillColor);
		this->DrawPixel(uiX + iX, uiY - iY, uiLineColor);
		this->DrawPixel(uiX - iX, uiY - iY, uiLineColor);
		this->DrawLine(uiX - iX + 1, uiY - iY, uiX + iX - 1, uiY - iY, uiFillColor);
		this->DrawPixel(uiX + iY, uiY + iX, uiLineColor);
		this->DrawPixel(uiX - iY, uiY + iX, uiLineColor);
		if(iX < uiR) {
			this->DrawLine(uiX + iY - 1, uiY + iX, uiX - iY + 1, uiY + iX, uiFillColor);
		}
		this->DrawPixel(uiX + iY, uiY - iX, uiLineColor);
		this->DrawPixel(uiX - iY, uiY - iX, uiLineColor);
		if(iX < uiR) {
			this->DrawLine(uiX + iY - 1, uiY - iX, uiX - iY + 1, uiY - iX, uiFillColor);
		}
		if(iF >= 0) {
			iX--;
			iF -= 4 * iX;
		}
		iY++;
		iF += 4 * iY + 2;
	}
}

void SSD1331_SPI::ActivateScroll(const unsigned int uiX, const unsigned int uiY, const unsigned int uiInterval) {
	unsigned char aucCommands[7];

	aucCommands[0] = 0x27; //Continuous scroll
    aucCommands[1] = uiX; //Horizontal scroll
		aucCommands[2] = 0; //Define row 0 as start row address
		aucCommands[3] = this->ucMaxY; //Scrolling rows
		aucCommands[4] = uiY; // Set vertical scrolling offset
    aucCommands[5] = uiInterval; // Set time interval
	aucCommands[6] = 0x2F; // Activate scrolling;

	this->CommandWriteBytes(aucCommands, 7);
}

void SSD1331_SPI::DeactivateScroll(void) {
	this->CommandWrite(0x2E);
}

unsigned char SSD1331_SPI::IsSleep(void) {
	return this->ucSleep;
}

void SSD1331_SPI::Sleep(void) {
	if(this->IsSleep()) {
		return;
	}

	this->CommandWrite(0xAE); //Display Off 0xAE

	this->ucSleep = 1;
}

void SSD1331_SPI::Wakeup(void) {
	if(!this->IsSleep()) {
		return;
	}

	this->CommandWrite(0xAF); //Display On 0xAF

	this->ucSleep = 0;
}

