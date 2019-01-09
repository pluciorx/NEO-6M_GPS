#include <neo6mGPS.h>


void setup()
{
  	Serial.begin(115200);
  	while (!Serial);

  	Serial3.begin(9600);

  	//initialize GPS class
  	myGPS.begin(Serial3, Serial);
}




void loop()
{
  	myGPS.grabData_LatLong();
  
  	//debugging prints
	Serial.print("UTC HOUR: "); Serial.println(myGPS.GPS_data[UTC_HOUR_POS]);
	Serial.print("UTC MIN: "); Serial.println(myGPS.GPS_data[UTC_MINUTE_POS]);
	Serial.print("UTC SEC: "); Serial.println(myGPS.GPS_data[UTC_SECOND_POS]);
	Serial.print("LAT (dd): "); Serial.println(myGPS.GPS_data[LAT_POS], 10);
	Serial.print("LON (dd): "); Serial.println(myGPS.GPS_data[LON_POS], 10);
}



