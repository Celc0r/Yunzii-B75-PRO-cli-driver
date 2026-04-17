#pragma once
#include <hidapi/hidapi.h>

#define YUNZII_VID  0x3151
#define YUNZII_PID  0x4015

/*  Used in static and animation modes
    returns checksum bytes that used to validate packet not being corrupted 
*/
unsigned char get_checksum(unsigned char *data);

hid_device* open_yunzii_rgb_interface();

void set_static_color(hid_device* dev, unsigned char brightness, unsigned char r, unsigned char g, unsigned char b);
void set_animation(hid_device* dev, struct UserOptions);
void set_perkey(hid_device* dev);

/* Sends last 2 packets of perkey LED
*/
void send_flush(hid_device* dev);

void send_packet(hid_device* dev, unsigned char *data);

/*  Animation modes */
enum YunziiMode
{
    MODE_1  = 0x00,
    MODE_2  = 0x10,
    MODE_3  = 0x20,
    MODE_4  = 0x30,
};

/*  Animation   */
struct YunziiAnimation
{
    const char*   name;
    unsigned char anim_b;
    unsigned char mode_count;
};

static const YunziiAnimation animations[] = {
    {"Wave", 0x04, 0x04},
    {"Riple", 0x05, 0x00,},
    {"Starlight", 0x06, 0x00,},
    {"Stream", 0x07, 0x02},
    {"Shadow", 0x08, 0x00},
    {"Mountain Wave", 0x09, 0x00},
    {"Sine Wave", 0x0a, 0x00},
    {"Color Spring", 0x0b,0x02},
    {"Snow Trace", 0x13,  0x00},
    {"Flower Wave", 0x0c,  0x02},
    {"Kill Two Birds", 0x0e,  0x00},
    {"Circle Wave", 0x0f,  0x02},
    {"Colorful Cross", 0x10,  0x00},
    {"Snow Fall", 0x11,  0x00},
    {"Meteor", 0x12,  0x00},
    {"Dynamic Breathing", 0x02, 0x00},
    {"Spectrum Cycle", 0x02, 0x00}
};

enum eKeys {
    ESC = 0,
    TILDE = 1,
    TAB = 2,
    CAPS_LOCK = 3,
    LEFT_SHIFT = 4,
    LEFT_CTRL = 5,
    F1 = 6,
    ONE = 7,
    Q = 8,
    A = 9,
    F2 = 10,
    TWO = 11,
    W = 12,
    S = 13,
    Z = 14,
    WIN = 15,
    F3 = 16,
    THREE = 17,
    E = 18,
    D = 19,
    X = 20,
    LEFT_ALT = 21,
    F4 = 22,
    FOUR = 23,
    R = 24,
    F = 25,
    C = 26,
    F5 = 27,
    FIVE = 28,
    T = 29,
    G = 30,
    V = 31,
    F6 = 32,
    SIX = 33,
    Y = 34,
    H = 35,
    B = 36,
    SPACE = 37,
    F7 = 38,
    SEVEN = 39,
    U = 40,
    J = 41,
    N = 42,
    F8 = 43,
    EIGHT = 44,
    I = 45,
    K = 46,
    M = 47,
    F9 = 48,
    NINE = 49,
    O = 50,
    L = 51,
    LESS_THAN = 52,
    RIGHT_ALT = 53,
    F10 = 54,
    ZERO = 55,
    P = 56,
    SEMICOLON = 57,
    GREATER_THAN = 58,
    FN = 59,
    F11 = 60,
    MINUS = 61,
    SQUARE_OPEN = 62,
    QUOTE = 63,
    QUESTION = 64,
    RIGHT_CTRL = 65,
    F12 = 66,
    EQUALS = 67,
    SQUARE_CLOSE = 68,
    RIGHT_SHIFT = 69,
    LEFT_ARROW = 70,
    DEL = 71,
    BACKSPACE = 72,
    BACKSLASH = 73,
    ENTER = 74,
    UP_ARROW = 75,
    DOWN_ARROW = 76,
    INSERT = 77,
    END = 78,
    PGUP = 79,
    PGDN = 80,
    RIGHT_ARROW = 81
};

/*  Key settings    */
struct YunziiKey
{
    const char*   name;
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
};

static YunziiKey keys[] =
{
    {"Esc"},
    {"`"},
    {"Tab"},
    {"Caps Lock"},
    {"Left Shift"},
    {"Left Ctrl"},
    {"F1"},
    {"1"},
    {"Q"},
    {"A"},
    // blank 30
    // blank 33
    {"F2"},
    {"2"},
    {"W"},
    {"S"}, 
    {"Z"}, 
    {"Win"}, 
    {"F3"},
    {"3"}, 
    {"E"}, 
    {"D"}, 
    {"X"}, 
    {"Left Alt"}, 
    {"F4"}, 
    {"4"}, 
    {"R"}, 
    {"F"}, 
    {"C"},
    // blank 87
    {"F5"}, 
    {"5"}, 
    {"T"}, 
    {"G"}, 
    {"V"},
    //blank 105
    {"F6"},
    {"6"},
    {"Y"}, 
    {"H"}, 
    {"B"}, 
    {"Space"}, 
    {"F7"}, 
    {"7"}, 
    {"U"}, 
    {"J"}, 
    {"N"},
    // blank 141
    {"F8"}, 
    {"8"}, 
    {"I"}, 
    {"K"}, 
    {"M"},
    // blank 159
    {"F9"},
    {"9"},
    {"O"}, 
    {"L"}, 
    {"<"},
    {"Right Alt"}, 
    {"F10"}, 
    {"0"}, 
    {"P"}, 
    {":"},
    {">"},
    {"Fn"},
    {"F11"}, 
    {"-"},
    {"["},
    {"'"},
    {"?"},
    {"Right Ctrl"},
    {"F12"},
    {"="},
    {"]"},
    // blank 222
    {"Right Shift"},
    {"Left Arrow"},
    {"Del"},
    {"Backspace"}, 
    {"\\"},
    {"Enter"}, 
    {"Up Arrow"}, 
    {"Down Arrow"},
    // blank 249
    {"Insert"}, 
    {"End"}, 
    {"PgUp"}, 
    {"PgDn"}, 
    {"Right Arrow"}
};

static const int KEY_COUNT = sizeof(keys) / sizeof(keys[0]);