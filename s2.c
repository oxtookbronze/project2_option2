#include <stdio.h>
#include <math.h>
#include <limits.h>


double integMax = 5;
double integMin = -5;
double Desired;
double Actual;
double Error;
double Deriv;
double Integ;
double calcAct;
double ActualPrev;
double Kp = .05; 
double Ki = 1;
double Kd = 2.5;
double maxThreshold;

struct Ball{
	double pos;
	double vel;
	double acc;
	double mass;
} ball;

struct Fan{
	double force;
	double acc;
	double mass;
} fan;

double updateError(){
	Desired = 100;
	Actual = ball.pos;
	Error = Desired - Actual;
	Integ+=Error;
	if(Integ > integMax) Integ = integMax;
	if(Integ < integMin) Integ = integMin;
	Deriv = Actual - ActualPrev;
	
	
	calcAct = (Kp * Error) + (Ki * Integ) - (Kd*Deriv);
	if(calcAct > 10) calcAct = 10;
	if(calcAct < 0) calcAct = 0;
	printf("%f\n",calcAct);
	return calcAct;
	ActualPrev = Actual;
}

void updateSystem(){
	fan.acc = Actuator;
	fan.force= fan.mass * fan.acc;
	ball.acc = (fan.force / ball.m) - g;
	ball.vel+= (ball.acc * dt);
	if(ball.pos < ballMin){
		ball.v = 0;
		ball.pos = ballMin;
	}
	if(ball.pos > ballMax){
		ball.vel = 0;
		ball.pos = ballMax;
	}
	ActuatorPrev = Actuator;
	ActualPrev = Actual;
	Actual = ball.pos;
}

void updateController(){
	Error = Desired - Actual;
	Deriv = Actual - ActualPrev;
	Integ += Error;
	if(Integ > 1000){
		Integ = 1000;
	}
	if(Integ < -1000){
		integMin = -1000;
	
	}
	Actuator = (p * Error) + (i * Integ) - (d * Deriv);
}

void updatePhysics(){
	fan.acc = calcAct;
	fan.force = fan.mass * fan.acc;
	ball.acc = (fan.force / ball.mass) - 9.8;
	ball.vel+=(ball.acc * 0.005);
	ball.pos+= (ball.vel* 0.005);
	printf("%f ",ball.pos);

}

int main(){
	Actual = 0;
	ball.pos = 0;
	fan.mass = .5;
	ball.mass = .027;	
	double totalError = 0;
	double bestKp = 0;
	double bestError = INT_MAX;
	
		for(int i = 0; i < 100;i++){
			updatePhysics();
			totalError += updateError();
		}

		
	
	return 0;

}
