#include "RIMS.h"
/*Define user variables for this state machine here. No functions; make them global.*/
#define Actuator B
#define period 200
int integMax = 1000;
int integMin = -1000;
int Desired;
int Actual;
int Error;
double Deriv;
int Integ;
double calcAct;
double ActualPrev;
double Kp = 0.05; 
double Ki = 0.001;
double Kd = 2.5;
double baseSpeed;
double ballPosition;
double ballVelocity;
double ballAcceleration;
double ballMass = 0.027;
double fanForce = 0;
double fanMass = 0;
double fanAcceleration = 0;
double gravity = 9.8;
double dt = period / 1000;

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
         //desired actual:
         Desired = (A & 0x0F);
         Actual = ((A & 0xF0)>>4);

         // Calculate proportional error
         Error = Desired - Actual;
        printf("Desired: %d Actual: %d",Desired,Actual);
         // Calculate integral 
         Integ += Error;
         if(Integ > integMax)Integ=integMax;
         if(Integ < integMin)Integ=integMin;
        printf(" Integral: %d\n",Integ);

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

   const unsigned int periodOnOff_Ctrl = period;
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