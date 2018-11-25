#include "neo6mGPS.h"




//initialize the GPS data extractor class and the GPS itself
void neo6mGPS::begin(HardwareSerial &_GPS_SERIAL, HardwareSerial &_PC_SERIAL)
{
  //update serial streams
  GPS_SERIAL = &_GPS_SERIAL;
  PC_SERIAL = &_PC_SERIAL;
  
  //setup GPS
  setupGPS();
  
  return;
}




//initialize the GPS data extractor class and the GPS itself
void neo6mGPS::begin(HardwareSerial &_GPS_SERIAL)
{
  //update serial stream
  GPS_SERIAL = &_GPS_SERIAL;
  
  //setup GPS
  setupGPS();
  
  return;
}




//setup GPS and load non-default configuration settings
void neo6mGPS::setupGPS()
{
  if(PC_SERIAL != 0)
    PC_SERIAL->println("Starting auto-configuration...");

  // Restore the receiver default configuration.
  for (byte i = 0; i < sizeof(possibleBaudrates) / sizeof(*possibleBaudrates); i++)
  {
    if(PC_SERIAL != 0)
    {
      PC_SERIAL->print("Trying to restore defaults at ");
      PC_SERIAL->print(possibleBaudrates[i]);
      PC_SERIAL->println(" baudrate...");
    }

    if (i != 0)
    {
      delay(100); // Little delay before flushing.
      GPS_SERIAL->flush();
    }

    GPS_SERIAL->begin(possibleBaudrates[i]);
    restoreDefaults();
  }

  // Switch the receiver serial to the default baudrate.
  if (possibleBaudrates[sizeof(possibleBaudrates) / sizeof(*possibleBaudrates) - 1] != GPS_DEFAULT_BAUDRATE)
  {
    if(PC_SERIAL != 0)
    {
      PC_SERIAL->print("Switching to the default baudrate which is ");
      PC_SERIAL->print(GPS_DEFAULT_BAUDRATE);
      PC_SERIAL->println("...");
    }

    delay(100); // Little delay before flushing.
    GPS_SERIAL->flush();
    GPS_SERIAL->begin(GPS_DEFAULT_BAUDRATE);
  }

  // Disable NMEA messages by sending appropriate packets.
  if(PC_SERIAL != 0)
    PC_SERIAL->println("Disabling NMEA messages...");
  disableNmea();

  // Switch the receiver serial to the wanted baudrate.
  if (GPS_WANTED_BAUDRATE != GPS_DEFAULT_BAUDRATE)
  {
    if(PC_SERIAL != 0)
    {
      PC_SERIAL->print("Switching receiver to the wanted baudrate which is ");
      PC_SERIAL->print(GPS_WANTED_BAUDRATE);
      PC_SERIAL->println("...");
    }

    changeBaudrate();

    delay(100); // Little delay before flushing.
    GPS_SERIAL->flush();
    GPS_SERIAL->begin(GPS_WANTED_BAUDRATE);
  }

  // Increase frequency to 100 ms.
  if(PC_SERIAL != 0)
    PC_SERIAL->println("Changing receiving frequency to 100 ms...");
  changeFrequency();

  // Disable unnecessary channels like SBAS or QZSS.
  if(PC_SERIAL != 0)
    PC_SERIAL->println("Disabling unnecessary channels...");
  disableUnnecessaryChannels();

  // Enable NAV-PVT messages.
  if(PC_SERIAL != 0)
    PC_SERIAL->println("Enabling NAV-PVT messages...");
  enableNavPvt();

  if(PC_SERIAL != 0)
    PC_SERIAL->println("Auto-configuration is complete!");

  delay(100); // Little delay before flushing.
  GPS_SERIAL->flush();

  return;
}




//send a packet to the receiver to restore default configuration.
void neo6mGPS::restoreDefaults()
{
  // CFG-CFG packet.
  byte packet[] = {
        0xB5, // sync char 1
        0x62, // sync char 2
        0x06, // class
        0x09, // id
        0x0D, // length
        0x00, // length
        0xFF, // payload
        0xFF, // payload
        0x00, // payload
        0x00, // payload
        0x00, // payload
        0x00, // payload
        0x00, // payload
        0x00, // payload
        0xFF, // payload
        0xFF, // payload
        0x00, // payload
        0x00, // payload
        0x17, // payload
        0x2F, // CK_A
        0xAE, // CK_B
  };

  sendPacket(packet, sizeof(packet));
  
  return;
}




//send a set of packets to the receiver to disable NMEA messages.
void neo6mGPS::disableNmea()
{
  // Array of two bytes for CFG-MSG packets payload.
  byte messages[][2] = {
        {0xF0, 0x0A},
        {0xF0, 0x09},
        {0xF0, 0x00},
        {0xF0, 0x01},
        {0xF0, 0x0D},
        {0xF0, 0x06},
        {0xF0, 0x02},
        {0xF0, 0x07},
        {0xF0, 0x03},
        {0xF0, 0x04},
        {0xF0, 0x0E},
        {0xF0, 0x0F},
        {0xF0, 0x05},
        {0xF0, 0x08},
        {0xF1, 0x00},
        {0xF1, 0x01},
        {0xF1, 0x03},
        {0xF1, 0x04},
        {0xF1, 0x05},
        {0xF1, 0x06},
  };

  // CFG-MSG packet buffer.
  byte packet[] = {
        0xB5, // sync char 1
        0x62, // sync char 2
        0x06, // class
        0x01, // id
        0x03, // length
        0x00, // length
        0x00, // payload (first byte from messages array element)
        0x00, // payload (second byte from messages array element)
        0x00, // payload (not changed in the case)
        0x00, // CK_A
        0x00, // CK_B
  };
  byte packetSize = sizeof(packet);

  // Offset to the place where payload starts.
  byte payloadOffset = 6;

  // Iterate over the messages array.
  for (byte i = 0; i < sizeof(messages) / sizeof(*messages); i++)
  {
    // Copy two bytes of payload to the packet buffer.
    for (byte j = 0; j < sizeof(*messages); j++)
    {
      packet[payloadOffset + j] = messages[i][j];
    }

    // Set checksum bytes to the null.
    packet[packetSize - 2] = 0x00;
    packet[packetSize - 1] = 0x00;

    // Calculate checksum over the packet buffer excluding sync (first two) and checksum chars (last two).
    for (byte j = 0; j < packetSize - 4; j++)
    {
      packet[packetSize - 2] += packet[2 + j];
      packet[packetSize - 1] += packet[packetSize - 2];
    }

    sendPacket(packet, packetSize);
  }

  return;
}




//send a packet to the receiver to change baudrate to 115200.
void neo6mGPS::changeBaudrate()
{
  // CFG-PRT packet.
  byte packet[] = {
        0xB5, // sync char 1
        0x62, // sync char 2
        0x06, // class
        0x00, // id
        0x14, // length
        0x00, // length
        0x01, // payload
        0x00, // payload
        0x00, // payload
        0x00, // payload
        0xD0, // payload
        0x08, // payload
        0x00, // payload
        0x00, // payload
        0x00, // payload
        0xC2, // payload
        0x01, // payload
        0x00, // payload
        0x07, // payload
        0x00, // payload
        0x03, // payload
        0x00, // payload
        0x00, // payload
        0x00, // payload
        0x00, // payload
        0x00, // payload
        0xC0, // CK_A
        0x7E, // CK_B
  };

  sendPacket(packet, sizeof(packet));

  return;
}




//send a packet to the receiver to change frequency to 100 ms.
void neo6mGPS::changeFrequency()
{
  // CFG-RATE packet.
  byte packet[] = {
        0xB5, // sync char 1
        0x62, // sync char 2
        0x06, // class
        0x08, // id
        0x06, // length
        0x00, // length
        0x64, // payload
        0x00, // payload
        0x01, // payload
        0x00, // payload
        0x01, // payload
        0x00, // payload
        0x7A, // CK_A
        0x12, // CK_B
  };

  sendPacket(packet, sizeof(packet));

  return;
}




//send a packet to the receiver to disable unnecessary channels.
void neo6mGPS::disableUnnecessaryChannels()
{
  // CFG-GNSS packet.
  byte packet[] = {
        0xB5, // sync char 1
        0x62, // sync char 2
        0x06, // class
        0x3E, // id
        0x24, // length
        0x00, // length

        0x00, 0x00, 0x16, 0x04, 0x00, 0x04, 0xFF, 0x00, // payload
        0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x03, 0x00, // payload
        0x00, 0x00, 0x00, 0x01, 0x05, 0x00, 0x03, 0x00, // payload
        0x00, 0x00, 0x00, 0x01, 0x06, 0x08, 0xFF, 0x00, // payload
        0x00, 0x00, 0x00, 0x01,                         // payload

        0xA4, // CK_A
        0x25, // CK_B
  };

  sendPacket(packet, sizeof(packet));

  return;
}




//send a packet to the receiver to enable NAV-PVT messages.
void neo6mGPS::enableNavPvt()
{
  // CFG-MSG packet.
  byte packet[] = {
        0xB5, // sync char 1
        0x62, // sync char 2
        0x06, // class
        0x01, // id
        0x03, // length
        0x00, // length
        0x01, // payload
        0x07, // payload
        0x01, // payload
        0x13, // CK_A
        0x51, // CK_B
  };

  sendPacket(packet, sizeof(packet));

  return;
}




//send the packet specified to the receiver.
void neo6mGPS::sendPacket(byte *packet, byte len)
{
  for (byte i = 0; i < len; i++)
  {
    GPS_SERIAL->write(packet[i]);
  }

  printPacket(packet, len);

  return;
}




//print the packet specified to the PC serial in a hexadecimal form.
void neo6mGPS::printPacket(byte *packet, byte len)
{
  char temp[3];
  
  if(PC_SERIAL != 0)
  {
    PC_SERIAL->print("\t");
    
    for (byte i = 0; i < len; i++)
    {
      sprintf(temp, "%.2X", packet[i]);
      PC_SERIAL->print(temp);

      if (i != len - 1)
      {
        PC_SERIAL->print(' ');
      }
    }
    
    PC_SERIAL->println();
  }
  
  

  return;
}




//update lat and lon in the GPS_data array
void neo6mGPS::grabData_LatLong()
{
  byte buffLen = 64;
  byte buff[buffLen] = {" "};

  byte trash = 0;

  /* See if GSP data stream started */
  if(GPS_SERIAL->available())
  {
    while(GPS_SERIAL->available() == 0);
    trash = GPS_SERIAL->read();
    
    while(trash != 'L')
    {
      while(GPS_SERIAL->available() == 0);
      trash = GPS_SERIAL->read(); //trash
    }
    
    for(int i=0; i<buffLen; i++)
    {
      while(GPS_SERIAL->available() == 0);
      trash = GPS_SERIAL->read();

      if(trash == '\n')
      {
        break;
      }
      
      buff[i] = trash;
    }

    extractLatLong(buff, buffLen);
  }
  
  return;
}




//extract lat and lon data from the GPS stream buffer
void neo6mGPS::extractLatLong()
{
  byte i = 0;

  byte latLonLen = 12;
  byte latBuff[latLonLen] = {' '};
  byte latIndex = 1; //start at 1 instead of 0 since lat only has 11 values
  byte lonBuff[latLonLen] = {' '};
  byte lonIndex = 0;
  
  //find first comma
  while((buff[i] != ',') && (i < buffLen) && (lonIndex < latLonLen))
  {
    i++;
  }

  //increment once past the comma
  i++;

  //grab all lat (if available) data except for the N/S
  while((buff[i] != ',') && (i < buffLen) && (lonIndex < latLonLen))
  {
    latBuff[latIndex] = buff[i] - '0';
    
    latIndex++;
    i++;
  }

  //increment once past the comma
  i++;

  //grab the N/S (if available) char
  while((buff[i] != ',') && (i < buffLen) && (lonIndex < latLonLen))
  {
    latBuff[latIndex] = buff[i];
    
    latIndex++;
    i++;
  }

  //increment once past the comma
  i++;

  //grab all lat (if available) data except for the E/W
  while((buff[i] != ',') && (i < buffLen) && (lonIndex < latLonLen))
  {
    lonBuff[lonIndex] = buff[i] - '0';
    
    lonIndex++;
    i++;
  }

  //increment once past the comma
  i++;

  //grab the E/W (if available) char
  while((buff[i] != ',') && (i < buffLen) && (lonIndex < latLonLen))
  {
    lonBuff[lonIndex] = buff[i];
    
    lonIndex++;
    i++;
  }

  if((latBuff[1] != ' ') && (lonBuff[0] != ' '))
  {
    convertLatLong(latBuff, lonBuff);
  }

  return;
}


//convert from minutes seconds to degrees
void neo6mGPS::convertLatLong(byte latBuff[], byte lonBuff[])
{
  latitude = ((latBuff[1]*10) + latBuff[2]) + ((latBuff[3]*10) + latBuff[4])/60.0 + ((latBuff[6]*10) + latBuff[7] + (latBuff[8]/10.0) + (latBuff[9]/100.0) + (latBuff[10]/1000.0))/3600.0;
  if(latBuff[11] == 'S')
  {
    latitude = -latitude;
  }

  longitude = ((lonBuff[0]*100) + (lonBuff[1]*10) + lonBuff[2]) + ((lonBuff[3]*10) + lonBuff[4])/60.0 + ((lonBuff[6]*10) + lonBuff[7] + (lonBuff[8]/10.0) + (lonBuff[9]/100.0) + (lonBuff[10]/1000.0))/3600.0;
  if(lonBuff[11] == 'W')
  {
    longitude = -longitude;
  }

  return;
}



