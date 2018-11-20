#ifndef _DHT22_H_
#define _DHT22_H_

#include "mbed.h"

class DHT {
public:
    enum Family {
        DHT11,
        DHT22
    };
    enum Status {
        SUCCESS,
        ERROR_BUS_BUSY,
        ERROR_NOT_DETECTED,
        ERROR_BAD_START,
        ERROR_SYNC_TIMEOUT,
        ERROR_DATA_TIMEOUT,
        ERROR_BAD_CHECKSUM,
        ERROR_TOO_FAST,
    };
    enum Unit {
        CELCIUS,
    };
    DHT(PinName pin, Family DHTtype);
    ~DHT();
    int read(void);
    int* getRawData();
    float getHumidity(void);
    float getTemperature(Unit unit = CELCIUS);
private:
    PinName _pin;
    Family _family;
    time_t  _lastReadTime;
    float _lastTemperature;
    float _lastHumidity;
    int _data[5];
    float calcTemperature();
    float calcHumidity();
};

#endif