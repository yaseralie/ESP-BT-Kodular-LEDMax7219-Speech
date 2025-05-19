/*  using ESP32 with bluetooth
    to select your board follow these steps.
    Tools → boards → scroll down to find ESP32 Dev Module → select the port
    Reference GPIO  https://www.botnroll.com/en/esp32/3639-wemos-d1-r32-w-esp32-uno-r3-pinout.html
    Wiring LED Max7219 in ESP32 Dev (Uno Board):
      - CLK : 18
      - CS  : 5
      - DIN : 23
*/
#include <BluetoothSerial.h>

// Pengaturan Bluetooth
BluetoothSerial SerialBT;

//Setup for LED dot Max7219==============================
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8   //I use 8 dot matrix
#define CS_PIN 5
// Hardware SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
//Setup for LED dot Max7219==============================

String msg = "-";;

//for input/output
int BUTTON_STOP = 26;
bool status_stop = false;

void setup() {
  Serial.begin(9600);
  P.begin();
  SerialBT.begin("ESP32"); // Nama bluetooth
  delay(500);

  //setup pin input
  pinMode(BUTTON_STOP, INPUT_PULLUP);

  Serial.println("Display LED Matrix from Kodular Using Bluetooth");
  delay(1000);
}

void loop() {
  // Read bluetooth
  processIncomingSerial();

  //check button
  int buttonStop = digitalRead(BUTTON_STOP);
  //buttonstop-----------------------------
  if (buttonStop == LOW )
  {
    if (status_stop == false)
    {
      //Show Stop
      status_stop = true;
      msg = "STOP";
      SerialBT.println(msg);
      Serial.println("STOP detected");
      delay(100);
    }
  }
  else if (buttonStop == HIGH)
  {
    if (status_stop == true)
    {
      status_stop = false;
      msg = "-";
      SerialBT.println(msg);
    }
  }
  //buttonstop-----------------------------
  
  //Show Text in LED Matrix
  if (msg.length() < 10) {
    P.setTextAlignment(PA_CENTER);
    P.print(msg.c_str());
  }
  else {
    if (P.displayAnimate())
      P.displayText(msg.c_str(), PA_LEFT, 70, 70, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  }
}

// INCOMING SERIAL DATA PROCESSING CODE----------------------------------------
void processIncomingSerial()
{
  if (SerialBT.available()) {
    // Baca pesan dari port serial
    msg = SerialBT.readStringUntil('\n');
    Serial.println(msg);

  }
}
