#include "neo6mGPS.h"




void neo6mGPS::begin(Stream &_GPS_SERIAL, Stream &_PC_SERIAL, uint32_t _PC_BAUDRATE)
{
  //update serial streams
  GPS_SERIAL = &_GPS_SERIAL;
  PC_SERIAL = &_PC_SERIAL;
  PC_BAUDRATE = &_PC_BAUDRATE;
  
  //initialise PC serial connection if baud given
  PC_SERIAL.begin(this->PC_BAUDRATE);
  
  //setup GPS
  setupGPS()
  
  return;
}




void neo6mGPS::begin(Stream &GPS_SERIAL, Stream &_PC_SERIAL)
{
  //update serial streams
  GPS_SERIAL = &_GPS_SERIAL;
  PC_SERIAL = &_PC_SERIAL;
  
  //setup GPS
  setupGPS()
  
  return;
}




void neo6mGPS::begin(Stream &_GPS_SERIAL)
{
  //update serial stream
  GPS_SERIAL = &_GPS_SERIAL;
  
  //setup GPS
  setupGPS()
  
  return;
}




void neo6mGPS::setupGPS()
{
  
}



