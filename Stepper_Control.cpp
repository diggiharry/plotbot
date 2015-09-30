/**
    Stepper_Control.cpp
	Class used for holding, constructing, adminstering and "stitching" movements  

    @author sei
*/
#include "Stepper_Control.h"

Stepper_Control::Stepper_Control(Stepper* top_stepper, Stepper* bottom_stepper) {
	this->top = top_stepper;
	this->bottom = bottom_stepper;
	
	this->idle_move = Movement(); 
	this->move = &idle_move;
}


void Stepper_Control::linear_move(int steps_top, int steps_bottom) {

	Vec start;
	if (moves.size() <= 0) {
		int ptop = this->top->get_pos();
		int pbottom = this->bottom->get_pos();
		start = Vec(float(ptop),float(pbottom));	
	} else {
		Movement *first = moves.get_first();
		start = first->stop;	
	}

	Vec steps = Vec(float(steps_top), float(steps_bottom));
	Vec stop = start+steps;

	float stopv = V_MAX;		
	float startv = V_MIN;
	float acc_time = ACCTIME;	
	float steps2acc = LinearMovement::calc_steps2acc(acc_time, startv, stopv);
	//Serial.print("steps2acc: ");
	//Serial.println(steps2acc);

	LinearMovement *acc;
	LinearMovement *cruise;
	LinearMovement *decc;

	Vec dir = stop - start;
	dir.normalise_max();	
	/*if (dir.x > dir.y) {
		dir.y = dir.y*(1/dir.x);		
		dir.x = 1.0;
	} else if (dir.x > dir.y) {
		dir.x = dir.x*(1/dir.y);
		dir.y = 1.0;
	} else {
		dir.x = 1.0;
		dir.y = 1.0;
	}*/

	Vec acc_steps = Vec(steps2acc*dir.x, steps2acc*dir.y);

	acc = new LinearMovement(start, acc_time, acc_steps, startv, stopv);
	//cruise = new LinearMovement(acc, 0.0, steps-acc_steps-acc_steps, stopv);
	cruise = new LinearMovement(start+acc_steps, 0.0, steps-acc_steps-acc_steps, stopv, stopv);
	//decc = new LinearMovement(cruise, acc_time, acc_steps, startv);
	decc = new LinearMovement(stop-acc_steps, acc_time, acc_steps, stopv, startv);

	moves.push(acc);
	moves.push(cruise);
	moves.push(decc);
	check_move();

	/*Movement *tmp;
	for (int i = 0;i<moves.size();i++) {
		tmp = moves.get(i);
		Serial.print(i);
		Serial.print(" id: ");
		Serial.print(tmp->id);
		Serial.print(" x0: ");
		Serial.print(tmp->start.x);
		Serial.print(" x1: ");
		Serial.print(tmp->stop.x);
		Serial.print(" v0: ");
		Serial.print(tmp->startv);
		Serial.print(" v1: ");
		Serial.println(tmp->stopv);
	}*/
}

void Stepper_Control::cruise(int steps_top, int steps_bottom) {
	int ptop = this->top->get_pos();
	int pbottom = this->bottom->get_pos();

	Vec start;
	float v;
	if (moves.size() <= 0) {
		start = Vec(float(ptop),float(pbottom));
		v = V_MIN;
	} else {
		Movement *first = moves.get_first();
		start = first->stop;	
		v = first->stopv;
	}

	Vec steps = Vec(float(steps_top), float(steps_bottom));

	LinearMovement *cruise;
	cruise = new LinearMovement(start, 0.0, steps, v, v);

	moves.push(cruise);
	check_move();
}

void Stepper_Control::x_callback() {
	float x_pos = 	float(this->top->get_pos());
	float y_pos = 	float(this->bottom->get_pos());
	if (!move->at_target(x_pos, y_pos)) {
		if (move->dir.x > 0.0) top->step();
		PITimer1.period(move->calcdtx());
		//Serial.println( (move->dt.x-move->t0)*1000 );
	} else {
		check_move();
	}
}

void Stepper_Control::y_callback() {
	float x_pos = 	float(this->top->get_pos());
	float y_pos = 	float(this->bottom->get_pos());
	if (!move->at_target(x_pos, y_pos)) {
		if (move->dir.y > 0.0) bottom->step();
		PITimer2.period(move->calcdty());
	} else {
		check_move();
	}
}

inline void Stepper_Control::check_move() {
	if (moves.size() > 0) {
		//Serial.println("checked");
		Movement *next = moves.pop();			
		move = next;
		float t = mus/1000000.0;
		move->init(t);
		PITimer1.period(move->calcdtx());
		PITimer2.period(move->calcdty());
		PITimer1.start();
		PITimer2.start();	
	} else {
		PITimer1.stop();
		PITimer2.stop();
		move = &idle_move;		
	}	
}			





