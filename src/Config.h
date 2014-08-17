//
//  Config.h
//  lightSequencerGipsy
//
//  Created by Tim TVL on 06/07/14.
//
//

#ifndef lightSequencerGipsy_Config_h
#define lightSequencerGipsy_Config_h

#define DISPLAY_W 1000
#define DISPLAY_H 600

#define PROJECTOR_W 1000 //1400
#define PROJECTOR_H 1000 //1050

#define COLUMNS_WIDTH 20
#define COLUMNS_OFFSET 40

#define PRESETS_NUM 32

#define MATRIX_W 8;
#define MATRIX_H 5;

#define DMX_CHANNELS_IN_PAR 4
#define DMX_PORT "tty.usbserial-EN133019"

#define MIDI_VOLUME_START_CC 110
#define MIDI_MIX_SATURATION 103

#define ABLETON_CTRL true;//true; //false;

#define OSC_PORT_IN 7777

const int DMX_ADDR_MAP[] =
{
    133, 41, 21, 1, 61, 81, 117, 157,
    137, 45, 25, 5, 65, 85, 113, 121,
    141, 49, 29, 9, 69, 89, 109, 125,
    145,53, 33, 13,73, 93, 105,  129,
    196,57, 37, 17, 77, 97, 101,  196,
};


#endif
