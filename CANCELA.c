//*******************************************************************************
// UFSC- Universidade Federal de Santa Catarina
// Projeto: Cancela
// Autor: Douglas Bomfim de Sousa
// Cancela com interrupção
//*******************************************************************************


#include <xc.h>
#include <pic16f877a.h>
#include <stdio.h>
void esperandoSensor(); //Função que espera receber sinal do sensor de automóvel
void abrircancela(); //Função de abertura da cancela
void fecharcancela();//Função que fecha cancela
// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit

#define _XTAL_FREQ 4000000
int flagInt=0; // varíavel que controla se houve ou não uma interrupção
void main(void) 
{
    OPTION_REG = 0b00111111;
    INTCONbits.GIE = 1;
    INTCONbits.INTE = 1;
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB3 =1;
    TRISD = 0;
    while(1) //loop principal
    {
        flagInt=0; // reseta a flag de interrupção
        esperandoSensor();
        abrircancela();
    }
}
// função que fica esperando o sensor de automóvel
void esperandoSensor()
{
    PORTD = 0; // desliga todas as saídas
    while(1 && flagInt==0)// Enquanto o sensor de automóvel estiver em 1 e a flag em 0 fica dentro desse loop
    {
        if(PORTBbits.RB1==0)// Se o sesor de automóvel for para 0 sai do loop
        {
            break;
        }
    }
}
//Função que faz a abertura da cancela
void abrircancela()
{
    PORTDbits.RD0=1; //Acende a luz de abrindo
    PORTDbits.RD1=1; // Liga o motor da cancela
    
    while(1 && flagInt==0) // Loop infinito que sai somente quando a flag de interrupção for 1
    {
        if(PORTBbits.RB3==0) //verifica se o sensor de cancela aberta foi ou não acionada, se for verdade ela entra no if, caso contrário fica no loop
        {
            PORTDbits.RD0=0; //Desliga o led de abrindo
            PORTDbits.RD1=0; // Desliga o motor de abrindo
            __delay_ms(20000);
            fecharcancela(); // vai para a função de fechar cancela
            break;// sai do loop
        }
    }
}

void fecharcancela()
{
    PORTDbits.RD2 = 1;//liga o LED de fechando
    PORTDbits.RD3 = 1;//liga o Motor de fechando
    
    while(1 && flagInt==0) // Loop infinito que sai somente quando a flag de interrupção for 1
    {
        if(PORTBbits.RB2==0) //verifica se o sensor de cancela fechada foi ou não acionada, se for verdade ela entra no if, caso contrário fica no loop 
        {
            PORTDbits.RD2 = 0;//deliga o LED de fechando
            PORTDbits.RD3 = 0;//desliga o motor de fechando
            break;// sai do loop
        }
    }
}
//função de interrupção de quando o trem está passando
void __interrupt() tremchegando()
{
    if(INTF)//verifica se a flag INTF foi acionada
    {
    INTCONbits.INTF = 0;//reseta a flag
    PORTDbits.RD4=1;//liga o buzzer
    if(PORTBbits.RB1==0 && PORTBbits.RB3==0)//se o sensor de carro estiver acionado e a cancela aberta, ele entra nesse if
    {
        PORTDbits.RD0=0;//desliga a luz de abrindo
        PORTDbits.RD1=0;//desliga o motor
        __delay_ms(20000);
        fecharcancela();//vai para a função fechar cancela
    }
    if(PORTBbits.RB1==0)//verifica se o sensor de automóvel está acionado, se estiver ele entra
    {
       PORTDbits.RD0=0;//desliga a luz de abrindo
       PORTDbits.RD1=0;//desliga o motor
       fecharcancela();//vai para a função fechar cancela 
    }
    if(PORTDbits.RD1==1)//se o motor de abrir cancela estiver acionado, ele entra
    {
        PORTDbits.RD0=0;//desliga a luz de abrindo
        PORTDbits.RD1=0;//desliga o motor
        fecharcancela();//vai para a função fechar cancela 
    }
    if(PORTBbits.RB3==0)//verifica se o sensor de cancela aberta está acionada
    {
        PORTDbits.RD0=0;//desliga a luz de abrindo
        PORTDbits.RD1=0;//desliga o motor
        fecharcancela();//vai para a função fechar cancela 
    }
    while(1)//loop espera o sensor do trem ser desativado
    {
        if(PORTBbits.RB0==1)//se a entrada do sensor for acionado ele entra
        {
            PORTDbits.RD0=0;//desliga a luz de abrindo
            PORTDbits.RD1=0;//desliga o motor
            break;// sai do loop
        }
    }
    PORTDbits.RD4=0;//desliga o buzzer
    }
    flagInt=1;//coloca a flag de interrupção em 1
}