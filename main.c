#include "RIMS.h"

/*
Scaled code up to account for no floating point. Only scaled the
display down.
*/

// constants used to compute Actuation
#define dt 50 
#define baseSpeed 200 
#define p 50 
#define i 1
#define d 2500 

// structs to organize physics
struct Ball{
	int a;
	int v;
	int pos;


}ball;
struct Fan{

	int a;
	int v;
	int f;
}fan;

// PID controller global variables
int POS = 0; 
int ACTUATOR = 0;
int ACTUALPREV = 0;
int INTEG = 0;

// updates controller
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

//updates the physics
void updateSystem(int* Actuator, int* Actual ,int* ActualPrev){
	fan.a = *Actuator;
	fan.f = 500 * fan.a;
	ball.a = (fan.f / 27) - 9800; 
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


//converts the raw binary display to the other output mode
int standardToGame(int pos){
   if(pos < 25) return 128; // B7
   else if( pos < 50) return 192; // B6
   else if( pos < 75) return 224; // B5
   else if( pos < 90) return 240; // B4
   else if( pos < 115) return 248; // B3
   else if( pos < 130) return 252; // B2
   else if( pos < 140) return 254; // B1
   else return 255; // B0
}

// manual game mode Actuation++ -> bounds checking

void increaseActuator(int* a){
   *a+=100;
   if(*a < 0) *a = 0;
   else if(*a > 1000) *a = 1000;
}

// manual game mode Actuation-- -> bounds checking 

void decreaseActuator(int* a){
   *a-=100;
   if(*a < 0) *a = 0;
   else if(*a > 1000) *a = 1000;
}

//timing
unsigned char clk;
void TimerISR() {
   clk = 1;
}


//states
enum States {Init, PID_Standard, Manual_Standard, PID_Game,Manual_Game} state;

tick() {
   switch(state) { // Transitions
      // transitions are the same no matter what
      default:
         if(A0){
            if(A1) state = Manual_Game;
            else state = Manual_Standard;
         }
         else{
            if(A1) state = PID_Game;
            else state = PID_Standard;
         }
         break;

   } // Transitions

   switch(state) { // Actions
      case PID_Standard:
         updateSystem(&ACTUATOR,&POS,&ACTUALPREV);
		   updateController(&POS,&ACTUALPREV,&ACTUATOR,&INTEG);
         B = POS / 1000;
         break;
      case Manual_Standard:
         updateSystem(&ACTUATOR,&POS,&ACTUALPREV);
         if(A6 && !A7){
            increaseActuator(&ACTUATOR);
         }
         if(A7 && !A6){
            decreaseActuator(&ACTUATOR);
         }
         B = POS / 1000;
         break;
      case PID_Game:
         updateSystem(&ACTUATOR,&POS,&ACTUALPREV);
		   updateController(&POS,&ACTUALPREV,&ACTUATOR,&INTEG);
         B = standardToGame(POS / 1000);
         break;
      case Manual_Game:
         updateSystem(&ACTUATOR,&POS,&ACTUALPREV);
         if(A6 && !A7){
            increaseActuator(&ACTUATOR);
         }
         if(A7 && !A6){
            decreaseActuator(&ACTUATOR);
         }
         B = standardToGame(POS / 1000);
         break;
   } // Actions

}

int main() {

   const unsigned int periodOnOff_Ctrl = 200;
   TimerSet(periodOnOff_Ctrl);
   TimerOn();
   
   state = Init; // Initial state
   B = 0; // Init outputs

   while(1) {
      tick();
      while(!clk);
      clk = 0;
   } // while (1)
} // Main