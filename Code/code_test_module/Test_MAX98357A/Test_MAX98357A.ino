/*
  Simple Internet Radio Demo
  esp32-i2s-simple-radio.ino
  Simple ESP32 I2S radio
  Uses MAX98357 I2S Amplifier Module
  Uses ESP32-audioI2S Library - https://github.com/schreibfaul1/ESP32-audioI2S
 
  DroneBot Workshop 2022
  https://dronebotworkshop.com
*/
 
// Include required libraries
#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"
 
// Define I2S connection
#define I2S_DOUT  22
#define I2S_BCLK  26
#define I2S_LRC   25

#define button_play 14
//#define stop 33

char announce[200];
 
// Create audio object
Audio audio;
 
// Wifi Credentials
String ssid =    "Be Kind Home";
String password = "tothon1%";
 
void setup() {
 
  // Start Serial Monitor
  Serial.begin(115200);
  pinMode(button_play,INPUT);
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
 
  // Connect MAX98357 I2S Amplifier Module
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  // Set thevolume (0-100)
  audio.setVolume(50);

}
void loop()
{
  // Run audio player  
  audio.loop();  
  if(digitalRead (button_play)==1) Play();
  //if(digitalRead (stop)==1) audio.stopSong();  
}
void Play(){ 
  sprintf(announce, "Xin chào buổi chiều bây giờ là 3 giờ chiều");
  Serial.println(announce);
  audio.connecttospeech(announce, "vi");  
//  audio.connecttohost("http://iskatel.hostingradio.ru:8015/iskatel-320.aac");  // aac
//  audio.connecttohost("http://mcrscast.mcr.iol.pt/cidadefm");                  // mp3
//  audio.connecttohost("http://www.wdr.de/wdrlive/media/einslive.m3u");         // m3u
//  audio.connecttohost("https://stream.srg-ssr.ch/rsp/aacp_48.asx");            // asx
//  audio.connecttohost("http://tuner.classical102.com/listen.pls");             // pls
//  audio.connecttohost("http://stream.radioparadise.com/flac");                 // flac
//  audio.connecttohost("http://stream.sing-sing-bis.org:8000/singsingFlac");    // flac (ogg)
//  audio.connecttohost("http://s1.knixx.fm:5347/dein_webradio_vbr.opus");       // opus (ogg)
//  audio.connecttohost("http://26373.live.streamtheworld.com:3690/XHQQ_FMAAC/HLSTS/playlist.m3u8");  // HLS
//  audio.connecttohost("http://eldoradolive02.akamaized.net/hls/live/2043453/eldorado/master.m3u8"); // HLS (ts)
//  audio.connecttoFS(SD, "/test.wav");     // SD
//  audio.connecttoFS(SD_MMC, "/test.wav"); // SD_MMC
//  audio.connecttoFS(SPIFFS, "/test.wav"); // SPIFFS
//  audio.connecttospeech("Wenn die Hunde schlafen, kann der Wolf gut Schafe stehlen.", "de"); // Google TTS
   
  delay(1000);
}


