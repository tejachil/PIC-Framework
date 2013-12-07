#ifndef PUBLIC_MESSAGES_H
#define	PUBLIC_MESSAGES_H

/**
 * I2C slave address for the Motor Controller PIC.  This is the 7-bit address
 * value without any shifting or other considerations for the R/W bit.
 */
#define MOTOR_PIC_ADDR (0x25)
/**
 * I2C slave address for the Proximity Sensors PIC.  This is the 7-bit address
 * value without any shifting or other considerations for the R/W bit.
 */
#define SENSOR_PIC_ADDR (0x2A)

/**
 * I2C2 address for the gyro. This address is used for initializing the gyro.
 */
#define GYRO_SLAVE_ADDRESS (0b1101000)

/** Number of encoder ticks for each revolution of a wheel. */
#define ENCODER_TICKS_PER_REVOLUTION (6207)

/** Public message type values. */
typedef enum {
    /** Distance reading for a given sensor. */
    PUB_MSG_T_SENS_DIST = 0,
    /** Instruction to move rover forward or stop. */
    PUB_MSG_T_MOV_CMD,
    /** Instruction to rotate rover specified direction and angle. */
    PUB_MSG_T_TURN_CMD,
    /** Fix parallelism with the side wall*/
    PUB_MSG_T_FIX_CMD,
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
    /** Encoder data for distance. */
    PUB_MSG_T_ENCODER_DATA,
    /** Gyro data for angle. */
    PUB_MSG_T_GYRO_DATA,
    /** Information about whether or not the rover has finished turning*/
    PUB_MSG_T_TURN_STATUS,
    // ADD ANY NEW MESSAGE TYPES ABOVE THIS LINE
    // So that NUM_PUB_MSG_T will be correct.

    /** Number of defined public message types. */
    NUM_PUB_MSG_T
} public_message_type_t;

/** Data byte values for MOV_CMD. */
typedef enum {
    /** Command to start moving forward fast. */
    MOV_CMD_GO_FAST,
    /** Command to start moving forward. */
    MOV_CMD_GO,
    /** Command to start moving forward. */
    MOV_CMD_GO_SLOW,
    /** Command to stop moving. */
    MOV_CMD_STOP,
    /** Command to move forward a given distance. */
    MOV_CMD_GO_DIST
} MOV_CMD_VALUE;

/** Data byte values for FIX_CMD. */
typedef enum {
    /** Command to fix to the left slow*/
    FIX_CMD_LEFT_SLOW,
    /** Command to fix to the left*/
    FIX_CMD_LEFT,
    /** Command to fix to the left fast*/
    FIX_CMD_LEFT_FAST,
    /** Command to fix to the right*/
    FIX_CMD_RIGHT_SLOW,
    /** Command to fix to the right*/
    FIX_CMD_RIGHT,
    /** Command to fix to the right*/
    FIX_CMD_RIGHT_FAST

} FIX_CMD_VALUE;

/**
 * Array containing data sizes for each message type.  Indexed by the message
 * type as defined in public_message_type_t.  For example, the data size for a
 * message of type PUB_MSG_T_SENS_DIST should be determined by:
 * public_message_data_size[PUB_MSG_T_SENS_DIST].
 * The values are defined in public_messages.c.
 */
extern const unsigned char public_message_data_size[NUM_PUB_MSG_T];

/** Minimum size of a public message (with data length 0). */
#define PUB_MSG_MIN_SIZE (3)
/** Maximum size of a public message (including data). */
#define PUB_MSG_MAX_SIZE (15)

/** Max number of bytes of data allowed to be attached to a public message. */
#define PUB_MSG_MAX_DATA_SIZE (PUB_MSG_MAX_SIZE - PUB_MSG_MIN_SIZE)

// Javadoc for this union placed below so that it will appear in MPLAB X
// autocomplete info window.

typedef union {

    /** Message structure gives access to individual named fields. */
    struct {
        public_message_type_t message_type;
        unsigned char message_count;
        unsigned char data_length;
        unsigned char data[PUB_MSG_MAX_DATA_SIZE];
    };
    /**
     * Byte array gives direct access to the message contents without using
     * named fields.
     */
    unsigned char raw_message_bytes[PUB_MSG_MAX_SIZE];
}
/**
 * Structure of public messages.  This union causes the struct and the
 * message_bytes array to occupy the same memory space so that they will both
 * read/write the same location.
 */
public_message_t;

/**
 * Shortcut macro to get the offset (i.e. byte index) of a field in the
 * public_message_t structure.
 */
// Note: cannot put parentheses around __field because it causes an error in
// the offsetof macro.
#define PUB_MSG_FIELD_OFFSET(__field) (offsetof(public_message_t, __field))

/**
 * Retrieve the next message count value for a given message type.  The counter
 * is incremented internally.
 * @param type Message type whose count is being requested.
 * @return Count value to be sent
 */
unsigned char public_message_get_count(const public_message_type_t type);

#endif	/* PUBLIC_MESSAGES_H */

