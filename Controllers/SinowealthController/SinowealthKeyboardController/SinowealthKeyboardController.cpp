/*------------------------------------------*\
|  SinowealthKeyboardController.cpp          |
|                                            |
|  Definitions and types for Sinowealth      |
|  Keyboard, Hopefully generic, this was     |
|  made spefically for FL eSports F11 KB     |
|                                            |
|  Dmitri Kalinichenko (Dima-Kal) 23/06/2021 |
\*-----------------------------------------=*/

#include <cstring>
#include "SinowealthKeyboardController.h"
#include "StringUtils.h"

// static unsigned char send_per_key_part_of_command_packet[]
// static unsigned char mode_brightness_speed_packet[]         = { 0x06, 0x03, 0xB6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//                                                                 0x00, 0x00, 0x5A, 0xA5, 0x03, 0x03, 0x00, 0x00, 0x00, 0x02, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00,
//                                                                 0x55, 0x55, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x20, 0x00, 0x44, 0x07, 0x30,
//                                                                 0x07, 0x23, 0x00, 0x23, 0x00, 0x23, 0x07, 0x33, 0x07, 0x23, 0x07, 0x23, 0x07, 0x23, 0x07, 0x23,
//                                                                 0x07, 0x23, 0x07, 0x23, 0x07, 0x23, 0x07, 0x23, 0x07, 0x23, 0x07, 0x23, 0x07, 0x23, 0x07, 0x23,
//                                                                 0x07, 0x23, 0x00, 0x10, 0x00, 0x10, 0x07, 0x44, 0x07, 0x44, 0x07, 0x44, 0x07, 0x44, 0x07, 0x44,
//                                                                 0x07, 0x44, 0x07, 0x44, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00,
//                                                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//                                                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5A, 0xA5, 0x03, 0x03 };

// NaN,    Esc  7, Tab 10,  Caps 13,   Shift 16,    Ctrl_L 19
// NaN,     1  25,   Q 28,     A 31,       Z 34,    Win 37
// NaN,     2  43,   W 46,     S 49,       X 52,    Alt 55
// NaN,     3  61,   E 64,     D 67,       C 70,    NaN
// NaN,     4  79,   R 82,     F 85,       V 88,    NaN,
// NaN,     5  97,  T 100,     G 103,      B 106,   Space 109,
// NaN,     6 115,  Y 118,     H 121,      N 124,   NaN,
// NaN,     7 133,  U 136,     J 139,      M 142,   NaN,
// NaN,     8 151,  I 154,     K 157,      , 160,   Alt_R 163,
// NaN,     9 169,  O 172,     L 175,      . 178,   Fn 181,
// NaN,     0 187,  P 190,     ; 193,      / 196,   Fn2 199,
// NaN,     - 205,  [ 208,     ' 211,        NaN,   NaN,
// NaN,     = 223,  ] 226,       NaN,        NaN,   NaN,
// NaN,  Bksp 241,  \ 244, Enter 247,  Shift 250,   ← 253,
// NaN,       NaN,    NaN,       NaN,      ↑ 268,   ↓ 271,
// NaN,   Del 277,  ` 280,  PgUp 283,   PgDn 286,   → 289
static unsigned int tkl_keys_per_key_index[68] = {
    // Order these to match your LED names array
    7,   // Esc
    25,  // 1
    43,  // 2
    61,  // 3
    79,  // 4
    97,  // 5
    115, // 6
    133, // 7
    151, // 8
    169, // 9
    187, // 0
    205, // -
    223, // =
    241, // Backspace
    277, // Delete
    10,  // Tab
    28,  // Q
    46,  // W
    64,  // E
    82,  // R
    100, // T
    118, // Y
    136, // U
    154, // I
    172, // O
    190, // P
    208, // [
    226, // ]
    244, // \
    280, // `
    13,  // Caps
    31,  // A
    49,  // S
    67,  // D
    85,  // F
    103, // G
    121, // H
    139, // J
    157, // K
    175, // L
    193, // ;
    211, // '
    247, // Enter
    283, // PgUp
    16,  // LShift
    34,  // Z
    52,  // X
    70,  // C
    88,  // V
    106, // B
    124, // N
    142, // M
    160, // ,
    178, // .
    196, // /
    250, // RShift
    268, // Up
    286, // PgDn
    19,  // LCtrl
    37,  // LWin
    55,  // LAlt
    109, // Space
    163, // RAlt
    181, // Fn
    199, // Fn2
    253, // Left
    271, // Down
    289  // Right
};

SinowealthKeyboardController::SinowealthKeyboardController(hid_device *dev_cmd_handle, hid_device *dev_data_handle, char *path, std::string dev_name)
{
    dev_cmd = dev_cmd_handle;
    dev_data = dev_data_handle;
    name = dev_name;

    led_count = sizeof(tkl_keys_per_key_index);

    current_mode = MODE_PER_KEY;
    current_speed = SPEED_NORMAL;

    location = path;
}

SinowealthKeyboardController::~SinowealthKeyboardController()
{
    hid_close(dev_cmd);
    hid_close(dev_data);
}

std::string SinowealthKeyboardController::GetLocation()
{
    return ("HID: " + location);
}

std::string SinowealthKeyboardController::GetName()
{
    return (name);
}

unsigned char SinowealthKeyboardController::GetCurrentMode()
{
    return current_mode;
}

unsigned int SinowealthKeyboardController::GetLEDCount()
{
    return (sizeof(tkl_keys_per_key_index));
}

std::string SinowealthKeyboardController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev_cmd, serial_string, 128);

    if (ret != 0)
    {
        return ("");
    }

    return (StringUtils::wstring_to_string(serial_string));
}

void SinowealthKeyboardController::SetLEDsDirect(std::vector<RGBColor> colors)
{
    const int buffer_size = 382;

    unsigned char buf[buffer_size];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    for (int i = 0; i < 67; i++)
    {
        buf[tkl_keys_per_key_index[i]] = RGBGetRValue(colors[i]);
        buf[tkl_keys_per_key_index[i] + 1] = RGBGetGValue(colors[i]);
        buf[tkl_keys_per_key_index[i] + 1 + 1] = RGBGetBValue(colors[i]);
        // printf("i: %d code: %d R: %d G: %d B: %d\n", i, tkl_keys_per_key_index[i], buf[tkl_keys_per_key_index[i]], buf[tkl_keys_per_key_index[i] + 1], buf[tkl_keys_per_key_index[i] + 1 + 1]);
    }

    /*-----------------------------------------------------*\
    | Set up Direct packet                                  |
    \*-----------------------------------------------------*/
    buf[0] = 0x08;
    buf[1] = 0x0a;
    buf[2] = 0x7a;
    buf[3] = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev_data, buf, sizeof(buf));
}

void SinowealthKeyboardController::SetMode(unsigned char mode, unsigned char brightness, unsigned char speed, unsigned char color_mode)
{
    return;
}
