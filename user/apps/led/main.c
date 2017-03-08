#include <platform/link.h>
#include <user_runtime.h>
#include <l4/ipc.h>
#include <l4/utcb.h>
#include <l4/pager.h>
#include <l4/thread.h>
#include <l4io.h>
#include INC_PLAT(gpio.h)
#include INC_PLAT(rcc.h)

#define STACK_SIZE 512

static L4_ThreadId_t thread __USER_DATA;

__USER_TEXT
void *led(void *arg)
{
    RCC_AHB1PeriphClockCmd(*(RCC_AHB1RSTR), 1);
    gpio_config_output(GPIOG, 13, GPIO_OTYPER_PP, GPIO_OSPEEDR_25M);
    gpio_out_high(GPIOG, 13);

    while (1) {
	for (uint32_t i=0; i<1000000; ++i) {
	    gpio_out_high(GPIOG, 13);
	}
        for (uint32_t i=0; i<1000000; ++i) { 
            gpio_out_low(GPIOG, 13);
        }
    }
}


__USER_TEXT
static void *main(void *user)
{
    thread = pager_create_thread();
    pager_start_thread(thread, led, NULL);
    return 0;
}

DECLARE_USER(
    8,
    led,
    main,
    DECLARE_FPAGE(0x0, 4 * UTCB_SIZE + 4 * STACK_SIZE)
    DECLARE_FPAGE(0x0, 512)
    DECLARE_FPAGE(0x40005000, 0x1000)
    DECLARE_FPAGE(0x40015000, 0x0c00)
    DECLARE_FPAGE(0x40020000, 0x3c00)
    DECLARE_FPAGE(0x40028000, 0x8000)
    DECLARE_FPAGE(0x42470000, 0x0c00)
    DECLARE_FPAGE(0x40016800, 0x0c00)
    DECLARE_FPAGE(0xA0000000, 0x1000)
    DECLARE_FPAGE(0xD0000000, 0xA0000)
    DECLARE_FPAGE(0xD00A0000, 0xA0000)
);

