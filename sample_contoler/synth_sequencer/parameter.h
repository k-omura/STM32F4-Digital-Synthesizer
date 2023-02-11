/*
 * synthesizer_parameters.h
 *
 *  Created on: Aug. 13, 2020
 *      Author: k-omura
 */

//Common
#define SIZE_PARAMATER 0x0055

//Signal
#define M_VOLUME 0x0000 //Master Volume:0-100[%]
#define SYNC_WAVFORM 0x0001 //Synchronize signal components to S1 for all channels:ON:1/OFF:0
#define S1_ENABLE 0x0002 //Signal1 Volume:0-100[%]
#define S1_FREQ 0x0003 //Frequency of the Signal1:0-34579 (See Freq. Trans. tab)
#define S1_SIN 0x0004 //Sine amplitude of the Signal1:0-512
#define S1_SQU 0x0005 //Square amplitude of the Signal1:0-512
#define S1_SAW 0x0006 //Saw amplitude of the Signal1:0-512
#define S1_TRI 0x0007 //Triangle amplitude of the Signal1:0-512
#define S2_ENABLE 0x0008 //Signal2 Volume:0-100[%]
#define S2_FREQ 0x0009 //Frequency of the Signal2:0-34579 (See Freq. Trans. tab)
#define S2_SIN 0x000A //Sine amplitude of the Signal2:0-512
#define S2_SQU 0x000B //Square amplitude of the Signal2:0-512
#define S2_SAW 0x000C //Saw amplitude of the Signal2:0-512
#define S2_TRI 0x000D //Triangle amplitude of the Signal2:0-512
#define S3_ENABLE 0x000E //Signal3 Volume:0-100[%]
#define S3_FREQ 0x000F //Frequency of the Signal3:0-34579 (See Freq. Trans. tab)
#define S3_SIN 0x0010 //Sine amplitude of the Signal3:0-512
#define S3_SQU 0x0011 //Square amplitude of the Signal3:0-512
#define S3_SAW 0x0012 //Saw amplitude of the Signal3:0-512
#define S3_TRI 0x0013 //Triangle amplitude of the Signal3:0-512
#define S4_ENABLE 0x0014 //Signal4 Volume:0-100[%]
#define S4_FREQ 0x0015 //Frequency of the Signal4:0-34579 (See Freq. Trans. tab)
#define S4_SIN 0x0016 //Sine amplitude of the Signal4:0-512
#define S4_SQU 0x0017 //Square amplitude of the Signal4:0-512
#define S4_SAW 0x0018 //Saw amplitude of the Signal4:0-512
#define S4_TRI 0x0019 //Triangle amplitude of the Signal4:0-512
#define S5_ENABLE 0x001A //Signal5 Volume:0-100[%]
#define S5_FREQ 0x001B //Frequency of the Signal5:0-34579 (See Freq. Trans. tab)
#define S5_SIN 0x001C //Sine amplitude of the Signal5:0-512
#define S5_SQU 0x001D //Square amplitude of the Signal5:0-512
#define S5_SAW 0x001E //Saw amplitude of the Signal5:0-512
#define S5_TRI 0x001F //Triangle amplitude of the Signal5:0-512
#define S6_ENABLE 0x0020 //Signal6 Volume:0-100[%]
#define S6_FREQ 0x0021 //Frequency of the Signal6:0-34579 (See Freq. Trans. tab)
#define S6_SIN 0x0022 //Sine amplitude of the Signal6:0-512
#define S6_SQU 0x0023 //Square amplitude of the Signal6:0-512
#define S6_SAW 0x0024 //Saw amplitude of the Signal6:0-512
#define S6_TRI 0x0025 //Triangle amplitude of the Signal6:0-512
#define S7_ENABLE 0x0026 //Signal7 Volume:0-100[%]
#define S7_FREQ 0x0027 //Frequency of the Signal7:0-34579 (See Freq. Trans. tab)
#define S7_SIN 0x0028 //Sine amplitude of the Signal7:0-512
#define S7_SQU 0x0029 //Square amplitude of the Signal7:0-512
#define S7_SAW 0x002A //Saw amplitude of the Signal7:0-512
#define S7_TRI 0x002B //Triangle amplitude of the Signal7:0-512
#define S8_ENABLE 0x002C //Signal8 Volume:0-100[%]
#define S8_FREQ 0x002D //Frequency of the Signal8:0-34579 (See Freq. Trans. tab)
#define S8_SIN 0x002E //Sine amplitude of the Signal8:0-512
#define S8_SQU 0x002F //Square amplitude of the Signal8:0-512
#define S8_SAW 0x0030 //Saw amplitude of the Signal8:0-512
#define S8_TRI 0x0031 //Triangle amplitude of the Signal8:0-512
#define S9_ENABLE 0x0032 //Signal9 Volume:0-100[%]
#define S9_FREQ 0x0033 //Frequency of the Signal9:0-34579 (See Freq. Trans. tab)
#define S9_SIN 0x0034 //Sine amplitude of the Signal9:0-512
#define S9_SQU 0x0035 //Square amplitude of the Signal9:0-512
#define S9_SAW 0x0036 //Saw amplitude of the Signal9:0-512
#define S9_TRI 0x0037 //Triangle amplitude of the Signal9:0-512
#define S10_ENABLE 0x0038 //Signal10 Volume:0-100[%]
#define S10_FREQ 0x0039 //Frequency of the Signal10:0-34579 (See Freq. Trans. tab)
#define S10_SIN 0x003A //Sine amplitude of the Signal10:0-512
#define S10_SQU 0x003B //Square amplitude of the Signal10:0-512
#define S10_SAW 0x003C //Saw amplitude of the Signal10:0-512
#define S10_TRI 0x003D //Triangle amplitude of the Signal10:0-512
#define N1_ENABLE 0x003E //Noise without frequency band limitation:0-100[%]
#define N1_setting 0x003F //Reserved:-
#define N2_ENABLE 0x0040 //Noise with LPF:0-100[%]
#define N2_LPFFREQ 0x0041 //Cutoff frequency of Low-Pass Filter for N2:0-34579 (See Freq. Trans. tab)
#define N3_ENABLE 0x0042 //Noise with HPF:0-100[%]
#define N3_HPFFREQ 0x0043 //Cutoff frequency of High-Pass Filter for N3:0-34579 (See Freq. Trans. tab)

//Behavior
#define EG_ATTACK_TIM 0x0044 //Envelope Generator Attack:0-65535[ms]
#define EG_ATTACK_LEVEL 0x0045 //Envelope Generator Attack Max Level:100< (SUSTAIN_LEVEL*EG_ATTACK_LEVEL/100 (Sustain level = Sx_ENABLE))
#define EG_DECAY_TIME 0x0046 //Envelope Generator Decay:0-65535[ms]
#define EG_RELEASE_TIME 0x0047 //Envelope Generator Release:0-65535[ms]
#define LPF_CUTOFF 0x0048 //Cutoff frequency of Low-Pass Filter:0-34579 (See Freq. Trans. tab)
#define LPF_Q 0x0049 //Q value of Low-Pass Filter:LPF_Q/100
#define WOW_FORM 0x004A //WOW waveform.:0-4.[SIN,SQU,SAW1,TRI,SAW2]
#define WOW_LFO 0x004B //WOW frequency:0-65535 (See Freq. Trans. LFO tab)
#define WOW_AMP 0x004C //WOW amplitude:0-34579 (See Freq. Trans. tab)
#define VIB_FORM 0x004D //Vibrato waveform.:0-4.[SIN,SQU,SAW1,TRI,SAW2]
#define VIB_LFO 0x004E //Vibrato frequency:0-65535 (See Freq. Trans. LFO tab)
#define VIB_AMP 0x004F //Vibrato amplitude:0-34579 (See Freq. Trans. tab)
#define TRE_FORM 0x0050 //Tremolo waveform.:0-4.[SIN,SQU,SAW1,TRI,SAW2]
#define TRE_LFO 0x0051 //Tremolo frequency:0-65535 (See Freq. Trans. LFO tab)
#define TRE_AMP 0x0052 //Tremolo amplitude:0-512
#define PITCH 0x0053 //Pitch:0-512 (Sx_FREQ + Pitch - 256)

//Command
#define COM_PAGE 0x0054 //:
