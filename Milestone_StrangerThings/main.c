//https://www.embeddedrelated.com/showarticle/420.php
//TI REsource explorer msp430g2xx3_uscia0_uart_01_9600.c
#include <msp430.h>


/**
 * main.c
 */
char commandlength; //Sets Command Length Inputted
char count; //Keeps track of light being set by input in if statements


void configureLED ()
{
    //P1.6 = Red LED
    //P2.1 = Green LED
    //P2.4 = Blue LED

    P1SEL |= BIT6; //Sets P1.6 as Timer_A0
    P1SEL2 &= ~BIT6;
    P1DIR |= BIT6; //Sets P1.6 as Output

    P2SEL |= BIT1; //Sets P2.1 as Timer_A1
    P2SEL2 &= ~BIT1;
    P2DIR |= BIT1; //Sets P2.1 as Output

    P2SEL |= BIT4; //Sets P2.4 as Timer_A1
    P2SEL2 &= ~BIT4;
    P2DIR |= BIT4; //Sets P2.4 as Output

    P2DIR |= BIT5; //Sets P2.5 as Output
    P2OUT &= ~BIT5; //Sets P2.5 as Off
}

void configureTimer ()
{
    TA0CTL = TASSEL_2 + MC_1 + ID_2 + TACLR; //Sets TimerA_0 as SMCLK, UP_Mode, 4 Divder, Clear Clock
    TA1CTL = TASSEL_2 + MC_1 + ID_2 + TACLR; //Sets TimerA_1 as SMCLK, UP_Mode, 4 Divder, Clear Clock

    TA0CCR0 = 255;//
    TA1CCR0 = 255; //Sets Timers to 1 MHz
    
    TA0CCR1 = 0; //
    TA1CCR1 = 0; //
    TA1CCR2 = 0; //Sets Capture Compare Registers to Initial Value


    TA0CCTL1 = OUTMOD_3; //
    TA1CCTL1 = OUTMOD_3; //
    TA1CCTL2 = OUTMOD_3; //Sets clocks to Set/Reset

}

void configureUART ()
{
    P1SEL |= BIT1+BIT2; //Sets P1.1 to RXD and P1.2 to TXD
    P1SEL2 |= BIT1+BIT2; //Sets P1.1 to RXD and P1.2 to TXD
    UCA0CTL1 |= UCSSEL_2; // SMCLK
    UCA0BR0 = 109; //sets to specified baud rate of 9600
    UCA0BR1 = 0; //sets to specified baud rate of 9600
    UCA0MCTL = UCBRS_2; // Modulation UCBRSx = 5
    UCA0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**
    UC0IE |= UCA0RXIE; // Enable USCI_A0 RX interrupt
}
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    configureTimer(); //Sets Timer intial settings
    configureUART(); //Sets UART Settings
    __bis_SR_register(LPM0_bits + GIE); //Sets Low Power Mode 0 and Global Interrupts

}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
    configureLED(); //Sets LEDs put in the interrupt to get rid of flicker bug
    P2OUT |= BIT5; //Turns on P2.5 Used to debugging
    UC0IE |= UCA0TXIE; //Sets Interrupt when transmitting
   char input = UCA0RXBUF; //Reads instructions sets input to the value


if (count == 0) 
{
    count = input; //Sets initial count value
    commandlength = input; //Set commandlength value

        UCA0TXBUF = input - 3; //Gives the next board in the line the command length minus the instructions used by the current board


}
else if (commandlength - count ==1) //Sets duty cycle percentage for Red LED
{
    TA0CCR1 = input; //Sets TimerA_0 Capture Compare Register 1 value
}
else if(commandlength - count ==2) //Sets duty cycle percentage for Green LED
{
    TA1CCR1=input; //Sets TimerA_1 Capture Compare Register 1 value
}
else if(commandlength -count ==3) //Sets duty cycle percentage for Blue LED
{
    TA1CCR2 = input; //Sets TimerA_1 Capture Compare Register 2 value
}
else //Transmits the last of the package inputted
{

        UCA0TXBUF = input; //Tansmits Instruction

}
count --; //Subtracts from count keeping track of inputted instruction
P2OUT &= ~BIT5; //Turns P2.5 Off
}

#pragma vector=USCIAB0TX_VECTOR //Used for debugging and verifying tranmitting 
__interrupt void USCI0TX_ISR(void) {
     P1OUT |= BIT0; //Turns P1.0 On
     UC0IE &= ~UCA0TXIE; //Clears interrupt flags 
     P1OUT &= ~BIT0; //Turns P1.0 Off
}
