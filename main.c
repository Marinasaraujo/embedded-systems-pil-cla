//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "scicomm.h"



#pragma DATA_SECTION(vg,"CpuToCla1MsgRAM"); // aloca a variavel em espaços especificos da memória, definidos no .cmd
#pragma DATA_SECTION(ig,"CpuToCla1MsgRAM"); 
#pragma DATA_SECTION(ig_ref,"CpuToCla1MsgRAM"); 
float vg;
float ig;
float ig_ref;

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
    protocolReceiveData(SCIA_BASE, &vg,     sizeof(float)); 
    protocolReceiveData(SCIA_BASE, &ig,     sizeof(float)); 
    protocolReceiveData(SCIA_BASE, &ig_ref, sizeof(float)); 
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
