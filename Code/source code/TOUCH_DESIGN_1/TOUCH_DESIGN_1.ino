// Touch handling for XPT2046 based screens is handled by
// the TFT_eSPI library.

// Calibration data is stored in SPIFFS so we need to include it

#include "FS.h"
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include "Free_Fonts.h"
#include <WiFi.h>
#include <FirebaseESP32.h>

// Invoke custom library

#define CALIBRATION_FILE "/TouchCalData3"
#define REPEAT_CAL true
#define BLACK_SPOT

#define button_record 14
#define button_stop 12

#define WIFI_SSID "Thao"
#define WIFI_PASSWORD "11113333"

#define FIREBASE_HOST "https://todo-list-726ea-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "R8ltARJHs9zv803gr6JnPvmcEVLE7rFxt5jHt0Xr"

TFT_eSPI tft = TFT_eSPI(); 
uint16_t x = 0, y = 0; // To store the touch coordinates
bool pressed;
int current_screen = 0;
char note[200];
char hour[2];
char minute[2];
int play;

FirebaseData fbdo;

void setup(void) {
  Serial.begin(115200);
  pinMode(button_record, OUTPUT);
  pinMode(button_stop, OUTPUT);

  connect_wifi();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  tft.begin();
  tft.setRotation(1);
  tft.init();
  tft.fillScreen(TFT_BLACK);

}

void connect_wifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(100);
}


void Main()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setFreeFont(FF48);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Start", 160, 40);
  tft.fillRect(20, 100, 120, 80, TFT_PINK);
  tft.fillRect(180, 100, 120, 80, TFT_PINK);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(FSS9);
  tft.drawString("Record", 80, 140);
  tft.drawString("Announce", 240, 140);

  delay(400);
  while(current_screen == 0){ 
    pressed = tft.getTouch(&x, &y);
    if (pressed && x > 180 && x < 300 && (240 - y) > 100 && (240 - y) < 180) {
      current_screen = 1;
    }

    if (pressed && x > 20 && x < 140 && (240 - y) > 100 && (240 - y) < 180) {
      current_screen = 2;
    }
    delay(100);
    if(Firebase.getInt(fbdo, "/Todo_List/play"))
    {
      if(fbdo.intData()!=0){  
        current_screen = 1;
      };    
    }else {
      Serial.println("Download fail"); 
    } 
  }
}

void NameList(){
  tft.fillScreen(TFT_BLACK);
  tft.fillCircle(10, 10, 2, TFT_WHITE);
  tft.fillCircle(20, 10, 2, TFT_WHITE);
  tft.fillCircle(30, 10, 2, TFT_WHITE);
  tft.setTextColor(TFT_PINK);
  tft.setTextSize(1);
  tft.setFreeFont(FF22);
  tft.drawString("Schedule", 160, 20);

  // Vẽ đường kẻ ngang cho tiêu đề
  tft.drawFastHLine(0, 40, 320, ILI9341_WHITE);
  tft.drawFastHLine(0, 80, 320, ILI9341_WHITE);
  tft.drawFastHLine(0, 200, 320, ILI9341_WHITE);

  // Vẽ tên cột
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(15, 70);
  tft.print("Time:");
  tft.setCursor(15, 110);
  tft.print("To-do List:");

  // Vẽ dữ liệu
  tft.setFreeFont(FF45);
  tft.fillRect(30, 202, 100, 38, TFT_RED); 
  tft.setCursor(43, 225);
  tft.printf("UPDATE");
  tft.fillRect(205, 202, 70, 38, TFT_RED);
  tft.setCursor(218, 225);
  tft.printf("STOP");

  tft.setCursor(100, 70);
  Firebase.getString(fbdo,"/Todo_List/hour", hour);
  tft.print(String(hour) + " : " );
  Firebase.getString(fbdo,"/Todo_List/minute", minute);
  tft.print(String(minute));

  tft.setCursor(150, 110);
  Firebase.getString(fbdo,"/Todo_List/Desc", note);
  tft.print(note);

  delay(400);
  while(current_screen == 1){
    pressed = tft.getTouch(&x, &y);
    if (pressed && x > 0 && x < 50 && (240 - y) > 0 && (240 - y) < 30) {
      current_screen = 0;
    }

    if (pressed && x > 205 && x < 275 && (240 - y) > 200 && (240 - y) < 240){
      digitalWrite(button_stop, 1);
      tft.fillRect(205, 202, 70, 38, TFT_GREEN);
      tft.setCursor(218, 225);
      tft.printf("STOP");
      delay(500);
      digitalWrite(button_stop, 0);
      tft.fillRect(205, 202, 70, 38, TFT_RED);
      tft.setCursor(218, 225);
      tft.printf("STOP");
    }
    if (pressed && x > 30 && x < 130 && (240 - y) > 200 && (240 - y) < 240){
      tft.fillRect(30, 202, 100, 38, TFT_GREEN); 
      tft.setCursor(43, 225);
      tft.printf("UPDATE");
      break;
    }
    delay(100); // thêm đoạn delay để giảm tải CPU
  }
}

void Recording(){
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setFreeFont(FF48);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Recording...", 160, 120);
  
  digitalWrite(button_record, 1);
  delay(5000);
  digitalWrite(button_record, 0);
  delay(16000);
  current_screen = 0;

}

void loop() {

  switch(current_screen){
    case 0: Main(); break;
    case 1: NameList(); break;
    case 2: Recording(); break;
  }  

}

