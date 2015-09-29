/**
    Stepper_Control.h
	Class used for holding, constructing, adminstering and "stitching" movements  

    @author sei
*/

#ifndef Stepper_Control_h
#define Stepper_Control_h

#include "Stepper.h"
#include "elapsedMillis.h"
#include <Arduino.h>
#include <math.h>
#include "MovementList.h"
#include "Vec.h"
#include "Movement.h"
#include "PITimer.h"

class Stepper_Control {
  public:
    Stepper_Control(Stepper* top_stepper, Stepper* bottom_stepper);

	void linear_move(int steps_top, int steps_bottom);
	void cruise(int steps_top, int steps_bottom);

	void x_callback();
	void y_callback();

	Stepper* top;
	Stepper* bottom;

	elapsedMicros mus;

	void check_move();

	MovementList<Movement*> moves;
	Movement *move;
	Movement idle_move;	

	int count = 0;
};

#endif
