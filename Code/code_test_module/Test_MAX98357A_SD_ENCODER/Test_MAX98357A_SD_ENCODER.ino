// Include required libraries
#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"
 
// Define I2S connection
#define I2S_DOUT  22
#define I2S_BCLK  26
#define I2S_LRC   25

//encoder
#define pinA  33  // Connected to CLK on KY-040
#define pinB  32  // Connected to DT on KY-040

// microSD Card Reader connections
#define SD_CS          5  
#define SPI_MOSI      23 
#define SPI_MISO      19 
#define SPI_SCK       18 

#define button 14
#define stop 12

//char announce[200];
 
// Create audio object
Audio audio;
 
// Wifi Credentials
String ssid =    "Be Kind Home";
String password = "tothon1%";

int encoderPosCount = 10; 
int pinALast;  
int aVal;
boolean bCW;
 
void setup() {
 
  // Start Serial Monitor
  Serial.begin(115200);
  pinMode(stop,INPUT);
  pinMode(button,INPUT);  
  pinMode (pinA,INPUT);
  pinMode (pinB,INPUT);
//  pinMode(stop,INPUT);
 
  // Setup WiFi in Station mode
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str()); 
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  // WiFi Connected, print IP to serial monitor
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");

  if(!SD.begin(SD_CS))
  {
    Serial.println("Error accessing microSD card!");
    while(true);
  }
  Serial.println("accessing microSD card!");
 
  // Connect MAX98357 I2S Amplifier Module
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(encoderPosCount);// default 0...21

  pinALast = digitalRead(pinA);
  attachInterrupt(digitalPinToInterrupt(pinA), encoder_isr, CHANGE);   

}
void loop()
{
  // Run audio player  
  audio.loop();  
  if(digitalRead (button)==1) Play();
  if(digitalRead (stop)==1) Stop();
}
void encoder_isr() {
   aVal = digitalRead(pinA);
   if (aVal != pinALast){ // Means the knob is rotating
     // if the knob is rotating, we need to determine direction
     // We do that by reading pin B.
     if (digitalRead(pinB) != aVal) {  // Means pin A Changed first - We're Rotating Clockwise
       if(encoderPosCount<21) encoderPosCount ++;
       bCW = true;
     } else {// Otherwise B changed first and we're moving CCW
       bCW = false;
       if(encoderPosCount>0) encoderPosCount--;
     }
     Serial.print ("Rotated: ");
     if (bCW){
       Serial.println ("clockwise");
     }else{
       Serial.println("counterclockwise");
     }
     Serial.print("Encoder Position: ");
     Serial.println(encoderPosCount);
     
   } 
   pinALast = aVal;
  audio.setVolume(encoderPosCount);
}
void Play(){ 
  Serial.println("play");  
  //sprintf(announce, "Xin chào buổi chiều bây giờ là 3 giờ chiều");
  //Serial.println(announce);
  //audio.connecttospeech(announce, "vi");  
  //audio.connecttohost("http://iskatel.hostingradio.ru:8015/iskatel-320.aac");  // aac
  audio.connecttoFS(SD, "/Gorgeous.mp3");     // SD
  //audio.connecttoFS(SD_MMC, "/test.wav"); // SD_MMC
  //audio.connecttoFS(SPIFFS, "/test.wav"); // SPIFFS
  //audio.connecttospeech(note, "vi"); // Google TTS
  //audio.connecttospeech(note, "vi"); // Google TTS      
  delay(500);
}
void Stop(){
  Serial.println("stop");
  audio.stopSong();  
  delay(500);
}



