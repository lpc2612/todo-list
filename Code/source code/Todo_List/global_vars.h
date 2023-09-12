#ifndef ALL_DEF_H
#define ALL_DEF_H

#include <FirebaseESP32.h> 
//this file defines definitions for everything

#define WIFI_SSID "Thao"
#define WIFI_PASSWORD "11113333"

#define FIREBASE_HOST "https://todo-list-726ea-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "R8ltARJHs9zv803gr6JnPvmcEVLE7rFxt5jHt0Xr"

#define button_record 14
#define button_stop 12

const int headerSize = 44;
byte header[headerSize];

char filename[100];
int n = 0;
String recordname;
char note[200];
int play;
int volume = 10;

FirebaseData fbdo;

#endif
