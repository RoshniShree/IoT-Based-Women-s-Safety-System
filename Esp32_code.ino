#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <esp_now.h>
#include <WiFi.h>

SoftwareSerial SIM900A(16,17);
const int button1=19;
int i=0;
int j=0;
const int button2=12;

LiquidCrystal_I2C lcd(0x27, 16, 2); 

// GPS setup
#define GPS_TX_PIN 27
#define GPS_RX_PIN 26
TinyGPSPlus gps;
unsigned long timestamp;
float lat = 0;
float lng = 0;
String message = "";
char phone_no[] = "+918********1";

// ESP-NOW setup
typedef struct test_struct {
  int a;
} test_struct;

test_struct test;

uint8_t broadcastAddress1[] = {0xFC, 0xE8, 0xC0, 0xCE, 0x6C, 0xD0}; // Replace with receiver MAC
esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("ESP-NOW Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

// Setup function
void setup() {
  SIM900A.begin(9600);
  Serial.begin(115200);
  Serial.println("Wait ...");

  // GPS Serial port
  Serial1.begin(9600, SERIAL_8N1, GPS_TX_PIN, GPS_RX_PIN);
  Serial.println ("Text Messege Module Ready & Verified");

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
 
  delay(100);

  // ESP-NOW init
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

// Main loop
void loop() {
  get_gps_data();
  i = digitalRead(button1);
  j = digitalRead(button2);
  
  Serial.print("button1:");
  Serial.println(i);
  Serial.print("button2:");
  Serial.println(j);

  if (i == HIGH) {
    // Send ESP-NOW signal

    Serial.println ("Sending Message please wait….");
    SIM900A.println("AT+CMGF=1");
    delay(1000);
    Serial.println ("Set SMS Number");
    SIM900A.println("AT+CMGS=\"+918********1\"\r");
    delay(1000);
    Serial.println ("Set SMS Content");
    SIM900A.println("Help,I am in danger");
    SIM900A.print("https://www.google.com/maps/place/");
    SIM900A.print(lat, 6);
    SIM900A.print(",");
    SIM900A.print(lng, 6);
    delay(100);
    Serial.println ("Done");
    SIM900A.println((char)26);
    Serial.println ("Message sent succesfully");

    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Message Sent!");
    delay(20000);

    Serial.println("Initializing....");
    initModule("AT", "OK", 1000);
    callUp(phone_no);
    delay(10000);

   /* if (SIM900A.available() > 0) {
      lcd.clear();
      String message = "";
      int newlineCount = 0;
      bool messageStart = false;
      while (SIM900A.available()) {
        char c = SIM900A.read();
        if (c == '\n') {
          newlineCount++;
          if (newlineCount == 2) {
            messageStart = true;
            continue;
          }
        }
        if (messageStart) {
          message += c;
        }
        delay(10);
      }
      lcd.setCursor(0, 0);
      lcd.print(message.substring(0, 16));
      lcd.setCursor(0, 1);
      if (message.length() > 16) {
        lcd.print(message.substring(16, 32));
      }

    }
    lcd.clear();*/
    test.a = 1;
    esp_err_t result = esp_now_send(broadcastAddress1, (uint8_t *) &test, sizeof(test_struct));
    if (result == ESP_OK) {
      Serial.println("ESP-NOW signal sent successfully");
    } else {
      Serial.println("ESP-NOW signal failed");
    }

    // GSM + GPS functions
    
  }

  if (j == HIGH) {
    Serial.println ("Sending Message please wait….");
    SIM900A.println("AT+CMGF=1");
    delay(1000);
    Serial.println ("Set SMS Number");
    SIM900A.println("AT+CMGS=\"+918********1\"\r");
    delay(1000);
    Serial.println ("Set SMS Content");
    SIM900A.println("Sorry,sent by mistake");
    delay(100);
    Serial.println ("Done");
    SIM900A.println((char)26);
    Serial.println ("Message sent succesfully");
  }

  delay(2000);
}

// Function to get GPS data
void get_gps_data() {
  Serial.println("Getting GPS data: ");
  while (Serial1.available() > 0) {
    gps.encode(Serial1.read());
  }
  if (gps.location.isValid()) {
    lat = gps.location.lat();
    lng = gps.location.lng();
    Serial.print("Latitude = ");
    Serial.println(lat, 6);
    Serial.print("Longitude = ");
    Serial.println(lng, 6);
  } else {
    Serial.println(F("Invalid GPS data"));
  }
  Serial.println();
}

void callUp(char *number) {
  SIM900A.print("ATD + "); 
  SIM900A.print(number); 
  SIM900A.println(";"); 
  delay(20000);
  SIM900A.println("ATH");
  delay(100);
}

void initModule(String cmd, char *res, int t) {
  while (1) {
    Serial.println(cmd);
    SIM900A.println(cmd);
    delay(100);
    while (SIM900A.available() > 0) {
      if (SIM900A.find(res)) {
        Serial.println(res);
        delay(t);
        return;
      } else {
        Serial.println("Error");
      }
    }
    delay(t);
  }
}

void ReceiveMessage() {
  Serial.println("Receiving Messages");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Receiving...");
  SIM900A.println("AT+CNMI=2,2,0,0,0");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Msg Received");
}
