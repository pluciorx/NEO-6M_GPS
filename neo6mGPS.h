#pragma once
#include "Arduino.h"




constexpr byte NEO_BUFF_LEN   = 10;
constexpr byte NEO_HEADER_LEN = 6;
constexpr byte NUM_FIELDS     = 13;
constexpr byte FIELD_LEN      = 10;
constexpr byte HEADER_LEN     = 6;




const byte NMEA_LEN = 16;
const byte FREQ_LEN = 14;
const byte BAUD_LEN = 28;

const byte GPS_NO_DATA  = 0;
const byte GPS_NEW_DATA = 1;

const byte GPGGA = 0;
const byte GPGLL = 1;
const byte GPGLV = 2;
const byte GPGSA = 3;
const byte GPRMC = 4;
const byte GPVTG = 5;

const byte NMEA_ID_POS  = 7;
const byte DDC_POS      = 8;
const byte SERIAL_1_POS = 9;
const byte SERIAL_2_POS = 10;
const byte USB_POS      = 11;
const byte SPI_POS      = 12;

const byte BAUD_0 = 14;
const byte BAUD_1 = 15;
const byte BAUD_2 = 16;
const byte BAUD_3 = 17;

const byte NAV_RATE_1 = 8;
const byte NAV_RATE_2 = 9;




const char CFG_MSG[NMEA_LEN] = {
	0xB5, // Header char 1
	0x62, // Header char 2
	0x06, // class
	0x01, // id
	0x08, // length char 1
	0x00, // length char 2
	0xF0, // payload (NMEA sentence ID char 1)
	0x00, // payload (NMEA sentence ID char 2)
	0x00, // payload I/O Target 0 - DDC           - (1 - enable sentence, 0 - disable)
	0x00, // payload I/O Target 1 - Serial Port 1 - (1 - enable sentence, 0 - disable)
	0x00, // payload I/O Target 2 - Serial Port 2 - (1 - enable sentence, 0 - disable)
	0x00, // payload I/O Target 3 - USB           - (1 - enable sentence, 0 - disable)
	0x00, // payload I/O Target 4 - SPI           - (1 - enable sentence, 0 - disable)
	0x00, // payload I/O Target 5 - Reserved      - (1 - enable sentence, 0 - disable)
	0x00, // CK_A
	0x00  // CK_B
};

const char CFG_RATE[FREQ_LEN] = {
	0xB5, // sync char 1
	0x62, // sync char 2
	0x06, // class
	0x08, // id
	0x06, // length
	0x00, // length
	0x64, // payload measRate (ms) 1
	0x00, // payload measRate (ms) 2
	0x00, // payload navRate (cycles) 1
	0x00, // payload navRate (cycles) 2
	0x01, // payload timeRef 1
	0x00, // payload timeRef 2
	0x00, // CK_A
	0x00  // CK_B
};

const char CFG_PRT[BAUD_LEN] = {
	0xB5, // sync char 1
	0x62, // sync char 2
	0x06, // class
	0x00, // id
	0x14, // length
	0x00, // length
	0x01, // payload portID
	0x00, // payload reserved0
	0x00, // payload txReady 1
	0x00, // payload txReady 2
	0xD0, // payload mode 1
	0x08, // payload mode 2
	0x00, // payload mode 3
	0x00, // payload mode 4
	0x00, // payload baudRate 0 (LSB)
	0x00, // payload baudRate 1
	0x00, // payload baudRate 2
	0x00, // payload baudRate 3 (MSB)
	0x07, // payload inProtoMask 1
	0x00, // payload inProtoMask 2
	0x03, // payload outProtoMask 1
	0x00, // payload outProtoMask 2
	0x00, // payload reserved4 1
	0x00, // payload reserved4 2
	0x00, // payload reserved5 1
	0x00, // payload reserved5 2
	0x00, // CK_A
	0x00  // CK_B
};




class neo6mGPS
{
public:// <<---------------------------------------------------------------------------//public
	char data[NUM_FIELDS][FIELD_LEN];
	float utc       = 0;
	char navStatus  = 'V';
	float lat       = 0;
	char latDir     = ' ';
	float lon       = 0;
	char lonDir     = ' ';
	float sog_knots = 0;
	float cog_true  = 0;




	void begin(HardwareSerial &port);
	void begin(usb_serial_class &port);
	void begin(HardwareSerial &port, uint32_t baud, uint16_t hertz);
	void begin(usb_serial_class &port, uint32_t baud, uint16_t hertz);

	void setupGPS(uint32_t baud, uint16_t hertz);
	void disableAllNmea();
	void enableAllNmea();
	void setSentence(char NMEA_num, bool enable);
	void changeBaud(uint32_t baud);
	void changeFreq(uint16_t hertz);
	bool available();




private:// <<---------------------------------------------------------------------------//private
	HardwareSerial* _port;
	usb_serial_class* usb_port;
	bool usingUSB = false;

	const char header[HEADER_LEN] = { '$', 'G', 'P', 'R', 'M', 'C' };
	bool headerFound = false;
	
	byte fieldNum = 0;
	byte fieldIndex = 0;




	void enableSelectedNmea();
	bool parseData();
	bool parseData_usb();
	void updateValues();
	void insertChecksum(char packet[], const byte len);
	void sendPacket(char packet[], const byte len);
	void sendPacket(const char packet[], const byte len);
};
