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
         /*
         PID control is common in many embedded systems, like keeping speed constant in an RC car, keeping a motor spinning at a steady rate, keeping a liquid's temperature steady at a desired rate, keeping a flying drone steady, etc. For this project, you can choose from two options. Both are simulations. Embedded programmers commonly make simulations to ensure their systems will work.

        Create a PID simulator in RIMS. The P, I, and D values would all be hardcoded in the C code. A0 controls whether to use pid control (0) or manual control (1). For manual control, A6 is fan speed up and A7 is fan speed down (if both pressed, ignore both). B represents the ball's height, with the target being 100 (as in the web PID simulator). The above is worth 95/100 -- a strong A. For the last 5 points, let A1 control the output mode, with 0 being as above, but 1 meaning the 8 LEDs visually depict the balls height, with the goal to have the ball steady at B3.

        You can use the P, I, and D constants used in the zyBook's simulator: Kp 0.05, Ki 0.001, Kd 2.5

        Submit your code so TAs can see it; you'll get 0 of 1 points but that is irrelevant.

        To ensure you are starting early and developing incrementally, you should submit your code AFTER EVERY 30 MINUTES OF WORK or more often if desired. Just copy-paste, click Submit, and then keep working, so we can see a history of your work, including dates/times and the code itself.
	*/
        fanAcceleration = calcAct;
        fanForce = fanMass * fanAcceleration;
        ballAcceleration = (fanForce / ballMass) - gravity;
        ballVelocity = ballVelocity + (ballAcceleration *dt);
        ballPosition = ballPosition + (ballVelocity * dt);
        if(ball.pos < ballMin){
            ballVelocity = 0;
            ballPosition = ballMin;
        }
        if(ball.pos > ballMax){
            ball.v = 0;
            ball.pos = ballMax;
        }
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
