#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include <stdint.h>

struct protocol_packet
{
    uint8_t cmd;
    uint8_t data[7]; /* FIXME: hardcode */
} __attribute__((packed));

#endif /* _PROTOCOL_H */
