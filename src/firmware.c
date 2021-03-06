/*

 The MIT License (MIT)

 Copyright (c) 2015-2016 Douglas J. Bakkum

 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.

*/


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <mcu.h>
#include "usb.h"
#include "ecc.h"
#include "led.h"
#include "memory.h"
#include "random.h"
#include "systick.h"
#include "commander.h"
#include "board_com.h"


uint32_t __stack_chk_guard = 0;

extern void __attribute__((noreturn)) __stack_chk_fail(void);
void __attribute__((noreturn)) __stack_chk_fail(void)
{
    while (1) {
        led_toggle();
        delay_ms(300);
    }
}


void SysTick_Handler(void)
{
    systick_update_time();
}


void HardFault_Handler(void)
{
    while (1) {
        led_toggle();
        delay_ms(500);
    }
}


void MemManage_Handler(void)
{
    while (1) {
        led_toggle();
        delay_ms(1000);
    }
}


char usb_serial_number[USB_DEVICE_GET_SERIAL_NAME_LENGTH];

int main (void)
{
    // printf("Initializing Board Systems and Peripherals...\n");
    wdt_disable(WDT);
    irq_initialize_vectors();
    cpu_irq_enable();
    sleepmgr_init();
    sysclk_init();
    flash_init(FLASH_ACCESS_MODE_128, 6);
    board_com_init();
    __stack_chk_guard = random_uint32(0);
    pmc_enable_periph_clk(ID_PIOA);
    delay_init(F_CPU);
    systick_init();
    ecc_context_init();

#ifdef ECC_USE_SECP256K1_LIB
    /* only init the context if libsecp256k1 is present */
    /* otherwise we would re-init the context of uECC */
    bitcoin_ecc.ecc_context_init();
#endif
       
    if (memory_read_erased()) {
        usb_serial_number[USB_DEVICE_GET_SERIAL_NAME_LENGTH - 2] = '-';
        usb_serial_number[USB_DEVICE_GET_SERIAL_NAME_LENGTH - 1] = '-';
    }

    memset(usb_serial_number, 0, sizeof(usb_serial_number));

    static usart_serial_options_t usart_options = {
        .baudrate = CONF_UART_BAUDRATE,
        .charlength = US_MR_CHRL_8_BIT,
        .paritytype = US_MR_PAR_NO,
        .stopbits = US_MR_NBSTOP_1_BIT
    };
     
    udc_start();
    memory_setup();
    pio_configure_pin_group(CONF_UART_PIO, CONF_PINS_UART, CONF_PINS_UART_FLAGS);
    stdio_serial_init(CONF_UART, &usart_options);

    printf("Board systems and peripherals initialized...\n\r");
    printf("Entering Main loop...\n\r");

    while (1) {
    printf("Start of Loop\n\r");
    uint8_t data[4] = {'e', 'w', 't', 0};
    bool send_report_in = udi_hww_send_report_in(data);
    printf("REPORT OUT STATUS: %d\n\r", (int)send_report_in);
    printf("LED will flash\n\r");
    led_on();
    delay_s(2);
    led_off();
    delay_s(2);
    printf("End of Loop\n\r");
    }
}
