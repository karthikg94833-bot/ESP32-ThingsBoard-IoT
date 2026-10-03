#include <Arduino.h>
#include <DHT.h>
#include "State.h"
#include "Peripherals.h"
#include "config.h"


DHT   dht(DHT_PIN , DHT_TYPE);
float mapFloat(long x, long inMin, long inMax, float outMin, float outMax) {
    return (x - inMin) * (outMax - outMin) / (float)(inMax - inMin) + outMin;
}
void sample_sensor(void)
{
   int raw_current = analogRead(CURRENT_PIN); // 0 to 4095
    int raw_voltage = analogRead(VOLTAGE_PIN); // 0 to 4095
    //to read temperature 



    voltage=mapFloat(raw_voltage,0,4095,0,250);

    if(bayStatus == "CHARGING"){
    current=mapFloat(raw_current,0,4095,0,250);
    }
else{
   current=0;
}

    power=voltage*current;
float t = dht.readTemperature();
if(!isnan(t)) temperature=t;

     }


float recentAvgcurrent(void)
{
   float sum=0;
   for(int i=0;i<5;i++)
{
   sum=sum+current;

}
return sum/5;
}
bool plugin_flag_once = 1;
bool plugout_flag_once = 1;
void plug_status(void)
{
   bool pluginReading = digitalRead(BTN_PLUGIN );
   if(pluginReading == LOW && plugin_flag_once){
 sessionStartMs=millis();


   plugin_flag_once = 0;
      if(bayStatus == "FREE"){
      bayStatus ="CHARGING";
      Serial.println("Bay Status is changed to Charging");
      digitalWrite(RELAY_PIN,HIGH);

      }
   }
   if(pluginReading==HIGH)
   {
      plugin_flag_once=1;
   }
   bool plugoutReading = digitalRead(BTN_PLUGOUT);
   if(plugoutReading == LOW && plugout_flag_once){
   plugout_flag_once = 0;
      if(bayStatus == "CHARGING"){
      bayStatus ="FREE";
      Serial.println("Bay Status is changed to FREE");
      digitalWrite(RELAY_PIN,LOW);
      }
   }
   if(plugoutReading==HIGH)
   {
      plugout_flag_once=1;
   }
}
/*void update_led_Status(void){
   if(bayStatus =="FREE"){
      digitalWrite(LED_GREEN,HIGH);
      digitalWrite(LED_YELLOW,LOW);
   }
else{
    digitalWrite(LED_GREEN,LOW);
      digitalWrite(LED_YELLOW,HIGH);
}
}*/