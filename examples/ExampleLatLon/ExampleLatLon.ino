#include "neo6mGPS.h"




neo6mGPS myGPS;




void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
  Serial.begin(115200);
  while(!Serial);
  
  myGPS.begin(Serial1);
}




void loop()
{
  if(myGPS.available())
  {
    Serial.print(myGPS.utc);       Serial.print(" | ");
    Serial.print(myGPS.navStatus); Serial.print(" | ");
    Serial.print(myGPS.lat);       Serial.print(" | ");
    Serial.print(myGPS.latDir);    Serial.print(" | ");
    Serial.print(myGPS.lon);       Serial.print(" | ");
    Serial.print(myGPS.lonDir);    Serial.print(" | ");
    Serial.print(myGPS.sog_knots); Serial.print(" | ");
    Serial.print(myGPS.cog_true);  Serial.print(" | ");
    Serial.println();
  }
}



