#ifndef ENCODER_H
#define ENCODER_H

#include "global_vars.h"

//encoder
#define pinA  33  // Connected to CLK on KY-040
#define pinB  32  // Connected to DT on KY-040
 
int pinALast;  
int aVal;
boolean bCW;

void Encoder_volume() {
   aVal = digitalRead(pinA);
   if (aVal != pinALast){ // Means the knob is rotating
     // if the knob is rotating, we need to determine direction
     // We do that by reading pin B.
     if (digitalRead(pinB) != aVal) {  // Means pin A Changed first - We're Rotating Clockwise
       if(volume<21) volume ++;
       bCW = true;
     } else {// Otherwise B changed first and we're moving CCW
       bCW = false;
       if(volume>0) volume--;
     }
     /*
     Serial.print ("Rotated: ");
     if (bCW){
       Serial.println ("clockwise");
     }else{
       Serial.println("counterclockwise");
     }
     */
     Serial.print("volume: ");
     Serial.println(volume);
     
   } 
   pinALast = aVal;
}

#endif