#include <WiFi.h>
#include "global_vars.h"
#include "i2s_play_memo.h"

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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(button_record, INPUT);
  pinMode(button_stop, INPUT);

  pinMode (pinA,INPUT);
  pinMode (pinB,INPUT);

  // WiFi Connected
  connect_wifi();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  //initialise the SD card. (connects to SPI)
  SDInit();

  //initialise the wave header for recording. Note FLASH_RECORD_SIZE is default for 20s.
  wavHeader(header, FLASH_RECORD_SIZE);
  
  clear_i2s_driver();
  
  pinALast = digitalRead(pinA);
  attachInterrupt(digitalPinToInterrupt(pinA), Encoder_volume, CHANGE);
  
}

void loop() {
  if(digitalRead(button_record)!=0) i2s_record(); 
  if(Firebase.getInt(fbdo, "/Todo_List/play"))
  {
    play = fbdo.intData();
    while(play--!=0){  
      Serial.println("play: " + String(fbdo.intData()));       
      i2s_play();
      Firebase.setDouble(fbdo, "/Todo_List/play",play);
    };    
  }else {
    Serial.println("Download fail"); 
  }    
}