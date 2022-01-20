// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * epd_board.c
 *
 *  Created on: Mar 29, 2021
 *      Author: Germain
 */
#include "epd_board.h"
#include "xtimer.h"
uint32_t lasttime=0;

void start_EPD_timer(void){
	lasttime=xtimer_now_usec();
}
void stop_EPD_timer(void){

}
uint32_t get_EPD_timer_value(void){
	return (xtimer_now_usec()-lasttime)/1000;
}

void Wait_10us(void){
	xtimer_usleep(10);
}

/**
 * \brief SPI synchronous write
 */
uint8_t epd_spi_write(unsigned char Data) {
    return spi_transfer_byte(EPD_PARAM_SPI,SPI_CS_UNDEF, false, Data);
}
/**
 * \brief SPI synchronous read
 */
uint8_t spi_buf;
uint8_t epd_spi_read(unsigned char Data) {
    return spi_transfer_byte(EPD_PARAM_SPI,SPI_CS_UNDEF, false, Data);
}

void epd_spi_send_byte (uint8_t register_index, uint8_t register_data) {
    EPD_cs_low ();
    epd_spi_write (0x70); // header of Register Index
    epd_spi_write (register_index);

    EPD_cs_high ();
    Wait_10us ();
    EPD_cs_low ();
    epd_spi_write (0x72); // header of Register Data
    epd_spi_write (register_data);
    EPD_cs_high ();
}

void epd_spi_send (unsigned char register_index, unsigned char *register_data,
               unsigned length) {
    EPD_cs_low ();
    epd_spi_write (0x70); // header of Register Index
    epd_spi_write (register_index);

    EPD_cs_high ();
    Wait_10us ();
    EPD_cs_low ();

    epd_spi_write (0x72); // header of Register Data of write command
    while(length--) {
        epd_spi_write (*register_data++);
    }
    EPD_cs_high ();
}

uint8_t SPI_R(uint8_t Register, uint8_t Data) {
    uint8_t result;
    (void)Data;
    EPD_cs_low ();
    epd_spi_write (0x70); // header of Register Index
    epd_spi_write (Register);

    EPD_cs_high ();
    Wait_10us ();
    EPD_cs_low ();

    epd_spi_write (0x73); // header of Register Data of read command
    result=epd_spi_read (0x00);
    EPD_cs_high ();

    return result;
}
void epd_spi_init(void){
    spi_acquire(EPD_PARAM_SPI,SPI_CS_UNDEF, SPI_MODE_0,SPI_CLK_100KHZ);
}

void DelayMs(uint16_t ms){
	xtimer_msleep(ms);
}

void Config_Busy_Pin(void)
{
    gpio_init(EPD_PARAM_BUSY,GPIO_IN);
}
bool EPD_IsBusy(void)
{
	return (bool)gpio_read(EPD_PARAM_BUSY);
}

void Config_EPD_cs_Pin(void)
{
    gpio_init(EPD_PARAM_CS,GPIO_OUT);
}
void EPD_cs_high(void)
{
	gpio_set(EPD_PARAM_CS);
}
void EPD_cs_low(void)
{
	gpio_clear(EPD_PARAM_CS);
}

void Config_EPD_rst_Pin(void)
{
    gpio_init(EPD_PARAM_RST,GPIO_OUT);
}
void EPD_rst_high(void)
{
	gpio_set(EPD_PARAM_RST);
}
void EPD_rst_low(void)
{
	gpio_clear(EPD_PARAM_RST);
}

void Config_EPD_discharge_Pin(void)
{
    gpio_init(EPD_PARAM_DC,GPIO_OUT);
}
void EPD_discharge_high(void)
{
	gpio_set(EPD_PARAM_DC);
}
void EPD_discharge_low(void)
{
	gpio_clear(EPD_PARAM_DC);
}

void Config_EPD_panelon_Pin(void)
{
    gpio_init(EPD_PARAM_PANELON,GPIO_OUT);
}
void EPD_Vcc_turn_on(void)
{
	gpio_set(EPD_PARAM_PANELON);
}
void EPD_Vcc_turn_off(void)
{
	gpio_clear(EPD_PARAM_PANELON);
}

void Config_EPD_border_Pin(void)
{
    gpio_init(EPD_PARAM_BORDER,GPIO_OUT);
}
void EPD_border_high(void)
{
	gpio_set(EPD_PARAM_BORDER);
}
void EPD_border_low(void)
{
	gpio_clear(EPD_PARAM_BORDER);
}

void Config_EPD_pwm_Pin(void)
{
    gpio_init(EPD_PARAM_PWM,GPIO_OUT);
}
void EPD_pwm_high(void)
{
	gpio_set(EPD_PARAM_PWM);
}
void EPD_pwm_low(void)
{
	gpio_clear(EPD_PARAM_PWM);
}

void SPIMISO_low(void)
{
	gpio_clear(EPD_PARAM_MISO);
}
void SPIMOSI_low(void)
{
	gpio_clear(EPD_PARAM_MOSI);
}
void SPICLK_low(void)
{
	gpio_clear(EPD_PARAM_CLK);
}
int16_t get_temperature(void){
    return 20;
}


void EPD_display_hardware_init(void){
    Config_Busy_Pin();
    Config_EPD_cs_Pin();
    Config_EPD_rst_Pin();
    Config_EPD_discharge_Pin();
    Config_EPD_panelon_Pin();
    Config_EPD_border_Pin();
    Config_EPD_pwm_Pin();
}
