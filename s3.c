#include <stdio.h>

#define dt 0.05
#define baseSpeed 0.2
#define p 0.05
#define i 0.001
#define d 2.5

struct Ball{
	double a;
	double v;
	double pos;
	double m;

}ball;

struct Fan{
	double m;
	double a;
	double v;
	double f;
}fan;

void updateController(double* Actual, double* ActualPrev,double* Actuator, double* Integ){
	double Error = 100 - *Actual;
	double Deriv = *Actual - *ActualPrev;
	*Integ += Error;
	if(*Integ > 1000) *Integ = 1000;
	if(*Integ < -1000) *Integ = -1000;
	*Actuator = (p * Error) + (i* *Integ) - (d*Deriv);
	if(*Actuator < 0) *Actuator = 0;
	if(*Actuator > 10) *Actuator = 10;
	
	*Actuator += baseSpeed;
}

void updateSystem(double* Actuator, double* Actual ,double* ActualPrev){
	fan.a = *Actuator;
	fan.f = fan.m * fan.a;
	ball.a = (fan.f / ball.m) - 9.8;
	ball.v+= (ball.a * dt);
	ball.pos+= (ball.v * dt);
	if(ball.pos < 0){
		ball.v = 0;
		ball.pos = 0;
	}
	if(ball.pos > 200){
		ball.v = 0;
		ball.pos = 200;
	}
	
	*ActualPrev = *Actual;
	*Actual = ball.pos;
}

int main(){
	fan.m = 0.5;
	ball.m = 0.027;
	double POS = 0; 
	double ACTUATOR = 0;
	double ACTUALPREV = 0;
	double INTEG = 0;

	for(int j = 0; j <200;j++){
		updateSystem(&ACTUATOR,&POS,&ACTUALPREV);	
		updateController(&POS,&ACTUALPREV,&ACTUATOR,&INTEG);	
		printf("%f %f\n",ACTUATOR,POS);	
	}
	return 0;
}
