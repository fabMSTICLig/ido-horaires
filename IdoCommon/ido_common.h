
#ifndef IDO_COMMON_H
#define IDO_COMMON_H

#define IDO_CMD_DEFAULT     0
#define IDO_CMD_TIME        1
#define IDO_CMD_OPENTIME    2
#define IDO_CMD_IDLE        3

struct ido_msg {
    uint8_t cmd;
    uint32_t time;
};

typedef union {
    struct ido_msg msg;
    uint8_t aes_plain[16];
} ido_msg_t;


#endif
