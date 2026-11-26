#ifndef UART_H
#define UART_H

#include <stdint.h>

// --- RCC Layout ---
typedef struct {
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    uint32_t RESERVED0;
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;  // Offset: 0x30
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
    uint32_t RESERVED2;
    volatile uint32_t APB1ENR;
} RCC_TypeDef;


// --- GPIO Layout ---
typedef struct {
    volatile uint32_t MODER;    // 0x00
    volatile uint32_t OTYPER;   // 0x04
    volatile uint32_t OSPEEDR;  // 0x08
    volatile uint32_t PUPDR;    // 0x0C
    volatile uint32_t IDR;      // 0x10
    volatile uint32_t ODR;      // 0x14
    volatile uint32_t BSRR;     // 0x18
    volatile uint32_t LCKR;     // 0x1C
    volatile uint32_t AFR[2];   // 0x20 (AFRL), 0x24 (AFRH)
} GPIO_TypeDef;


// --- USART Layout (STM32F7 v2 USART) ---
typedef struct {
    volatile uint32_t CR1;    // 0x00
    volatile uint32_t CR2;    // 0x04
    volatile uint32_t CR3;    // 0x08
    volatile uint32_t BRR;    // 0x0C
    volatile uint32_t GTPR;   // 0x10
    volatile uint32_t RTOR;   // 0x14
    volatile uint32_t RQR;    // 0x18
    volatile uint32_t ISR;    // 0x1C
    volatile uint32_t ICR;    // 0x20
    volatile uint32_t RDR;    // 0x24
    volatile uint32_t TDR;    // 0x28
} USART_TypeDef;

#define GPIOD  ((GPIO_TypeDef *) 0x40020C00)
#define USART3 ((USART_TypeDef *)0x40004800)
// --- Base Pointers ---
#define RCC   ((RCC_TypeDef *) 0x40023800)
#define GPIOB ((GPIO_TypeDef *)0x40020400)

// --- Prototypes ---
void USART3_SendInt(int32_t value);
void USART3_Init(void);
void USART3_SendChar(char c);
void USART3_SendString(const char *str);
void USART3_SendHex(uint8_t value);
void USART3_SendTemp(int32_t temp_x100);
void USART3_SendPressure(uint32_t pressure);
void USART3_SendHumidity(uint32_t humidity);
void USART3_SendBytes(const uint8_t *data, uint32_t len);

#endif // UART_H
