/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAMS70 has 4 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3

#define PA25 GPIO(GPIO_PORTA, 25)
#define PA26 GPIO(GPIO_PORTA, 26)
#define PA27 GPIO(GPIO_PORTA, 27)
#define PA28 GPIO(GPIO_PORTA, 28)
#define PA30 GPIO(GPIO_PORTA, 30)
#define PA31 GPIO(GPIO_PORTA, 31)
#define LED_ACT GPIO(GPIO_PORTB, 0)
#define LED_RDY GPIO(GPIO_PORTB, 1)
#define LED_ERR GPIO(GPIO_PORTB, 2)
#define BUTTON GPIO(GPIO_PORTB, 3)
#define CARD_EN GPIO(GPIO_PORTD, 15)
#define CARD_PWR GPIO(GPIO_PORTD, 16)
#define AB_SELECT GPIO(GPIO_PORTD, 17)
#define CARD_DETECT_A GPIO(GPIO_PORTD, 25)
#define CARD_DETECT_B GPIO(GPIO_PORTD, 26)
#define BUTTON_ALT GPIO(GPIO_PORTD, 30)

#endif // ATMEL_START_PINS_H_INCLUDED
