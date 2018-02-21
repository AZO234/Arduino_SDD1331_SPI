#include <SSD1331_SPI.h>

class SSD1331_SPI oMyDisplay(96, 64, 10, 8, 9);

void setup() {
  // put your setup code here, to run once:
  oMyDisplay.Initialize(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  oMyDisplay.DrawPixel(random(96), random(64), SSD1331_SPI_RGBTOCOLOR(random(32), random(64), random(32));
}
