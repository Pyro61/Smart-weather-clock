#include <stdint.h>


void NMI_Handler(void)
{
    while(1);
}


__attribute__((naked))
void HardFault_Handler(void)
{
    asm volatile (
    	"tst lr, 1 << 2\n\t"
    	"ite eq\n\t"
    	"mrseq r0, msp\n\t"
    	"mrsne r0, psp\n\t"
    	"ldr r3, =HardFault_HandlerC\n\t"
    	"bx r3\n\t"
    );
}


void HardFault_HandlerC(uint32_t *hardfault_dump)
{
	__attribute__((unused)) volatile uint32_t r0 = hardfault_dump[0];
	__attribute__((unused)) volatile uint32_t r1 = hardfault_dump[1];
	__attribute__((unused)) volatile uint32_t r2 = hardfault_dump[2];
	__attribute__((unused)) volatile uint32_t r3 = hardfault_dump[3];
	__attribute__((unused)) volatile uint32_t r12 = hardfault_dump[4];
	__attribute__((unused)) volatile uint32_t lr = hardfault_dump[5];
	__attribute__((unused)) volatile uint32_t pc = hardfault_dump[6];
	__attribute__((unused)) volatile uint32_t xpsr = hardfault_dump[7];

    while (1);
}


void MemManage_Handler(void)
{
    while(1);
}


void BusFault_Handler(void)
{
    while(1);
}


void UsageFault_Handler(void)
{
    while(1);
}


void SVC_Handler(void)
{
    
}


void DebugMon_Handler(void)
{
    while(1);
}


void PendSV_Handler(void)
{

}


void SysTick_Handler(void)
{

}