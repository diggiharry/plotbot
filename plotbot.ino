
#include "Stepper.h"
#include "Stepper_Control.h"
#include "MovementList.h"
#include "Movement.h"
#include <PITimer.h>

#define button 0
#define up 1
#define down 3
#define left 2
#define right 4

int stick_pin[] = {23,22,21,20,19};
boolean stick[5]; 

#define TOP_STEP 3
#define TOP_DIR 2
#define TOP_MS1 6
#define TOP_MS2 5
#define TOP_MS3 4
#define TOP_ENABLE 7

#define BOTTOM_STEP 14
#define BOTTOM_DIR 13
#define BOTTOM_MS1 17
#define BOTTOM_MS2 16
#define BOTTOM_MS3 15
#define BOTTOM_ENABLE 18

// LED pin ist schon belegt !!!
//const int ledPin = 13;
//boolean on = false;

elapsedMicros mu;
elapsedMicros pressed;

Stepper stepper_top(200, TOP_DIR, TOP_STEP, TOP_MS1,TOP_MS2, TOP_MS3, TOP_ENABLE);
Stepper stepper_bottom(200, BOTTOM_DIR, BOTTOM_STEP, BOTTOM_MS1, BOTTOM_MS2, BOTTOM_MS3, BOTTOM_ENABLE);

Stepper_Control steppers(&stepper_top, &stepper_bottom);

void x_callback() {
  steppers.x_callback();  
}

void y_callback() {
  steppers.y_callback();  
}

void setup() {
  Serial.begin(38400);
  for (int i = 0; i<5; i++) {
      pinMode(stick_pin[i], INPUT);
      stick[i] = digitalRead(stick_pin[i]);
  }
  
  //pinMode(ledPin, OUTPUT);
  stepper_top.enableDriver();
  stepper_bottom.enableDriver();

  PITimer1.set_callback(x_callback);
  PITimer2.set_callback(y_callback);
  //PITimer1.period(0.00001);
  //PITimer2.period(0.00001);  
  //PITimer1.start();
  //PITimer2.start();
  Serial.println("Initialization finished");
}

void loop() {

  for (int i = 0; i<5; i++) {  
    stick[i] = digitalRead(stick_pin[i]);   
  }
  
  if ((stick[button]) & (pressed > 500000)) {
    steppers.linear_move(15000,15000);
    //steppers.cruise(10000,5000);
    pressed = 0;
  }
  
  //steppers.run();
  
  if (mu > 100000) {
    //on = !on;
    //digitalWrite(ledPin, on);   // set the LED on
    mu = 0;
    double t = steppers.mus/1000000.0; 
    Serial.print(t);
    Serial.print("| moves: ");
    Serial.print(steppers.moves.size());
    Serial.print("| dt: ");
    Serial.print(t-steppers.move->t0,1.0);
    //Serial.print("| f: ");
    //Serial.print(AccelMovement::calc_factor(t-steppers.move->t0,1.0));
    Serial.print("| x: ");
    Serial.print(steppers.bottom->get_pos());
    Serial.print("| y: ");
    Serial.print(steppers.top->get_pos());  
    Serial.print("| dtx: ");
    Serial.print((t-steppers.move->dt.x)*1000000);
    Serial.print("| dty:");
    Serial.print((t-steppers.move->dt.y)*1000000);
    //Serial.print("| x0: ");
    //Serial.print(steppers.move->start.x);
    //Serial.print("| x1:");
    //Serial.print(steppers.move->stop.x);
    //Serial.print("| y0: ");
    //Serial.print(steppers.move->start.y);
    //Serial.print("| y1:");
    //Serial.println(steppers.move->stop.y);
    Serial.print("| dir.x: ");
    Serial.print(steppers.move->dir.x);
    Serial.print("| dir.y: ");
    Serial.print(steppers.move->dir.y);
    //Serial.print(" ");   
    //Serial.print(Movement::calc_v(t, steppers.move->t0, steppers.move->acc_time, steppers.move->startv, steppers.move->stopv));
    Serial.println();
  }
  /*
  if (stick[button]) {
    for (int i = 0; i < 1000; i++) {
      double t = i/100.0;
      double v = steppers.calc_factor(t, 1.0);
      double f = steppers.calc_v(t, 0.0, 1.0, 5000.0, 1000.0);
      Serial.print(i);
      Serial.print(" ");
      Serial.print(t);
      Serial.print(" ");
      Serial.print(f);
      Serial.print(" ");
      Serial.println(v);
      delay(10);    
    }
  }*/
}

