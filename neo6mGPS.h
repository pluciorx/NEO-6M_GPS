#include "Arduino.h"
#include "HardwareSerial.h"




#ifndef neo6mGPS_cpp
#define neo6mGPS_cpp

#define NMEA_LEN 16
#define FREQ_LEN 14
#define BAUD_LEN 28
#define DATA_LEN 10

#define UTC_YEAR_POS 0
#define UTC_MONTH_POS 1
#define UTC_DAY_POS 2
#define UTC_HOUR_POS 3
#define UTC_MINUTE_POS 4
#define UTC_SECOND_POS 5
#define LAT_POS 6
#define LON_POS 7
#define SPD_POS 8
#define COG_POS 9

#define NO_DATA 0
#define NEW_DATA 1




class neo6mGPS
{
public:// <<---------------------------------------------------------------------------//public
	HardwareSerial*  GPS_SERIAL;

	String inputString = "";      // a String to hold incoming data
	bool stringComplete = false;  // whether the string is complete




	 //array holding all GPS values that can be updated
	float GPS_data[DATA_LEN] = {
		//UTC Time, Time of position fix - Year:
		0.0,

		//UTC Time, Time of position fix - Month:
		0.0,

		//UTC Time, Time of position fix - Day:
		0.0,

		//UTC Time, Time of position fix - Hour:
		0.0,

		//UTC Time, Time of position fix - Minute:
		0.0,

		//UTC Time, Time of position fix - Second:
		0.0,

		//Latitude (dd):
		0.0,

		//Longitude (dd):
		0.0,

		//Speed over ground (knots):
		0.0,

		//Course over ground (degrees):
		0.0,
	};




	//initialize the GPS data extractor class and the GPS itself (overloaded)
	void begin(HardwareSerial& _GPS_SERIAL, usb_serial_class& _PC_SERIAL_USB);
	void begin(HardwareSerial& _GPS_SERIAL, HardwareSerial& _PC_SERIAL);
	void begin(HardwareSerial& _GPS_SERIAL);

	//update lat and lon in the GPS_data array
	int grabData_LatLong();




private:// <<---------------------------------------------------------------------------//private
	//hardware serial stream variables (can be ONLY behardware serial ports)
	usb_serial_class* PC_SERIAL_USB = 0;
	HardwareSerial*  PC_SERIAL = 0;



	// <<---------------------------------------------------------------------------//CFG-MSG packets:
	// <<---------------------------------------------------------------------------//off packets
	byte turn_Off_GPGGA[NMEA_LEN] = {//---------------------------------------------//GPGGA off
	  0xB5, // Header char 1
	  0x62, // Header char 2
	  0x06, // class
	  0x01, // id
	  0x08, // length char 1
	  0x00, // length char 2
	  0xF0, // payload (NMEA sentence ID char 1)
	  0x00, // payload (NMEA sentence ID char 2)
	  0x00, // payload I/O Target 0 - DDC       - (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 1 - Serial Port 1 - (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 2 - Serial Port 2 - (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 3 - USB       - (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 4 - SPI       - (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 5 - Reserved    - (1 - enable sentence, 0 - disable)
	  0xFF, // CK_A
	  0x23  // CK_B
	};

	byte turn_Off_GPGLL[NMEA_LEN] = {//---------------------------------------------//GPGLL off
	  0xB5, // Header char 1
	  0x62, // Header char 2
	  0x06, // class
	  0x01, // id
	  0x08, // length char 1
	  0x00, // length char 2
	  0xF0, // payload (NMEA sentence ID char 1)
	  0x01, // payload (NMEA sentence ID char 2)
	  0x00, // payload I/O Target 0 - DDC       - (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 1 - Serial Port 1 - (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 2 - Serial Port 2 - (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 3 - USB       - (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 4 - SPI       - (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 5 - Reserved    - (1 - enable sentence, 0 - disable)
	  0x00, // CK_A
	  0x2A  // CK_B
	};

	byte turn_Off_GPGSA[NMEA_LEN] = {//---------------------------------------------//GPGSA off
	  0xB5, // Header char 1
	  0x62, // Header char 2
	  0x06, // class
	  0x01, // id
	  0x08, // length char 1
	  0x00, // length char 2
	  0xF0, // payload (NMEA sentence ID char 1)
	  0x02, // payload (NMEA sentence ID char 2)
	  0x00, // payload I/O Target 0 - DDC			- (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 1 - Serial Port 1 - (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 2 - Serial Port 2 - (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 3 - USB			- (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 4 - SPI			- (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 5 - Reserved		- (1 - enable sentence, 0 - disable)
	  0x01, // CK_A
	  0x31  // CK_B
	};

	byte turn_Off_GPGLV[NMEA_LEN] = {//---------------------------------------------//GPGSV off
	  0xB5, // Header char 1
	  0x62, // Header char 2
	  0x06, // class
	  0x01, // id
	  0x08, // length char 1
	  0x00, // length char 2
	  0xF0, // payload (NMEA sentence ID char 1)
	  0x03, // payload (NMEA sentence ID char 2)
	  0x00, // payload I/O Target 0 - DDC			- (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 1 - Serial Port 1 - (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 2 - Serial Port 2 - (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 3 - USB			- (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 4 - SPI			- (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 5 - Reserved		- (1 - enable sentence, 0 - disable)
	  0x02, // CK_A
	  0x38  // CK_B
	};

	byte turn_Off_GPRMC[NMEA_LEN] = {//---------------------------------------------//GPRMC off
	  0xB5, // Header char 1
	  0x62, // Header char 2
	  0x06, // class
	  0x01, // id
	  0x08, // length char 1
	  0x00, // length char 2
	  0xF0, // payload (NMEA sentence ID char 1)
	  0x04, // payload (NMEA sentence ID char 2)
	  0x00, // payload I/O Target 0 - DDC			- (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 1 - Serial Port 1 - (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 2 - Serial Port 2 - (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 3 - USB			- (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 4 - SPI			- (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 5 - Reserved		- (1 - enable sentence, 0 - disable)
	  0x03, // CK_A
	  0x3F  // CK_B
	};

	byte turn_Off_GPVTG[NMEA_LEN] = {//---------------------------------------------//GPVTG off
	  0xB5, // Header char 1
	  0x62, // Header char 2
	  0x06, // class
	  0x01, // id
	  0x08, // length char 1
	  0x00, // length char 2
	  0xF0, // payload (NMEA sentence ID char 1)
	  0x05, // payload (NMEA sentence ID char 2)
	  0x00, // payload I/O Target 0 - DDC			- (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 1 - Serial Port 1 - (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 2 - Serial Port 2 - (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 3 - USB			- (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 4 - SPI			- (1 - enable sentence, 0 - disable)
	  0x00, // payload I/O Target 5 - Reserved		- (1 - enable sentence, 0 - disable)
	  0x04, // CK_A
	  0x46  // CK_B
	};

	// <<---------------------------------------------------------------------------//on packets
	byte turn_On_GPGGA[NMEA_LEN] = {//---------------------------------------------//GPGGA on
	  0xB5, // Header char 1
	  0x62, // Header char 2
	  0x06, // class
	  0x01, // id
	  0x08, // length char 1
	  0x00, // length char 2
	  0xF0, // payload (NMEA sentence ID char 1)
	  0x00, // payload (NMEA sentence ID char 2)
	  0x01, // payload I/O Target 0 - DDC			- (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 1 - Serial Port 1 - (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 2 - Serial Port 2 - (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 3 - USB			- (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 4 - SPI			- (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 5 - Reserved		- (1 - enable sentence, 0 - disable)
	  0x05, // CK_A
	  0x38  // CK_B
	};

	byte turn_On_GPGLL[NMEA_LEN] = {//---------------------------------------------//GPGLL on
	  0xB5, // Header char 1
	  0x62, // Header char 2
	  0x06, // class
	  0x01, // id
	  0x08, // length char 1
	  0x00, // length char 2
	  0xF0, // payload (NMEA sentence ID char 1)
	  0x01, // payload (NMEA sentence ID char 2)
	  0x01, // payload I/O Target 0 - DDC			- (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 1 - Serial Port 1 - (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 2 - Serial Port 2 - (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 3 - USB			- (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 4 - SPI			- (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 5 - Reserved		- (1 - enable sentence, 0 - disable)
	  0x06, // CK_A
	  0x3F  // CK_B
	};

	byte turn_On_GPGSA[NMEA_LEN] = {//---------------------------------------------//GPGSA on
	  0xB5, // Header char 1
	  0x62, // Header char 2
	  0x06, // class
	  0x01, // id
	  0x08, // length char 1
	  0x00, // length char 2
	  0xF0, // payload (NMEA sentence ID char 1)
	  0x02, // payload (NMEA sentence ID char 2)
	  0x01, // payload I/O Target 0 - DDC			- (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 1 - Serial Port 1 - (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 2 - Serial Port 2 - (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 3 - USB			- (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 4 - SPI			- (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 5 - Reserved		- (1 - enable sentence, 0 - disable)
	  0x07, // CK_A
	  0x46  // CK_B
	};

	byte turn_On_GPGLV[NMEA_LEN] = {//---------------------------------------------//GPGSV on
	  0xB5, // Header char 1
	  0x62, // Header char 2
	  0x06, // class
	  0x01, // id
	  0x08, // length char 1
	  0x00, // length char 2
	  0xF0, // payload (NMEA sentence ID char 1)
	  0x03, // payload (NMEA sentence ID char 2)
	  0x01, // payload I/O Target 0 - DDC			- (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 1 - Serial Port 1 - (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 2 - Serial Port 2 - (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 3 - USB			- (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 4 - SPI			- (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 5 - Reserved		- (1 - enable sentence, 0 - disable)
	  0x08, // CK_A
	  0x4D  // CK_B
	};

	byte turn_On_GPRMC[NMEA_LEN] = {//---------------------------------------------//GPRMC on
	  0xB5, // Header char 1
	  0x62, // Header char 2
	  0x06, // class
	  0x01, // id
	  0x08, // length char 1
	  0x00, // length char 2
	  0xF0, // payload (NMEA sentence ID char 1)
	  0x04, // payload (NMEA sentence ID char 2)
	  0x01, // payload I/O Target 0 - DDC			- (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 1 - Serial Port 1 - (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 2 - Serial Port 2 - (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 3 - USB			- (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 4 - SPI			- (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 5 - Reserved		- (1 - enable sentence, 0 - disable)
	  0x09, // CK_A
	  0x54  // CK_B
	};

	byte turn_On_GPVTG[NMEA_LEN] = {//---------------------------------------------//GPVTG on
	  0xB5, // Header char 1
	  0x62, // Header char 2
	  0x06, // class
	  0x01, // id
	  0x08, // length char 1
	  0x00, // length char 2
	  0xF0, // payload (NMEA sentence ID char 1)
	  0x05, // payload (NMEA sentence ID char 2)
	  0x01, // payload I/O Target 0 - DDC			- (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 1 - Serial Port 1 - (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 2 - Serial Port 2 - (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 3 - USB			- (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 4 - SPI			- (1 - enable sentence, 0 - disable)
	  0x01, // payload I/O Target 5 - Reserved		- (1 - enable sentence, 0 - disable)
	  0x0A, // CK_A
	  0x5B  // CK_B
	};
	// <<---------------------------------------------------------------------------//CFG-MSG packets

	// <<---------------------------------------------------------------------------//CFG-RATE packets:
	byte updateFreq[FREQ_LEN] = {
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
	// <<---------------------------------------------------------------------------//CFG-RATE packets

	// <<---------------------------------------------------------------------------//CFG-PRT packets:
	byte changeBaud[BAUD_LEN] = {
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
	// <<---------------------------------------------------------------------------//CFG-PRT packets

	


	//index for buffer
	byte buffIndex = 0;




	//https://www.arduino.cc/en/Tutorial/SerialEvent
	friend void serialEvent1();
	friend void serialEvent2();
	friend void serialEvent3();
	friend void serialEvent4();
	friend void serialEvent5();
	friend void serialEvent6();

	//setup GPS and load non-default configuration settings
	void setupGPS();

	//send a set of packets to the receiver to disable NMEA messages
	void disableNmea();

	//send a set of packets to the receiver to enabel NMEA messages
	void enableNmea();

	//send a packet to the receiver to change baudrate to 115200
	void changeBaudrate();

	//send a packet to the receiver to change frequency to 100 ms
	void changeFrequency();

	//send the packet specified to the receiver.
	void writeConfigPacket(byte packet[], byte len);

	//extract lat and lon data from the GPS stream buffer
	int extractLatLong(byte startingIndex);
};

//create GPS class http://forum.arduino.cc/index.php?topic=163004.0
extern neo6mGPS myGPS;

#endif
