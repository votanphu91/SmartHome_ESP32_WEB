#include <WiFi.h>
#include "FirebaseESP32.h"


#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include <Keypad.h>
#include <EEPROM.h>
#define SS_PIN 5
#define RST_PIN 15
#define NUTNHAN 4

//-----------------kết nối firebase-------------//
#define FIREBASE_HOST "smarthome-88f4f-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "WQC7BgU1GgJvERfJR80Qzkh3mxQAQRpoFYjXWQUj"
#define WIFI_SSID "Trieu"
#define WIFI_PASSWORD "123456789"
FirebaseData firebaseData;

unsigned long timer = millis();

LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int servoPin = 2;
int biennho = 1;
String huynh = "OFF";
int button = 0;
bool tam = false;
Servo myServo;
const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 4;
byte rowPins[KEYPAD_ROWS] = {13, 12 , 14, 27};
byte colPins[KEYPAD_COLS] = {26, 25, 33, 32};
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);
int passwordAddress = 0;
String password = "1234";
String enteredPassword = "";

void IRAM_ATTR handleButtonPress();
void Mocuaphong()
{
  huynh = "ON";
  Firebase.setString(firebaseData, "/security/door", huynh);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TIME CLOSE: ");
  myServo.write(0);
  for(int i=9; i> 0; i--){
      lcd.setCursor(13, 0);
      lcd.print(i);
      delay(1000);
  }
  lcd.clear();
  lcd.setCursor(3, 0);
  myServo.write(90);
  lcd.print("CLOSE DOOR");
  delay(1000);
  enteredPassword = "";
  tam = true;
  lcd.clear();
  huynh = "OFF";
  Firebase.setString(firebaseData, "/security/door", huynh);
}
void showSplashScreen() {
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight
  lcd.setCursor(3, 0);
  lcd.print("SMART HOME");
  lcd.setCursor(0, 1);
  String message = "    GROUP: 2    ";
  delay(200);
  for (byte i = 0; i < message.length(); i++) {
    lcd.print(message[i]);
    delay(100);
  }
  delay(1000);
}
void updatePassword() {
  lcd.clear();
  lcd.print("Enter New Pass:");
  lcd.setCursor(0, 1);
  String newPassword = "";
  while (newPassword.length() < 4) {
    char key = keypad.getKey();
    if (key) {
      if ('#' == key) {
        if (newPassword.length() > 0) {
          lcd.clear();
          lcd.print("Saving...");
          delay(1000);
          int address = passwordAddress;
          for (byte i = 0; i < newPassword.length(); i++) {
            EEPROM.write(address, newPassword[i]);
            address++;
          }
          for (byte i = newPassword.length(); i < 4; i++) {
            EEPROM.write(address, '\0');
            address++;
          }
          EEPROM.commit();
          lcd.clear();
          lcd.print("Password saved");
          delay(1000);
          lcd.clear();
          tam =true;
          password = newPassword;
          return;
        }
      }   
      else {
        newPassword += key;
        lcd.print(key);
      }
    }
  }
}
void processInput(char key) {
  int passwordIndex = 0;
  if ('#' == key) {
    if (enteredPassword == password) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" TRUE PASSWORD  ");
      delay(2000);
      lcd.setCursor(0, 1);
      lcd.print(" OPEN THE DOOR  ");
      Mocuaphong();
      tam = true;
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" WRONG PASSWORD ");
      delay(2000);
      lcd.setCursor(0, 1);
      lcd.print("    GET AWAY    ");
      enteredPassword = "";
      lcd.clear();
      tam = true;
    }
  } 
  else if ('A' == key) 
  { 
    lcd.clear();
    lcd.print("  RETURN RFID  ");
    delay(1500);
    lcd.clear();
    lcd.print("   SMART HOME   ");
    lcd.setCursor(0,1);
    lcd.print("SCAN YOUR CARD");
    button = 0;
  } 
  else if ('B' == key) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("NHAP PASS CU: ");
    lcd.setCursor(0, 1);
    bool tam1 = true;
    while (tam1) {
      char key = keypad.getKey();
      if (key) {
        if ('#' == key) {
          if (enteredPassword == password) {
            updatePassword();
            tam1 = false;
          }
        }
        else if ('*' == key) {
          if (enteredPassword.length() > 0) {
            enteredPassword = enteredPassword.substring(0, enteredPassword.length() - 1);
            lcd.setCursor(enteredPassword.length(), 1);
            lcd.print(" ");
          }
        }
        else {
          enteredPassword += key;
          lcd.setCursor(enteredPassword.length() - 1, 1);
          lcd.print(key);
        }
      }
    }
  }
  else if('*'== key){
    if (enteredPassword.length() > 0) 
    {
      enteredPassword = enteredPassword.substring(0, enteredPassword.length() - 1);
      lcd.setCursor(enteredPassword.length(), 1);
      lcd.print(" ");
    }
  }
  else {
    enteredPassword += key;
    lcd.setCursor(enteredPassword.length() - 1, 1);
    lcd.print(key); // Display the entered number instead of '*'
  }
}
void IRAM_ATTR handleButtonPress()
{
  button = 1;
  tam = true;
}
void setup() 
{
  attachInterrupt(NUTNHAN, handleButtonPress, FALLING);
  myServo.attach(servoPin);
  lcd.init();
  lcd.backlight();
  pinMode(NUTNHAN, INPUT_PULLUP);
  Serial.begin(115200);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  showSplashScreen();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("      SMART HOME   ");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("  SCAN YOUR CARD");
  //setup wifi với firebase
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}
void doccua()
{
  if ((unsigned long)(millis() - timer) > 1000) {
    if (Firebase.getString(firebaseData, "/security/door")) {
    String cua = firebaseData.stringData();
    if(cua == "ON"){
    myServo.write(0);
    }
    else if(cua == "OFF"){
      myServo.write(90);
      }
    }
    timer = millis();
  }
}
void loop() 
{ 
  doccua();
  if (button == 0)
  {
    if ( ! mfrc522.PICC_IsNewCardPresent()) 
    {
      return;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) 
    {
      return;
    }
    Serial.print("UID tag :");
    String content= "";
    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.print("Message : ");
    content.toUpperCase();
    if (content.substring(1) == "73 02 CF A7") //change here the UID of the card/cards that you want to give access
    { 
      if (biennho == 1){
        huynh = "ON";
        lcd.clear();
        biennho = 0;
        myServo.write(0);
        Serial.println("Authorized access");
        Serial.println();
        lcd.setCursor(0,0);
        lcd.print("   TRUE CARD    ");
        delay(1000);
        lcd.setCursor(0,1);
        lcd.print("WELCOME TO IHOME");
        Firebase.setString(firebaseData, "/security/door", huynh);
      }
      else if (biennho == 0){
        huynh = "OFF";
        lcd.clear();
        biennho = 1;
        myServo.write(90);
        Serial.println("Authorized access");
        Serial.println();
        lcd.setCursor(0,0);
        lcd.print("   TRUE CARD    ");
        delay(1000);
        lcd.setCursor(0,1);
        lcd.print("   CLOSE DOOR   ");
        delay(2500);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(" SMART HOME   ");
        delay(1000);
        lcd.setCursor(0,1);
        lcd.print("SCAN YOUR CARD");
        Firebase.setString(firebaseData, "/security/door", huynh);
      }
    }
  else{
      lcd.clear();
      Serial.println(" Access denied");
      lcd.setCursor(0,0);
      lcd.print("  WRONG CARD    ");
      delay(1000);
      lcd.setCursor(0,1);
      lcd.print("  Access denied ");
      delay(2500);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("   SMART HOME   ");
      delay(1000);
      lcd.setCursor(0,1);
      lcd.print("SCAN YOUR CARD");
    }
  }
  if (button)
  {
    if(tam)
    {
      lcd.clear(); 
      lcd.setCursor(0,0);
      lcd.print("ENTER PASS:");
      tam = false;
      lcd.setCursor(0,1);
    }
    char key = keypad.getKey();
    if (key) 
    {
      processInput(key);
    }
  }
} 




