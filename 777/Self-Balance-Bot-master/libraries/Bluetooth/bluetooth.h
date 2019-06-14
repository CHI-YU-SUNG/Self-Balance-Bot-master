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
#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

// Includes
#include <Arduino.h>
#include <SoftwareSerial.h>

// constant
#define BT_LONGEST_SIZE			40
#define BT_DEV_WRITE_WAIT		1200
#define HC_06_DEFAULT_BAUDRATE	9600
#define HC_05_DEFAULT_BAUDRATE	38400

class Bluetooth{
	private:
		SoftwareSerial*	_device;
		unsigned int	_RXpin;
		unsigned int	_TXpin;
		unsigned int	_baudrate;
		char*			_specification;  // "HC05" "HC06"
		char*			_devicename;
		char*			_pincode;
		char*			_buffer;

    void ClearMessageBuffer();

	public:
		Bluetooth(unsigned int RX = 2, unsigned int TX = 3, char specification[4]={"HC05"});
		~Bluetooth();
		void Begin();
		char* ReadString();
		void WriteString(const char *message);
		int available();
		// following functions are usable in AT mode only.
		bool CheckATMode();
		char* setName(const char *name);
		char* setPinCode(const char *pincode);
		char* getName();
		char* getPinCode();
};

#endif
