//https://www.embeddedrelated.com/showarticle/420.php
//TI REsource explorer msp430g2xx3_uscia0_uart_01_9600.c
#include <msp430.h>


/**
 * main.c
 */
char commandlength;
char count;


void configureLED ()
{
    //P1.6 = Red LED
    //P2.1 = Green LED
    //P2.4 = Blue LED

    P1SEL |= BIT6;
    P1SEL2 &= ~BIT6;
    P1DIR |= BIT6;

    P2SEL |= BIT1;
    P2SEL2 &= ~BIT1;
    P2DIR |= BIT1;

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
    TA1CCR0 = 255;
    TA0CCR1 = 0;
    TA1CCR1 = 0;
    TA1CCR2 = 0;


    TA0CCTL1 = OUTMOD_3;
    TA1CCTL1 = OUTMOD_3;
    TA1CCTL2 = OUTMOD_3;

}

void configureUART ()
{
    P1SEL |= BIT1+BIT2;
    P1SEL2 |= BIT1+BIT2;
    UCA0CTL1 |= UCSSEL_2; // SMCLK
    UCA0BR0 = 104; //sets to specified baud rate of 9600
    UCA0BR1 = 0; //sets to specified baud rate of 9600
    UCA0MCTL = UCBRS2 + UCBRS0; // Modulation UCBRSx = 5
    UCA0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**
    UC0IE |= UCA0RXIE; // Enable USCI_A0 RX interrupt
}
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    configureLED();
    configureTimer();
    configureUART();
    __bis_SR_register(LPM0_bits + GIE);

}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
    //int commandlength = 0; =total
    //int count = 0; =counter
    //input = data

    P2OUT |= BIT5;
    UC0IE |= UCA0TXIE;
   char Input = UCA0RXBUF;

if (count == 0)
{
    count = Input;
    commandlength = Input;
    if (Input>=8)
    {
        UCA0TXBUF = Input - 3;
    }
}
else if (commandlength - count ==1)
{
    TA0CCR1 = Input;
}
else if(commandlength - count ==2)
{
    TA1CCR1=Input;
}
else if(commandlength -count ==3)
{
    TA1CCR2 = Input;
}
else
{
    if(commandlength>=8)
    {
        UCA0TXBUF = Input;
    }
}
count --;
P2OUT &= ~BIT5;
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void) {
     P1OUT |= BIT0;
     UC0IE &= ~UCA0TXIE;
     P1OUT &= ~BIT0;
}
