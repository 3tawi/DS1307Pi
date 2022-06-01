

#ifndef DS1307PICO_H
#define DS1307PICO_H

#include <Arduino.h>

#define SECONDS_FROM_1970_TO_2000 946684800L
#define DS1307_I2C_ADD   0x68

const uint8_t daysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


class DS1307Pico {
private:
    void DSwrite();
public:
    void begin();
    void write(uint8_t addr, uint8_t val);
    uint8_t read(uint8_t addr);
    void DSread();
    String DSgetTime(String format);
    void DSadjust(uint32_t t);
    void DSadjust(uint8_t _hour, uint8_t _minute, uint8_t _second, uint16_t _year, uint8_t _month, uint8_t _day);

    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t dayOfWeek;// day of week, 1 = Monday
    uint8_t dayOfMonth;
    uint8_t month;
    uint16_t year;

    long getEpoch();
};

#endif
