/*
 *	This is an Arduino-based class written for the HC-06/HC-05 Bluetooth module.
 *	It depends on the sourece file of Arduino SoftwareSerial library.
 *  URL: https://www.arduino.cc/en/Reference/SoftwareSerial
 *
 *	Author: Jyun-Hua Jhu
 *
 *	Last Changed: 2018-11-14
 *
 *  ----------------------------------------
 *  bluetooth.h is placed under the MIT license
 *  Copyright (C) 2018 Jyun-Hua Jhu
 * 
 *  Permission is hereby granted, free of charge, to any person obtaining a 
 *  copy of this software and associated documentation files (the "Software"), 
 *  to deal in the Software without restriction, including without limitation 
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 *  and/or sell copies of the Software, and to permit persons to whom the 
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 *  DEALINGS IN THE SOFTWARE.
 *  ----------------------------------------
 */
#include "bluetooth.h"

/*
 *	Constructor of Bluetooth class
 *
 *	@param
 *  	RX 				: Number of the pin that shall be used as RX
 *      TX         	 	: Number of the pin that shall be used as TX
 *      specification   : The specification of the bluetooth module (HC06/HC05)
 */
Bluetooth::Bluetooth(unsigned int RX, unsigned int TX, char specification[4]) {

  // memory locate
  _specification = new char[4];
  _devicename = new char[BT_LONGEST_SIZE];
  _pincode = new char[4];  //unknow
  _buffer = new char[BT_LONGEST_SIZE];

  _RXpin = RX;
  _TXpin = TX;
  _specification = specification;

  if (strcmp(specification, "HC06") == 0)
    _baudrate = HC_06_DEFAULT_BAUDRATE;
  else
    _baudrate = HC_05_DEFAULT_BAUDRATE;

  _device = new SoftwareSerial(RX, TX);
}

/*
 *	Destructor of Bluetooth class
 */
Bluetooth::~Bluetooth() {

  delete _device;
  delete _specification;
  delete _pincode;
  delete _devicename;
  delete _buffer;
}

/*
 *	Initializes the bluetooth controller
 */
void Bluetooth::Begin() {

  _device->begin(_baudrate);
  delay(BT_DEV_WRITE_WAIT);
}

/*
 *	Checks is there any data in buffer
 *
 *	@return
 *  	The number of bytes remain in buffer
 */
int Bluetooth::available() {
  return _device->available();
}

/*
 *	Delete the remain messeges
 */
void Bluetooth::ClearMessageBuffer() {
  while (_device->available()) {
    _device->read();
  }
}

/*
 *	Reads the complete sentence from buffer
 *
 *	@return
 *  	The complete sentence
 */
char* Bluetooth::ReadString() {

  int index = 0;
  memset(_buffer, 0, sizeof(_buffer));
  while (_device->available() && index < BT_LONGEST_SIZE) {
    _buffer[index] = _device->read();
    if (_buffer[index] == '\n') {
      index++;
      break;
    }
    index++;
  }
  _buffer[index] = '\0';
  return _buffer;
}

/*
 *	Writes the complete sentence
 *
 *	@param
 *		message			:The complete sentence
 */
void Bluetooth::WriteString(const char *message) {
  _device->write(message);
}

/*
 *	Checks device mode
 *
 *	@return
 * 	 	boolean for success
 */
bool Bluetooth::CheckATMode() {

  if (strcmp(_specification, "HC06") == 0) {
    WriteString("AT");
  } else {
    WriteString("AT\r\n");
  }
  delay(BT_DEV_WRITE_WAIT);
  
  if (strncmp(ReadString(), "OK", 2) == 0)
    return true;
  else
    return false;
 
}

/*
 *	Sets the device name
 *
 *	@param
 *		name 			: the new name for device
 *
 *	@return
 *		The result message
 */
char* Bluetooth::setName(const char *newname) {

  if (strcmp(_specification, "HC06") == 0) {
    WriteString(strcat("AT+NAME", newname));
  } else {
    WriteString(strcat(strcat("AT+NAME=", newname), "\r\n"));
  }
  delay(BT_DEV_WRITE_WAIT);

  strcpy(_buffer, ReadString());
  ClearMessageBuffer();

  if (strncmp(_buffer, "OK", 2) == 0) {
    strcpy(_devicename, newname);
    return "Set name successfully\n";
  }
  else {
    return "Set name unsuccessfully\n";
  }
}

/*
 *	Sets the device pin code
 *
 *  @param
 *		pincode			:the new pin code for device
 *
 * 	@return
 *		The result message
 */
char* Bluetooth::setPinCode (const char *pincode) {

  if (strcmp(_specification, "HC06") == 0) {
    WriteString(strcat("AT+PIN", pincode));
  } else {
    WriteString(strcat(strcat("AT+PSWD=", pincode), "\r\n"));
  }
  delay(BT_DEV_WRITE_WAIT);

  strcpy(_buffer, ReadString());
  ClearMessageBuffer();

  if (strncmp(_buffer, "OK", 2) == 0) {
    strcpy(_pincode, pincode);
    return "Set pin code successfully\n";
  }
  else {
    return "Set pin code unsuccessfully\n";
  }
}

/*
 *  Gets the device name
 *
 *	@return
 *		The complete name or error message
 */
char* Bluetooth::getName() {

  if (strcmp(_specification, "HC06") == 0) {
    return strcat(_devicename,"\n");
  } else {
    WriteString("AT+NAME?\r\n");
    delay(BT_DEV_WRITE_WAIT);

    if (_device->available()) {
      strcpy(_buffer, ReadString());
      ClearMessageBuffer();
      return _buffer;
    } else {
      return "Read name ERROR\n";
    }
  }
}

/*
 *	Gets the device pin code
 *
 *	@return
 *		The complete name or error message
 */
char* Bluetooth::getPinCode() {

  if (strcmp(_specification, "HC06") == 0) {
    return strcat(_pincode,"\n");
  } else {
    WriteString("AT+PSWD?\r\n");
    delay(BT_DEV_WRITE_WAIT);

    if (_device->available()) {
      strcpy(_buffer, ReadString());
      ClearMessageBuffer();
      return _buffer;
    } else {
      return "Read name ERROR\n";
    }
  }
}
