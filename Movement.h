/**
    Movement.h
    contains a base class (Movement) as well as derived classes that 
	describe different movement of the steppers.
	Movement: Base class, used for idling of the motors
	CruiseMovement: continuous movement in one direction
	AccelMovement: accelerating/deccelerating movement, based on sinusoidal velocity profile

    @author sei
*/

#ifndef Movement_h
#define Movement_h

#define ACCTIME 1.0 // Time used for Acceleration [s]
#define V_MAX 6000.0 // maximal velocity [steps/s]
#define V_MIN 2000.0 // minimal velocity [steps/s]

#include <Arduino.h>
#include <math.h>
#include "Vec.h"

/**
    Base class, used for different more complex movements, and directly used for idling of the motors
	Holds basic parameters of the movement like start/stop point, velocity, etc.
	Contains 3 virtual functions that must be re-implemented at derived classes.
*/
class Movement {

	public:

	Movement();

	virtual float calcdtx();
	virtual float calcdty();
	virtual bool finished(float x_pos, float y_pos);	

	void init(float t);
	
	Vec start;
	Vec stop;

	float startv;
	float stopv;

	float t0;

	Vec dt;
	Vec dir;

	int id;

};

/**
    Class for constant velocity movement in one direction, derived from Movement
*/
class CruiseMovement : public Movement {

	public:

	CruiseMovement();
	CruiseMovement(Movement *move, Vec steps);
	CruiseMovement(Vec start, Vec steps, float v);

	virtual float calcdtx();
	virtual float calcdty();
	virtual bool finished(float x_pos, float y_pos);	
};

/**
    Class for accelerated/deccelerated movement in one direction, derived from Movement
*/
class AccelMovement : public Movement {

	public:

	AccelMovement();
	AccelMovement(Movement *move, float acc_time, Vec steps, float stopv);
	AccelMovement(Vec start, float acc_time, Vec steps, float startv, float stopv);
	
	static float calc_v_primitive(float t,float acc_time, float startv, float stopv);
	static float calc_steps2acc(float acc_time, float startv, float stopv);
	static float calc_factor(float dt, float acc_time);
	float calc_v(float t);

	virtual float calcdtx();
	virtual float calcdty();
	virtual bool finished(float x_pos, float y_pos);	

	float acc_time;

};


#endif
