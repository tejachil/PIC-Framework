#ifndef PUBLIC_MESSAGES_H
#define	PUBLIC_MESSAGES_H

/** Public message type values. */
typedef enum {
    /** Distance reading for a given sensor. */
    PUB_MSG_T_SENS_DIST = 0,
    /** Instruction to move rover forward specified distance. */
    PUB_MSG_T_MOV_CMD,
    /** Instruction to rotate rover specified direction and angle. */
    PUB_MSG_T_TURN_CMD,
    /** Angle of wall to side relative to rover. */
    PUB_MSG_T_SIDE_ANGLE,
    /** Angle of wall in front relative to rover. */
    PUB_MSG_T_FRONT_ANGLE,
    /** Information about the current enclosure. */
    PUB_MSG_T_ENC_INFO,
    /** Position of rover. */
    PUB_MSG_T_ROV_POS,
    /** Notification of new enclosure corner. */
    PUB_MSG_T_NEW_CORNER,
    /** Notification for webserver of new enclosure corner. */
    PUB_MSG_T_CORNER_WEB,

    // ADD ANY NEW MESSAGE TYPES ABOVE THIS LINE
    // So that NUM_PUB_MSG_T will be correct.

    /** Number of defined public message types. */
    NUM_PUB_MSG_T
} public_message_type_t;

/** Minimum size of a public message (with data length 0). */
#define PUB_MSG_MIN_SIZE (3)
/** Maximum size of a public message (including data). */
#define PUB_MSG_MAX_SIZE (10)

/** Max number of bytes of data allowed to be attached to a public message. */
#define PUB_MSG_MAX_DATA_SIZE (PUB_MSG_MAX_SIZE - PUB_MSG_MIN_SIZE)

/** Structure of public messages. */
typedef struct {
    public_message_type_t message_type;
    unsigned char message_count;
    unsigned char data_length;
    unsigned char data[PUB_MSG_MAX_DATA_SIZE];
} public_message_t;

#endif	/* PUBLIC_MESSAGES_H */

