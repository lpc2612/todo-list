// most of the code snippets in this file are copied from
//    http://www.xtronical.com/i2s-ep3

#ifndef I2S_PLAY_
#define I2S_PLAY_

#include "i2s_record_memo.h"
#include "Audio.h"
#include "encoder.h"

Audio audio;

// I2S Connections
#define I2S_DOUT  22
#define I2S_BCLK  26
#define I2S_LRC   25

#define NUM_BYTES_TO_READ_FROM_FILE 1024 

static const i2s_port_t i2s_num = I2S_NUM_0;  // i2s port number    

void i2sInit_play(){
  static const i2s_config_t i2s_config = 
  {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
      .sample_rate = 44100,                                 // Note, this will be changed later
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
      .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,             // high interrupt priority
      .dma_buf_count = 8, //8,                                   // 8 buffers
      .dma_buf_len = 64, //64,                                    // 64 bytes per buffer, so 8K of buffer space
      .use_apll=0,
      .tx_desc_auto_clear= true, 
      .fixed_mclk=0    
  };
      
  // These are the physical wiring connections to our I2S decoder board/chip from the esp32, there are other connections
  // required for the chips mentioned at the top (but not to the ESP32), please visit the page mentioned at the top for
  // further information regarding these other connections.
      
  static const i2s_pin_config_t pin_config = 
  {
      .bck_io_num = I2S_BCLK,                           // The bit clock connectiom, goes to pin 27 of ESP32
      .ws_io_num = I2S_LRC,                             // Word select, also known as word select or left right clock
      .data_out_num = I2S_DOUT,                         // Data out from the ESP32, connect to DIN on 38357A
      .data_in_num = I2S_PIN_NO_CHANGE                  // we are not interested in I2S data into the ESP32
  };

  i2s_driver_install(i2s_num, &i2s_config, 0, NULL);
  i2s_set_pin(i2s_num, &pin_config);

}




//------------------------------------------------------------------------------------------------------------------------

//  Global Variables/objects    
    
//File file;                                 // Object for root of SD card directory

uint32_t BytesReadSoFar_1 = 1;
void i2s_play(){

  //install the its driver
  i2sInit_play();

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(volume); // 0...21
  bool isrunning=true;

  if(Firebase.getString(fbdo, "/Todo_List/mode"))
  {
    Serial.println("mode: " + String(fbdo.stringData()));
    if(fbdo.stringData()=="1"){  
      Firebase.getString(fbdo,"/Todo_List/Desc",filename);
      audio.connecttoFS(SD, filename); 
    }else{
      if(fbdo.stringData()=="0"){
        Firebase.getString(fbdo,"/Todo_List/Desc",note);
        audio.connecttospeech(note, "vi");       
      };
    };
  }else {
    Serial.println("fail: " + String(fbdo.stringData())); 
  }     

  while(isrunning){
    audio.loop();
    audio.setVolume(volume);
    if(digitalRead(button_stop)) {
      audio.stopSong(); 
      play = 0;
    };    
    if (!audio.isRunning()) {
      if(isrunning){ //give back semaphore
        isrunning = false;
      }
    }
  }

  //uninstall the i2s driver.
  clear_i2s_driver();  

}


#endif
