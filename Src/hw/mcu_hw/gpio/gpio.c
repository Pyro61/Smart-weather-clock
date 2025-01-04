#include "gpio.h"
#include "stm32g4xx.h"
#include "stdbool.h"

/* Quantity defines (mcu specific) */
#define PORT_QUANTITY           7
#define PIN_QUANTITY            15
#define AF_QUANTITY             15

/* Swap 1 and 0 macro */
#define SWAP_1_AND_0(x)         (x == 1 ? 0 : 1)

/* Port address pointer */
GPIO_TypeDef *port_ptr;

/* port address pointers array */
GPIO_TypeDef *const port_ptr_array[PORT_QUANTITY] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG};

/* Support function declarations */
static bool is_port_correct(enum gpio_port port);
static bool is_pin_correct(uint8_t pin);
static bool is_mode_correct(enum gpio_mode mode);
static bool is_pp_od_correct(enum gpio_pp_od pp_od);
static bool is_pull_correct(enum gpio_pull pull);
static bool is_speed_correct(enum gpio_speed speed);
static bool is_af_correct(uint8_t af);
static uint8_t get_af_reg(uint8_t pin);


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
        port_ptr_af -> AFR[af_reg] &= ~(0xF << (pin * 4));
        port_ptr_af -> AFR[af_reg] |= (af << (pin * 4));

        return NO_ERR;
    }

    else
    {
        /* Something is wrong, don't configure af */
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