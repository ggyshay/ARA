#define C_BLACK 0x0
#define C_RED 0x1
#define C_GREEN 0x2
#define C_BLUE 0x4
#define C_CYAN C_GREEN | C_BLUE
#define C_YELLOW C_RED | C_GREEN
#define C_WHITE C_RED | C_GREEN | C_BLUE

#define MSG_CHANGE_INSTRUMENT 0
#define MSG_CHANGE_LENGTH 1
#define MSG_CLEAR_ALL 2
#define MSG_CLEAR_INSTRUMENT 3
#define MSG_TOGGLE_MUTE_INSTRUMENT 4
#define MSG_DUPLICATE_LOOP 5

extern unsigned long MAX_U_LONG_VAL;