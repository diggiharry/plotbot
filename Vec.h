/**
    Vec.h
  	Simple 2D vector class, uses two floats for x and y direction, has basic functions 
	for vector arithmetic

    modified from http://snipplr.com/view/18098/2d-vector-class/
*/

#ifndef Vec_h
#define Vec_h

#include <Arduino.h>

class Vec {
 
	public:
		Vec(float X = 0, float Y = 0) {
			x = X;
			y = Y;
		};
		~Vec() {} ;
		 
		float x, y;
	 
		Vec operator*(float scalar) const {
			return Vec(x * scalar, y * scalar);
		}
		 
		 
		Vec operator+(const Vec &vect) const {
			return Vec(x + vect.x, y + vect.y);
		}
		 
		Vec operator-(const Vec &vect) const {
			return Vec(x - vect.x, y - vect.y);
		}
		 
		void rotate(float angle) {
			float xt = (x * cosf(angle)) - (y * sinf(angle));
			float yt = (y * cosf(angle)) + (x * sinf(angle));
			x = xt;
			y = yt;
		}
		 
		float crossproduct(const Vec &vect2) const {
			return (this->x * vect2.y) - (this->y * vect2.x);
		}
		 
		float magnitude() {
			return sqrtf(x * x +y * y);
		}
		 
		void normalise()
		{
			float mag = sqrtf(x* x + y * y);
			this->x = x / mag;
			this->y = y / mag;
		}
		 
		// normalise vector so that higher component is 1.0
		void normalise_max() 
		{
			if (this->x > this->y) {
				this->y = this->y*(1/this->x);		
				this->x = 1.0;
			} else if (this->x > this->y) {
				this->x = this->x*(1/this->y);
				this->y = 1.0;
			} else {
				this->x = 1.0;
				this->y = 1.0;
			}
		}

		float dotproduct(const Vec &vect) const {
			return (x * vect.x) + (y * vect.y);
		}
};
#endif
