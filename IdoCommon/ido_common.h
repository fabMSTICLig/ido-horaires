// SPDX-License-Identifier: LGPL-2.1-or-later
#ifndef IDO_COMMON_H
#define IDO_COMMON_H

// COMMANDES define
#define IDO_CMD_DEFAULT     0
#define IDO_CMD_TIME        1
#define IDO_CMD_OPENTIME    2
#define IDO_CMD_CLOSETIME   3
#define IDO_CMD_IDLE        4

//Standard message
struct ido_msg {
    uint8_t cmd;
    uint32_t time;
    uint32_t time_close;
};

//Union used for aes crypto
typedef union {
    struct ido_msg msg;
    uint8_t aes_plain[16];
} ido_msg_t;


#endif
