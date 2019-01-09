#include "neo6mGPS.h"




//create GPS class
neo6mGPS myGPS;




//initialize the GPS data extractor class and the GPS itself
void neo6mGPS::begin(HardwareSerial &_GPS_SERIAL, usb_serial_class &_PC_SERIAL_USB)
{
	// reserve 75 bytes for the inputString:
	inputString.reserve(75);

	//update serial streams
	GPS_SERIAL = &_GPS_SERIAL;
	PC_SERIAL_USB = &_PC_SERIAL_USB;

	//setup GPS
	setupGPS();

	return;
}




//initialize the GPS data extractor class and the GPS itself
void neo6mGPS::begin(HardwareSerial &_GPS_SERIAL, HardwareSerial &_PC_SERIAL)
{
	// reserve 75 bytes for the inputString:
	inputString.reserve(75);

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
	// reserve 75 bytes for the inputString:
	inputString.reserve(75);

	//update serial stream
	GPS_SERIAL = &_GPS_SERIAL;

	//setup GPS
	setupGPS();

	return;
}




//https://www.arduino.cc/en/Tutorial/SerialEvent
void serialEvent1()
{
	if (myGPS.GPS_SERIAL == &Serial1)
	{
		while (Serial1.available())
		{
			// get the new byte:
			char inChar = (char)Serial1.read();

			// add it to the inputString:
			if (!myGPS.stringComplete)
				myGPS.inputString += inChar;

			// if the incoming character is a newline, set a flag so the main loop can
			// do something about it:
			if (inChar == '\n')
			{
				myGPS.stringComplete = true;
			}
		}
	}

	return;
}

//https://www.arduino.cc/en/Tutorial/SerialEvent
void serialEvent2()
{
	if (myGPS.GPS_SERIAL == &Serial2)
	{
		while (Serial2.available())
		{
			// get the new byte:
			char inChar = (char)Serial2.read();

			// add it to the inputString:
			if (!myGPS.stringComplete)
				myGPS.inputString += inChar;

			// if the incoming character is a newline, set a flag so the main loop can
			// do something about it:
			if (inChar == '\n')
			{
				myGPS.stringComplete = true;
			}
		}
	}

	return;
}

//https://www.arduino.cc/en/Tutorial/SerialEvent
void serialEvent3()
{
	if (myGPS.GPS_SERIAL == &Serial3)
	{
		while (Serial3.available())
		{
			// get the new byte:
			char inChar = (char)Serial3.read();

			// add it to the inputString:
			if(!myGPS.stringComplete)
				myGPS.inputString += inChar;

			//Serial.write(inChar);

			// if the incoming character is a newline, set a flag so the main loop can
			// do something about it:
			if (inChar == '\n')
			{
				myGPS.stringComplete = true;
			}
		}
	}

	return;
}

//https://www.arduino.cc/en/Tutorial/SerialEvent
void serialEvent4()
{
	if (myGPS.GPS_SERIAL == &Serial4)
	{
		while (Serial4.available())
		{
			// get the new byte:
			char inChar = (char)Serial4.read();

			// add it to the inputString:
			if (!myGPS.stringComplete)
				myGPS.inputString += inChar;

			// if the incoming character is a newline, set a flag so the main loop can
			// do something about it:
			if (inChar == '\n')
			{
				myGPS.stringComplete = true;
			}
		}
	}

	return;
}

//https://www.arduino.cc/en/Tutorial/SerialEvent
void serialEvent5()
{
	if (myGPS.GPS_SERIAL == &Serial5)
	{
		while (Serial5.available())
		{
			// get the new byte:
			char inChar = (char)Serial5.read();

			// add it to the inputString:
			if (!myGPS.stringComplete)
				myGPS.inputString += inChar;

			// if the incoming character is a newline, set a flag so the main loop can
			// do something about it:
			if (inChar == '\n')
			{
				myGPS.stringComplete = true;
			}
		}
	}

	return;
}

//https://www.arduino.cc/en/Tutorial/SerialEvent
void serialEvent6()
{
	if (myGPS.GPS_SERIAL == &Serial6)
	{
		while (Serial6.available())
		{
			// get the new byte:
			char inChar = (char)Serial6.read();

			// add it to the inputString:
			if (!myGPS.stringComplete)
				myGPS.inputString += inChar;

			// if the incoming character is a newline, set a flag so the main loop can
			// do something about it:
			if (inChar == '\n')
			{
				myGPS.stringComplete = true;
			}
		}
	}
	
	return;
}




//setup GPS and load non-default configuration settings
void neo6mGPS::setupGPS()
{
	if (PC_SERIAL != 0)
		PC_SERIAL->println("Starting NEO-6M GPS auto-configuration...");
	else if (PC_SERIAL_USB != 0)
		PC_SERIAL_USB->println("Starting NEO-6M GPS auto-configuration...");
	
	// Disable NMEA messages by sending appropriate packets.
	disableNmea();

	// Enable NMEA messages by sending appropriate packets.
	enableNmea();

	// Increase frequency to 100 ms.
	changeFrequency();

	// Switch the receiver serial to the wanted baudrate.
	changeBaudrate();

	if (PC_SERIAL != 0)
		PC_SERIAL->println("Finished NEO-6M GPS auto-configuration!");
	else if (PC_SERIAL_USB != 0)
		PC_SERIAL_USB->println("Finished NEO-6M GPS auto-configuration!");

	return;
}




//send a set of packets to the receiver to disable NMEA messages.
void neo6mGPS::disableNmea()
{
	if (PC_SERIAL != 0)
		PC_SERIAL->println("\tTurning off all NMEA sentences...");
	else if (PC_SERIAL_USB != 0)
		PC_SERIAL_USB->println("\tTurning off all NMEA sentences...");

	writeConfigPacket(turn_Off_GPGGA, NMEA_LEN);
	writeConfigPacket(turn_Off_GPGLL, NMEA_LEN);
	writeConfigPacket(turn_Off_GPGSA, NMEA_LEN);
	writeConfigPacket(turn_Off_GPGLV, NMEA_LEN);
	writeConfigPacket(turn_Off_GPRMC, NMEA_LEN);
	writeConfigPacket(turn_Off_GPVTG, NMEA_LEN);

	return;
}




//send a set of packets to the receiver to enable NMEA messages.
void neo6mGPS::enableNmea()
{
	if (PC_SERIAL != 0)
		PC_SERIAL->println("\tTurning on desired NMEA sentences...");
	else if (PC_SERIAL_USB != 0)
		PC_SERIAL_USB->println("\tTurning on desired NMEA sentences...");

	//comment or uncomment based on what sentences desired

	//writeConfigPacket(turn_On_GPGGA, NMEA_LEN);
	//writeConfigPacket(turn_On_GPGLL, NMEA_LEN);
	//writeConfigPacket(turn_On_GPGSA, NMEA_LEN);
	//writeConfigPacket(turn_On_GPGLV, NMEA_LEN);
	writeConfigPacket(turn_On_GPRMC, NMEA_LEN);
	//writeConfigPacket(turn_On_GPVTG, NMEA_LEN);

	return;
}




//send a packet to the receiver to change baudrate to 115200.
void neo6mGPS::changeBaudrate()
{
	if (PC_SERIAL != 0)
		PC_SERIAL->println("\tChanging baud rate...");
	else if (PC_SERIAL_USB != 0)
		PC_SERIAL_USB->println("\tChanging baud rate...");

	writeConfigPacket(changeBaud, BAUD_LEN);

	delay(100); // Little delay before flushing.
	GPS_SERIAL->flush();
	GPS_SERIAL->begin(115200); //reset serial port to new baud

	return;
}




//send a packet to the receiver to change frequency to 100 ms.
void neo6mGPS::changeFrequency()
{
	if (PC_SERIAL != 0)
		PC_SERIAL->println("\tUpdating refresh frequency...");
	else if (PC_SERIAL_USB != 0)
		PC_SERIAL_USB->println("\tUpdating refresh frequency...");

	writeConfigPacket(updateFreq, FREQ_LEN);

	return;
}




//update lat and lon in the GPS_data array
int neo6mGPS::grabData_LatLong()
{
	int returnVal = NO_DATA;

	// print the string when a newline arrives:
	if (stringComplete)
	{
		buffIndex = inputString.indexOf("RMC");

		if (buffIndex == 3)
		{
			//Serial.println(inputString);

			returnVal = extractLatLong(buffIndex);
		}

		// clear the string:
		inputString = "";
		stringComplete = false;
	}

	//nothing found
	return returnVal;
}


//																			  00000000001111111111222222222233333333334444444444555555555566666666667
//																			  01234567890123456789012345678901234567890123456789012345678901234567890
//extract lat and lon data from the GPS stream buffer (Example NMEA Sentence: $GPRMC,083559.00,A,4717.11437,N,00833.91522,E,0.004,77.52,091202,,,A*57)
int neo6mGPS::extractLatLong(byte startingIndex)
{
	if (inputString[startingIndex + 14] == 'A')
	{
		GPS_data[UTC_HOUR_POS] = ((inputString[startingIndex + 4] - '0') * 10) + (inputString[startingIndex + 5] - '0');

		GPS_data[UTC_MINUTE_POS] = ((inputString[startingIndex + 6] - '0') * 10) + (inputString[startingIndex + 7] - '0');

		GPS_data[UTC_SECOND_POS] = ((inputString[startingIndex + 8] - '0') * 10) + (inputString[startingIndex + 9] - '0') + ((inputString[startingIndex + 11] - '0') / 10) + ((inputString[startingIndex + 12] - '0') / 100);

		GPS_data[LAT_POS] = (((inputString[startingIndex + 16] - '0') * 10) + (inputString[startingIndex + 17] - '0'))																																											//degrees
						  + ((((inputString[startingIndex + 18] - '0') * 10) + (inputString[startingIndex + 19] - '0')) / 60.0)																																									//minutes
						  + ((((inputString[startingIndex + 20] - '0') * 10) + (inputString[startingIndex + 21] - '0') + ((inputString[startingIndex + 23] - '0') / 10.0) + ((inputString[startingIndex + 24] - '0') / 100.0) + ((inputString[startingIndex + 25] - '0') / 1000.0)) / 3600.0);	//seconds
	
		GPS_data[LON_POS] = (((inputString[startingIndex + 29] - '0') * 100) + ((inputString[startingIndex + 30] - '0') * 10) + (inputString[startingIndex + 31] - '0'))																														//degrees
						  + ((((inputString[startingIndex + 32] - '0') * 10) + (inputString[startingIndex + 33] - '0')) / 60.0)																																									//minutes
						  + ((((inputString[startingIndex + 35] - '0') * 10) + (inputString[startingIndex + 36] - '0') + ((inputString[startingIndex + 37] - '0') / 10.0) + ((inputString[startingIndex + 38] - '0') / 100.0) + ((inputString[startingIndex + 39] - '0') / 1000.0)) / 3600.0);	//seconds

		if (inputString[startingIndex + 27] == 'S')
		{
			GPS_data[LAT_POS] = -GPS_data[LAT_POS];
		}

		if (inputString[startingIndex + 41] == 'W')
		{
			GPS_data[LON_POS] = -GPS_data[LON_POS];
		}

		if (inputString[startingIndex + 43] != ',')
		{
			GPS_data[SPD_POS] = (inputString[startingIndex + 43] - '0') + ((inputString[startingIndex + 45] - '0') / 10) + ((inputString[startingIndex + 46] - '0') / 100) + ((inputString[startingIndex + 47] - '0') / 1000);
			
			if (inputString[startingIndex + 49] != ',')
			{
				//TO DO
				//GPS_data[COG_POS] = (inputString[startingIndex + 49] - '0')
			}
		}
		else if (inputString[startingIndex + 47] != ',')
		{
			//TO DO
		}
		else
		{
			//TO DO
		}

		//debugging prints
		/*Serial.print("UTC HOUR: "); Serial.println(GPS_data[UTC_HOUR_POS]);
		Serial.print("UTC MIN: "); Serial.println(GPS_data[UTC_MINUTE_POS]);
		Serial.print("UTC SEC: "); Serial.println(GPS_data[UTC_SECOND_POS]);
		Serial.print("LAT (dd): "); Serial.println(GPS_data[LAT_POS], 10);
		Serial.print("LON (dd): "); Serial.println(GPS_data[LON_POS], 10);*/

		return NEW_DATA;
	}
	else
	{
		return NO_DATA;
	}
}




//send the packet specified to the receiver.
void neo6mGPS::writeConfigPacket(byte packet[], byte len)
{
	for (byte i = 0; i < len; i++)
	{
		GPS_SERIAL->write(packet[i]);
	}

	return;
}



