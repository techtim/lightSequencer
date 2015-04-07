//
//  Config.h
//  lightSequencerGipsy
//
//  Created by Tim TVL on 06/07/14.
//
//

#ifndef lightSequencerGipsy_Config_h
#define lightSequencerGipsy_Config_h

#define USE_AUDIO 1

#define DISPLAY_W 1200
#define DISPLAY_H 800

#define PROJECTOR_W 1000 //1400
#define PROJECTOR_H 1000 //1050

#define COLUMNS_WIDTH 20
#define COLUMNS_OFFSET 40

#define PRESETS_NUM 32

#define MATRIX_W 24
#define MATRIX_H 6

#define DMX_CHANNELS_IN_PAR 4
#define DMX_PORT "tty.usbserial-EN133019"
#define DMX_NUM_CHANNELS 512

#define MIDI_VOLUME_START_CC 110
#define MIDI_MIX_SATURATION 103
#define MIDI_MIX_BRIGHTNESS 70
#define MIDI_SEQ_PLAY_CC 41
#define MIDI_SEQ_START_CC 43
#define MIDI_PRESET_BEGIN_CC 10


#define ABLETON_CTRL false;//true; //false;

#define OSC_PORT_IN 7777

enum ledDmxType
{
	LED_DMX_RGB = 0,
	LED_DMX_RGBA = 1,
    LED_DMX_ARGB = 2,
    LED_DMX_RGBW = 3,
    LED_DMX_W = 4
};

//const std::string dt[5] = {"RGB","RGB+A","A+RGB","RGB+W","W"};
//const std::vector<std::string> dmxTypes(dt, dt+5);

//const int DMX_ADDR_MAP[] =
//{
//    1, 37, 21, 0 , 61, 81, 117, 157,
//    37, 45, 25, 5, 65, 85, 113, 121,
//    141, 49, 29, 9, 69, 89, 109, 125,
//    145,53, 33, 13,73, 93, 105,  129,
//    196,57, 0, 17, 77, 97, 101,  196,
//};


//24x6
const int DMX_ADDR_MAP[] =
{
    253,256,259,262,265,268, 271,274,277,280,283,286, 0,0,0,0,0,0, 0,0,0,0,0,0,
    0,0, 145,148,151,154,157,160, 0,0, 163,166,169,172,175,178, 0,0, 181,184,187,190,193,196,
    1,4,7,10,13,16, 19,22,25,28,31,34, 37,40,43,46,49,52, 55,58,61,64,67,70,
    73,76,79,82,85,88, 91,94,97,100,103,106, 109,112,115,118,121,124, 127,130,133,136,139,142,
    0,0,199,202,205,208,211,214,0,0,217,220,223,226,229,232,0,0,235,238,241,244,247,250,
    289,292,295,298,301,304, 307,310,313,316,319,322, 0,0,0,0,0,0, 0,0,0,0,0,0
    
};

//const int DMX_ADDR_MAP[] =
//{
//    1,4,7,10,13,16, 19,22,25,28,31,34, 37,40,43,46,49,52, 55,58,61,64,67,70, 73,76,79,82,85,88,
//    91,94,97,100,103,106, 109,112,115,118,121,124, 127,130,133,136,139,142, 145,148,151,154,157,160, 163,166,169,172,175,178
//};


#endif
