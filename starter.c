#include "rims.h"

/*Define user variables for this state machine here. No functions; make them global.*/
#define dt 50 
#define baseSpeed 200 
#define p 50 
#define i 1
#define d 2500 

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

int POS = 0; 
int ACTUATOR = 0;
int ACTUALPREV = 0;
int INTEG = 0;

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



unsigned char SM1_Clk;
void TimerISR() {
   SM1_Clk = 1;
}

enum SM1_States { SM1_NONAME, SM1_Init, SM1_Ctrl } SM1_State;

TickFct_OnOff_Ctrl() {
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_Init;
         break;
         case SM1_Init:
            SM1_State = SM1_Ctrl;
         
         break;
      case SM1_Ctrl:
            SM1_State = SM1_Ctrl;
         break;
      default:
         SM1_State = SM1_Init;
   } // Transitions

   switch(SM1_State) { // State actions
      case SM1_Init:
         Actuator = 0;
         ActualPrev = 0;
         Integ = 0;
         break;
      case SM1_Ctrl:
         Desired = (A & 0x0F);
         Actual = ((A & 0xF0)>>4);

         // Calculate proportional error
         Error = Desired - Actual;

         // Calculate integral 
         Integ += Error;
         if(Integ > integMax)Integ=integMax;
         if(Integ < integMin)Integ=integMin;

         // Calculate derivative
         Deriv = Actual - ActualPrev;

         // Calculate actuator output
         calcAct = Kp*Error + Ki*Integ - Kd*Deriv;
         if(calcAct < 0) calcAct = 0;

         Actuator = calcAct;
         ActualPrev = Actual;
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {

   const unsigned int periodOnOff_Ctrl = 200;
   TimerSet(periodOnOff_Ctrl);
   TimerOn();
   
   SM1_State = -1; // Initial state
   B = 0; // Init outputs

   while(1) {
      TickFct_OnOff_Ctrl();
      while(!SM1_Clk);
      SM1_Clk = 0;
   } // while (1)
} // Main