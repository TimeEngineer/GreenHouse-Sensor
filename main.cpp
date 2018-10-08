#include "mbed.h"
#include "TSL2561.h"
 
Serial PC(USBTX, USBRX);
 
#define PC_PRINTX(z,x)             if(z==1) PC.printf(x);
#define PC_PRINTLNX(z,x)           if(z==1) {PC.printf(x);        PC.printf("\r\n");}
#define PC_PRINTXY(z,x, y)         if(z==1) PC.printf(x, y);
#define PC_PRINTLNXY(z,x, y)       if(z==1) {PC.printf(x, y);     PC.printf("\r\n");}
 
DigitalOut myled(LED1);
TSL2561 tsl2561(TSL2561_ADDR_FLOAT);
 
Timer setuptimer;
Timer executetimer;
 
void setup(void){
 
    if (tsl2561.begin()) {    
        PC_PRINTLNX(1,"TSL2561 Sensor Found");        
    } else {    
        PC_PRINTLNX(1,"TSL2561 Sensor not Found");   
    }
    
    // You can change the gain on the fly, to adapt to brighter/dimmer tsl2561 situations
  tsl2561.setGain(TSL2561_GAIN_0X);         // set no gain (for bright situtations)
  //tsl2561.setGain(TSL2561_GAIN_16X);      // set 16x gain (for dim situations)
  
  // Changing the integration time gives you a longer time over which to sense tsl2561
  // longer timelines are slower, but are good in very low tsl2561 situtations!
  //tsl2561.setTiming(TSL2561_INTEGRATIONTIME_13MS);  // shortest integration time (bright tsl2561)
  //tsl2561.setTiming(TSL2561_INTEGRATIONTIME_101MS);  // medium integration time (medium tsl2561)
  tsl2561.setTiming(TSL2561_INTEGRATIONTIME_402MS);  // longest integration time (dim tsl2561)
  
  // Now we're ready to get readings!
}
 
int main() {  
    
    PC_PRINTLNX(1,"----------START-------------");
    setuptimer.start();
    setup();    
    setuptimer.stop();
    PC_PRINTLNXY(1,"Setup time: %f",setuptimer.read());
    setuptimer.reset();
    
    uint16_t x,y,z;
      
    while(1) {      
    
       
        // Simple data read example. Just read the infrared, fullspecrtrum diode 
        // or 'visible' (difference between the two) channels.
        // This can take 13-402 milliseconds! Uncomment whichever of the following you want to read
        
        executetimer.start();
        x = tsl2561.getLuminosity(TSL2561_VISIBLE);     
        y = tsl2561.getLuminosity(TSL2561_FULLSPECTRUM);
        z = tsl2561.getLuminosity(TSL2561_INFRARED);
        executetimer.stop();
        
        PC_PRINTLNXY(1,"Visible: %d",x);
        PC_PRINTLNXY(1,"Full Spectrum: %d",y);
        PC_PRINTLNXY(1,"Infrared: %d",z);
        PC_PRINTLNXY(1,"Execution Time: %f",executetimer.read());
        executetimer.reset();
        
        //More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
        //That way you can do whatever math and comparisons you want!
         // uint32_t lum = tsl2561.getFullLuminosity();
       // uint16_t ir, full;
      //  ir = lum >> 16;
      //  full = lum & 0xFFFF;
  
     //   PC_PRINTLNXY(1,"Visible: %d",full - ir);
     //   PC_PRINTLNXY(1,"Full Spectrum: %d",full);
     //   PC_PRINTLNXY(1,"Infrared: %d",ir);  
  
    //    wait(1);
     PC_PRINTLNX(1,"----------COMPLETE-------------");
    }
}