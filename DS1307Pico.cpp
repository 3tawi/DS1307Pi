
#include "DS1307Pico.h"
#include <Wire.h>
#include <sys/time.h>                   // struct timeval

static uint8_t decToBcd (uint8_t val) { return ((val / 10 * 16) + (val % 10)); }
static uint8_t bcdToDec (uint8_t val) { return ((val / 16 * 10) + (val % 16)); }


static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d) {
    if (y >= 2000)
        y -= 2000;
    uint16_t days = d;
    for (uint8_t i = 1; i < m; i++)
        days += daysInMonth[i - 1];
    if (m > 2 && y % 4 == 0)
        days++;
    return days + 365 * y + (y + 3) / 4 - 1;
}

static inline long time2long(uint16_t days, uint8_t h, uint8_t m, uint8_t s) {
    return ((days * 24L + h) * 60 + m) * 60 + s;
}

static uint8_t dOfW(uint16_t y, uint8_t m, uint8_t d) {
    uint16_t day = date2days(y, m, d);
    return (day + 6) % 7;
}

static uint8_t _read(int dev, uint8_t addr) {
    Wire.beginTransmission(dev);
    Wire.write(addr+0x08);
    Wire.endTransmission();
    Wire.requestFrom(dev, 1);
    uint8_t s = Wire.read();
    return s;
}

static void _write(int dev, uint8_t addr, uint8_t val) {
    Wire.beginTransmission(dev);
    Wire.write(addr+0x08);
    Wire.write(val);
    Wire.endTransmission();
}

uint8_t DS1307Pico::read(uint8_t addr) {
    return _read(DS1307_I2C_ADD, addr);
}

void DS1307Pico::write(uint8_t addr, uint8_t val) {
    _write(DS1307_I2C_ADD, addr, val);
}

long DS1307Pico::getEpoch() {
    uint16_t days = date2days(year, month, dayOfMonth);
    uint32_t t = time2long(days, hour, minute, second);
    t += SECONDS_FROM_1970_TO_2000;
    return t;
}

/*!
    get the time as an Arduino String object with the specified format
	https://www.cplusplus.com/reference/ctime/tm/
	https://www.cplusplus.com/reference/ctime/strftime/
*/

String DS1307Pico::DSgetTime(String format){
  char s[128];
  char c[128];
  format.toCharArray(c, 127);
  struct tm timeinfo = {0};        // Initalize to all 0's
  timeinfo.tm_year = year + 100;    // This is year-1900, so 122 = 2022
  timeinfo.tm_mon = month - 1;
  timeinfo.tm_mday = dayOfMonth;
  timeinfo.tm_hour = hour;
  timeinfo.tm_min = minute;
  timeinfo.tm_sec = second;
  timeinfo.tm_wday = dayOfWeek;
  strftime(s, 127, c, &timeinfo);
  return String(s);
}

void DS1307Pico::begin() {
    Wire.begin();
}

void DS1307Pico::DSread() {
    // Reset the register pointer
    Wire.beginTransmission(DS1307_I2C_ADD);
    Wire.write((uint8_t)0x00);
    Wire.endTransmission();
    Wire.requestFrom(DS1307_I2C_ADD, 7);
    // A few of these need masks because certain bits are control bits
    second     = bcdToDec(Wire.read() & 0x7f);
    minute     = bcdToDec(Wire.read());
    hour       = bcdToDec(Wire.read() & 0x3f);
    dayOfWeek  = bcdToDec(Wire.read());
    dayOfMonth = bcdToDec(Wire.read());
    month      = bcdToDec(Wire.read());
    year       = bcdToDec(Wire.read());
    dayOfWeek %= 7;
}

void DS1307Pico::DSwrite() {
    Wire.beginTransmission(DS1307_I2C_ADD);
    Wire.write((uint8_t)0x00);
    Wire.write(decToBcd(second));
    Wire.write(decToBcd(minute));
    Wire.write(decToBcd(hour));
    Wire.write(decToBcd(dayOfWeek));
    Wire.write(decToBcd(dayOfMonth));
    Wire.write(decToBcd(month));
    Wire.write(decToBcd(year));
    Wire.endTransmission();
}

void DS1307Pico::DSadjust(uint8_t _hour, uint8_t _minute, uint8_t _second, uint16_t _year, uint8_t _month, uint8_t _day) {
    // assign variables
    hour = _hour;
    minute = _minute;
    second = _second;
    year = _year - 2000;
    month = _month;
    dayOfMonth = _day;
    uint16_t day = date2days(year, month, dayOfMonth);
    dayOfWeek = (day + 6) % 7;
    DSwrite();
}

void DS1307Pico::DSadjust(uint32_t t) {
    t -= SECONDS_FROM_1970_TO_2000; // bring to 2000 timestamp from 1970
    second = t % 60;
    t /= 60;
    minute = t % 60;
    t /= 60;
    hour = t % 24;
    uint16_t days = t / 24;
    uint8_t leap;
    for (year = 0; ;year++) {
        leap = year % 4 == 0;
        if (days < 365u + leap)
            break;
        days -= 365 + leap;
    }

    for (month = 1; ; month++) {
        uint8_t daysPerMonth = daysInMonth[month - 1];
        if (leap && month == 2)
            daysPerMonth++;
        if (days < daysPerMonth)
            break;
        days -= daysPerMonth;
    }

    dayOfMonth = days + 1;
    uint16_t day = date2days(year, month, dayOfMonth);
    dayOfWeek = (day + 6) % 7;
    DSwrite();
}