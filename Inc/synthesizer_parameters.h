/*
 * synthesizer_parameters.h
 *
 *  Created on: Jan. 28, 2022
 *      Author: k-omura
 */

//Common
#define SIZE_PARAMATER 0x004D

//Signal
#define M_VOLUME 0x0000 //Master Volume:0-100[%]
#define S1_ENABLE 0x0001 //Signal1 Volume:0-100[%]
#define S1_FREQ 0x0002 //Frequency of the Signal1:0-34579 (See Freq. Trans. tab)
#define S1_SIN 0x0003 //Sine amplitude of the Signal1:0-512
#define S1_SQU 0x0004 //Square amplitude of the Signal1:0-512
#define S1_SAW 0x0005 //Saw amplitude of the Signal1:0-512
#define S1_TRI 0x0006 //Triangle amplitude of the Signal1:0-512
#define S2_ENABLE 0x0007 //Signal2 Volume:0-100[%]
#define S2_FREQ 0x0008 //Frequency of the Signal2:0-34579 (See Freq. Trans. tab)
#define S2_SIN 0x0009 //Sine amplitude of the Signal2:0-512
#define S2_SQU 0x000A //Square amplitude of the Signal2:0-512
#define S2_SAW 0x000B //Saw amplitude of the Signal2:0-512
#define S2_TRI 0x000C //Triangle amplitude of the Signal2:0-512
#define S3_ENABLE 0x000D //Signal3 Volume:0-100[%]
#define S3_FREQ 0x000E //Frequency of the Signal3:0-34579 (See Freq. Trans. tab)
#define S3_SIN 0x000F //Sine amplitude of the Signal3:0-512
#define S3_SQU 0x0010 //Square amplitude of the Signal3:0-512
#define S3_SAW 0x0011 //Saw amplitude of the Signal3:0-512
#define S3_TRI 0x0012 //Triangle amplitude of the Signal3:0-512
#define S4_ENABLE 0x0013 //Signal4 Volume:0-100[%]
#define S4_FREQ 0x0014 //Frequency of the Signal4:0-34579 (See Freq. Trans. tab)
#define S4_SIN 0x0015 //Sine amplitude of the Signal4:0-512
#define S4_SQU 0x0016 //Square amplitude of the Signal4:0-512
#define S4_SAW 0x0017 //Saw amplitude of the Signal4:0-512
#define S4_TRI 0x0018 //Triangle amplitude of the Signal4:0-512
#define S5_ENABLE 0x0019 //Signal5 Volume:0-100[%]
#define S5_FREQ 0x001A //Frequency of the Signal5:0-34579 (See Freq. Trans. tab)
#define S5_SIN 0x001B //Sine amplitude of the Signal5:0-512
#define S5_SQU 0x001C //Square amplitude of the Signal5:0-512
#define S5_SAW 0x001D //Saw amplitude of the Signal5:0-512
#define S5_TRI 0x001E //Triangle amplitude of the Signal5:0-512
#define S6_ENABLE 0x001F //Signal6 Volume:0-100[%]
#define S6_FREQ 0x0020 //Frequency of the Signal6:20-20k
#define S6_SIN 0x0021 //Sine amplitude of the Signal6:0-512
#define S6_SQU 0x0022 //Square amplitude of the Signal6:0-512
#define S6_SAW 0x0023 //Saw amplitude of the Signal6:0-512
#define S6_TRI 0x0024 //Triangle amplitude of the Signal6:0-512
#define S7_ENABLE 0x0025 //Signal7 Volume:0-100[%]
#define S7_FREQ 0x0026 //Frequency of the Signal7:0-34579 (See Freq. Trans. tab)
#define S7_SIN 0x0027 //Sine amplitude of the Signal7:0-512
#define S7_SQU 0x0028 //Square amplitude of the Signal7:0-512
#define S7_SAW 0x0029 //Saw amplitude of the Signal7:0-512
#define S7_TRI 0x002A //Triangle amplitude of the Signal7:0-512
#define S8_ENABLE 0x002B //Signal8 Volume:0-100[%]
#define S8_FREQ 0x002C //Frequency of the Signal8:0-34579 (See Freq. Trans. tab)
#define S8_SIN 0x002D //Sine amplitude of the Signal8:0-512
#define S8_SQU 0x002E //Square amplitude of the Signal8:0-512
#define S8_SAW 0x002F //Saw amplitude of the Signal8:0-512
#define S8_TRI 0x0030 //Triangle amplitude of the Signal8:0-512
#define S9_ENABLE 0x0031 //Signal9 Volume:0-100[%]
#define S9_FREQ 0x0032 //Frequency of the Signal9:0-34579 (See Freq. Trans. tab)
#define S9_SIN 0x0033 //Sine amplitude of the Signal9:0-512
#define S9_SQU 0x0034 //Square amplitude of the Signal9:0-512
#define S9_SAW 0x0035 //Saw amplitude of the Signal9:0-512
#define S9_TRI 0x0036 //Triangle amplitude of the Signal9:0-512
#define S10_ENABLE 0x0037 //Signal10 Volume:0-100[%]
#define S10_FREQ 0x0038 //Frequency of the Signal10:0-34579 (See Freq. Trans. tab)
#define S10_SIN 0x0039 //Sine amplitude of the Signal10:0-512
#define S10_SQU 0x003A //Square amplitude of the Signal10:0-512
#define S10_SAW 0x003B //Saw amplitude of the Signal10:0-512
#define S10_TRI 0x003C //Triangle amplitude of the Signal10:0-512

//Behavior
#define EG_ATTACK_TIM 0x003D //Envelope Generator Attack:0-65535[ms]
#define EG_ATTACK_LEVEL 0x003E //Envelope Generator Attack Max Level:SUSTAIN_LEVEL*EG_ATTACK_LEVEL/100 (Sustain level = Sx_ENABLE)
#define EG_DECAY_TIME 0x003F //Envelope Generator Decay:0-65535[ms]
#define EG_RELEASE_TIME 0x0040 //Envelope Generator Release:0-65535[ms]
#define LPF_CUTOFF 0x0041 //Cutoff frequency of Low-Pass Filter:0-34579 (See Freq. Trans. tab)
#define LPF_Q 0x0042 //Q value of Low-Pass Filter:LPF_Q/100
#define WOW_FORM 0x0043 //WOW waveform.:0-4.[SIN,SQU,SAW1,SAW2]
#define WOW_LFO 0x0044 //WOW frequency:0-65535 (See Freq. Trans. LFO tab)
#define WOW_AMP 0x0045 //WOW amplitude:0-2000
#define VIB_FORM 0x0046 //Vibrato waveform.:0-4.[SIN,SQU,SAW1,SAW2]
#define VIB_LFO 0x0047 //Vibrato frequency:0-65535 (See Freq. Trans. LFO tab)
#define VIB_AMP 0x0048 //Vibrato amplitude:0-2000
#define TRE_FORM 0x0049 //Tremolo waveform.:0-4.[SIN,SQU,SAW1,SAW2]
#define TRE_LFO 0x004A //Tremolo frequency:0-65535 (See Freq. Trans. LFO tab)
#define TRE_AMP 0x004B //Tremolo amplitude:0-2000

//Command
#define COM_PAGE 0x004C //:
