/**
 * @file LCDemo7Segment.ino
 * @author Noa Sakurajin (noasakurajin@web.de)
 * @brief using the ledcontroller with 7-segment displays
 * @version 0.1
 * @date 2020-12-30
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "LedController.hpp"
/*
 You might need to change the following 3 Variables depending on your board.
 pin 15 is connected to the DataIn
 pin 14 is connected to the CLK
 pin 13 is connected to LOAD/ChipSelect
*/
#define DIN 4
#define CS 16
#define CLK 17

/*
 Now we need a LedController Variable to work with.
 We have only a single MAX72XX so the Dimensions are 1,1.
 */
LedController<1,1> lc;

/* we always wait a bit between updates of the display */
unsigned long delaytime=1000;

void setup() {
  //Here a new LedController object is created without hardware SPI.
  lc=LedController<1,1>(DIN,CLK,CS);
  lc.setIntensity(8); /* Set the brightness to a medium values */
  lc.clearMatrix(); /* and clear the display */
}

void loop() {
  for(int i=0; i<8; i++) {
    lc.setDigit(0,i,i,false);
    delay(delaytime);
  }
  lc.clearMatrix();
  delay(delaytime);
}