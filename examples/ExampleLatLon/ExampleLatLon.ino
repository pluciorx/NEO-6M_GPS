#include <neo6mGPS.h>


void setup()
{
  Serial.begin(2000000);
  while (!Serial);

  Serial3.begin(9600);

  //initialize GPS class
  myGPS.begin(Serial3, Serial);
}




void loop()
{
  myGPS.grabData_LatLong();
}



