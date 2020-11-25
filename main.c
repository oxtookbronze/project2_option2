#include "RIMS.h"

/*Define user variables for this state machine here. No functions; make them global.*

/*Define user variables for this state machine here. No functions; make them global.*/
#define Actuator B
int integMax = 5;
int integMin = -5;
int integCount = 0;
int Desired;
int Actual;
int Error;
int Deriv;
int Integ;
int calcAct;
int ActualPrev;
int Kp = 8; 
int Ki = 1;
int Kd = 10;
int manualControl = 10;
struct Ball{
   int pos;
   int vel;
   int acc;
   int mass;
   }ball;
struct Fan{
   int force;
   int acc;
   int mass;
   
} fan;
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
         ball.pos = 0;
         fan.mass = 5;
         ball.mass = 3;
         manualControl = 10;

         
         break;
      case SM1_Ctrl:
         
         Desired = 100;

         //tick physics

         if (A0){
            if(A6 && !A7){
               manualControl +=3;
            }
            if(A7 && !A6){
               manualControl -=3;
            }
         }
         if(A1){
            if(ball.pos < 25){
               B = 128;
            }
            else if(ball.pos < 50){
               B = 192;
            }
            else if(ball.pos < 70){
               B = 224;
            }
            else if(ball.pos < 110){
               B = 240;
            }
            else if(ball.pos < 140){
               B = 248;
            }
            else if(ball.pos < 170){
               B = 252;
            }
            else if(ball.pos < 200){
               B = 254;
            }
            else{
               B = 255;
            }
         }
         else{
            B = ball.pos % 1000;
         }
         printf("fan.acc: %d fan.force: %d ball.acc: %d ball.vel: %d ball.pos: %d\n",fan.acc,fan.force,ball.acc,ball.vel,ball.pos);
         fan.acc = (A0)? manualControl: calcAct;
         if(fan.acc > 10) fan.acc = 10;
         fan.force = fan.mass * fan.acc;
         ball.acc = (fan.force / ball.mass) - 3;
         ball.vel+=ball.acc;
         ball.pos+=ball.vel;
         
         if(ball.pos > 200){
            ball.pos = 200;
            ball.vel = -10;
         }
         if(ball.pos <=0){
            ball.pos = 0;
            ball.vel = 10;
         }

         //printf("%d ",ball.pos);
         // Calculate proportional error
         Error = (Desired - ball.pos);
         printf("%d %d\n",Desired, ball.pos);

         // Calculate integral
         Integ += Error;
         if(Integ > integMax) Integ = integMax;
         if(Integ < integMin) Integ = integMin;
      

         // Calculate derivative
         Deriv = calcAct - ActualPrev;

         // Calculate actuator output
         calcAct = Kp*Error + Ki*Integ -Kd*Deriv;
         if(calcAct < 0) calcAct = 0;
         if(calcAct > 100) calcAct = 100;
         printf("%d\n",calcAct);
         

        
         ActualPrev = calcAct;
         
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {

   const unsigned int periodOnOff_Ctrl = 500;
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