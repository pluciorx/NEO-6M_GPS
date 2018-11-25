#include "Arduino.h"

#ifndef neo6mGPS_cpp

  #define neo6mGPS_cpp
  
  class neo6mGPS
  {
    public:
      //serial stream variable (can be hardware or software serial)
      Stream* _serial;
    
      //array of possible baudrates that can be used by the receiver, sorted descending to prevent excess Serial flush/begin
      //after restoring defaults. You can uncomment values that can be used by your receiver before the auto-configuration.
      uint32_t possibleBaudrates[] = {
                                          //921600L,
                                          //460800L,
                                          //230400L,
                                          115200L,
                                          //57600L,
                                          //38400L,
                                          //19200L,
                                          9600L,
                                          //4800L,
                                        };
                                        
      //array holding all GPS values that can be updated
      float GPS_data[] = {
                            //latitude:
                            0.0,
                            
                            //longitude:
                            0.0,
                          };
      
      //initialize the GPS data extractor class and the GPS itself
      void begin(Stream& stream);
      
      //update lat and lon in the GPS_data array
      void grabData_LatLong();
  };

#endif
