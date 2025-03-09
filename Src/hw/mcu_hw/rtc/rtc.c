#include "rtc.h"
#include "stm32g4xx.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>


/* Callback function address holders */
static cb_t alarm_cb;
static cb_t second_elapsed_cb;


/* Helper functions */
static bool has_rtc_reset_occured(void)
{
    return (!(RCC -> BDCR & RCC_BDCR_RTCEN));
}


static void unlock_rtc_registers_write(void)
{
    /* Write special values to write protection register */
    RTC -> WPR = 0xCA;
    RTC -> WPR = 0x53;
}


static void lock_rtc_registers_write(void)
{
    /* Write random value except 0xCA and 0x53 */
    RTC -> WPR = 0xFF;
}


/* RTC functions */
void rtc_init(void) 
{
    RCC -> APB1ENR1 |= RCC_APB1ENR1_PWREN; /* Turn on PWR and RTC clock */
    PWR -> CR1 |= PWR_CR1_DBP; /* Unlock backup domain access */
    
	if (has_rtc_reset_occured())
	{
        /* Backup domain configuration (affected only by RTC reset) */
		RCC -> BDCR &= ~RCC_BDCR_BDRST; /* Turn off RTC reset */
        
        /* Turn on LSE */
        RCC -> BDCR |= RCC_BDCR_LSEON; 
        while (!(RCC->BDCR & RCC_BDCR_LSERDY)); 

        RCC -> BDCR |= RCC_BDCR_RTCSEL_0; /* Choose LSE as RTC clock */
        RCC -> BDCR |= RCC_BDCR_RTCEN; /* Turn on RTC */

        unlock_rtc_registers_write();

        /* Enter initialisation state */
        RTC -> ICSR |= RTC_ICSR_INIT;
        while (!(RTC -> ICSR & RTC_ICSR_INITF));

        RTC -> PRER = (127 << 16) | 255; /* Set prescaler to 1Hz (32768 Hz / (128 * 256) */
        RTC -> ICSR &= ~RTC_ICSR_INIT; /* Exit initialisation state */

        /* Wakuep timer (1s) configuration */
        RTC -> CR &= ~RTC_CR_WUTE; /* Turn off timer before initialisation */
        while (!(RTC -> ICSR & RTC_ICSR_WUTWF)); /* Wait for hw response */
        RTC -> CR &= ~RTC_CR_WUCKSEL; /* Select clock source (RTC clock (LSE) / 16) */
        RTC -> WUTR = 0x0800; /* Set counter value */
        RTC -> CR |= RTC_CR_WUTIE; /* Enable Wakeup timer interrupt */

        lock_rtc_registers_write();
	}

    /* Rest of the microcontroller configuration (affected by every reset) */
    /* EXTI configuration */
	EXTI -> IMR1 |= EXTI_IMR1_IM20 | EXTI_IMR1_IM17; /* Disable mask for input 17 (alarm) and input 20 (wakeup timer) */
    EXTI -> RTSR1 |= EXTI_RTSR1_RT20 | EXTI_RTSR1_RT17; /* Set rising edge detection */

    /* NVIC configuration */
    /* Set priority and enable Wakeup timer interrupt */
    NVIC_SetPriority(RTC_WKUP_IRQn, 1); 
    NVIC_EnableIRQ(RTC_WKUP_IRQn);
    /* Set priority and enable Alarm interrupt */
    NVIC_SetPriority(RTC_Alarm_IRQn, 0);
    NVIC_EnableIRQ(RTC_Alarm_IRQn);

    /* Clear wakeup timer irq (without it irq was not handled) */
    RTC->SCR = RTC_SCR_CWUTF; /* Clear wakeup timer interrupt flag */
    EXTI->PR1 = EXTI_PR1_PIF20; /* Clear EXTI20 interrupt flag */
    while(RTC->SR & RTC_SR_WUTF); /* Wait for wakeup timer interrupt flag to be cleared */
}


static void rtc_start_1s_timer(cb_t second_elapsed_callback)
{
    unlock_rtc_registers_write();

    second_elapsed_cb = second_elapsed_callback; /* Save 1s elapsed callback function address */
    RTC -> CR |= RTC_CR_WUTE; /* Enable Wakeup timer */

    lock_rtc_registers_write();
}


static void rtc_set_time(struct time t)
{
    unlock_rtc_registers_write();

    RTC -> ICSR |= RTC_ICSR_INIT; /* Enter initialisation state */
    while (!(RTC -> ICSR & RTC_ICSR_INITF)); /* Wait for hw response */

    RTC->SCR = RTC_SCR_CWUTF; /* Clear wakeup timer interrupt flag */

    /* Write time to register */
    RTC -> TR = ((t.hours / 10) << 20) | ((t.hours % 10) << 16) |
              ((t.minutes / 10) << 12) | ((t.minutes % 10) << 8) |
              ((t.seconds / 10) << 4) | (t.seconds % 10);

    RTC -> ICSR &= ~RTC_ICSR_INIT; /* Exit initialisation state */

    lock_rtc_registers_write();
}


static struct time rtc_get_time(void)
{
    struct time time;
    uint32_t tmp = RTC -> TR;
    time.hours = ((tmp >> 20) & 0x3) * 10 + ((tmp >> 16) & 0xF);
    time.minutes = ((tmp >> 12) & 0x7) * 10 + ((tmp >> 8) & 0xF);
    time.seconds = ((tmp >> 4) & 0x7) * 10 + (tmp & 0xF);
    
    uint32_t date = RTC -> DR; /* Used for correct time read */

    return time;
}


static void rtc_set_alarm(struct time t, cb_t cb) 
{
    unlock_rtc_registers_write();
        
    /* Clear alarm A */
    RTC -> CR &= ~RTC_CR_ALRAE;
    while (!(RTC -> ICSR & RTC_ICSR_ALRAWF));

    /* Clear alarm interrupt flag */
    RTC->SCR = RTC_SCR_CALRAF;

    /* Write alarm time to register */
    RTC -> ALRMAR = ((t.hours / 10) << 20) | ((t.hours % 10) << 16) |
                  ((t.minutes / 10) << 12) | ((t.minutes % 10) << 8) |
                  ((t.seconds / 10) << 4) | (t.seconds % 10);

    RTC -> ALRMAR |= RTC_ALRMAR_MSK4; /* Disable data comparision */
    RTC -> CR |= RTC_CR_ALRAE | RTC_CR_ALRAIE; /* Enable alarm A and its interrupt */
    
    lock_rtc_registers_write();

    /* Save alarm callback function address */
    alarm_cb = cb;
}


static bool rtc_is_alarm_set(void)
{
    if (RTC -> CR & RTC_CR_ALRAE)
    {
        return true;
    }

    else
    {
        return false;
    }
}


/* Return API functions */
const struct time_interface time_funs =
{
    rtc_init,
    rtc_start_1s_timer,
    rtc_set_time,
    rtc_get_time,
};


const struct alarm_time_interface alarm_time_funs = 
{
    rtc_init,
    rtc_is_alarm_set,
    rtc_set_alarm,
};


const struct time_interface *rtc_get_time_funs(void)
{
    return &time_funs;
}


const struct alarm_time_interface *rtc_get_alarm_time_funs(void)
{
    return &alarm_time_funs;
}


/* Interrupts */

/* Alarms interrupt */
void RTC_ALARM_IRQHandler(void) 
{
    /* Alarm A */
    if (RTC -> SR & RTC_SR_ALRAF)
    {
        RTC -> SCR = RTC_SCR_CALRAF; /* Clear alarm A interrupt flag */
        EXTI -> PR1 = EXTI_PR1_PIF17; /* Clear EXTI17 interrupt flag */
        while (RTC -> SR & RTC_SR_ALRAF); /* Wait for alarm A interrupt flag to be cleared */

        /* Call callback function */
        if (alarm_cb != NULL) 
        {
            alarm_cb();
        }
    }
}


/* Wakeup timer interrupt */
void RTC_WKUP_IRQHandler(void)
{
    if (RTC->SR & RTC_SR_WUTF)
    {
    	RTC->SCR = RTC_SCR_CWUTF; /* Clear wakeup timer interrupt flag */
    	EXTI->PR1 = EXTI_PR1_PIF20; /* Clear EXTI20 interrupt flag */
    	while(RTC->SR & RTC_SR_WUTF); /* Wait for wakeup timer interrupt flag to be cleared */

        /* Call callback function */
        if (second_elapsed_cb != NULL)
        {
            second_elapsed_cb();
        }
    }
}
