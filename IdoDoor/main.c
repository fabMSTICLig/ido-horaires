/*
 * Copyright (C) 2021 LIG
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Affichage horraire porte avec idosens base
 *
 * @author      Germain Lemasson <germain.lemasson@univ-grenoble-alpes.fr>
 *
 * @}
 */

#include <stdio.h>

#include "thread.h"
#include "xtimer.h"
#include "crypto/aes.h"

#include "fmt.h"
#include "board.h"

#include "periph/uart.h"
#include "periph/rtc.h"
#include "periph/pm.h"
#include "pm_layered.h"

#include "net/netdev.h"
#include "net/netdev/lora.h"
#include "net/lora.h"

#include "sx127x_internal.h"
#include "sx127x_params.h"
#include "sx127x_netdev.h"

///////Include  Epaper
#include "epd_board.h"
#include "COG_FPL.h"
#include "BufferDraw.h"
#include "Arial24x23.h"
#include "font_big.h"
#include "horaires.h"
#include "fermeture.h"

//include common info between transmitter and receiver
#include "ido_common.h"
//include AES Crypto Key
#include "ido_crypto.h"

#define SX127X_LORA_MSG_QUEUE   (16U)
#define SX127X_STACKSIZE        (THREAD_STACKSIZE_DEFAULT)

#define MSG_TYPE_ISR            (0x3456)
#define MSG_TYPE_BTN            (0x1024)
#define MSG_TYPE_ALARM_CLOSE    (0x2048)
#define MSG_TYPE_ALARM_OPEN     (0x2049)

static kernel_pid_t _recv_pid;

static uint8_t message_enc[AES_BLOCK_SIZE];
static sx127x_t sx127x;

//Two image buffer for EPD
#define IMAGE_W (264)
#define IMAGE_H (176)
#define IMAGE_LEN (IMAGE_W*IMAGE_H/8)
static uint8_t image[IMAGE_LEN];
static uint8_t old_image[IMAGE_LEN];

cipher_context_t cipher;
ido_msg_t msg={0};
uint32_t opentime=0;


int state=0;

//simple memcopy function
void memcopy(const uint8_t * buffsource, uint8_t * buffdest, uint16_t len)
{
    for(uint16_t i =0; i<len; i++)
    {
        buffdest[i]=buffsource[i];
    }
}

//Print formated time
static int _print_time(struct tm *time)
{
    printf("%04i-%02i-%02i %02i:%02i:%02i\n",
            time->tm_year + 1900, time->tm_mon + 1, time->tm_mday,
            time->tm_hour, time->tm_min, time->tm_sec
          );
    return 0;
}
//Switch lora to listen mode 
static void lora_rx(void)
{
    ////////LORA RX MODE
    netdev_t *netdev = (netdev_t *)&sx127x;
    const netopt_enable_t single = false;
    netdev->driver->set(netdev, NETOPT_SINGLE_RECEIVE, &single, sizeof(single));
    const uint32_t timeout = 0;
    netdev->driver->set(netdev, NETOPT_RX_TIMEOUT, &timeout, sizeof(timeout));

    /* Switch to RX state */
    netopt_state_t state = NETOPT_STATE_RX;
    netdev->driver->set(netdev, NETOPT_STATE, &state, sizeof(state));

    printf("Listen mode set\n");
 
}
//Switch lora to sleep mode 
static void lora_sleep(void)
{
    /* Switch to SLEEP state */
    netdev_t *netdev = (netdev_t *)&sx127x;
    netopt_state_t state = NETOPT_STATE_SLEEP;
    netdev->driver->set(netdev, NETOPT_STATE, &state, sizeof(state));
    puts("lora sleep");
}

//Callback for RTC wakeup interup
static void cb_alarm(void * arg){
    (void) arg;
    msg_t msg;
    msg.type = (uint32_t) arg;

    if (msg_send(&msg, _recv_pid) <= 0) {
        puts("gnrc_netdev: possibly lost interrupt.");
    }
}

//set time of the RTC
static void set_time(uint32_t time)
{
    puts("time");
    struct tm ot;
    rtc_localtime(time,&ot);
    rtc_set_time(&ot);
    _print_time(&ot);
}

//Draw default picture on EPD
static void set_default(void)
{
    puts("Default");
    EPD_power_on(EPD_270, 18);
    puts("drive on");
    EPD_initialize_driver ();
    puts("drive init");
    memcopy(horaires,image,IMAGE_LEN);
    EPD_display_from_array_prt ( old_image, image );
    puts("disp");
    memcopy(horaires,old_image,IMAGE_LEN);
    EPD_power_off();
    spi_release(EPD_PARAM_SPI);
    puts("drive off");

}
//Set rtc alarm to close time
static int set_close(uint32_t close_time, uint32_t time)
{
    puts("close time");
    opentime=time;
    struct tm ct;
    rtc_localtime(close_time,&ct);
    _print_time(&ct);
    //Set rtc alarm
    rtc_set_alarm(&ct,&cb_alarm,(void *)MSG_TYPE_ALARM_CLOSE);
    return 0;
}
//Draw closed picture with reopen time
//Set rtc alarm to reopen time
static int set_open(uint32_t time)
{
    puts("open time");
    struct tm ot;
    char buffer[20];
    rtc_localtime(time,&ot);
    _print_time(&ot);
    //Set rtc alarm
    rtc_set_alarm(&ot,&cb_alarm,(void *)MSG_TYPE_ALARM_OPEN);

    //build image to draw
    BD_clear();
    BD_clip_image(fermeture,0,0,264,100);
    BD_locate(40,100);
    sprintf(buffer,"%02i-%02i-%04i",
             ot.tm_mday, ot.tm_mon + 1, ot.tm_year + 1900
           );
    puts(buffer);
    BD_puts(buffer);
    BD_locate(90,136);
    sprintf(buffer,"%02i:%02i",
            ot.tm_hour, ot.tm_min
          );
    BD_puts(buffer);
    
    //Draw image on EPD
    EPD_power_on(EPD_270, 18);
    EPD_initialize_driver ();
    EPD_display_from_array_prt ( old_image, image );
    memcopy(image,old_image,IMAGE_LEN);
    EPD_power_off();
    //EDP power use spi_aquire but you must use spi_release manualy
    spi_release(EPD_PARAM_SPI);
    puts("draw_fermeture end");
    
    return 0;
}

//Set lora to sleep and board to sleep
static void set_idle(void){
    puts("idle");
    lora_sleep();
    state=0;
    gpio_set(LED_RED1_PIN);
    pm_set(1);
} 

//handle received command
void handle_msg(struct ido_msg msg)
{
    printf("cmd:0x%x time:%ld\n", msg.cmd, msg.time);
    switch(msg.cmd)
    {
        case IDO_CMD_DEFAULT:
            set_default();
            break;
        case IDO_CMD_CLOSETIME:
            set_close(msg.time_close,msg.time);
            break;
        case IDO_CMD_OPENTIME:
            set_open(msg.time);
            break;
        case IDO_CMD_TIME:
            set_time(msg.time);
            break;
        case IDO_CMD_IDLE:
            set_idle();
            break;
    }
}

//Callback for RX LORA
static void _event_cb(netdev_t *dev, netdev_event_t event)
{
    if (event == NETDEV_EVENT_ISR) {
        msg_t msg;

        msg.type = MSG_TYPE_ISR;
        msg.content.ptr = dev;

        if (msg_send(&msg, _recv_pid) <= 0) {
            puts("gnrc_netdev: possibly lost interrupt.");
        }
    }
    else {
        size_t len;
        netdev_lora_rx_info_t packet_info;
        switch (event) {
            case NETDEV_EVENT_RX_STARTED:
                puts("Data reception started");
                break;

            case NETDEV_EVENT_RX_COMPLETE:
                len = dev->driver->recv(dev, NULL, 0, 0);
                dev->driver->recv(dev, message_enc, len, &packet_info);
                aes_decrypt(&cipher, message_enc, msg.aes_plain);
                handle_msg(msg.msg);
                break;

            case NETDEV_EVENT_CAD_DONE:
                break;

            default:
                printf("Unexpected netdev event received: %d\n", event);
                break;
        }
    }
}

//Callback for button interupt
static void cb_btn(void * arg){
    (void) arg;
    msg_t msg;
    msg.type = MSG_TYPE_BTN;

    if (msg_send(&msg, _recv_pid) <= 0) {
        puts("gnrc_netdev: possibly lost interrupt.");
    }
}

int main(void)
{
    //Set interupt for the button used for waking up the board 
    if (gpio_init_int(BOUTON_VOLUME_MINUS_PIN,GPIO_IN_PU , GPIO_FALLING, cb_btn, 0)<0) {
        puts("[FAILED] init BTN minus!");
        return 1;
    }
    //Disable the audio driver (used 100mA when idle otherwise)
	gpio_init(GPIO_PIN(PORT_C, 1), GPIO_OUT);
	gpio_clear(GPIO_PIN(PORT_C, 1));
    
    //Init buffer image
    BD_init( image, IMAGE_LEN, IMAGE_W, IMAGE_H, 0, 1);
    BD_clear();
    BD_set_font(Courier_New19x36, Courier_New19x36_conf);
    //Init EPD
    spi_init(EPD_PARAM_SPI);
    EPD_display_hardware_init();
    set_default();
    //Init RTC
    rtc_poweron();

    //Init crypto
    aes_init(&cipher, IDO_KEY, 16);
    
    //Init LORA
    sx127x.params = sx127x_params[0];
    netdev_t *netdev = (netdev_t *)&sx127x;
    netdev->driver = &sx127x_driver;
    if (netdev->driver->init(netdev) < 0) {
        puts("Failed to initialize SX127x device, exiting");
        return 1;
    }
    netdev->event_callback = _event_cb;
    _recv_pid = thread_getpid();
    
    uint8_t lora_bw=125;
    uint8_t lora_sf=7;
    uint8_t lora_cr=5;
    netdev->driver->set(netdev, NETOPT_BANDWIDTH,
                        &lora_bw, sizeof(lora_bw));
    netdev->driver->set(netdev, NETOPT_SPREADING_FACTOR,
                        &lora_sf, sizeof(lora_sf));
    netdev->driver->set(netdev, NETOPT_CODING_RATE,
                        &lora_cr, sizeof(lora_cr));

    //start in idle mode
    set_idle();
    
    ///// MAIN LOOP MESSAGE HANDLING
    static msg_t _msg_q[SX127X_LORA_MSG_QUEUE];
    msg_init_queue(_msg_q, SX127X_LORA_MSG_QUEUE);
    
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == MSG_TYPE_ISR) {
            netdev_t *dev = msg.content.ptr;
            dev->driver->isr(dev);
        }
        else if(msg.type == MSG_TYPE_BTN){
            if(state==0)
            {
                state=1;
                puts("wakeup button");
                struct tm ot;
                rtc_get_time(&ot);
                _print_time(&ot);
                gpio_clear(LED_RED1_PIN);
                lora_rx();
            }
        }
        else if(msg.type == MSG_TYPE_ALARM_OPEN){
            puts("wakeup RTC open");
            struct tm ot;
            rtc_get_time(&ot);
            _print_time(&ot);
            set_default();
            set_idle();
        }
        else if(msg.type == MSG_TYPE_ALARM_CLOSE){
            puts("wakeup RTC close");
            set_open(opentime);
            set_idle();
        }
        else {
            puts("Unexpected msg type");
        }
    }
    return 0;
}
