#include "public_messages.h"

const unsigned char public_message_data_size[NUM_PUB_MSG_T] = {
    12, //SENS_DIST
    2, //MOV_CMD
    3, //TURN_CMD
    2, //SIDE_ANGLE
    2, //FRONT_ANGLE
    1, //ENC_INFO
    4, //ROV_POS
    4, //NEW_CORNER
    2, //NEW_CORNER_WEB
};

static unsigned char message_counts[NUM_PUB_MSG_T] = {0};

unsigned char public_message_get_count(const public_message_type_t type) {
    if (type < NUM_PUB_MSG_T) {
        return message_counts[type]++;
    } else {
        return 0;
    }
}