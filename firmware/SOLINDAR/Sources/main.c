/* ###################################################################
**     Filename    : main.c
**     Project     : SOLINDAR
**     Processor   : MC9S08QE128CLK
**     Version     : Driver 01.12
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2019-10-02, 22:57, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.12
** @brief
**         Main module.
**         This module contains user's application code.
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "TI1.h"
#include "Bits1.h"
#include "AS1.h"
#include "TI2.h"
#include "AD1.h"
#include "Cap1.h"
#include "FC81.h"
#include "Bit1.h"
#include "Bit2.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "math.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "ComDriver.h"
#include "functions.h"


 bool tick_motor = 0; //For timer

void main(void)
{
  /* Write your local variable definition here */

  //Motor variables-------------------------------------------------------------
  bool dir = 1;
  int counter = 0; //motor position as well
  int max = 63;
  int seq_index = 0;
  bool sequence[8][4] = { {1,0,1,0},
                          {1,0,0,0},
                          {1,0,0,1},
                          {0,0,0,1},
                          {0,1,0,1},
                          {0,1,0,0},
                          {0,1,1,0},
                          {0,0,1,0}};

  //End of motor variables-------------------------------------------------------------
  //Lidar variables--------------------------------------------------------------------  
  unsigned short lidar_measure = 0; 
  unsigned short lidar_value = 0;
  
  //End of Lidar variables-------------------------------------------------------------
  //Sonar variables--------------------------------------------------------------------  
  unsigned short sonar_measure = 0; 	  
  unsigned short sonar_value = 0;
  unsigned short sonar_UStimer = 0;
  
  
  //End of Sonar variables-------------------------------------------------------------  
  //Other variables--------------------------------------------------------------------
  int i = 0; //used in for statements
  unsigned short max_measures = 10; //number of measures to take at each point
  unsigned short counter_measures = 0;  //counter of measures done at a particular point
 
  bool tick_sensors = 1; //For timer
  bool clk=0;
  
  
  //End of other variables--------------------------------------------------------------------
  //Comunication variables--------------------------------------------------------------------
   unsigned long data=0;
   word Sent; //data block sent
   byte err; 
   bool is_send = 0;
  //End of Comunication variables-------------------------------------------------------------

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  AS1_Enable();
  while(1){
	  if(tick_motor){
		  tick_motor=0;
		  do
			 err=AS1_SendChar('a');		  			
		  while(err!=ERR_OK);
		  Bit2_PutVal(0);
		  clk=!clk;
		  
	  }
	  
  }
  /* For example: for(;;) { } */

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
