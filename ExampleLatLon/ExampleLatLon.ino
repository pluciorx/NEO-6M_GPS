#include <neo6mGPS.h>

//create GPS class
neo6mGPS myGPS;

void setup()
{
  //initialize debugging serial
  Serial.begin(115200);

  while(!Serial);

  //initialize GPS class
  myGPS.begin(Serial3, Serial);
}

void loop()
{  
  //update lat and long
  if(myGPS.grabData_LatLong())
  {
    //print results
    Serial.print("Lat: ");
    Serial.println(myGPS.GPS_data[0], 5);
    Serial.print("Lon: ");
    Serial.println(myGPS.GPS_data[1], 5);
    Serial.println();
  }
  else
  {
    Serial.println("No Lat/Lon Data at this time...");
    Serial.println("Last known position:");
    Serial.print("Lat: ");
    Serial.println(myGPS.GPS_data[0], 5);
    Serial.print("Lon: ");
    Serial.println(myGPS.GPS_data[1], 5);
    Serial.println();
  }

  //optional 10 Hz refresh delay
  delay(100);
}





