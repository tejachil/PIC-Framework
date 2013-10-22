/*
 * GPIO driver definition.
 * Allows initialization of PORTB as output and allows writing a byte to the
 * PORTB pins.
 */
#ifndef MY_GPIO_H
#define	MY_GPIO_H

void gpio_init_portb_output(void);
void gpio_write_portb(const unsigned char val);
unsigned int gpio_read_portb(void);

#endif	/* MY_GPIO_H */

