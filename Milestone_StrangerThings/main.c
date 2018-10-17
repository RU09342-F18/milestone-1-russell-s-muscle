#include <msp430.h> 


/**
 * main.c
 */
int commandlength = 0;
int Input = 0;
int count = 0;
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	

}

void configureLED ()
{
    //P1.6 = Red LED
    //P2.0 = Green LED
    //P2.4 = Blue LED

    P1SEL |= BIT6;
    P1SEL2 &= ~BIT6;
    P1DIR |= BIT6;

    P2SEL |= BIT0;
    P2SEL2 &= ~BIT0;
    P2DIR |= BIT0;

    P2SEL |= BIT4;
    P2SEL2 &= ~BIT4;
    P2DIR |= BIT4;

    P2DIR |= BIT5;
    P2OUT &= ~BIT5;
}

void configureTimer ()
{
    TA0CTL = TASSEL_2 + MC_1 + ID_2 + TACLR;
    TA1CTL = TASSEL_2 + MC_1 + ID_2 + TACLR;

    TA0CCR0 = 255;
    TA0CCR1 = 0;
    TA1CCR1 = 0;
    TA1CCR2 = 0;

    TA0CCTL = OUTMOD_3;
    TA1CCTL = OUTMOD_3;
    TA2CCTL = OUTMOD_3;

}

void configureUART ()
{
    
    UCOIE |=UCARXIE;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {

    P2OUT |= BIT5;

    UC0IE |= UCA0TXIE;
    Input |= UCA0RXBUF;
    
    switch (count)
    
    case (0)
    commandlength |= Input;
    break;
    case (1)
    
    
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void) {
     P1OUT |= BIT0;
     P1OUT &= ~BIT0;
     
     UCOIE &= ~UCA0TXIE;
}
