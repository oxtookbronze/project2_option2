#include <stdio.h>

// BEGIN Items to mimick RIMS. Don't modify this code. 

unsigned short A7,A6,A5,A4,A3,A2,A1,A0; // Note: Setting or reading A or B as a group, as in B = 0, isn't supported
unsigned short B7,B6,B5,B4,B3,B2,B1,B0;
char inputValues[100][8]; A// Inputs A7..A0 for up to 100 ticks.
int currTick = 0;

void SetAInputs() {
   A0 = (inputValues[currTick][7] == '0') ? 0 : 1;
   A1 = (inputValues[currTick][6] == '0') ? 0 : 1;
   A2 = (inputValues[currTick][5] == '0') ? 0 : 1;
   A3 = (inputValues[currTick][4] == '0') ? 0 : 1;
   A4 = (inputValues[currTick][3] == '0') ? 0 : 1;
   A5 = (inputValues[currTick][2] == '0') ? 0 : 1;
   A6 = (inputValues[currTick][1] == '0') ? 0 : 1;
   A7 = (inputValues[currTick][0] == '0') ? 0 : 1;
}


void PrintAInputs() {
   printf("A7..A0: %d%d%d%d%d%d%d%d\n",A7,A6,A5,A4,A3,A2,A1,A0);
}

void TimerTick() {
   printf("%d: %d%d%d%d%d%d%d%d\n",currTick, B7,B6,B5,B4,B3,B2,B1,B0);
   currTick += 1;
   SetAInputs();
}

// END Items to mimick RIMS


// Write your synchSM tick function here
enum BL_States { BL_SMStart, BL_LedOff, BL_LedOn } BL_State;

void TickFct_Blink() {

   switch ( BL_State ) { //Transitions
      case BL_SMStart:
         BL_State = BL_LedOff; //Initial state
         break;
      case BL_LedOff:
         BL_State = (A0) ? BL_LedOn: BL_LedOff;
         break;
      case BL_LedOn:
         BL_State = (A0) ? BL_LedOff: BL_LedOn;
         break;
      default:
         BL_State = BL_SMStart;
         break;
   }

   switch (BL_State ) { //State actions
      case BL_LedOff:
         B0 = 0;
         break;
      case BL_LedOn:
         B0 = 1;
         break;
      default:
         break;
   }
}




int main() {
   // The items below mimick RIMS by pre-reading all input vectors; don't modify this code 
   int numInputVectors; 
   scanf("%d", &numInputVectors); // Reads input test vectors
   for (int i = 0; i < numInputVectors; ++i) {
      scanf("%s", inputValues[i]);
   }
   SetAInputs(); // Set initial values for input A
   

   // B = 0 itself doesn't work, so assign all bits individually
   B7 = 0; B6 = 0; B5 = 0; B4 = 0;
   B3 = 0; B2 = 0; B1 = 0; B0 = 0;
   
   // Initialize your synchSM state here
   BL_State = BL_SMStart; // Indicates initial call to tick-fct


   while (currTick < numInputVectors) {
      // Call your synchSM tick function here
      TickFct_Blink();      // Execute one synchSM tick

      TimerTick(); // This function prints all B output values and updates a global tick variable
   }
   
   return 0;
}
