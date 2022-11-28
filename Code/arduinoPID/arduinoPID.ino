#include <Preferences.h>
#include <TM1637Display.h>
#define CLK 23
#define DIO 22
TM1637Display display(CLK, DIO);

Preferences preferences;

const uint8_t segPID[] = {
  SEG_A | SEG_B | SEG_E | SEG_F | SEG_G,
  SEG_E | SEG_F,
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,
  NULL
};
const uint8_t segHello[] = {
  SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,
  SEG_B | SEG_C | SEG_E | SEG_F,
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
};

#include "fsmset.h"
#include "fsmbutton.h"
#include "fsmpid.h"
#include "fsmdisplay.h"

const int potPin = 34;
float potValue = 0;

int button1 = 33;
int button2 = 32;
int button3 = 35;

int but1in = 0;
int but2in = 0;
int but3in = 0;

int stateset = 0;
int counterset = 0;
int outputset = 0;

int state1 = 0;
int counter1 = 0;
int output1 = 0;

int state2 = 0;
int counter2 = 0;
int output2 = 0;

int statepid = 0;
float kp;
float ki;
float kd;

int statedis = 0;

float integral = 0;
float last_err = 0;
float error = 0;
float deriv = 0;

void pid(float input, float *output, float setpoint, float kp, float ki, float kd, float time){
  
    error = setpoint - input;
    integral += error * time;
    deriv = (last_err - error)/time;
    last_err = error;
    *output = kp * error + ki * integral + kd * deriv;
}

int next_valid = 0;
float outmot = 0.0;
float outpid = 0.0;
String pcdata;
int pcsent = 0;

void setup() {
  Serial.begin(115200);

  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  
  display.setBrightness(6);
  display.setSegments(segHello);
  delay(1000);
  display.setSegments(segPID);
  delay(1000);

  preferences.begin("parameters", false);
  kp = preferences.getFloat("kp",false);
  ki = preferences.getFloat("ki",false);
  kd = preferences.getFloat("kd",false);
}

void loop() {
  // Reading potentiometer value
  but1in = digitalRead(button1);
  but2in = digitalRead(button2);
  but3in = digitalRead(button3);

  /*
  Serial.print("Button 1: ");
  Serial.println(but1in);
  Serial.print("Button 2: ");
  Serial.println(but2in);
  Serial.print("Button 3: ");
  Serial.println(but3in);
  */
  
  potValue = (analogRead(potPin)*99.0/4095);

  fsmset(but1in, &stateset, &counterset, &outputset);
  fsmbutton(but2in, &state1, &counter1, &output1);
  fsmbutton(but3in, &state2, &counter2, &output2);

  /*
  Serial.print("Set 1: ");
  Serial.println(stateset);
  Serial.print("Button 2: ");
  Serial.println(output1);
  Serial.print("Button 3: ");
  Serial.println(output2);
  */
  
  fsmpid(outputset, output1, output2, &statepid, &kp, &ki, &kd);
  fsmdis(&statedis, statepid, potValue, kp, ki, kd);

  while(Serial.available()>0){
    pcdata = Serial.readStringUntil(';');
    Serial.flush();
    pcsent = 1;
  }

  if(pcsent == 1){
    outmot = pcdata.toFloat();
    pid(outmot, &outpid, potValue, kp, ki, kd, 0.1);
    if(statedis != 0){
      potValue = 0;
      outpid = 0;
    }
  }

  Serial.print(potValue);
  Serial.print(";");
  Serial.println(outpid);

  pcsent = 0;

  delay(100);
}
