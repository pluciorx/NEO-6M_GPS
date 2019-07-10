#pragma once
#include "Arduino.h"




#define NEO_BUFF_LEN   10
#define NEO_HEADER_LEN 6




const byte NMEA_LEN       = 16;
const byte FREQ_LEN       = 14;
const byte BAUD_LEN       = 28;

const byte GPS_NO_DATA    = 0;
const byte GPS_NEW_DATA   = 1;

const byte GPGGA          = 0;
const byte GPGLL          = 1;
const byte GPGLV          = 2;
const byte GPGSA          = 3;
const byte GPRMC          = 4;
const byte GPVTG          = 5;

const byte NMEA_ID_POS    = 7;
const byte DDC_POS        = 13;
const byte SERIAL_1_POS   = 14;
const byte SERIAL_2_POS   = 15;
const byte USB_POS        = 16;
const byte SPI_POS        = 17;
const byte CK_A           = 19;
const byte CK_B           = 20;




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
	0x64, // payload
	0x00, // payload
	0x01, // payload
	0x00, // payload
	0x01, // payload
	0x00, // payload
	0x7A, // CK_A
	0x12  // CK_B
};

const char CFG_PRT[BAUD_LEN] = {
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
	0x7E  // CK_B
};




class neo6mGPS
{
public:// <<---------------------------------------------------------------------------//public
	struct neo6m_data {
		uint16_t utc_year;
		uint8_t utc_month;
		uint8_t utc_day;
		uint8_t utc_hour;
		uint8_t utc_min;
		uint8_t utc_sec;
		float lat_dd;
		float lon_dd;
		float sog; //Speed over ground (knots)
		float cog; //Course over ground (degrees)
	};
	neo6m_data telem;




	void begin(HardwareSerial &port);
	void begin(usb_serial_class &port);
	int grabData_GPS();




private:// <<---------------------------------------------------------------------------//private
	HardwareSerial* _port;
	usb_serial_class* usb_port;
	bool usingUSB = false;

	struct temp_neo6m_data {
		char utc_year[NEO_BUFF_LEN];
		char utc_month[NEO_BUFF_LEN];
		char utc_day[NEO_BUFF_LEN];
		char utc_hour[NEO_BUFF_LEN];
		char utc_min[NEO_BUFF_LEN];
		char utc_sec[NEO_BUFF_LEN];
		char lat_dd[NEO_BUFF_LEN];
		char lon_dd[NEO_BUFF_LEN];
		char sog[NEO_BUFF_LEN]; //Speed over ground (knots)
		char cog[NEO_BUFF_LEN]; //Course over ground (degrees)
	};
	temp_neo6m_data tempData;
	
	const byte header[NEO_HEADER_LEN] = { '$', 'G', 'P', 'R', 'M', 'C' };
	byte headerIndex = 0;
	byte fieldIndex = 0;




	void setupGPS();
	void setSentence(char NMEA_num, bool enable);
	void disableAllNmea();
	void enableSelectedNmea();
	void insertChecksum(char packet[], const byte len);
	void changeBaudrate();
	void changeFrequency();
	void sendPacket(char packet[], const byte len);
	void sendPacket(const char packet[], const byte len);
};
