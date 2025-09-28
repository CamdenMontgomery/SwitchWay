#include <Arduino.h>
#include <BleGamepad.h>
#include <Wire.h>
#include <Adafruit_PN532.h>


//Define Controler Inputs
#define JOYSTICK_X 2
#define JOYSTICK_Y 15

#define ACCEL_BUTTON 4 //Map ACCEL_BUTTON -> Generic BUTTON_1
#define BRAKE_BUTTON 23 //Map BRAKE_BUTTON -> Generic BUTTON_2
#define DRIFT_BUTTON 18 //Map DRIFT_BUTTON -> Generic BUTTON_3
#define ITEM_BUTTON 19 //Map ITEM_BUTTON -> Generic BUTTON_4

//Define serial data and system clock pins for PN532 NFC reqader input
#define SDA_PIN 21
#define SCL_PIN 22



//Initialize Hardware
Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);                          //NFC Reader
BleGamepad bleGamepad("SwitchWay Z", "Afro Industries", 100);  //Gamepad Emulation
BleGamepadConfiguration bleGamepadConfig;


//Initialize Game Logic
#define AUTH_BYTES 0xCAB1  //Used To Verify The Scanned Target Is A SwitchWayz Car
uint16_t scanned_car_code = 0x0000;




void setup() {

  Serial.begin(115200);

  //Assign Pinmodes For Inputs
  Serial.println("Assigning Pins...");
  pinMode(JOYSTICK_X, INPUT);
  pinMode(JOYSTICK_Y, INPUT);
  pinMode(ACCEL_BUTTON, INPUT_PULLDOWN);
  pinMode(BRAKE_BUTTON, INPUT_PULLDOWN);
  pinMode(DRIFT_BUTTON, INPUT_PULLDOWN);
  pinMode(ITEM_BUTTON, INPUT_PULLDOWN);


  //Activate NFC Reader
  Serial.println("Searching For PN532 Board...");
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    //Error If No Board Attached
    Serial.print("Didn't find PN53x board");
    while (1)
      ;
  }

  Serial.print("Found chip PN5");
  Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. ");
  Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.');
  Serial.println((versiondata >> 8) & 0xFF, DEC);

  Serial.println("Waiting for an ISO14443A Card ...");

  nfc.SAMConfig();


  //[Warning] Re-Pair Device When Changing Config

  //Set Config For Bluetooth Device [Must Appear As Gamepad With Vid () & Pid ()]
  Serial.println("Configuring BLE...");
  bleGamepadConfig.setAutoReport(false);  //Turned Off To Avoid Spamming Updates From Each Action
  bleGamepadConfig.setControllerType(CONTROLLER_TYPE_GAMEPAD);
  bleGamepadConfig.setVid(0x8971);  //[MUST BE CONSISTENT]
  bleGamepadConfig.setPid(0xB101);  //[MUST BE CONSISTENT]


  //Set Axes To Access Full Range Of 2 Bytes | Used For Transporting The Car Switch Code (2 Byte Hex)
  bleGamepadConfig.setAxesMin(0x0000);
  bleGamepadConfig.setAxesMax(0xFFFF);

  //Begin Advertising as Gamepad
  Serial.println("Advertising...");
  bleGamepad.begin(&bleGamepadConfig);
}

void loop() {

  //Check For Connection First To Avoid Performing Reads Unneccesarily
  if (bleGamepad.isConnected()) {

    //Update Accel Button Press State On Gamepad To Reflect Actual State
    bool accel_is_pressed = digitalRead(ACCEL_BUTTON);
    if (accel_is_pressed && !bleGamepad.isPressed(BUTTON_1)) {bleGamepad.press(BUTTON_1);}
    if (!accel_is_pressed && bleGamepad.isPressed(BUTTON_1)) {bleGamepad.release(BUTTON_1);}


    //Update Brake Button Press State On Gamepad To Reflect Actual State
    bool brake_is_pressed = digitalRead(BRAKE_BUTTON);
    if (brake_is_pressed && !bleGamepad.isPressed(BUTTON_2)) { bleGamepad.press(BUTTON_2);}
    if (!brake_is_pressed && bleGamepad.isPressed(BUTTON_2)) { bleGamepad.release(BUTTON_2);}


    //Update Drift Button Press State On Gamepad To Reflect Actual State
    bool drift_is_pressed = digitalRead(DRIFT_BUTTON);
    if (drift_is_pressed && !bleGamepad.isPressed(BUTTON_3)) { bleGamepad.press(BUTTON_3);}
    if (!drift_is_pressed && bleGamepad.isPressed(BUTTON_3)) { bleGamepad.release(BUTTON_3);}


    //Update Item Button Press State On Gamepad To Reflect Actual State
    bool item_is_pressed = digitalRead(ITEM_BUTTON);
    if (item_is_pressed && !bleGamepad.isPressed(BUTTON_4)) { bleGamepad.press(BUTTON_4);}
    if (!item_is_pressed && bleGamepad.isPressed(BUTTON_4)) { bleGamepad.release(BUTTON_4);}


    //Read Car Switch Code From PN532 NFC Reader
    scanned_car_code = 0x0000;                //Reset car code
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)



    uint8_t tag_was_read = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 100/*10 ms timeout*/);

    if (tag_was_read) {

      Serial.println("Found an ISO14443A tag");
      Serial.print("  UID Length: ");
      Serial.print(uidLength, DEC);
      Serial.println(" bytes");
      Serial.print("  UID Value: ");
      nfc.PrintHex(uid, uidLength);

      uint8_t data[32];  //32 bytes available to store NFC bytes

      uint8_t success = nfc.ntag2xx_ReadPage(4, data);  //Read fourth page, this is where we store the authenticator and identifier 4 digit hexes
      if (success) {

        nfc.PrintHexChar(data, 4);
        Serial.println("Checking Authenticator Bytes");
        Serial.println(data[0] == 0xCA ? "CA Found" : "CA Not Found");
        Serial.println(data[1] == 0xB1 ? "B1 Found" : "B1 Not Found");

        //If Authenticated, Update
        if (data[0] == 0xCA && data[1] == 0xB1) {

          Serial.println("Validator Found");
          int code = (data[2] << 8) | data[3];
          Serial.println("Car Code:");
          Serial.println(code);
          scanned_car_code = code;
        }
      }


    }



      //Update Joystick Positions | & Faux Joystick [Car Code]
      uint16_t xaxis_val = analogRead(JOYSTICK_X);
      uint16_t yaxis_val = analogRead(JOYSTICK_Y);
      bleGamepad.setAxes(map(xaxis_val, 0, 4095, -32767, 32767), map(yaxis_val, 0, 4095, -32767, 32767), scanned_car_code /*Car Switch Code as Joystick*/);



      //For Demo Surposes Use Buttons To Swap
      bleGamepad.release(BUTTON_5);
      bleGamepad.release(BUTTON_6);
      if (scanned_car_code == 0xA60F) { Serial.println("Offroad"); bleGamepad.press(BUTTON_5);}
      if (scanned_car_code == 0xA60E) { Serial.println("Onroad"); bleGamepad.press(BUTTON_6); }

      bleGamepad.sendReport();
  }
}
