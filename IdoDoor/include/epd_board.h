// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * epd_board.h
 *  Low level hardware for EPD Driver
 *
 *  Created on: Mar 29, 2021
 *      Author: Germain Lemasson
 */

#ifndef INC_EPD_BOARD_H_
#define INC_EPD_BOARD_H_


#include "periph/gpio.h"
#include "periph/spi.h"

#ifndef EPD_PARAM_SPI
#define EPD_PARAM_SPI                   SPI_DEV(1)
#endif

#ifndef EPD_PARAM_BUSY
#define EPD_PARAM_BUSY                  GPIO_PIN(PORT_E, 7)
#endif

#ifndef EPD_PARAM_PANELON
#define EPD_PARAM_PANELON               GPIO_PIN(PORT_D, 9)
#endif

#ifndef EPD_PARAM_DC
#define EPD_PARAM_DC                  GPIO_PIN(PORT_D, 11)
#endif

#ifndef EPD_PARAM_BORDER
#define EPD_PARAM_BORDER                GPIO_PIN(PORT_D, 10)
#endif

#ifndef EPD_PARAM_RST
#define EPD_PARAM_RST                  GPIO_PIN(PORT_C, 5)
#endif

#ifndef EPD_PARAM_MISO
#define EPD_PARAM_MISO                  GPIO_PIN(PORT_A, 6)
#endif

#ifndef EPD_PARAM_MOSI
#define EPD_PARAM_MOSI                  GPIO_PIN(PORT_A, 7)
#endif

#ifndef EPD_PARAM_CLK
#define EPD_PARAM_CLK                  GPIO_PIN(PORT_A, 5)
#endif

#ifndef EPD_PARAM_CS
#define EPD_PARAM_CS                  GPIO_PIN(PORT_E, 8)
#endif


#ifndef EPD_PARAM_PWM
#define EPD_PARAM_PWM                  GPIO_PIN(PORT_A, 10)
#endif



void Config_Busy_Pin(void);
void Config_EPD_cs_Pin(void);
void Config_EPD_rst_Pin(void);
void Config_EPD_discharge_Pin(void);
void Config_EPD_panelon_Pin(void);
void Config_EPD_border_Pin(void);
void Config_EPD_pwm_Pin(void);
bool EPD_IsBusy(void);
void EPD_cs_high(void);
void EPD_cs_low(void);
void EPD_rst_high(void);
void EPD_rst_low(void);
void EPD_discharge_high(void);
void EPD_discharge_low(void);
void EPD_Vcc_turn_on(void);
void EPD_Vcc_turn_off(void);
void EPD_border_high(void);
void EPD_border_low(void);
void EPD_pwm_high(void);
void EPD_pwm_low(void);
void SPIMISO_low(void);
void SPIMOSI_low(void);
void SPICLK_low(void);

#define board_reset_watchdog_counter()

int16_t get_temperature(void);

void DelayMs(uint16_t ms);
void epd_spi_init(void);
void EPD_display_hardware_init(void);
void start_EPD_timer(void);
void stop_EPD_timer(void);
uint32_t get_EPD_timer_value(void);
void epd_spi_send_byte (uint8_t register_index, uint8_t register_data) ;
uint8_t SPI_R(uint8_t Register, uint8_t Data);
void epd_spi_send (unsigned char register_index, unsigned char *register_data,
               unsigned length);
#endif /* INC_EPD_BOARD_H_ */
