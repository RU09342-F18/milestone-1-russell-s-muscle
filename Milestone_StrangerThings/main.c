//https://www.embeddedrelated.com/showarticle/420.php
//TI REsource explorer msp430g2xx3_uscia0_uart_01_9600.c
#include <msp430.h>


/**
 * main.c
 */
int commandlength = 0;
int Input = 0;
int count = 0;
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer


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

    TA0CCTL1 = OUTMOD_3;
    TA1CCTL1 = OUTMOD_3;
    TA1CCTL2 = OUTMOD_3;

}

void configureUART ()
{
    P1SEL |= BIT1+BIT2;
    P1SEL2 |= BIT1+BIT2;
    UCA0CTL1 |= UCSSEL_2; // SMCLK
    UCA0BR0 = 9600; //sets to specified baud rate of 9600
    UCA0BR1 = 9600; //sets to specified baud rate of 9600
    UCA0MCTL = UCBRS2 + UCBRS0; // Modulation UCBRSx = 5
    UCA0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**
    UC0IE |= UCA0RXIE; // Enable USCI_A0 RX interrupt
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
