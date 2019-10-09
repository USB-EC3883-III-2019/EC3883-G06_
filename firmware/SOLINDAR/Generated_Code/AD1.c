/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : AD1.c
**     Project     : SOLINDAR
**     Processor   : MC9S08QE128CLK
**     Component   : ADC
**     Version     : Component 01.690, Driver 01.30, CPU db: 3.00.067
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2019-10-07, 20:40, # CodeGen: 18
**     Abstract    :
**         This device "ADC" implements an A/D converter,
**         its control methods and interrupt/event handling procedure.
**     Settings    :
**          Component name                                 : AD1
**          A/D converter                                  : ADC
**          Sharing                                        : Disabled
**          Interrupt service/event                        : Enabled
**            A/D interrupt                                : Vadc
**            A/D interrupt priority                       : medium priority
**          A/D channels                                   : 1
**            Channel0                                     : 
**              A/D channel (pin)                          : PTF7_ADP17
**              A/D channel (pin) signal                   : 
**          A/D resolution                                 : Autoselect
**          Conversion time                                : 46 �s
**          Low-power mode                                 : Disabled
**          Sample time                                    : short
**          Internal trigger                               : Disabled
**          Number of conversions                          : 1
**          Initialization                                 : 
**            Enabled in init. code                        : yes
**            Events enabled in init.                      : yes
**          CPU clock/speed selection                      : 
**            High speed mode                              : This component enabled
**            Low speed mode                               : This component disabled
**            Slow speed mode                              : This component disabled
**          High input limit                               : 1
**          Low input limit                                : 0
**          Get value directly                             : yes
**          Wait for result                                : yes
**     Contents    :
**         Enable       - byte AD1_Enable(void);
**         Disable      - byte AD1_Disable(void);
**         EnableEvent  - byte AD1_EnableEvent(void);
**         DisableEvent - byte AD1_DisableEvent(void);
**         Start        - byte AD1_Start(void);
**         Stop         - byte AD1_Stop(void);
**         Measure      - byte AD1_Measure(bool WaitForResult);
**         MeasureChan  - byte AD1_MeasureChan(bool WaitForResult, byte Channel);
**         GetChanValue - byte AD1_GetChanValue(byte Channel, void* Value);
**         GetValue16   - byte AD1_GetValue16(word *Values);
**
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file AD1.c
** @version 01.30
** @brief
**         This device "ADC" implements an A/D converter,
**         its control methods and interrupt/event handling procedure.
*/         
/*!
**  @addtogroup AD1_module AD1 module documentation
**  @{
*/         


/* MODULE AD1. */

#include "Events.h"
#include "AD1.h"

#pragma MESSAGE DISABLE C5703          /* Disable warning C5703 "Parameter is not referenced" */



#define STOP            0x00U          /* STOP state           */
#define MEASURE         0x01U          /* MESURE state         */
#define CONTINUOUS      0x02U          /* CONTINUOS state      */
#define SINGLE          0x03U          /* SINGLE state         */


static const  byte Channels = 0x51U;   /* Content for the device control register */

static bool EnUser;                    /* Enable/Disable device */
volatile bool AD1_EnEvent;             /* Enable/Disable events */
static volatile bool OutFlg;           /* Measurement finish flag */
static volatile byte ModeFlg;          /* Current state of device */

volatile word AD1_OutV;                /* Sum of measured values */





/*
** ===================================================================
**     Method      :  AD1_Interrupt (component ADC)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes event(s) of the component.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
ISR(AD1_Interrupt)
{
  if (ModeFlg == STOP) {               /* If the driver is in STOP mode */
    (void)ADCRL;                       /* Clear interrupt flag */
    return;                            /* Return from interrupt */
  }
  if (ModeFlg != SINGLE) {
    /*lint -save  -e926 -e927 -e928 -e929 Disable MISRA rule (11.4) checking. */
    ((TWREG volatile*)(&AD1_OutV))->b.high = ADCRH; /* Save measured value */
    ((TWREG volatile*)(&AD1_OutV))->b.low = ADCRL; /* Save measured value */
    /*lint -restore Enable MISRA rule (11.4) checking. */
    OutFlg = TRUE;                     /* Measured value is available */
    if (AD1_EnEvent) {                 /* Are events enables? */
      AD1_OnEnd();                     /* If yes then invoke user event */
    }
    if (ModeFlg == MEASURE) {          /* Is the device in the measure state? */
      ModeFlg = STOP;                  /* Set the device to the stop mode */
      return;                          /* Return from interrupt */
    }
    ADCSC1 = Channels;                 /* Restart measurement */
  }
  else {
    /*lint -save  -e926 -e927 -e928 -e929 Disable MISRA rule (11.4) checking. */
    ((TWREG volatile*)(&AD1_OutV))->b.high = ADCRH; /* Save measured value */
    ((TWREG volatile*)(&AD1_OutV))->b.low = ADCRL; /* Save measured value */
    /*lint -restore Enable MISRA rule (11.4) checking. */
    OutFlg = TRUE;                     /* Measured value is available */
    if (AD1_EnEvent) {                 /* Are events enables? */
      AD1_OnEnd();                     /* If yes then invoke user event */
    }
    ModeFlg = STOP;                    /* Set the device to the stop mode */
  }
}

/*
** ===================================================================
**     Method      :  AD1_HWEnDi (component ADC)
**
**     Description :
**         Enables or disables the peripheral(s) associated with the 
**         component. The method is called automatically as a part of the 
**         Enable and Disable methods and several internal methods.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void AD1_HWEnDi(void)
{
  if (EnUser) {                        /* Enable device? */
    if (ModeFlg) {                     /* Start or stop measurement? */
      OutFlg = FALSE;                  /* Output value isn't available */
      ADCSC1 = Channels;               /* If yes then start the conversion */
    }
  }
  else {
    ADCSC1 = 0x1FU;                    /* Disable the device */
  }
}

/*
** ===================================================================
**     Method      :  AD1_Enable (component ADC)
*/
/*!
**     @brief
**         Enables A/D converter component. [Events] may be generated
**         ([DisableEvent]/[EnableEvent]). If possible, this method
**         switches on A/D converter device, voltage reference, etc.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
*/
/* ===================================================================*/
byte AD1_Enable(void)
{
  if (EnUser) {                        /* Is the device enabled by user? */
    return ERR_OK;                     /* If yes then set the flag "device enabled" */
  }
  EnUser = TRUE;                       /* Set the flag "device enabled" */
  AD1_HWEnDi();                        /* Enable the device */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AD1_Disable (component ADC)
*/
/*!
**     @brief
**         Disables A/D converter component. No [events] will be
**         generated. If possible, this method switches off A/D
**         converter device, voltage reference, etc.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
*/
/* ===================================================================*/
byte AD1_Disable(void)
{
  if (!EnUser) {                       /* Is the device disabled by user? */
    return ERR_OK;                     /* If yes then OK */
  }
  EnUser = FALSE;                      /* If yes then set the flag "device disabled" */
  AD1_HWEnDi();                        /* Enable the device */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AD1_EnableEvent (component ADC)
**     Description :
**         Enables the <events>.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
/*
byte AD1_EnableEvent(void)

**      This method is implemented as a macro. See header module. **
*/

/*
** ===================================================================
**     Method      :  AD1_DisableEvent (component ADC)
**     Description :
**         Disables the <events>.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
/*
byte AD1_DisableEvent(void)

**      This method is implemented as a macro. See header module. **
*/

/*
** ===================================================================
**     Method      :  AD1_Start (component ADC)
*/
/*!
**     @brief
**         This method starts continuous conversion on all channels
**         that are set in the component inspector. When each
**         measurement on all channels has finished the [OnEnd ] event
**         may be invoked. This method is not available if the
**         [interrupt service] is disabled and the device doesn't
**         support the continuous mode. Note: If time of measurement is
**         too short and the instruction clock is too slow then the
**         conversion complete interrupt and its handler may cause a
**         system overflow.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - Device is disabled
**                           ERR_BUSY - A conversion is already running
*/
/* ===================================================================*/
byte AD1_Start(void)
{
  if (!EnUser) {                       /* Is the device disabled by user? */
    return ERR_DISABLED;               /* If yes then error */
  }
  if (ModeFlg != STOP) {               /* Is the device in running mode? */
    return ERR_BUSY;                   /* If yes then error */
  }
  ModeFlg = CONTINUOUS;                /* Set state of device to the continuos mode */
  AD1_HWEnDi();                        /* Enable the device */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AD1_Stop (component ADC)
**     Description :
**         This method stops the continuous measurement or disables
**         a trigger mode (if supported by HW), which has been
**         started by one of the following methods:
**         Version specific information for Freescale HCS08 and HC08
**         derivatives ] 
**         - <Start> 
**         - <EnableIntChanTrigger>
**         - <EnableExtChanTrigger>
**         The Stop method is available if one of the previously
**         mentioned methods is supported by A/D converter device
**         and is enabled to be generated.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_BUSY - No continuous measurement is
**                           running. Neither internal trigger nor
**                           external trigger have been enabled (if
**                           these are supported by HW).
** ===================================================================
*/
byte AD1_Stop(void)
{
  if (ModeFlg != CONTINUOUS) {         /* Is the device in different mode than "continuous"? */
    return ERR_BUSY;                   /* If yes then error */
  }
  ModeFlg = STOP;                      /* Set state of device to the stop mode */
  ADCSC1 = 0x1FU;                      /* Abort the current conversion */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AD1_Measure (component ADC)
*/
/*!
**     @brief
**         This method performs one measurement on all channels that
**         are set in the component inspector. (Note: If the [number of
**         conversions] is more than one the conversion of A/D channels
**         is performed specified number of times.)
**     @param
**         WaitForResult   - Wait for a result of a
**                           conversion. If [interrupt service] is
**                           disabled, A/D peripheral doesn't support
**                           measuring all channels at once or Autoscan
**                           mode property isn't enabled and at the same
**                           time the [number of channels] is greater
**                           than 1, then the WaitForResult parameter is
**                           ignored and the method waits for each
**                           result every time. If the [interrupt
**                           service] is disabled and a [number of
**                           conversions] is greater than 1, the
**                           parameter is ignored and the method also
**                           waits for each result every time.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - Device is disabled
**                           ERR_BUSY - A conversion is already running
*/
/* ===================================================================*/
#pragma MESSAGE DISABLE C5703 /* WARNING C5703: Parameter declared but not referenced */
byte AD1_Measure(bool WaitForResult)
{
  if (!EnUser) {                       /* Is the device disabled by user? */
    return ERR_DISABLED;               /* If yes then error */
  }
  if (ModeFlg != STOP) {               /* Is the device in different mode than "stop"? */
    return ERR_BUSY;                   /* If yes then error */
  }
  ModeFlg = MEASURE;                   /* Set state of device to the measure mode */
  AD1_HWEnDi();                        /* Enable the device */
  if (WaitForResult) {                 /* Is WaitForResult TRUE? */
    while (ModeFlg == MEASURE) {}      /* If yes then wait for end of measurement */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AD1_MeasureChan (component ADC)
*/
/*!
**     @brief
**         This method performs measurement on one channel. (Note: If
**         the [number of conversions] is more than one the conversion
**         of the A/D channel is performed specified number of times.)
**     @param
**         WaitForResult   - Wait for a result of
**                           conversion. If the [interrupt service] is
**                           disabled and at the same time a [number of
**                           conversions] is greater than 1, the
**                           WaitForResult parameter is ignored and the
**                           method waits for each result every time.
**     @param
**         Channel         - Channel number. If only one
**                           channel in the component is set this
**                           parameter is ignored, because the parameter
**                           is set inside this method.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - Device is disabled
**                           ERR_BUSY - A conversion is already running
**                           ERR_RANGE - Parameter "Channel" out of range
*/
/* ===================================================================*/
byte PE_AD1_MeasureChan(bool WaitForResult)
{
  if (!EnUser) {                       /* Is the device disabled by user? */
    return ERR_DISABLED;               /* If yes then error */
  }
  if (ModeFlg != STOP) {               /* Is the device in different mode than "stop"? */
    return ERR_BUSY;                   /* If yes then error */
  }
  ModeFlg = SINGLE;                    /* Set state of device to the measure mode */
  AD1_HWEnDi();                        /* Enable the device */
  if (WaitForResult) {                 /* Is WaitForResult TRUE? */
    while (ModeFlg == SINGLE) {}       /* If yes then wait for end of measurement */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AD1_GetChanValue (component ADC)
*/
/*!
**     @brief
**         Returns the last measured value of the required channel.
**         Format and width of the value is a native format of the A/D
**         converter.
**     @param
**         Channel         - Channel number. If only one
**                           channel in the component is set then this
**                           parameter is ignored.
**     @param
**         Value           - Pointer to the measured value. Data
**                           type is a byte, a word or an int. It
**                           depends on the supported modes, resolution,
**                           etc. of the AD converter. See the Version
**                           specific information for the current CPU in
**                           [General Info].
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_NOTAVAIL - Requested value not
**                           available
**                           ERR_RANGE - Parameter "Channel" out of
**                           range
**                           ERR_OVERRUN - External trigger overrun flag
**                           was detected after the last value(s) was
**                           obtained (for example by GetValue). This
**                           error may not be supported on some CPUs
**                           (see generated code).
*/
/* ===================================================================*/
byte AD1_GetChanValue(byte Channel,void* Value)
{
  if (OutFlg == 0U) {                  /* Is output flag set? */
    return ERR_NOTAVAIL;               /* If no then error */
  }
  *(word*)Value = AD1_OutV;            /* Save measured values to the output buffer */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AD1_GetValue16 (component ADC)
*/
/*!
**     @brief
**         This method returns the last measured values of all channels.
**         Compared with [GetValue] method this method returns more
**         accurate result if the [number of conversions] is greater
**         than 1 and [AD resolution] is less than 16 bits. In addition,
**         the user code dependency on [AD resolution] is eliminated.
**     @param
**         Values          - Pointer to the array that contains
**                           the measured data.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_NOTAVAIL - Requested value not
**                           available
**                           ERR_OVERRUN - External trigger overrun flag
**                           was detected after the last value(s) was
**                           obtained (for example by GetValue). This
**                           error may not be supported on some CPUs
**                           (see generated code).
*/
/* ===================================================================*/
byte AD1_GetValue16(word *Values)
{
  if (OutFlg == 0U) {                  /* Is output flag set? */
    return ERR_NOTAVAIL;               /* If no then error */
  }
  *Values = (word)((AD1_OutV) << 4);   /* Save measured values to the output buffer */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AD1_Init (component ADC)
**
**     Description :
**         Initializes the associated peripheral(s) and the component's 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void AD1_Init(void)
{
  /* ADCSC1: COCO=0,AIEN=0,ADCO=0,ADCH4=1,ADCH3=1,ADCH2=1,ADCH1=1,ADCH0=1 */
  setReg8(ADCSC1, 0x1FU);              /* Disable the module */ 
  /* ADCSC2: ADACT=0,ADTRG=0,ACFE=0,ACFGT=0,??=0,??=0,??=0,??=0 */
  setReg8(ADCSC2, 0x00U);              /* Disable HW trigger and autocompare */ 
  EnUser = TRUE;                       /* Enable device */
  AD1_EnEvent = TRUE;                  /* Enable events */
  OutFlg = FALSE;                      /* No measured value */
  ModeFlg = STOP;                      /* Device isn't running */
  /* ADCCFG: ADLPC=0,ADIV1=1,ADIV0=1,ADLSMP=0,MODE1=0,MODE0=1,ADICLK1=1,ADICLK0=1 */
  setReg8(ADCCFG, 0x67U);              /* Set prescaler bits */ 
  AD1_HWEnDi();                        /* Enable/disable device according to the status flags */
}


/* END AD1. */

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
