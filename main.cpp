#include "mbed.h"
#include "DHT22.h"
#include "TSL2561_I2C.h"
#include "DS1820.h"
#include "TCS3472_I2C.h"
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
Serial wisol(USBTX, USBRX); // tx, rx
DHT dht22(A5,DHT::DHT22);
TSL2561_I2C Lum(D12, A6);
TCS3472_I2C rgbc(D0, D1);
AnalogIn humidite(A1);
DS1820 probe(A0);

void init()
{
    Lum.enablePower();
    rgbc.enablePowerAndRGBC();
}

void send()
{
    int err = dht22.read();
    if (err == DHT::SUCCESS)
    {
        wait(1);
        if (dht22.read() == DHT::SUCCESS)
        {
            int temperature = int(dht22.getTemperature()*10);
            int humidity = int(dht22.getHumidity()*10);
            wisol.printf("AT$SF=%03X%03X\r\n", temperature, humidity);
        }
    } 
}

int main()
{
    init();
    int rgbc_datas[4];
    send();
    while(1)
    {
        wait(1);
        gOled.clearDisplay();
        if (dht22.read() == DHT::SUCCESS)
        {
            int temperature = int(dht22.getTemperature()*10);
            int humidity = int(dht22.getHumidity()*10);
            gOled.printf("T:%d H:%d x10\r", temperature, humidity);
        }
        else { gOled.printf("Error\r"); }
        gOled.display();
        wait(2);
        
        gOled.clearDisplay();
        int luminosity = int(Lum.getLux());
        gOled.printf("L:%d x1\r",luminosity);
        gOled.display(); 
        wait(2);
        
        gOled.clearDisplay();
        rgbc.getAllColors(rgbc_datas);
        //float sum = rgbc_datas[3];
        int red = int(rgbc_datas[0]/257.0);
        int green = int(rgbc_datas[1]/257.0);
        int blue = int(rgbc_datas[2]/257.0);
        gOled.printf("R:%d G:%d B:%d x1\r",red, green, blue);
        gOled.display(); 
        wait(2);
        
        gOled.clearDisplay();
        probe.convertTemperature(true, DS1820::all_devices); 
        int g_temperature = int(probe.temperature()*10);
        int g_humidity = int(humidite.read()*1000);
        gOled.printf("T:%d H:%d x10\r",g_temperature, g_humidity);
        gOled.display(); 
        wait(2);
    }
}    