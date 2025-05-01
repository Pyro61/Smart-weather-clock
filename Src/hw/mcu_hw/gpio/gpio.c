#include "gpio.h"
#include "stm32g4xx.h"
#include <stdbool.h>
#include <stdlib.h>

/* Quantity defines (mcu specific) */
#define PORT_QUANTITY           7
#define PIN_QUANTITY            15
#define AF_QUANTITY             15

/* Swap 1 and 0 macro */
#define SWAP_1_AND_0(x)         (x == 1 ? 0 : 1)

/* Port address pointer */
GPIO_TypeDef *port_ptr;

/* EXTI callback address holder */
cb_t exti_cb_array[PIN_QUANTITY];

/* Port address pointers array */
GPIO_TypeDef *const port_ptr_array[PORT_QUANTITY] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG};

/* Support function declarations */
static bool is_port_correct(enum gpio_port port);
static bool is_pin_correct(uint8_t pin);
static bool is_mode_correct(enum gpio_mode mode);
static bool is_pp_od_correct(enum gpio_pp_od pp_od);
static bool is_pull_correct(enum gpio_pull pull);
static bool is_speed_correct(enum gpio_speed speed);
static bool is_af_correct(uint8_t af);
static bool is_trigger_correct(enum exti_trigger trig);
static uint8_t get_af_reg(uint8_t pin);
static uint8_t get_exti_irq_n(uint8_t pin);


enum gpio_err gpio_config(enum gpio_port port, uint8_t pin, uint8_t mode, uint8_t pp_od, uint8_t pull, uint8_t speed)
{
    /* Check if all values are correct */
    if (is_port_correct(port) && is_pin_correct(pin) && is_mode_correct(mode) && is_pp_od_correct(pp_od) && is_pull_correct(pull) && is_speed_correct(speed))
    {
        /* Everything is alright, configure gpio */
        /* Enable clock */
        RCC -> AHB2ENR |= 1 << port;

        /* Reset and set gpio mode */
        port_ptr -> MODER &= ~(0x03 << (pin * 2));          
        port_ptr -> MODER |= (mode << (pin * 2));

        /* Reset and set gpio output type (push pull/open drain) */
        port_ptr->OTYPER &= ~(0x01 << pin);
        port_ptr->OTYPER |= (pp_od << pin);

        /* Reset and set gpio pull type */
        port_ptr->PUPDR &= ~(0x03 << (pin * 2));
        port_ptr->PUPDR |= (pull << (pin * 2));

        /* Reset and set gpio speed */
        port_ptr -> OSPEEDR &= ~(0x03 << (pin * 2));
        port_ptr -> OSPEEDR |= (speed << (pin * 2));

        return NO_ERR;
    }

    else
    {
        /* Something was wrong, don't configure gpio registers */
        return ERR;
    }
 
}


enum gpio_err gpio_set_af(enum gpio_port port, uint8_t pin, uint8_t af)
{
    /* Check if all values are correct */
    if(is_port_correct(port) && is_pin_correct(pin) && is_af_correct(af))
    {   
        /* Everything is alright, configure af */
        uint8_t af_reg;

        /* Get port address */
        GPIO_TypeDef *port_ptr_af = port_ptr_array[port];

        /* Get af register (depends on which pin is configured) */
        af_reg = get_af_reg(pin);

        /* Reset and set af register */
        port_ptr_af -> AFR[af_reg] &= ~(0xF << ((pin % 8) * 4));
        port_ptr_af -> AFR[af_reg] |= (af << ((pin % 8) * 4));

        return NO_ERR;
    }

    else
    {
        /* Something is wrong, don't configure af */
        return ERR;
    }
}


enum gpio_err gpio_set_exti(enum gpio_port port, uint8_t pin, enum exti_trigger trig, cb_t cb)
{
    /* Check if all values are correct */
    if (is_pin_correct(pin) && is_trigger_correct(trig) && is_port_correct(port))
    {   
        if (exti_cb_array[pin] != NULL)
        {
            /* EXTI on that pin is already configured, abort configuration */
            return ERR;
        }

        /* Set trigger edge */
        switch (trig)
        {
            case RISING_EDGE:
            {
                EXTI -> RTSR1 |= 1 << pin;
                break;
            }

            case FALLING_EDGE:
            {
                EXTI -> FTSR1 |= 1 << pin;
                break;
            }

            case BOTH_EDGES:
            {
                EXTI -> RTSR1 |= 1 << pin;
                EXTI -> FTSR1 |= 1 << pin;
                break;
            }
        }

        /* Select correct port to handle EXTI */
        RCC -> APB2ENR |= RCC_APB2ENR_SYSCFGEN;
        SYSCFG->EXTICR[pin / 4] &= ~(0xF << (4 * (pin % 4)));
        SYSCFG -> EXTICR[pin / 4] |= ((uint8_t)port << (4 * (pin % 4)));
        
        /* Configure NVIC */
        uint8_t irq_n = get_exti_irq_n(pin);
        EXTI -> IMR1 |= 1 << pin; /* Disable irq mask */
        NVIC_SetPriority(irq_n, 10); /* Set low priority because inside irq callback is called */
        NVIC_EnableIRQ(irq_n);

        /* Write callback to holder */
        exti_cb_array[pin] = cb;

        return NO_ERR;
    }

    else
    {
        /* Something is wrong, don't configure exti */
        return ERR;
    }
}


enum gpio_err gpio_output_write(enum gpio_port port, uint8_t pin, enum gpio_state state)
{
    if (is_port_correct(port) && is_pin_correct(pin))
    {
        /* Get port address */
        GPIO_TypeDef *ptr = port_ptr_array[port];

        /* Write pin state */
        if (state == STATE_ERR)
        {
            /* Wrong state */
           return ERR;
        }
        
        else
        {
            ptr -> BSRR = (1 << (pin + (16 * SWAP_1_AND_0(state))));
        }

        return NO_ERR;
    }

    else
    {
        return ERR;
    }
}


enum gpio_state gpio_input_read(enum gpio_port port, uint8_t pin)
{
    if (is_port_correct(port) && is_pin_correct(pin))
    {
        /* Get port address */
        GPIO_TypeDef *ptr = port_ptr_array[port];

        /* Read pin state */
        return ptr -> IDR & (1 << pin);
    }

    else
    {
        return STATE_ERR;
    }
}


/* EXTI IRQ */
void EXTI15_10_IRQHandler(void)
{
    /* EXTI line 12 IRQ */
    if (EXTI -> PR1 & EXTI_PR1_PIF12)
    {
        EXTI -> PR1 |= EXTI_PR1_PIF12;
        exti_cb_array[12]();
    }
}


void EXTI0_IRQHandler(void)
{
    /* EXTI line 0 IRQ */
    if (EXTI -> PR1 & EXTI_PR1_PIF0)
    {
        EXTI -> PR1 |= EXTI_PR1_PIF0;
        exti_cb_array[0]();
    }
}


void EXTI1_IRQHandler(void)
{
    /* EXTI line 1 IRQ */
    if (EXTI -> PR1 & EXTI_PR1_PIF1)
    {
        EXTI -> PR1 |= EXTI_PR1_PIF1;
        exti_cb_array[1]();
    }
}


void EXTI2_IRQHandler(void)
{
    /* EXTI line 2 IRQ */
    if (EXTI -> PR1 & EXTI_PR1_PIF2)
    {
        EXTI -> PR1 |= EXTI_PR1_PIF2;
        exti_cb_array[2]();
    }
}


void EXTI3_IRQHandler(void)
{
    /* EXTI line 3 IRQ */
    if (EXTI -> PR1 & EXTI_PR1_PIF3)
    {
        EXTI -> PR1 |= EXTI_PR1_PIF3;
        exti_cb_array[3]();
    }
}


void EXTI4_IRQHandler(void)
{
    /* EXTI line 4 IRQ */
    if (EXTI -> PR1 & EXTI_PR1_PIF4)
    {
        EXTI -> PR1 |= EXTI_PR1_PIF4;
        exti_cb_array[4]();
    }
}


/* Support functions */
static bool is_port_correct(enum gpio_port port)
{
    if (port >= START_PORT && port <= END_PORT)
    {
        port_ptr = port_ptr_array[port];
        return true;
    }
    
    else
    {
        return false;
    }
    
}


static bool is_pin_correct(uint8_t pin)
{
    return pin <= PIN_QUANTITY;
}


static bool is_mode_correct(enum gpio_mode mode)
{
    return mode >= START_MODE && mode <= END_MODE;
}


static bool is_pp_od_correct(enum gpio_pp_od pp_od)
{
    return pp_od >= START_PP_OD && pp_od <= END_PP_OD;
}


static bool is_pull_correct(enum gpio_pull pull)
{
    return pull >= START_PULL && pull <= END_PULL;
}


static bool is_speed_correct(enum gpio_speed speed)
{
    return speed >= START_SPEED && speed <= END_SPEED;
}


static bool is_af_correct(uint8_t af)
{
    return af <= AF_QUANTITY;
}


static bool is_trigger_correct(enum exti_trigger trig)
{
    return trig >= START_TRIG && trig <= END_TRIG;
}


static uint8_t get_af_reg(uint8_t pin)
{
    if(pin > 7)
    {
        return 1;
    }

    else
    {
        return 0;
    }
}


static uint8_t get_exti_irq_n(uint8_t pin)
{
    if (pin == 0) return EXTI0_IRQn;
    else if (pin == 1) return EXTI1_IRQn;
    else if (pin == 2) return EXTI2_IRQn;
    else if (pin == 3) return EXTI3_IRQn;
    else if (pin == 4) return EXTI4_IRQn;
    else if (pin >= 5 && pin <= 9) return EXTI9_5_IRQn;
    else if (pin >= 10 && pin <= 15) return EXTI15_10_IRQn;
}