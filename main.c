//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "scicomm.h"

// #define B0 0.001033
// #define B1 0.000767
// #define B2 -0.000266
// #define A1 -1.521886
// #define A2 0.521886

// #define REF 20
// #define UL 1
// #define LL 0

// float x[3]={0};
// float y[3]={0};


#pragma DATA_SECTION(vo,"CpuToCla1MsgRAM"); // aloca a variavel em espaços especificos da memória, definidos no .cmd
float vo;
#pragma DATA_SECTION(d,"Cla1ToCpuMsgRAM");
float d;


//
// Funcao Principal
//
void main(void)
{
    // Inicializacao do dispositivo
    Device_init();
    Interrupt_initModule();
    Interrupt_initVectorTable();
    Board_init();

    // Habilita interrupcoes globais e de tempo real
    EINT;
    ERTM;

    while (1)
    {

    }
}

//
// Rotina de Interrup��o da SCI (Recep��o)
//
__interrupt void INT_SCI0_RX_ISR(void)
{
    protocolReceiveData(SCI0_BASE,&vo,sizeof(float)); // recebe dado na SCI
    CLA_forceTasks(myCLA0_BASE,CLA_TASKFLAG_1); // dispara cla


    // Libera a interrupção no hardware
    SCI_clearInterruptStatus(SCI0_BASE, SCI_INT_RXFF);
    Interrupt_clearACKGroup(INT_SCI0_RX_INTERRUPT_ACK_GROUP);
}


__interrupt void cla1Isr1 () // chamada pela cla quando ela finaliza
{
    // CLA roda assincrona e o envio só acontece quando d foi calculado
    Interrupt_clearACKGroup(INT_myCLA01_INTERRUPT_ACK_GROUP);
    protocolSendData(SCI0_BASE, &d,sizeof(float));
}
