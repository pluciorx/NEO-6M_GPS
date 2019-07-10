#include "neo6mGPS.h"




//initialize the GPS data extractor class and the GPS itself
void neo6mGPS::begin(HardwareSerial &port)
{
	_port = &port;

	_port->begin(9600);
	setupGPS();
}




//initialize the GPS data extractor class and the GPS itself
void neo6mGPS::begin(usb_serial_class &port)
{
	usingUSB = true;
	usb_port = &port;

	usb_port->begin(9600);
	setupGPS();
}




//setup GPS and load non-default configuration settings
void neo6mGPS::setupGPS()
{
	disableAllNmea();
	enableSelectedNmea();

	// Increase frequency to 100 ms.
	changeFrequency();

	// Switch the receiver serial to the wanted baudrate.
	changeBaudrate();
}




void neo6mGPS::setSentence(char NMEA_num, bool enable)
{
	char configPacket[NMEA_LEN] = { 0 };

	strcpy(configPacket, CFG_MSG);

	if (enable)
	{
		configPacket[SERIAL_1_POS] = 1;
		configPacket[SERIAL_2_POS] = 1;
	}

	configPacket[NMEA_ID_POS] = NMEA_num;
	insertChecksum(configPacket, NMEA_LEN);

	sendPacket(configPacket, NMEA_LEN);
}





//send a set of packets to the receiver to disable NMEA messages.
void neo6mGPS::disableAllNmea()
{
	setSentence(GPGGA, false);
	//setSentence(GPGLL, false);
	//setSentence(GPGLV, false);
	//setSentence(GPGSA, false);
	//setSentence(GPRMC, false);
	//setSentence(GPVTG, false);
}




//send a set of packets to the receiver to enable NMEA messages.
void neo6mGPS::enableSelectedNmea()
{
	//comment or uncomment based on what sentences desired

	//setSentence(GPGGA, true);
	//setSentence(GPGLL, true);
	//setSentence(GPGSA, true);
	//setSentence(GPGLV, true);
	//setSentence(GPRMC, true);
	//setSentence(GPVTG, true);
}



void neo6mGPS::insertChecksum(char packet[], const byte len)
{
	uint16_t cs = 0;

	for (byte i = 2; i < (len - 2); i++)
		cs += packet[i];

	packet[CK_A] = (cs >> 8) & 0xFF;
	packet[CK_B] = cs & 0xFF;
}





//send a packet to the receiver to change baudrate to 115200.
void neo6mGPS::changeBaudrate()
{
	sendPacket(CFG_PRT, BAUD_LEN);

	delay(100);

	if (usingUSB)
	{
		usb_port->flush();
		usb_port->begin(115200);
	}
	else
	{
		_port->flush();
		_port->begin(115200);
	}
}




//send a packet to the receiver to change frequency to 100 ms.
void neo6mGPS::changeFrequency()
{
	sendPacket(CFG_RATE, FREQ_LEN);
}




//update data in the GPS_data array
int neo6mGPS::grabData_GPS()
{
	int returnVal = GPS_NO_DATA;

	while (_port->available())
	{
		char recChar = _port->read();

		if ((recChar == ',') && (headerIndex == sizeof(header)))
		{
			fieldIndex += 1;
			continue;
		}

		if (!fieldIndex)
		{
			if (recChar == header[headerIndex])
				headerIndex += 1;
		}
	}

	//nothing found
	return returnVal;
}




void neo6mGPS::sendPacket(char packet[], const byte len)
{
	if (usingUSB)
		usb_port->write(packet, len);
	else
		_port->write(packet, len);
}




void neo6mGPS::sendPacket(const char packet[], const byte len)
{
	if (usingUSB)
		usb_port->write(packet, len);
	else
		_port->write(packet, len);
}
