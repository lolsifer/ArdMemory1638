#include <TM1638.h>

#define MAX_MEMORY 100

TM1638 module(8, 9, 7);
TM1638* mod = &module;

int mem[MAX_MEMORY];
int mode;

int memIndex = 0;
int responseIndex = 0;

String mainString = "      MEMORY - PRESS 1 TO PLAY      ";
String readyString = "READY";
String setString = "  SET";
String goString = "     GO";
String playingString = "PLAYING";
String responseString = " RESPOND";


String correctString = "  NICE";
String incorrectString = "  NOPE";

String winnerString = "WINNER";

int counter = 0;
int index = 0;


/*
 * 0: MAIN MENU
 * 1: GET READY
 * 2: GENERATE MEMORY
 * 3: PLAY ALL MEMORY
 * 4: LISTENING FOR RESPONSE
 * 5: CONFIRM RESPONSE - CORRECT GO 2, WRONG GO 5
 * 6: PLAY AGAIN?
 */

// 0, 1, 2, 4, 8, 16, 32, 64

void setup() {
      Serial.begin(9600);
      
      randomSeed(analogRead(0));
   
      mod->setupDisplay(true, 7);
      mode = 0;
}

int pressed = -1;

void loop() {
    counter ++;
            
    byte buttons = mod->getButtons();
    
    if (buttons != 0 && pressed == -1) {
         pressed = buttons >> 1;
    } else if(pressed != -1) {
        buttons = mod->getButtons();

        if(buttons == 0) {
          Serial.print("Button: ");
          Serial.println(pressed, DEC);

          onKeypress(pressed);
          
          pressed = -1;
        }
    }

    if(mode == 0) {
          if(counter % 10) {
            index ++;
            if(index > mainString.length()) {
              index = 0;
            }
          }
          
          mod->setDisplayToString(mainString.substring(index));
          
          delay(250);
    } else if(mode == 1) {
          Serial.println("Displaying ready, set, go.");
          
          mod->clearDisplay();
          mod->setDisplayToString(readyString);
          delay(1000);
          mod->clearDisplay();
          mod->setDisplayToString(setString);
          delay(500);
          mod->clearDisplay();
          mod->setDisplayToString(goString);
          delay(500);

          mode = 2;
    } else if(mode == 2) {
          int randB = random(0, 8);

          mode = 3;
          
          Serial.print("Generated a new random number: ");
          Serial.println(randB);
          
          mem[memIndex ++] = randB;

          for(int i = 0; i <= 7; i++) {
              mod->setLED(TM1638_COLOR_RED, i);
              delay(50);
              mod->setLEDs(0);
          }
         
          mode = 3;
    } else if(mode == 3) {
          Serial.print("Playing current memory: ");

          mod->clearDisplay();
          mod->setDisplayToString(playingString);
          
          for(int i = 0; i <= memIndex-1; i++) {
              mod->setLEDs(0);
              
              delay(500);
              mod->setLED(TM1638_COLOR_RED, mem[i]);

              Serial.print(mem[i]);

              delay(500);
          }

          mod->setLEDs(0);
          Serial.println(".");
          
          delay(500);

          mod->clearDisplay();
          mod->setDisplayToString(responseString);

          responseIndex = 0;
          
          mode = 4;
    } else if(mode == 4) {
      
    }
}

void addResponse(int val) {
   if(mem[responseIndex++] == val) {
      Serial.println("Correct");

      if(responseIndex == memIndex) {
          mod->clearDisplay();
          mod->setDisplayToString(correctString);
          delay(2000);

          if(memIndex == MAX_MEMORY) {
              mod->clearDisplay();
              mod->setDisplayToString(winnerString); 
              
              mode = 10;
              return;
          }
              
          mode = 2;
      }
   } else {
      Serial.println("Wrong");    
      
      mod->clearDisplay();
      mod->setDisplayToString(incorrectString);
      delay(3000);

      mod->setDisplayToDecNumber(memIndex, 1 << 6, false);
      delay(5000);

      mode = 0;
   }
}

void onKeypress(int key) {
    switch(key) {
      case 0:
        if(mode == 0 || mode == 10) {
            mode = 1;
            memIndex = 0;
            responseIndex = 0;
        }

        if(mode == 4) {
             mod->setLED(TM1638_COLOR_RED, 0);
             delay(200);
             mod->setLEDs(0);

             addResponse(0);
        }
        
        break;
      case 1:
        if(mode == 4) {
             mod->setLED(TM1638_COLOR_RED, 1);
             delay(200);
             mod->setLEDs(0);

             addResponse(1);
        }
        break;
      case 2:
        if(mode == 4) {
             mod->setLED(TM1638_COLOR_RED, 2);
             delay(200);
             mod->setLEDs(0);

             addResponse(2);
        }
        break;
      case 4:
        if(mode == 4) {
             mod->setLED(TM1638_COLOR_RED, 3);
             delay(200);
             mod->setLEDs(0);

             addResponse(3);
        }
        break;
      case 8:
        if(mode == 4) {
             mod->setLED(TM1638_COLOR_RED, 4);
             delay(200);
             mod->setLEDs(0);

             addResponse(4);
        }
        break;      
      case 16:
        if(mode == 4) {
             mod->setLED(TM1638_COLOR_RED, 5);
             delay(200);
             mod->setLEDs(0);

             addResponse(5);
        }
        break;      
      case 32:
        if(mode == 4) {
             mod->setLED(TM1638_COLOR_RED, 6);
             delay(200);
             mod->setLEDs(0);

             addResponse(6);
        }
        break;      
      case 64:
        if(mode == 4) {
             mod->setLED(TM1638_COLOR_RED, 7);
             delay(200);
             mod->setLEDs(0);

             addResponse(7);
        }
        break;
    }
}

