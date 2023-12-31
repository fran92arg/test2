/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//******************************************************************************
//   Software Port Interrupt Service on S1 from LPM3 with
//                     Internal Pull-up Resistance Enabled
//
//   A hi "TO" low transition on S1 will trigger P1_ISR/P2_ISR which,
//   toggles LED1. S1 is internally enabled to pull-up. LPM3 current
//   can be measured with the LED removed, all
//   unused Px.x configured as output or inputs pulled high or low.
//   ACLK = n/a, MCLK = SMCLK = default DCO
//
//!           MSP430FR2xx_4xx Board
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//       S1-->|                 |-->LED1
//
//
//   This example uses the following peripherals and I/O signals.  You must
//   review these and change as needed for your own board:
//   - GPIO Port peripheral
//
//   This example uses the following interrupt handlers.  To use this example
//   in your own application you must add these interrupt handlers to your
//   vector table.
//   - PORT1_VECTOR/PORT2_VECTOR
//******************************************************************************
#include "driverlib.h"
#include "Board.h"

void main (void)
{
    //Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);
    //
    //Set LED1 to output direction
    GPIO_setAsOutputPin(GPIO_PORT_LED1,GPIO_PIN_LED1);

    //Enable S1 internal resistance as pull-Up resistance
    //pin 1.6 sin pullup para el touch
    GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN6);
    //GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_S1,GPIO_PIN_S1);

    //S1 interrupt enabled
    GPIO_enableInterrupt(GPIO_PORT_P1,GPIO_PIN6);

    //S1 Hi/Lo edge
    GPIO_selectInterruptEdge(
        GPIO_PORT_P1,GPIO_PIN6,GPIO_LOW_TO_HIGH_TRANSITION);


    //S1 IFG cleared
    GPIO_clearInterrupt(GPIO_PORT_P1,GPIO_PIN6);

    PMM_unlockLPM5();

    //Enter LPM3 w/interrupt
    __bis_SR_register(GIE);

    //For debugger
    while(1){
//        if(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN6)==GPIO_INPUT_PIN_HIGH){
//            GPIO_toggleOutputOnPin(
//                GPIO_PORT_LED1,
//                GPIO_PIN_LED1
//                );
//        }
    }

}



#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt
void P1_ISR (void)
#endif //
{
    //LED1 = toggle
    GPIO_toggleOutputOnPin(GPIO_PORT_LED1,GPIO_PIN_LED1);

    //S1 IFG cleared
    GPIO_clearInterrupt(GPIO_PORT_P1,GPIO_PIN6);
}
//******************************************************************************
//
//This is the PORT2_VECTOR interrupt vector service routine
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt
void P2_ISR (void)
#endif // #if GPIO_PORT_S1
{
    //LED1 = toggle
//    GPIO_toggleOutputOnPin(
//        GPIO_PORT_LED1,
//        GPIO_PIN_LED1
//        );
//
//    //S1 IFG cleared
//    GPIO_clearInterrupt(
//        GPIO_PORT_S1,
//        GPIO_PIN_S1
//        );
}


