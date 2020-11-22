#include "rims.h"

/*Define user variables for this state machine here. No functions; make them global.*/
#define Actuator B
short integMax = 5;
short integMin = -5;
short Desired;
short Actual;
short Error;
short Deriv;
short Integ;
short calcAct;
short ActualPrev;
float Kp = ; 
short Ki = 0;
short Kd = 0;

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