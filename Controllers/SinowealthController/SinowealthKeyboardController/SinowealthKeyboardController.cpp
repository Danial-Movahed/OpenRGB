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

// NaN 4, Esc 7, Tab 10, Caps 13, Shift 16, Ctrl_L 19
// NaN 22, 1 25,   Q 28,   A 31,    Z 34,     Win 37
// NaN 40, 2 43,   W 46,   S 49,    X 52,     Alt 55
// NaN 58, 3 61,   E 64,   D 67,    C 70,     NAN 73
// NaN 76, 4 79,   R 82,   F 85,    V 88,     NAN 91,
// NaN 94, 5 97,   T 100,   G 103,    B 106,     Space 109,
// NaN 112, 6 115,   Y 118,   H 121,    N 124,     NAN 127,
// NaN 130, 7 133,   U 136,   J 139,    M 142,     NAN 145,
// NaN 148, 8 151,   I 154,   K 157,    , 160,     Alt_R 163,
// NaN 166, 9 169,   O 172,   L 175,    . 178,     Fn 181,
// NaN 184, 0 187,   P 190,   ; 193,    / 196,     Fn2 199,
// NaN 202, - 205,   [ 208,   ' 211,    NAN 214,   NAN 217,
// NaN 220, = 223,   ] 226,   NAN 229,  NAN 232,   NAN 235,
// NaN 238, Bksp 241, \\ 244, Enter 247, Shift 250, ← 253,
// NaN 256, NAN 259, NAN 262, NAN 265, ↑ 268, ↓ 271,
// NaN 274, Del 277, ` 280, PgUp 283, PgDn 286, → 289
static unsigned int tkl_keys_per_key_index[] = {
    7, 10, 13, 16, 19,
    25, 28, 31, 34, 37,
    43, 46, 49, 52, 55,
    61, 64, 67, 70,
    79, 82, 85, 88,
    97, 100, 103, 106, 109,
    115, 118, 121, 124,
    133, 136, 139, 142,
    151, 154, 157, 160, 163,
    169, 172, 175, 178, 181,
    187, 190, 193, 196, 199,
    205, 208, 211,
    223, 226,
    241, 244, 247, 250, 253,
    268, 271,
    277, 280, 283, 286, 289
};


SinowealthKeyboardController::SinowealthKeyboardController(hid_device* dev_cmd_handle, hid_device* dev_data_handle, char* path, std::string dev_name)
{
    dev_cmd         = dev_cmd_handle;
    dev_data        = dev_data_handle;
    name            = dev_name;

    led_count       = sizeof(tkl_keys_per_key_index) ;

    current_mode    = MODE_PER_KEY;
    current_speed   = SPEED_NORMAL;

    location        = path;
}

SinowealthKeyboardController::~SinowealthKeyboardController()
{
    hid_close(dev_cmd);
    hid_close(dev_data);
}

std::string SinowealthKeyboardController::GetLocation()
{
    return("HID: " + location);
}

std::string SinowealthKeyboardController::GetName()
{
    return(name);
}

unsigned char SinowealthKeyboardController::GetCurrentMode()
{
    return current_mode;
}

unsigned int SinowealthKeyboardController::GetLEDCount()
{
    return(sizeof(tkl_keys_per_key_index));
}

std::string SinowealthKeyboardController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev_cmd, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void SinowealthKeyboardController::SetLEDsDirect(std::vector<RGBColor> colors)
{
    const int buffer_size = 382;

    unsigned char buf[buffer_size];
    // unsigned int num_keys = sizeof(tkl_keys_per_key_index) / sizeof(*tkl_keys_per_key_index);

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Direct packet                                  |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x08;
    buf[0x01]   = 0x0a;
    buf[0x02]   = 0x7a;
    buf[0x03]   = 0x01;

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    for(unsigned int i = 0; i < sizeof(tkl_keys_per_key_index); i++)
    {
        buf[tkl_keys_per_key_index[i]]          = RGBGetRValue(colors[i]);
        buf[tkl_keys_per_key_index[i] + 1]      = RGBGetGValue(colors[i]);
        buf[tkl_keys_per_key_index[i] + 1 + 1]  = RGBGetBValue(colors[i]);
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev_data, buf, sizeof(buf));
}

void SinowealthKeyboardController::SetMode(unsigned char mode, unsigned char brightness, unsigned char speed, unsigned char color_mode)
{
    const int buffer_size = 1032;

    int mode_byte_index = 0x15;
    const int speed_and_brightness_byte_index_start = 0x29; // Speed + brightnes level value, Seriously?

    unsigned int color_mode_value = color_mode == MODE_COLORS_RANDOM ? 0x07 : 0x00; // 0x07 - Value to set random color mode

    unsigned char usb_buf[buffer_size];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    unsigned int mode_brightness_speed_packet_length = sizeof(mode_brightness_speed_packet)/sizeof(char);

    for(unsigned int i = 0x00; i < mode_brightness_speed_packet_length; i++)
    {
        usb_buf[i] = mode_brightness_speed_packet[i];
    }

    usb_buf[mode_byte_index] = mode;

    int speed_and_brightness_byte_index = speed_and_brightness_byte_index_start + ((mode - 2) * 2);

    switch(mode)
    {
        case MODE_OFF:
            break;
        case MODE_RAINBOW:
            break;
        case MODE_FLASH_AWAY:
            break;
        case MODE_RAINDROPS:
            break;
        case MODE_RAINBOW_WHEEL:
            break;
        case MODE_RIPPLES_SHINING:
            break;
        case MODE_STARS_TWINKLE:
            break;
        case MODE_SHADOW_DISAPPEAR:
            break;
        case MODE_RETRO_SNAKE:
            break;
        case MODE_NEON_STREAM:
            break;
        case MODE_REACTION:
            break;
        case MODE_SINE_WAVE:
            break;
        case MODE_RETINUE_SCANNING:
            break;
        case MODE_ROTATING_WINDMILL:
            break;
        case MODE_COLORFUL_WATERFALL:
            break;
        case MODE_BLOSSOMING:
            break;
        case MODE_ROTATING_STORM:
            break;
        case MODE_COLLISION:
            break;
        case MODE_PERFECT:
            break;
        case MODE_PER_KEY:
            usb_buf[mode_byte_index - 1] = 0x01;
            usb_buf[0x27] = 0x24;
            break;
    }

    int color_mode_byte_index = speed_and_brightness_byte_index - 1;

    usb_buf[speed_and_brightness_byte_index] = speed + brightness;
    usb_buf[color_mode_byte_index] = color_mode_value;

    int result = hid_send_feature_report(dev_data, usb_buf, sizeof(usb_buf));

    if(result != -1)
    {
        current_mode = mode;
    }
}
