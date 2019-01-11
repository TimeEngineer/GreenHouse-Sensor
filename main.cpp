#include "mbed.h"
#include "DHT22.h"
#include "TSL2561_I2C.h"
#include "DS1820.h"
#include "TCS3472_I2C.h"
#include "Adafruit_SSD1306.h"
#include "WakeUp.h"

class SPIPreInit : public SPI {
public:
    SPIPreInit(PinName mosi, PinName miso, PinName clk) : SPI(mosi,miso,clk) {
        format(8,3);
        frequency(2000000);
    };
};
SPIPreInit gSpi(D11, NC, D13); // MOSI MISO CLK
Adafruit_SSD1306_Spi gOled(gSpi, D10, D4, D3); // DC RST CS

Serial wisol(USBTX, USBRX); // tx(A2), rx(A7)
DHT dht22(A5,DHT::DHT22); // analog
TSL2561_I2C Lum(D0, D1); // sda, scl
TCS3472_I2C rgbc(D12, A6); // sda, scl
AnalogIn humidite(A1); // analog
DS1820 probe(A0); // analog
DigitalIn flag(D6);

float water = 0.389;
float dry = 0.761;
void initialisation()
{
    Lum.enablePower();
    rgbc.enablePowerAndRGBC();
    probe.unassignedProbe(A0);
}

void send()
{
    if (dht22.read() == DHT::SUCCESS)
    {
        int temperature = int(dht22.getTemperature()*10);
        int humidity = int(dht22.getHumidity()*10);
        
        probe.convertTemperature(true, DS1820::this_device); 
        int g_temperature = int(probe.temperature()*10);
        
        float hvalue = humidite.read();
        if (hvalue > dry) { dry = hvalue; }
        if (hvalue < water) { water = hvalue; } 
        int g_humidity = int((-hvalue + dry)/(dry-water)*1000);
            
        int luminosity = int(Lum.getLux());
            
        int rgbc_datas[4];
        rgbc.getAllColors(rgbc_datas);
        int red = int(rgbc_datas[0]/257.0);
        int green = int(rgbc_datas[1]/257.0);
        int blue = int(rgbc_datas[2]/257.0);
        
        wisol.printf("AT$SF=%06X%02X%02X%02X%03X%03X%03X%03X\r\n", luminosity, red, green, blue, g_temperature, g_humidity, temperature, humidity);
    } 
}
void print() {
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
    int rgbc_datas[4];
    rgbc.getAllColors(rgbc_datas);
    int red = int(rgbc_datas[0]/257.0);
    int green = int(rgbc_datas[1]/257.0);
    int blue = int(rgbc_datas[2]/257.0);
    gOled.printf("R:%d G:%d B:%d x1\r",red, green, blue);
    gOled.display(); 
    wait(2);
    
    gOled.clearDisplay();
    probe.convertTemperature(true, DS1820::this_device); 
    int g_temperature = int(probe.temperature()*10);
    float hvalue = humidite.read();
    if (hvalue > dry) { dry = hvalue; }
    if (hvalue < water) { water = hvalue; } 
    int g_humidity = int((-hvalue + dry)/(dry-water)*1000);
    gOled.printf("T:%d H:%d x10\r",g_temperature, g_humidity);
    gOled.display(); 
    wait(2);
}
void mycallback(void){}
int main()
{
    WakeUp::calibrate();   
    WakeUp::attach(&mycallback);
    gOled.display();
    while(1)
    {
        initialisation();
        wait(2);
        if (flag) {
            print();
        }
        else {
            gOled.clearDisplay();
            gOled.printf("");
            gOled.display();
            send();
            wait(10);
            WakeUp::set_ms(548000);
            deepsleep();
        }
    }
}