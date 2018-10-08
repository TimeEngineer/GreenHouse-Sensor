#include "mbed.h"
#include "Adafruit_SSD1306.h"
#include "DHT22.h"

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

/* ------------------------------ ----- ------------------------------ */
/* ------------------------------ DHT22 ------------------------------ */
/* ------------------------------ ----- ------------------------------ */

DHT dht22(A5,DHT::DHT22);

/* ------------------------------ ---- ------------------------------ */
/* ------------------------------ MAIN ------------------------------ */
/* ------------------------------ ---- ------------------------------ */

int main() {
    gOled.printf("%ux%u OLED Display\r\n", gOled.width(), gOled.height());
    while(1)
    {
        wait(1);
        int err = dht22.read();
        if (err == DHT::SUCCESS) { gOled.printf("%.1f %.1f\r", dht22.getTemperature(), dht22.getHumidity()); }
        else { gOled.printf("Error\r"); }
        gOled.display();
        wait(1.0);
    }
}