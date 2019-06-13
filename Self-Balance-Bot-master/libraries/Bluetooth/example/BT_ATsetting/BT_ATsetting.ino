#include "bluetooth.h"
// constant for serial
#define SERIAL_BAUD_RATE  38400
// constant for bluetooth
#define BT_RX 8
#define BT_TX 9
//#define BT_SPECIFICATION "HC05"
#define BT_SPECIFICATION "HC06"

// create devices
Bluetooth myBT(BT_RX, BT_TX, BT_SPECIFICATION);

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  // Terminal setting
  // BT_SPECIFICATION: HC05 -> Both NL & CR
  // BT_SPECIFICATION: HC06 -> No line ending
  myBT.Begin();
  Serial.println("Ready!");

  // the name and the pin coed are changed in AT mode only
  //Serial.write(myBT.CheckATMode());
  if (myBT.CheckATMode()) {
    Serial.write("In AT mode.\n");
    Serial.write(myBT.setPinCode("2018"));
    Serial.write(myBT.setName("mars06"));
    Serial.write("set over\n");
    delay(1000);
    // no any meaning for HC06 because there is command to response wanted message
    Serial.write(myBT.getPinCode());
    Serial.write(myBT.getName());
    Serial.write("read over");
  } else {
    Serial.write("Not in AT mode\n");
  }
}

void loop() {
  // communicates with bluetooth through serial com port
  if (myBT.available() > 0) {
    Serial.println(myBT.ReadString());
  }
  if (Serial.available() > 0) {
    myBT.WriteString(Serial.readString().c_str());
  }
}
