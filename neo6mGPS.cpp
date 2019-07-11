#include "neo6mGPS.h"




//initialize the GPS data extractor class and the GPS itself
void neo6mGPS::begin(HardwareSerial &port)
{
	_port = &port;

	_port->begin(9600);
}




//initialize the GPS data extractor class and the GPS itself
void neo6mGPS::begin(usb_serial_class &port)
{
	usingUSB = true;
	usb_port = &port;

	usb_port->begin(115200);
}




//setup GPS and load non-default configuration settings
void neo6mGPS::setupGPS()
{
	disableAllNmea();
	enableSelectedNmea();
	changeBaud(115200);
	changeFreq(10);
}




//send a set of packets to the receiver to disable NMEA messages
void neo6mGPS::disableAllNmea()
{
	setSentence(GPGGA, false);
	setSentence(GPGLL, false);
	setSentence(GPGLV, false);
	setSentence(GPGSA, false);
	setSentence(GPRMC, false);
	setSentence(GPVTG, false);
}




//send a set of packets to the receiver to enable NMEA messages
void neo6mGPS::enableAllNmea()
{
	setSentence(GPGGA, true);
	setSentence(GPGLL, true);
	setSentence(GPGSA, true);
	setSentence(GPGLV, true);
	setSentence(GPRMC, true);
	setSentence(GPVTG, true);
}




//send a set of packets to the receiver to enable NMEA messages
void neo6mGPS::enableSelectedNmea()
{
	//comment or uncomment based on what sentences desired

	//setSentence(GPGGA, true);
	//setSentence(GPGLL, true);
	//setSentence(GPGSA, true);
	//setSentence(GPGLV, true);
	setSentence(GPRMC, true);
	//setSentence(GPVTG, true);
}




//send a packet to the receiver to change baudrate
void neo6mGPS::changeBaud(uint32_t baud)
{
	char configPacket[BAUD_LEN];
	memcpy(configPacket, CFG_PRT, BAUD_LEN);

	configPacket[BAUD_0] = (char)(baud & 0xFF);
	configPacket[BAUD_1] = (char)((baud >> 8) & 0xFF);
	configPacket[BAUD_2] = (char)((baud >> 16) & 0xFF);
	configPacket[BAUD_3] = (char)((baud >> 24) & 0xFF);

	insertChecksum(configPacket, BAUD_LEN);
	sendPacket(configPacket, BAUD_LEN);

	delay(100);

	if (usingUSB)
	{
		usb_port->flush();
		usb_port->begin(baud);
	}
	else
	{
		_port->flush();
		_port->begin(baud);
	}
}




//send a packet to the receiver to change frequency
void neo6mGPS::changeFreq(uint16_t hertz)
{
	uint16_t normHerz = hertz / 10;
	char configPacket[FREQ_LEN];
	memcpy(configPacket, CFG_RATE, FREQ_LEN);

	configPacket[NAV_RATE_1] = (char)(normHerz & 0xFF);
	configPacket[NAV_RATE_2] = (char)((normHerz >> 8) & 0xFF);

	insertChecksum(configPacket, FREQ_LEN);
	sendPacket(configPacket, FREQ_LEN);
}




void neo6mGPS::setSentence(char NMEA_num, bool enable)
{
	char configPacket[NMEA_LEN];
	memcpy(configPacket, CFG_MSG, NMEA_LEN);

	if (enable)
		configPacket[SERIAL_1_POS] = 1;

	configPacket[NMEA_ID_POS] = NMEA_num;
	insertChecksum(configPacket, NMEA_LEN);

	sendPacket(configPacket, NMEA_LEN);
}




void neo6mGPS::insertChecksum(char packet[], const byte len)
{
	uint8_t ck_a = 0;
	uint8_t ck_b = 0;

	// exclude the first and last two bytes in packet
	for (byte i = 2; i < (len - 2); i++)
	{
		ck_a += packet[i];
		ck_b += ck_a;
	}

	packet[len - 2] = ck_a;
	packet[len - 1] = ck_b;
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
