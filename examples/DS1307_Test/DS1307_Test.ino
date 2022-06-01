// - Lien vidéo: https://youtu.be/Hndon1lIXL0
// - DS1307 Library: https://github.com/3tawi/DS1307Pico
// - Getting Started Raspberry Pi Pico : https://youtu.be/KiM6-Hmxf3s
// - Find All "Great Projects" Videos : https://www.youtube.com/c/GreatProjects

#include <DS1307Pico.h>

DS1307Pico rtc;

uint32_t lastTime; 

void setup() {
  Serial.begin(115200);  
  rtc.begin();          
// rtc.DSadjust(0, 15, 21, 2022, 6, 1); // 00:19:21 16 Mar 2022
// rtc.DSadjust(1647216003); // 00:00:03 14 Mar 2022
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

      Serial.println(""); 
      Serial.println(rtc.getEpoch());         //  (long)    1647216003
      Serial.println(rtc.second);             //  (int)     30    (0-59)
      Serial.println(rtc.minute);             //  (int)     58    (0-59)
      Serial.println(rtc.hour);               //  (int)     0     (0-23)
      Serial.println(rtc.dayOfMonth);         //  (int)     14    (1-31)
      Serial.println(rtc.month);              //  (int)     3     (1-12)
      Serial.println(rtc.year + 2000);        //  (int)     2022
      Serial.println(rtc.dayOfWeek);          //  (int)     1     (0-6)
      Serial.println(""); 
} 