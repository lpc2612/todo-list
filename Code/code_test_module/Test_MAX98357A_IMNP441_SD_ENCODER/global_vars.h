#ifndef ALL_DEF_H
#define ALL_DEF_H

#include <WiFi.h>

//this file defines definitions for everything

const char filename[]="/recording_2.wav";

const int headerSize = 44;
byte header[headerSize];

int volume = 10;

char note[200];

const char *ssid = "Be Kind Home";
const char *password = "tothon1%";

#endif
