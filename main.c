#include <stdint.h>
#include "hardware/drivers/uart.h"

#define SAMPLE_WINDOW 512U

/* Linker script boundary symbols */
extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

int main(void);
void Reset_Handler(void);
void Default_Handler(void);

/* Minimal Cortex-M7 vector table placed at 0x08000000 by linker.ld */
__attribute__((section(".isr_vector")))
const uint32_t vector_table[] = {
    (uint32_t)&_estack,         /* Initial Stack Pointer */
    (uint32_t)Reset_Handler,    /* Reset */
    (uint32_t)Default_Handler,  /* NMI */
    (uint32_t)Default_Handler,  /* HardFault */
    (uint32_t)Default_Handler,  /* MemManage */
    (uint32_t)Default_Handler,  /* BusFault */
    (uint32_t)Default_Handler,  /* UsageFault */
};

void Reset_Handler(void) {
    /* Copy .data from Flash to RAM */
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    /* Zero .bss in RAM */
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0U;
    }

    main();

    while (1) {}
}

void Default_Handler(void) {
    while (1) {}
}

static void delay_cycles(volatile uint32_t count) {
    while (count--) {
        __asm__ volatile("nop");
    }
}

typedef enum StateMachine{
    INITIALIZE,
    STANDBY,
    OPERATE,
    TEST
}

int main(void) {

    StateMachine state = INITIALIZE;

    while (1) {
        switch (state){
            case INITIALIZE: {
                USART3_Init();

                state = STANDBY;
                break;
            }
            case STANDBY: {
                break;
            }   
            case OPERATE: {
                break;
            }
            case TEST: {
                break;
            }
        }
    }
}
