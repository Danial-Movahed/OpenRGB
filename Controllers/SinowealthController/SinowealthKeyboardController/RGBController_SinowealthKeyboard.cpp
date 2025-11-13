/*------------------------------------------*\
|  RGBController_SinowealthKeyboard.cpp      |
|                                            |
|  Definitions and types for Sinowealth      |
|  Keyboard, Hopefully generic, this was     |
|  made spefically for FL eSports F11 KB     |
|                                            |
|  Dmitri Kalinichenko (Dima-Kal) 23/06/2021 |
\*-----------------------------------------=*/

#include "RGBControllerKeyNames.h"
#include "RGBController_SinowealthKeyboard.h"

#define NA              0xFFFFFFFF

static unsigned int tkl_matrix_map[6][16] =
{
    // Row 0 (Top row - function keys, etc.)
    {  NA,   7,  10,  13,  16,  19,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA },

    // Row 1 (Number row)
    {  NA,  25,  28,  31,  34,  37,  43,  46,  49,  52,  55,  61,  64,  67,  70,  NA },

    // Row 2 (QWERTY row)
    {  NA,  79,  82,  85,  88,  NA,  97, 100, 103, 106, 109, 115, 118, 121, 124,  NA },

    // Row 3 (ASDF row)
    {  NA, 133, 136, 139, 142,  NA, 151, 154, 157, 160, 163, 169, 172, 175, 178,  NA },

    // Row 4 (ZXCV row)
    {  NA, 187, 190, 193, 196,  NA, 205, 208, 211,  NA,  NA, 223, 226,  NA,  NA,  NA },

    // Row 5 (Bottom row + arrows)
    {  NA, 241, 244, 247, 250, 253, 277, 280, 283, 286, 289,  NA,  NA, 268, 271,  NA }
};


static const char *led_names_tkl[] =
{
    KEY_EN_ESCAPE,
    KEY_EN_1,
    KEY_EN_2,
    KEY_EN_3,
    KEY_EN_4,
    KEY_EN_5,
    KEY_EN_6,
    KEY_EN_7,
    KEY_EN_8,
    KEY_EN_9,
    KEY_EN_0,
    KEY_EN_MINUS,
    KEY_EN_EQUALS,
    KEY_EN_BACKSPACE,
    KEY_EN_DELETE,

    KEY_EN_TAB,
    KEY_EN_Q,
    KEY_EN_W,
    KEY_EN_E,
    KEY_EN_R,
    KEY_EN_T,
    KEY_EN_Y,
    KEY_EN_U,
    KEY_EN_I,
    KEY_EN_O,
    KEY_EN_P,
    KEY_EN_LEFT_BRACKET,
    KEY_EN_RIGHT_BRACKET,
    KEY_EN_ANSI_BACK_SLASH,
    KEY_EN_BACK_TICK,

    KEY_EN_CAPS_LOCK,
    KEY_EN_A,
    KEY_EN_S,
    KEY_EN_D,
    KEY_EN_F,
    KEY_EN_G,
    KEY_EN_H,
    KEY_EN_J,
    KEY_EN_K,
    KEY_EN_L,
    KEY_EN_SEMICOLON,
    KEY_EN_QUOTE,
    KEY_EN_ANSI_ENTER,
    KEY_EN_PAGE_UP,

    KEY_EN_LEFT_SHIFT,
    KEY_EN_Z,
    KEY_EN_X,
    KEY_EN_C,
    KEY_EN_V,
    KEY_EN_B,
    KEY_EN_N,
    KEY_EN_M,
    KEY_EN_COMMA,
    KEY_EN_PERIOD,
    KEY_EN_FORWARD_SLASH,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_UP_ARROW,
    KEY_EN_PAGE_DOWN,

    KEY_EN_LEFT_CONTROL,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_LEFT_ALT,
    KEY_EN_SPACE,
    KEY_EN_RIGHT_ALT,
    "Key: Fn",
    "Key: Fn2",
    KEY_EN_LEFT_ARROW,
    KEY_EN_DOWN_ARROW,
    KEY_EN_RIGHT_ARROW
};

/**------------------------------------------------------------------*\
    @name Sinowealth Keyboard
    @category Keyboard
    @type USB
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectSinowealthKeyboard
    @comment
\*-------------------------------------------------------------------*/

RGBController_SinowealthKeyboard::RGBController_SinowealthKeyboard(SinowealthKeyboardController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetName();
    type                            = DEVICE_TYPE_KEYBOARD;
    description                     = "Sinowealth Keyboard Device";
    location                        = controller->GetLocation();
    serial                          = controller->GetSerialString();

    mode Custom;
    Custom.name                     = "Custom";
    Custom.flags                    = MODE_FLAG_HAS_PER_LED_COLOR;
    Custom.color_mode               = MODE_COLORS_PER_LED;
    Custom.value                    = MODE_PER_KEY;
    modes.push_back(Custom);

    // mode Off;
    // Off.name                        = "Off";
    // Off.flags                       = 0;
    // Off.color_mode                  = MODE_COLORS_NONE;
    // Off.value                       = MODE_OFF;
    // modes.push_back(Off);

    SetupZones();
}

RGBController_SinowealthKeyboard::~RGBController_SinowealthKeyboard()
{
    delete controller;
}

void RGBController_SinowealthKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    zone new_zone;

    new_zone.name                   = ZONE_EN_KEYBOARD;
    new_zone.type                   = ZONE_TYPE_MATRIX;
    new_zone.leds_min               = 68;
    new_zone.leds_max               = 68;
    new_zone.leds_count             = 68;
    new_zone.matrix_map             = new matrix_map_type;
    new_zone.matrix_map->height     = 6;
    new_zone.matrix_map->width      = 16;
    new_zone.matrix_map->map        = (unsigned int *)&tkl_matrix_map;

    zones.push_back(new_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < 68; led_idx++)
    {
        led new_led;
        new_led.name = led_names_tkl[led_idx];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_SinowealthKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SinowealthKeyboard::DeviceUpdateLEDs()
{
    controller->SetLEDsDirect(colors);
}

void RGBController_SinowealthKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SinowealthKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SinowealthKeyboard::DeviceUpdateMode()
{
    unsigned int brightness  = BRIGHTNESS_FULL;
    RGBColor* selected_color = (modes[active_mode].color_mode == MODE_COLORS_NONE) ? 0 : &modes[active_mode].colors[0];

    // if(modes[active_mode].value == MODE_STATIC)
    // {
    //     controller->SetStaticColor(selected_color);
    // }
    // else
    // {
        controller->SetMode(modes[active_mode].value, brightness, modes[active_mode].speed, modes[active_mode].color_mode);
    // }
}
