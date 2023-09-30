/* Code for displaying auto-leveling and drilling distance on an OLED screen, using two tilt sensors and one ultrasonic sensor */
/* Authors: POPOVICI ANDREI & SORODOC MATEI */
/* Date: 30-09-2023 */
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define trig 13
#define echo 35

#define setDistance 33
#define resetButton 32

#define red 25
#define green 27
#define blue 26

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

int mode = 19;

int tiltX = 18;
int tiltY = 23;

char arrowDown = 25;
char arrowRight = 26;
char arrowFilledLeft = 17;
char arrowFilledRight = 16;
char arrowFilledUp = 30;
char arrowFilledDown = 31;
char arrowLeft = 27;

long duration, cm , distanceWanted, distanceDrilled = 0, analogValue, initialCm;
bool resetState = 0, k = 0;

float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
    // Init Serial Monitor
    Serial.begin(115200);

    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);

    pinMode(setDistance, INPUT_PULLDOWN);
    pinMode(resetButton, INPUT_PULLDOWN);

    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(blue, OUTPUT);

      if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);

  pinMode(tiltX, INPUT);
  pinMode(tiltY, INPUT);
  pinMode(mode, INPUT);

}

void loop() {

  display.clearDisplay();

    digitalWrite(trig, LOW);
    delayMicroseconds(5);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    duration = pulseIn(echo, HIGH);

    cm = (duration/2) / 29.1;
    delay(50);

    analogWrite(red, 255);
    analogWrite(green, 255);
    analogWrite(blue, 255);

    analogValue = analogRead(setDistance);
    double distanceWanted = round(2. * floatMap(analogValue, 0, 4095, 0, 20)) / 2.;

    if(digitalRead(resetButton) == 1 && k == 0){
        resetState = 1;
        k = 1;
        initialCm = cm;
        Serial.print("Intial: ");
        Serial.println(initialCm);
        Serial.println();
    }

    if(resetState == 1 && k == 1){
        if(cm > (initialCm - distanceWanted )){

            if(cm - (initialCm - distanceWanted) <= distanceWanted){
              
                // display.setTextSize(1);
                // display.setCursor(40, 0);
                // display.print("Centimeters left: ");
                // display.setTextSize(1);
                // display.setCursor(80, 0);
                // display.print(cm - (initialCm - distanceWanted));
                // display.display();
                // delay(200);
            }
            else{
               //  display.setTextSize(1);
               //  display.setCursor(80, 0);
               //  display.print("Start!");
               //  display.display();
               //  delay(200);
               // 
                Serial.print("");
            }

            analogWrite(red, 0);
            analogWrite(green,0);
            analogWrite(blue,255);
        }
        else {

            display.setTextSize(3);
            display.setCursor(30, 20);
            display.print("STOP");
            display.display();
            analogWrite(red, 255);
            analogWrite(green, 0);
            analogWrite(blue, 0);
            resetState = 0;
            k = 0;
            delay(2000);

            analogWrite(red, 255);
            analogWrite(green, 255);
            analogWrite(blue, 255);
            
        }

    }
    else{
        Serial.println("Waiting...");
    }

      if (digitalRead(tiltY) == HIGH && digitalRead(mode) == LOW){
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print(arrowFilledUp);

    display.setTextSize(2);
    display.setCursor(0, 50);
    display.print(arrowFilledDown);

    display.setTextSize(4);
    display.setCursor(55, 20);
    display.print(arrowDown);
    display.setTextSize(1);
    display.setCursor(20, 0);
    display.print("Distance ");
    
    display.setTextSize(1);
    display.setCursor(70, 0);
    display.print(distanceWanted);
    display.display();
    delay(200);

  }

  else if(digitalRead(tiltY) == LOW && digitalRead(mode) == LOW){
    display.setTextSize(2);
    display.setCursor(50, 25);
    display.print("OK");
    display.setTextSize(1);
    display.setCursor(20, 0);
    display.print("Distance ");
    
    display.setTextSize(1);
    display.setCursor(70, 0);
    display.print(distanceWanted);
    display.display(); 
    delay(200);

  }

  else if(digitalRead(tiltX) == LOW && digitalRead(tiltY) == HIGH && digitalRead(mode) == HIGH){

    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print(arrowFilledLeft);

    display.setTextSize(2);
    display.setCursor(110, 0);
    display.print(arrowFilledRight);

    display.setTextSize(5);
    display.setCursor(50, 20);
    display.print(arrowRight);
    display.setTextSize(1);
    display.setCursor(20, 0);
    display.print("Distance ");
    
    display.setTextSize(1);
    display.setCursor(70, 0);
    display.print(distanceWanted);
    display.display(); 
    delay(200);
  }

  else if(digitalRead(tiltX) == HIGH && digitalRead(tiltY) == HIGH && digitalRead(mode) == HIGH){
    display.setTextSize(2);
    display.setCursor(50, 25);
    display.print("OK");
    display.setTextSize(1);
    display.setCursor(20, 0);
    display.print("Distance ");
    
    display.setTextSize(1);
    display.setCursor(70, 0);
    display.print(distanceWanted);
    display.display(); 
    delay(200);
  }

    else if(digitalRead(tiltX) == LOW && digitalRead(tiltY) == LOW && digitalRead(mode) == HIGH){

    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print(arrowFilledLeft);

    display.setTextSize(2);
    display.setCursor(110, 0);
    display.print(arrowFilledRight);

    display.setTextSize(5);
    display.setCursor(50, 20);
    display.print(arrowLeft);
    display.setTextSize(1);
    display.setCursor(20, 0);
    display.print("Distance ");
    
    display.setTextSize(1);
    display.setCursor(70, 0);
    display.print(distanceWanted);
    display.display(); 
    delay(200);


  }

  else if(digitalRead(tiltX) == HIGH && digitalRead(tiltY) == LOW && digitalRead(mode) == HIGH){

    display.setTextSize(2);
    display.setCursor(50, 25);
    display.print("OK");
    display.setTextSize(1);
    display.setCursor(20, 0);
    display.print("Distance ");
    
    display.setTextSize(1);
    display.setCursor(70, 0);
    display.print(distanceWanted);
    display.display(); 
    delay(200);
  }

  else{
    display.setTextSize(2);
    display.setCursor(40, 30);
    display.print("ERROR");
    display.display(); 
    delay(200);
  }

}



