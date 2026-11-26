#include "uart.h"

void USART3_SendInt(int32_t value)
{
    char buffer[12];
    int i = 0;

    if (value < 0)
    {
        USART3_SendChar('-');
        value = -value;
    }

    do
    {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }
    while (value > 0);

    while (i > 0)
    {
        USART3_SendChar(buffer[--i]);
    }
}

/**
 * @brief Initializes USART3 on PD8 (TX) and PD9 (RX) at 115200 baud.
 * @pre System running on 16MHz HSI clock.
 * @note Configured for ST-LINK Virtual COM port routing.
 */
void USART3_Init(void) {
    // 1. Enable Clocks for GPIOD (Bit 3) and USART3 (Bit 18)
    RCC->AHB1ENR |= (1 << 3);
    RCC->APB1ENR |= (1 << 18);

    // 2. Configure PD8 and PD9 to Alternate Function mode (10 binary)
    GPIOD->MODER &= ~((3 << 16) | (3 << 18));
    GPIOD->MODER |= ((2 << 16) | (2 << 18));

    // 3. Map PD8 and PD9 to AF7 (USART1..3)
    // AFR[1] is AFRH. PD8 = bits 3:0, PD9 = bits 7:4.
    GPIOD->AFR[1] &= ~((0xF << 0) | (0xF << 4));
    GPIOD->AFR[1] |= ((7 << 0) | (7 << 4));

    // 4. Configure USART3 Baud Rate to 1000000 (16MHz / 1000000 = 16)
    USART3->BRR = 16;

    // 5. Enable USART (UE = Bit 0), Transmitter (TE = Bit 3)
    USART3->CR1 |= (1 << 0) | (1 << 3);
}

/**
 * @brief Transmits a single character over USART3.
 * @param[in] c Character to transmit.
 */
void USART3_SendChar(char c) {
    // Wait for TXE (Transmit Data Register Empty, Bit 7)
    while (!(USART3->ISR & (1 << 7)));
    USART3->TDR = c;
}

void USART3_SendHex(uint8_t value)
{
    const char hex[] = "0123456789ABCDEF";

    USART3_SendString("0x");

    // Upper nibble
    USART3_SendChar(hex[(value >> 4) & 0x0F]);

    // Lower nibble
    USART3_SendChar(hex[value & 0x0F]);
}

/**
 * @brief Transmits a null-terminated string.
 */
void USART3_SendString(const char *str) {
    while (*str) {
        USART3_SendChar(*str++);
    }
}

/**
 * @brief Formats and prints the 32-bit BME280 temperature payload.
 * @param[in] temp_x100 Temperature multiplied by 100 (e.g. 2345 = 23.45 C).
 */
void USART3_SendTemp(int32_t temp_x100) {
    if (temp_x100 < 0) {
        USART3_SendChar('-');
        temp_x100 = -temp_x100;
    }

    int32_t integer_part = temp_x100 / 100;
    int32_t decimal_part = temp_x100 % 100;

    // Print integer portion
    char buf[12];
    int i = 0;
    if (integer_part == 0) {
        buf[i++] = '0';
    } else {
        while (integer_part > 0) {
            buf[i++] = (integer_part % 10) + '0';
            integer_part /= 10;
        }
    }
    while (i > 0) USART3_SendChar(buf[--i]);

    // Print decimal portion
    USART3_SendChar('.');
    if (decimal_part < 10) USART3_SendChar('0'); // Pad leading zero
    
    i = 0;
    if (decimal_part == 0) {
        buf[i++] = '0';
    } else {
        while (decimal_part > 0) {
            buf[i++] = (decimal_part % 10) + '0';
            decimal_part /= 10;
        }
    }
    while (i > 0) USART3_SendChar(buf[--i]);
    
    USART3_SendString(" C\r\n");
}

/**
 * @brief Formats and prints a 32-bit unsigned integer for pressure.
 * @param[in] pressure The atmospheric pressure in Pascals.
 */
void USART3_SendPressure(uint32_t pressure) {
    char buf[12];
    int i = 0;

    // Handle absolute zero case
    if (pressure == 0) {
        buf[i++] = '0';
    } else {
        // Extract digits right-to-left
        while (pressure > 0) {
            buf[i++] = (pressure % 10) + '0';
            pressure /= 10;
        }
    }

    // The digits are backward in the buffer, so print them in reverse
    while (i > 0) {
        USART3_SendChar(buf[--i]);
    }
    
    // Append the unit and a newline
    USART3_SendString(" Pa\r\n");
}

/**
 * @brief Formats and prints a 32-bit unsigned integer for humidity.
 * @param[in] humidity The relative humidity in percentage * 100 (e.g., 5680 for 56.80%).
 */
void USART3_SendHumidity(uint32_t humidity) {
    uint32_t int_part = humidity / 100;
    uint32_t frac_part = humidity % 100;
    char buf[12];
    int i = 0;

    // Handle integer part
    if (int_part == 0) {
        buf[i++] = '0';
    } else {
        // Extract digits right-to-left
        while (int_part > 0) {
            buf[i++] = (int_part % 10) + '0';
            int_part /= 10;
        }
    }

    // The integer digits are backward in the buffer, print them in reverse
    while (i > 0) {
        USART3_SendChar(buf[--i]);
    }
    
    // Print the decimal point
    USART3_SendChar('.');
    
    // Print the fractional part (hardcoded to exactly 2 digits to preserve leading zeros like .05)
    USART3_SendChar((frac_part / 10) + '0');
    USART3_SendChar((frac_part % 10) + '0');

    // Append the unit and a newline
    USART3_SendString(" %\r\n");
}


void USART3_SendBytes(const uint8_t *data, uint32_t len) {
    for (uint32_t i = 0; i < len; i++) {
        while (!(USART3->ISR & (1 << 7)));
        USART3->TDR = data[i];
    }
}
