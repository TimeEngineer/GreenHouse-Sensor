#include "mbed.h"
#include "TSL2561_I2C.h"
#include "Adafruit_SSD1306.h"


/* ------------------------------ -------- ------------------------------ */
/* ------------------------------ Adafruit ------------------------------ */
/* ------------------------------ -------- ------------------------------ */

class SPIPreInit : public SPI {
public:
    SPIPreInit(PinName mosi, PinName miso, PinName clk) : SPI(mosi,miso,clk) {
        format(8,3);
        frequency(2000000);
    };
};

SPIPreInit gSpi(D11, NC, D13);
Adafruit_SSD1306_Spi gOled(gSpi, D10, D4, D3);


/* ------------------------------ ---- ------------------------------ */
/* ------------------------------ TSL2561 ------------------------------ */
/* ------------------------------ ---- ------------------------------ */



int main()
{   
    gOled.clearDisplay();
    TSL2561_I2C Lum( D12, A6 );
    Lum.enablePower();
    while(1)
    {
     wait(1);
     gOled.printf("Lum:%4.2f\r\n",Lum.getLux());
     gOled.display(); 
     wait(1.0);

    }
}
         