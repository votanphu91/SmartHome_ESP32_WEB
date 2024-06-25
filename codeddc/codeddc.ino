#include <WiFi.h>
#include "DHT.h"
#include "FirebaseESP32.h"
//----------lcd làm điều hòa
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); 



#define ledPin 25       // Chân kết nối LED gass báo
#define den1 4            // phòng khách
#define den2 26           // phòng ngủ
#define den3 17           // phòng tắm
#define den4 5            // phòng thờ
#define den5 18           // phòng bếp
#define quat 19
#define gasSensorPin 16
#define sensor 23
#define coi 12
#define anhsang 34
#define denas 32


//---------khai báo dht11-------------//
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//-----------------kết nối firebase-------------//
#define FIREBASE_HOST "smarthome-88f4f-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "WQC7BgU1GgJvERfJR80Qzkh3mxQAQRpoFYjXWQUj"
#define WIFI_SSID "Trieu"
#define WIFI_PASSWORD "123456789"
FirebaseData firebaseData;
float h, hO = 0;
float t, tO = 0;
float hic;
int nhd;
unsigned long timer = millis();

void setDataMyHome(float h, float t, float hic);

void setup() {
  Serial.begin(115200);
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

  Serial.println("Initializing temperature and humidity sensor");
  dht.begin();
  // ---------setup đèn với quạt đồ----------------//
  pinMode(den1, OUTPUT);
  pinMode(den2, OUTPUT);
  pinMode(den3, OUTPUT);
  pinMode(den4, OUTPUT);
  pinMode(den5, OUTPUT);
  pinMode(quat, OUTPUT);
  pinMode(coi, OUTPUT);
  pinMode(denas, OUTPUT);
  pinMode(anhsang, INPUT);
  pinMode(sensor, INPUT);
  //gass
  pinMode(ledPin, OUTPUT);   // Thiết lập chân kết nối LED là OUTPUT
  digitalWrite(ledPin, LOW);
  pinMode(gasSensorPin, INPUT);

  lcd.init();                    
  lcd.backlight();
  lcd.setCursor(4,0);
  lcd.print("DIEU HOA");
}
void cbdht11()
{
  if ((unsigned long)(millis() - timer) > 2000) {
    h = dht.readHumidity();
    t = dht.readTemperature();
    hic = dht.computeHeatIndex(t, h, false);

    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
    }
    timer = millis();
  }
  // Push values to Firebase
  if (abs(t - tO) > 0.1 || abs(h - hO) > 0.5) {
    setDataMyHome(h, t, hic);
    hO = h;
    tO = t;
  }
}
void setDataMyHome(float h, float t, float hic) {
  if (Firebase.setFloat(firebaseData, "/H", h)) {
    Serial.println("PASSED H");
    Serial.print("VALUE: ");
    Serial.print(firebaseData.floatData());
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
  }

  if (Firebase.setFloat(firebaseData, "/T", t)) {
    Serial.println("PASSED T");
    Serial.print("VALUE: ");
    Serial.print(firebaseData.floatData());
  } else {
    Serial.println("FAILED");
  }

  if (Firebase.setFloat(firebaseData, "/HIC", hic)) {
    Serial.println("PASSED HIC");
    Serial.print("VALUE: ");
    Serial.print(firebaseData.floatData());
  } else {
    Serial.println("FAILED");
  }
}
void batden(){
    // đèn 1---------------//
  if (Firebase.getString(firebaseData, "/thietbi/den1")) {
      String d1 = firebaseData.stringData();
      if (d1 == "ON") {
        digitalWrite(den1, HIGH);
      }
      else if (d1 == "OFF") {
        digitalWrite(den1, LOW);
      }
  }
    // đèn 2---------------//
  if (Firebase.getString(firebaseData, "/thietbi/den2")) {
    String d2 = firebaseData.stringData();
    if(d2 == "ON"){
    digitalWrite(den2, HIGH);
  }
  else if(d2 == "OFF"){
    digitalWrite(den2, LOW);
  }
  }
    // đèn 3---------------//
  if (Firebase.getString(firebaseData, "/thietbi/den3")) {
    String d3 = firebaseData.stringData();
    if(d3 == "ON"){
    digitalWrite(den3, HIGH);
  }
  else if(d3 == "OFF"){
    digitalWrite(den3, LOW);
    }
  }
    // đèn 4---------------//
  if (Firebase.getString(firebaseData, "/thietbi/den4")) {
    String d4 = firebaseData.stringData();
    if(d4 == "ON"){
    digitalWrite(den4, HIGH);
  }
  else if(d4 == "OFF"){
    digitalWrite(den4, LOW);
    }
  }
    // đèn 5---------------//
  if (Firebase.getString(firebaseData, "/thietbi/den5")) {
    String d5 = firebaseData.stringData();
    if(d5 == "ON"){
    digitalWrite(den5, HIGH);
  }
  else if(d5 == "OFF"){
    digitalWrite(den5, LOW);
    }
  }
    // quạt---------------//
  if (Firebase.getString(firebaseData, "/thietbi/quat")) {
    String q = firebaseData.stringData();
    if(q == "ON"){
    digitalWrite(quat, HIGH);
  }
  else if(q == "OFF"){
    digitalWrite(quat, LOW);
    }
  }
}
void security(){
  // cam bien gas--------------
   int gasValue = digitalRead(gasSensorPin);

  // Kiểm tra nếu giá trị vượt ngưỡng
  if (gasValue == LOW) {
    digitalWrite(quat, HIGH);
    digitalWrite(coi,HIGH);
    String gas = "ON";
    Firebase.setString(firebaseData, "/security/Gas", gas);
    String mocua = "ON";
    Firebase.setString(firebaseData, "/security/door", mocua);
    String q = "ON";
    Firebase.setString(firebaseData, "/thietbi/quat", q);
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(70);
  } 
  else {
    String gas = "OFF";
    Firebase.setString(firebaseData, "/security/Gas", gas);
    String q = "OFF";
    digitalWrite(coi,LOW);
  //  digitalWrite(quat, LOW);
  }
}
void phathientrom(){
      //------------cảm biến phát hiện trộm-----------
  int sensorValue = digitalRead(sensor);
  if( sensorValue == HIGH ){
    digitalWrite(coi,HIGH);
    delay(2000);
    // String lamp = "ON";
    // Firebase.setString(firebaseData, "/security/lamm", lamp);
  }
  else{
    digitalWrite(coi,LOW);
    // String lamp = "OFF";
    // Firebase.setString(firebaseData, "/security/lamm", lamp);
  }
  // if (Firebase.getString(firebaseData, "/sc/trom")) {
  //   String tr = firebaseData.stringData();
  //   if(tr == "ON"){
  //   if (sensorValue == LOW) {
  //     digitalWrite(coi,HIGH);
  //   } else {
  //     digitalWrite(coi,LOW);
  //   }
  //   }
  //   else{}
  // }
}
void dieuhoa(){
  if (Firebase.getString(firebaseData, "/dieuhoa/nhietdo"))
  nhd = firebaseData.intData();
  if (Firebase.getString(firebaseData, "/trangthai/dh")) {
    String dieuhoa = firebaseData.stringData();
    if(dieuhoa == "ON"){
    lcd.setCursor(1,1);
    lcd.print("NHIET DO: ");
    lcd.setCursor(12,1);
    lcd.print(nhd);
  }
  else if(dieuhoa == "OFF"){
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("DIEU HOA");
    lcd.setCursor(6,1);
    lcd.print("OFF");
  }
  }
}
void quantro(){
  int qt = digitalRead(anhsang); 
  if( qt== 1){
    digitalWrite(denas,HIGH);
    String lamp = "ON";
    Firebase.setString(firebaseData, "/security/lamm", lamp);
  }
  else{
    digitalWrite(denas,LOW);
    String lamp = "OFF";
    Firebase.setString(firebaseData, "/security/lamm", lamp);
  }
}
void loop() {
  cbdht11();
  // ------------hàm bật đèn với quạt-----//
  batden();
  security();
  dieuhoa();
  quantro();
  // phathientrom();
}
