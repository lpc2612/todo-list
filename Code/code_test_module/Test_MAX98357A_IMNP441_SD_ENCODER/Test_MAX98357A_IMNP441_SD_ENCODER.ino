
#include "global_vars.h"
#include "i2s_play_memo.h"

#define button_record 14
#define button_play 12


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(button_record, INPUT);
  pinMode(button_play, INPUT);

  pinMode (pinA,INPUT);
  pinMode (pinB,INPUT);

  // WiFi Connected
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(100);
  
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
  if(digitalRead(button_play)!=0) i2s_play(); 
}