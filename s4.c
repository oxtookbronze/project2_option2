#include <stdio.h>

#define dt 50 
#define baseSpeed 200 
#define p 50 
#define i 1
#define d 2500 

struct Ball{
	int a;
	int v;
	int pos;
	int m;

}ball;

struct Fan{
	int m;
	int a;
	int v;
	int f;
}fan;

void updateController(int* Actual, int* ActualPrev,int* Actuator, int* Integ){
	int Error = 100000 - *Actual;
	int Deriv = *Actual - *ActualPrev;
	*Integ += Error;
	if(*Integ > 1000000) *Integ = 1000000;
	if(*Integ < -1000000) *Integ = -1000000;
	*Actuator =( (p * Error) + (i* *Integ) - (d*Deriv)) / 1000;
	if(*Actuator < 0) *Actuator = 0;
	if(*Actuator > 10000) *Actuator = 10000;
	
	*Actuator += baseSpeed;
}

void updateSystem(int* Actuator, int* Actual ,int* ActualPrev){
	fan.a = *Actuator;
	fan.f = fan.m * fan.a;
	ball.a = (fan.f / ball.m) - 9800; 
	ball.v+= (ball.a * dt) / 1000;
	ball.pos+= (ball.v * dt) / 1000;
	if(ball.pos < 0){
		ball.v = 0;
		ball.pos = 0;
	}
	if(ball.pos > 200000){
		ball.v = 0;
		ball.pos = 200000;
	}
	
	*ActualPrev = *Actual;
	*Actual = ball.pos;
}

int main(){
	fan.m = 500;
	ball.m = 27;
	int POS = 0; 
	int ACTUATOR = 0;
	int ACTUALPREV = 0;
	int INTEG = 0;

	for(int j = 0; j < 200;){
		updateSystem(&ACTUATOR,&POS,&ACTUALPREV);	
		updateController(&POS,&ACTUALPREV,&ACTUATOR,&INTEG);	
		printf("%d %d\n",ACTUATOR,POS / 1000);
	}
	return 0;
}
