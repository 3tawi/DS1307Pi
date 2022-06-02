// - Lien vidéo : https://youtu.be/9XcizjmSZNY
// - DS1307 Library : https://github.com/3tawi/DS1307Pi
// - Getting Started Raspberry Pi Pico : https://youtu.be/KiM6-Hmxf3s
// - Find All "Great Projects" Videos : https://www.youtube.com/c/GreatProjects

#include <DS1307Pico.h>

DS1307Pico rtc;

uint32_t lastTime; 

void setup() {
  Serial.begin(115200);
// The SDA and SCL lines of the I2C bus come from GP4 and GP5 of the Raspberry Pi Pico board
  rtc.begin();
// rtc.DSadjust(0, 15, 21, 2022, 3, 16); // 00:15:21 16 Mar 2022
// rtc.DSadjust(1647216003); // 00:00:03 14 Mar 2022
// rtc.write(1, 225); // DS1307_RAMSIZE 56 bytes
// rtc.write(3, 133); // DS1307_RAMSIZE 56 bytes
}
void loop() {
   if(millis() - lastTime >= 1000) {
    rtc.DSread(); 
    PrintTime();
    lastTime = millis();
    }
}
void PrintTime()
{ 
  Serial.println(rtc.DSgetTime("%A, %B %d %Y %H:%M:%S"));   // (String) returns time with specified format   
// formating options  https://www.cplusplus.com/reference/ctime/strftime/

// Serial.println(""); 
// Serial.println(rtc.getEpoch());         //  (long)    1647216003
// Serial.println(rtc.second);             //  (int)     30    (0-59)
// Serial.println(rtc.minute);             //  (int)     58    (0-59)
// Serial.println(rtc.hour);               //  (int)     0     (0-23)
// Serial.println(rtc.dayOfMonth);         //  (int)     14    (1-31)
// Serial.println(rtc.month);              //  (int)     3     (1-12)
// Serial.println(rtc.year + 2000);        //  (int)     2022
// Serial.println(rtc.dayOfWeek);          //  (int)     1     (0-6)
// Serial.println(""); 
// Serial.print("read from ds1307 ram add 1: "); 
// Serial.println(rtc.read(1)); 
// Serial.print("read from ds1307 ram add 3: "); 
// Serial.println(rtc.read(3)); 
// Serial.println(""); 
} 
