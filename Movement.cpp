/**
    Movement.cpp
    contains a base class (Movement) as well as derived classes that 
	describe different movement of the steppers.
	Movement: Base class, used for idling of the motors
	CruiseMovement: continuous movement in one direction
	AccelMovement: accelerating/deccelerating movement, based on sinusoidal velocity profile

    @author sei
*/
#include "Movement.h"



/**
    Constructor of base class
	sets basic parameters of the movement like start/stop point, velocity, etc.
*/
Movement::Movement() {
	start = Vec(0.0,0.0);	// Starting point of movement
	stop = Vec(0.0,0.0);	// Ending point of movement

	startv = 0.0; 			// starting velocity
	stopv = 0.0;			// ending velocity

	t0 = 0.0;				// time the movement started, gets set by init()

	dt = Vec(0.0,0.0);		// contains times where x/y steppers shall make next step
	dir = Vec(0.0,0.0);		// direction of the movement (should be normalised)
	
};

/**
    Checks if Movement is finished

    @param actual positions of x and y steppers 
    @return true if movement is finished, else fals
*/
bool Movement::finished(float x_pos, float y_pos) {
	return true;
}

/**
    Return delay in seconds until the next step should be made

    @return time until next step in x-direction
*/
float Movement::calcdtx() {
	return 0.0;
};

/**
    Return delay in seconds until the next step should be made

    @return time until next step in y-direction
*/
float Movement::calcdty() {
	return 0.0;
};

/**
    Initiliases movement object, sets starting time and other variables 

    @params actual time t in seconds
*/
void Movement::init(float t) {
	this->t0 = t;
	this->dt.x = t;
	this->dt.y = t;
	this->calcdtx();
	this->calcdty();
};



/**
    Constructor of CruiseMovement
	
	@param start Starting point of movement
	@param steps Steps to ending point
	@param v velocity
*/
CruiseMovement::CruiseMovement(Vec start, Vec steps, float v): Movement() {
	this->start = start;
	this->stop = start+steps; // set ending position to start + steps
	this->stopv = v;
	this->startv = v;
	this->dir = this->stop - this->start; // direction of movement is difference of start and stop vector
	this->dir.normalise_max(); // normalise direction	
};

/**
    Constructor of CruiseMovement
	
	@param move movement to which append to
	@param steps Steps to ending point
*/
CruiseMovement::CruiseMovement(Movement *move, Vec steps): Movement() {
	this->start = move->stop; // start of this movement is stop of last movement
	this->stop = start + steps;
	this->stopv = move->stopv;
	this->startv = move->stopv;
	this->dir = move->dir;
}

/**
    Checks if CruiseMovement is finished

    @param actual positions of x and y steppers 
    @return true if movement is finished, else fals
*/
bool CruiseMovement::finished(float x_pos, float y_pos) {
	if ((x_pos >= stop.x) & (y_pos >= stop.y)) {
		return true;
	} else {
		return false;
	}
}

/**
    Return delay in seconds until the next step should be made
	Saves absolute time of next step in dt.x

    @return time until next step in x-direction
*/
float CruiseMovement::calcdtx() {
	float d_t = 1.0/this->stopv; // calculate delay for next step [s/step] from velocity [steps/s]
	d_t = d_t/this->dir.x; // apply direction vector to delay time
	return d_t;
}

/**
    Return delay in seconds until the next step should be made
	Saves absolute time of next step in dt.y

    @return time until next step in y-direction
*/
float CruiseMovement::calcdty() {
	float d_t = 1.0/this->stopv; // calculate delay for next step [s/step] from velocity [steps/s]
	d_t = d_t/this->dir.y; // apply direction vector to delay time
	return d_t;
}

/**
    Constructor of AccelMovement
	
	@param start Starting point of movement
	@param acc_time how long the acceleration shall take, in seconds
	@param steps Steps to ending point
	@param stopv starting velocity
	@param stopv final velocity
*/
AccelMovement::AccelMovement(Vec start, float acc_time, Vec steps, float startv, float stopv): Movement() {
	this->start = start;
	this->stop = start+steps;
	this->acc_time = acc_time;
	this->startv = startv;		
	this->stopv = stopv;	
	this->dir = this->stop - this->start;
	this->dir.normalise_max();		
};

/**
    Constructor of AccelMovement
	
	@param move movement to which append to
	@param acc_time how long the acceleration shall take, in seconds
	@param steps Steps to ending point
	@param stopv final velocity
*/
AccelMovement::AccelMovement(Movement *move, float acc_time, Vec steps, float stopv): Movement() {
	this->start = move->stop;
	this->stop = start + steps;
	this->acc_time = acc_time;
	this->startv = move->stopv;		
	this->stopv = stopv;	
	this->dir = move->dir;	
};

/**
    Checks if AccelMovement is finished

    @param actual positions of x and y steppers 
    @return true if movement is finished, else fals
*/
bool AccelMovement::finished(float x_pos, float y_pos) {
	if ((x_pos >= stop.x) & (y_pos >= stop.y)) {
		return true;
	} else {
		return false;
	}
}

/**
    Calculates factor of the sinusoidal velocity profile at given time

    @param dt actual time of acceleration, should be value between 0.0 and acc_time 
    @param acc_time how long the acceleration shall take, in seconds
    @return number between 0.0 and 1.0 following 1/4 of a sinusoid
*/
float AccelMovement::calc_factor(float dt, float acc_time) {
	// calculate sinusoidal profile
	if (dt < acc_time) {
		return (0.5-0.5*cos((1.0/acc_time)*PI*dt));
	}
	return 1.0; //if acceleration is finished, return 1.0
	// calculate linear profile
	/*if (dt < acc_time) {
		return (1.0/acc_time)*dt;
	}
	return 1.0;*/
}

/**
    Calculates velocity of movement at given time

    @param t actual time of movement in seconds, global time 
    @return velocity in steps/s
*/
float AccelMovement::calc_v(float t) {
	// if actual time is earlier than start time, return start velocity
	if (t < this->t0) return this->startv;	

	float diff_v = this->stopv - this->startv;
	float diff_t = (t-this->t0);		

	if (diff_t <= this->acc_time) {
		return startv+diff_v*calc_factor(diff_t,this->acc_time);
	} 	

	// if time is later than time of acceleration, return finish velocity
	return this->stopv;
}


/**
    Calculates primitive function of the sinusoidal velocity profile, is used to calculate the 
	steps needed for the whole acceleration

    @param t time in seconds
    @param acc_time how long the acceleration shall take, in seconds
	@param stopv starting velocity
	@param stopv final velocity
    @return steps needed to perform acceleration until time t
*/
float AccelMovement::calc_v_primitive(float t,float acc_time, float startv, float stopv) {
	float diff_v = abs(stopv - startv);
	return (startv + 0.5*diff_v)*t - 0.159155*acc_time*diff_v*sin((PI*t)/acc_time);
}

/**
    Calculates steps needed for the whole acceleration

    @param t time in seconds
    @param acc_time how long the acceleration shall take, in seconds
	@param stopv starting velocity
	@param stopv final velocity
    @return steps needed to complete acceleration
*/
float AccelMovement::calc_steps2acc(float acc_time, float startv, float stopv) {
	return (calc_v_primitive(acc_time, acc_time, startv, stopv) - calc_v_primitive(0.0, acc_time, startv, stopv));///1.333;
}

/**
    Return delay in seconds until the next step should be made
	Saves absolute time of next step in dt.x

    @return time until next step in x-direction
*/
float AccelMovement::calcdtx() {
	float d_t = 1.0/calc_v(dt.x);
	d_t = d_t/dir.x;	
	dt.x += d_t;
	return d_t;
};

/**
    Return delay in seconds until the next step should be made
	Saves absolute time of next step in dt.y

    @return time until next step in y-direction
*/
float AccelMovement::calcdty() {
	float d_t = 1.0/calc_v(dt.y);
	d_t = d_t/dir.y;	
	dt.y += d_t;
	return d_t;
};

