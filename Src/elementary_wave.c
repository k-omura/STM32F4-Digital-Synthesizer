/*
 * elementary_wave.c
 *
 *  Created on: May 28, 2020
 *      Author: k-omura
 */

#include "elementary_wave.h"
#include "stdlib.h"

void add_square(float32_t *_audio, float32_t *_rad, uint32_t _amp, uint32_t _size) {
	if (_amp == 0) {
		return;
	}
	//pointer
	float32_t *p_audio = _audio;
	float32_t *p_rad = _rad;

	for(uint32_t t = 0; t < _size; t++){
		if((*p_rad++) < M_PIF32){
			*p_audio++ += (float32_t)_amp;
		}else{
			*p_audio++ -= (float32_t)_amp;
		}
	}

	return;
}

void add_triangle(float32_t *_audio, float32_t *_rad, uint32_t _amp, uint32_t _size) {
	if (_amp == 0) {
		return;
	}
	//pointer
	float32_t *p_audio = _audio;
	float32_t *p_rad = _rad;

	float32_t doubel_amp = 2.0f * (float32_t)_amp;
	float32_t doubel_amp_pi = doubel_amp / M_PIF32;
	float32_t tmp;
	for(uint32_t t = 0; t < _size; t++){
		tmp = doubel_amp_pi * (*p_rad) - doubel_amp;
		if((*p_rad++) < M_PIF32){
			*p_audio++ += tmp;
		}else{
			*p_audio++ -= tmp;
		}
	}

	return;
}

void add_sawtooth(float32_t *_audio, float32_t *_rad, uint32_t _amp, uint32_t _size) {
	if (_amp == 0) {
		return;
	}
	//pointer
	float32_t *p_audio = _audio;
	float32_t *p_rad = _rad;

	float32_t ampbypi = -(float32_t)_amp / M_PIF32;
	for(uint32_t t = 0; t < _size; t++){
		*p_audio++ += ampbypi * (*p_rad++) + _amp;
	}

	return;
}

void add_sin(float32_t *_audio, float32_t *_rad, uint32_t _amp, uint32_t _size) {
	if (_amp == 0) {
		return;
	}
	//pointer
	float32_t *p_audio = _audio;
	float32_t *p_rad = _rad;

	for(uint32_t t = 0; t < _size; t++){
		*p_audio++ += _amp * arm_sin_f32(*p_rad++);
	}

	return;
}

//RNG_HandleTypeDef *_hrng;
void add_noise(float32_t *_audio, uint32_t _amp, uint32_t _size) {
	if (_amp == 0) {
		return;
	}
	//pointer
	float32_t *p_audio = _audio;

	//Box–Muller's method
	uint32_t rn_u32;
	float32_t x, y;
	uint32_t x_temp;

	for(uint32_t t = 0; t < _size; t += 2){

		//TRNG
		/*
        do {
			HAL_RNG_GenerateRandomNumber(_hrng, &rn_u32);
			x_temp = rn_u32 >> 16;
        } while(x_temp == 0);
        //*/

		//c rnd
		//*
        do {
			rn_u32 = rand();
			x_temp = rn_u32 >> 16;
        } while(x_temp == 0);
        //*/

		x = (float32_t)(x_temp) / 0xffff;
		y = (float32_t)(rn_u32 & 0x0000ffff) / 0xffff;

		float32_t sqrtlogx;
		arm_sqrt_f32(-2.0f * log10f(x), &sqrtlogx);
		float32_t rad = SYNTH_DOUBLE_PI * y;

		*p_audio++ += sqrtlogx * arm_cos_f32(rad);
		*p_audio++ += sqrtlogx * arm_sin_f32(rad);
	}

	arm_scale_f32(_audio, (float32_t)(_amp), _audio, SYNTH_NUM_SAMPLING);
	return;
}
//

//
void float2uint(float32_t *_in, uint16_t *_out, uint32_t _start, uint32_t _end) {
	float32_t tmp;

	uint32_t out_counter = _start; //dac
	//uint32_t out_counter = 2 * _start + 1; //i2s

	//pointer
	float32_t *p_in = &_in[_start + SYNTH_PRE_SAMPLE];
	uint16_t *p_out = &_out[out_counter];

	for (uint32_t t = _start; t < _end; t++) {
		tmp = *p_in++;

		if(tmp < 0){
			tmp = 0;
		}else if(tmp > (float32_t)SYNTH_AMP_MAX){
			tmp = (float32_t)SYNTH_AMP_MAX;
		}

		*p_out++ = (uint16_t)tmp;
		//*p_out++;//i2s
	}
}

float32_t lfo_waveform(float32_t *_rad, uint32_t _amp, uint32_t _freq, uint32_t _type){
	float32_t output = 0;

	if ((0 < _freq) && (0 < _amp)) {
		switch(_type){
			case 0:
				output = arm_sin_f32(*_rad);
				break;
			case 1:
				if ((*_rad) < M_PIF32){
					output = 1.0f;
				}else{
					output = -1.0f;
				}
				break;
			case 2:
				output = (-(*_rad) / M_PIF32) + 1.0f;
				break;
			case 3:
				output = ((*_rad) / M_PIF32) - 1.0f;
				break;
			default:
				output = 0.0f;
				break;
		}
		output *= (float32_t)_amp;

		//refresh omega
		float32_t lfo_freq = 20.0f * (float32_t)_freq / 65535.0f;
		float32_t delta_omega = SYNTH_DOUBLE_PI * lfo_freq / (float32_t)SYNTH_SAMPLE_FREQ;
		(*_rad) = (*_rad) + ((float32_t)SYNTH_NUM_SAMPLING * delta_omega);
		(*_rad) = fmodf((*_rad), SYNTH_DOUBLE_PI);
	}else{
		(*_rad) = 0;
	}

	return output;
}

float32_t amp_adsr(float32_t *_adsr_state, uint32_t *_adsr_time, uint32_t _ampsig, uint32_t _a_level, uint32_t _a_time, uint32_t _d_time, uint32_t _r_time){
	float32_t output = 1;

	return output;
}

//array
uint16_t *_audio_dac;
uint16_t *_control_value;
float32_t _audio_out[SYNTH_SAMPLE_ALL] = { 0 };

//variables
uint32_t _audiodevice_tx_half_done = 0;
uint32_t _status = SYNTH_STATUS_LFOC;

//HAL interrupt
//void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s) {
void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac) {
	_audiodevice_tx_half_done = 0;
	float2uint(_audio_out, _audio_dac, SYNTH_HALF_NUM_SAMPLING, SYNTH_NUM_SAMPLING);
	_status = SYNTH_STATUS_LFOC;
}
//void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s) {
void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac) {
	if(_status == SYNTH_STATUS_WAIT){
		_audiodevice_tx_half_done = 0;
		float2uint(_audio_out, _audio_dac, 0, SYNTH_HALF_NUM_SAMPLING);
	}else{
		_audiodevice_tx_half_done = 1;
	}
}

void set_synth_para(uint16_t *__audio_dac, uint16_t *__control_value){
//void set_synth_para(uint16_t *__audio_dac, uint16_t *__control_value, RNG_HandleTypeDef *__hrng){
	_audio_dac = __audio_dac;
	_control_value = __control_value;

	//_hrng = __hrng;

	return;
}

uint8_t synth_process(){
	//variables
	static float32_t audio_in[SYNTH_SAMPLE_ALL] = { 0 };
	static float32_t audio_rad[SYNTH_NUM_SAMPLING] = { 0 };

	static float32_t audio_rad_chord[10] = { 0 };
	static uint32_t notenum_prev[10] = { 0 };
	static uint32_t sig_amp_prev[13] = { 0 };

	static float32_t lfo_rad[3] = { 0 };
	static float32_t vib = 0, tre = 0, wow = 0;

	static uint32_t adsr_time[13] = { 0 };
	static uint32_t adsr_state[13] = { 0 };
	static uint32_t adsr_a_time = 0;
	static uint32_t adsr_d_time = 0;
	static uint32_t adsr_r_time = 0;

	static arm_biquad_cascade_df2T_instance_f32 S_signal;
	static float32_t pCoeffs_signal[5];
	static float32_t buffer_signal[2];

	static float32_t cutoff_freq = 0.0f;
	static uint32_t cutoff_NN = 1;
	static int32_t wow_NN = 0;
	static float32_t q_factor = 0.8f;

	uint8_t ouput = 0;

	//process
	switch (_status) {
		case SYNTH_STATUS_LFOC: {
			//vibrato
			vib = lfo_waveform(&lfo_rad[SYNTH_IND_VIB], _control_value[VIB_AMP], _control_value[VIB_LFO], _control_value[VIB_FORM]);

			//tremolo
			tre = lfo_waveform(&lfo_rad[SYNTH_IND_TRE], _control_value[TRE_AMP], _control_value[TRE_LFO], _control_value[TRE_FORM]);
			tre = 1 + (tre / 1000.0f);

			//wow
			wow = lfo_waveform(&lfo_rad[SYNTH_IND_WOW], _control_value[WOW_AMP], _control_value[WOW_LFO], _control_value[WOW_FORM]);
			wow_NN = (int32_t)wow;

			//create filter
			cutoff_NN = _control_value[LPF_CUTOFF] + wow_NN;
			if (cutoff_NN > SYNTH_MAX_FREQ_NOTE) {
				cutoff_NN = SYNTH_MAX_FREQ_NOTE;
			}

			q_factor = (float32_t)_control_value[LPF_Q] / 100.0f;
			cutoff_freq = 440.0f * powf(2.0f, ((float32_t) cutoff_NN / 3072.0f) - 5.75f);

			calc_lpf_coeffs(pCoeffs_signal, cutoff_freq, q_factor);
			arm_biquad_cascade_df2T_init_f32(&S_signal, 1, pCoeffs_signal, buffer_signal);
			//calc_amp_char(amp_characteristic, cutoff_freq, q_factor); //filter stastic

			//ADSR prepare
			if(_control_value[EG_ATTACK_LEVEL] < 100){
				_control_value[EG_ATTACK_LEVEL] = 100;
			}
			if(_control_value[EG_ATTACK_TIM] == 0){
				_control_value[EG_ATTACK_TIM] = 1;
			}
			if(_control_value[EG_DECAY_TIME] == 0){
				_control_value[EG_DECAY_TIME] = 1;
			}
			if(_control_value[EG_RELEASE_TIME] == 0){
				_control_value[EG_RELEASE_TIME] = 1;
			}
			adsr_a_time = (uint32_t)((float32_t)_control_value[EG_ATTACK_TIM] / (1000.0f / (float32_t)SYNTH_SAMPLE_FREQ));
			adsr_d_time = (uint32_t)((float32_t)_control_value[EG_DECAY_TIME] / (1000.0f / (float32_t)SYNTH_SAMPLE_FREQ));
			adsr_r_time = (uint32_t)((float32_t)_control_value[EG_RELEASE_TIME] / (1000.0f / (float32_t)SYNTH_SAMPLE_FREQ));

			//Processing Exit
			if(_status == SYNTH_STATUS_LFOC){
				_status = SYNTH_STATUS_CHRD;
			}
			break;
		}
		case SYNTH_STATUS_CHRD: {
			arm_fill_f32(0, (audio_in + SYNTH_PRE_SAMPLE), SYNTH_NUM_SAMPLING); //audio array initialize
			uint32_t signal_set_index = S1_ENABLE;

			for(uint32_t chord = 0; chord < SYNTH_MAX_CHORD; chord++){
				float32_t amp_chord = 1;

				uint32_t ampsig = _control_value[signal_set_index];
				if((0 == ampsig) && (adsr_state[chord] > 4)){
					sig_amp_prev[chord] = 0;
					signal_set_index += (chord < 10) ? (6) : (2);
					continue;
				}else if((0 == ampsig) && (adsr_state[chord] < 4)) {
					adsr_state[chord] = 4;
					adsr_time[chord] = 0;
				}else if((ampsig > 0) && (adsr_state[chord] > 3)){
					adsr_state[chord] = 0;
				}

				//ADSR
				float32_t amp_adsr = 1;
				switch(adsr_state[chord]){
					case 0:
						adsr_state[chord] = 1;
						adsr_time[chord] = 0;
					case 1: //attack
						if(adsr_time[chord] < adsr_a_time){
							amp_adsr = powf(10.0f, -2.0f * (float32_t)adsr_time[chord] / adsr_a_time);
							amp_adsr = (_control_value[EG_ATTACK_LEVEL] / 100.0f) * (1 - amp_adsr);
							adsr_time[chord] += SYNTH_NUM_SAMPLING;
							break;
						}else{
							adsr_state[chord] = 2;
							adsr_time[chord] = 0;
						}
					case 2: //decay
						if(adsr_time[chord] < adsr_d_time){
							amp_adsr = powf(10.0f, -2.0f * (float32_t)adsr_time[chord] / adsr_d_time);
							amp_adsr = (ampsig / 100.0f) + ((_control_value[EG_ATTACK_LEVEL] - ampsig) / 100.0f) * amp_adsr;
							adsr_time[chord] += SYNTH_NUM_SAMPLING;
							break;
						}else{
							adsr_state[chord] = 3;
							adsr_time[chord] = 0;
							break;
						}
					case 3: //sustain
						break;
					case 4: //release
						ampsig = sig_amp_prev[chord];
						amp_adsr = powf(10.0f, -2.0f * (float32_t)adsr_time[chord] / adsr_r_time);

						if((0.00005f > amp_chord) || (adsr_time[chord] > adsr_r_time)){
							adsr_state[chord] = 5;
						}
						adsr_time[chord] += SYNTH_NUM_SAMPLING;
						break;
					default:
						break;
				}

				amp_chord *= ampsig / (float32_t)100;
				amp_chord *= amp_adsr;

				sig_amp_prev[chord] = ampsig;
				if ((0.00005f > amp_chord) || (adsr_state[chord] > 4)) {
					signal_set_index += (chord < 10) ? (6) : (2);
					continue;
				}

				float32_t audio_temp[SYNTH_NUM_SAMPLING] = {0};
				if(chord < 10){
					//Set base frequency & vibrato
					uint32_t notenum = _control_value[signal_set_index + 1] + vib + (_control_value[PITCH] - 256);
					float32_t freq = 440.0f * powf(2.0f, ((float32_t)notenum / 3072.0f) - 5.75f);

					//Convert frequency to phase
					float32_t delta_omega;
					audio_rad[0] = fmodf(audio_rad_chord[chord], SYNTH_DOUBLE_PI); //init
					float32_t temp = audio_rad[0];
					float32_t *p_rad = &audio_rad[1];

					if(notenum_prev[chord] != notenum) { //Smooth connection between previous and current frequencies
						float32_t prev_freq = 440.0f * powf(2.0f, ((float32_t)notenum_prev[chord] / 3072.0f) - 5.75f);
						float32_t delta_omega_slope = SYNTH_DOUBLE_PI * ((prev_freq - freq) / (float32_t)SYNTH_NUM_SAMPLING) / (float32_t)SYNTH_SAMPLE_FREQ;
						delta_omega = SYNTH_DOUBLE_PI * prev_freq / (float32_t)SYNTH_SAMPLE_FREQ;

						for(uint32_t t = 1; t < SYNTH_NUM_SAMPLING; t++){
							temp += delta_omega;
							delta_omega -= delta_omega_slope;
							if(SYNTH_DOUBLE_PI < temp){
								temp = fmodf(temp, SYNTH_DOUBLE_PI);
								//temp -= SYNTH_DOUBLE_PI; //noisy. why?
							}
							*p_rad++ = temp;
						}
					}else{
						delta_omega = SYNTH_DOUBLE_PI * freq / (float32_t)SYNTH_SAMPLE_FREQ;

						for(uint32_t t = 1; t < SYNTH_NUM_SAMPLING; t++){
							temp += delta_omega;
							if(SYNTH_DOUBLE_PI < temp){
								temp = fmodf(temp, SYNTH_DOUBLE_PI);
								//temp -= SYNTH_DOUBLE_PI; //noisy. why?
							}
							*p_rad++ = temp;
						}
					}
					notenum_prev[chord] = notenum;
					audio_rad_chord[chord] = audio_rad[SYNTH_NUM_SAMPLING - 1] + delta_omega;

					//create each wave
					if(_control_value[SYNC_WAVFORM] == 0){
						add_sin(audio_temp, audio_rad, (uint32_t) _control_value[signal_set_index + 2], SYNTH_NUM_SAMPLING);
						add_square(audio_temp, audio_rad, (uint32_t) _control_value[signal_set_index + 3], SYNTH_NUM_SAMPLING);
						add_sawtooth(audio_temp, audio_rad, (uint32_t) _control_value[signal_set_index + 4], SYNTH_NUM_SAMPLING);
						add_triangle(audio_temp, audio_rad, (uint32_t) _control_value[signal_set_index + 5], SYNTH_NUM_SAMPLING);
					}else{
						add_sin(audio_temp, audio_rad, (uint32_t) _control_value[S1_SIN], SYNTH_NUM_SAMPLING);
						add_square(audio_temp, audio_rad, (uint32_t) _control_value[S1_SQU], SYNTH_NUM_SAMPLING);
						add_sawtooth(audio_temp, audio_rad, (uint32_t) _control_value[S1_SAW], SYNTH_NUM_SAMPLING);
						add_triangle(audio_temp, audio_rad, (uint32_t) _control_value[S1_TRI], SYNTH_NUM_SAMPLING);
					}
					signal_set_index += 6;
				}else{
					add_noise(audio_temp, 256, SYNTH_NUM_SAMPLING);
					signal_set_index += 2;
				}

				arm_scale_f32(audio_temp, amp_chord, audio_temp, SYNTH_NUM_SAMPLING);
				arm_add_f32(audio_in + SYNTH_PRE_SAMPLE, audio_temp, audio_in + SYNTH_PRE_SAMPLE, SYNTH_NUM_SAMPLING);
			}
			//input filter
			arm_biquad_cascade_df2T_f32(&S_signal, audio_in, _audio_out, SYNTH_SAMPLE_ALL);
			//arm_copy_f32(audio_in, _audio_out, SYNTH_SAMPLE_ALL); //filter-less

			//Master volume & Tremolo
			arm_scale_f32((_audio_out + SYNTH_PRE_SAMPLE), tre * (_control_value[M_VOLUME] / 100.0f), (_audio_out + SYNTH_PRE_SAMPLE), SYNTH_NUM_SAMPLING);

			//pre-sample for filter (for next cycle)
			arm_copy_f32((audio_in + SYNTH_NUM_SAMPLING), audio_in, SYNTH_PRE_SAMPLE);

			//offset audio GND
			arm_offset_f32((_audio_out + SYNTH_PRE_SAMPLE), SYNTH_GND_LEVEL, (_audio_out + SYNTH_PRE_SAMPLE), SYNTH_NUM_SAMPLING);

			//Update buffer if half of audio device transmission is already done
			if(_audiodevice_tx_half_done == 1){
				_audiodevice_tx_half_done = 0;
				float2uint(_audio_out, _audio_dac, 0, SYNTH_HALF_NUM_SAMPLING);
			}

			//Processing Exit
			if(_status == SYNTH_STATUS_CHRD){
				_status = SYNTH_STATUS_WAIT;
			}
			break;
		}
		case SYNTH_STATUS_WAIT:
		default:
			ouput = 1;
			break;
	}
	return ouput;
}

void calc_lpf_coeffs(float32_t *_coeffs, float32_t _freq_cut, float32_t _q_factor) {
	float32_t omega_c = SYNTH_DOUBLE_PI * _freq_cut / (float32_t)SYNTH_SAMPLE_FREQ;
	float32_t alfa = arm_sin_f32(omega_c) / _q_factor;
	float32_t a0 = 1.0f + alfa;
	float32_t cos_omega_c = arm_cos_f32(omega_c);

	_coeffs[1] = (1.0f - cos_omega_c) / a0; //b1
	_coeffs[0] = _coeffs[1] / 2.0f; //b0
	_coeffs[2] = _coeffs[0]; //b2
	_coeffs[3] = (2.0f * cos_omega_c) / a0; //a1
	_coeffs[4] = 1.0f - (2.0f / a0); //a2
}

void calc_hpf_coeffs(float32_t *_coeffs, float32_t _freq_cut, float32_t _q_factor) {
	float32_t omega_c = SYNTH_DOUBLE_PI * _freq_cut / (float32_t)SYNTH_SAMPLE_FREQ;
	float32_t alfa = arm_sin_f32(omega_c) / _q_factor;
	float32_t a0 = 1.0f + alfa;
	float32_t cos_omega_c = arm_cos_f32(omega_c);

	_coeffs[1] = -(1.0f + cos_omega_c) / a0; //b1
	_coeffs[0] = -_coeffs[1] / 2.0f; //b0
	_coeffs[2] = _coeffs[0]; //b2
	_coeffs[3] = (2.0f * cos_omega_c) / a0; //a1
	_coeffs[4] = 1.0f - (2.0f / a0); //a2
}

void calc_amp_char(uint16_t *_amp, float32_t _freq_cut, float32_t _q_factor) {
	float32_t omega_c = SYNTH_DOUBLE_PI * (float32_t)_freq_cut;
	float32_t inv_q_sqare = 1.0f / (_q_factor * _q_factor);

	for (uint32_t i = 0; i < 240; i++) {
		float32_t omega = log_axis[i];
		float32_t omega_c_per = omega_c / omega;
		float32_t omega_c_per_square = omega_c_per * omega_c_per;
		float32_t sqrt_in, amp_temp;

		sqrt_in = omega_c_per_square + (1 / omega_c_per_square) + inv_q_sqare - 2.0f;
		arm_sqrt_f32(sqrt_in, &amp_temp);
		amp_temp = omega_c_per / amp_temp;
		amp_temp = -20.0f * log10f(amp_temp) + 20.0f;
		_amp[i] = (uint16_t) amp_temp;
	}
}

//------------
const float32_t log_axis[240] = { 125.663706143592, 129.348728894708,
		133.141813019254, 137.046127360741, 141.064933687363, 145.201589416971,
		149.459550421943, 153.842373916319, 158.353721427585, 162.997361855615,
		167.777174621311, 172.697152907572, 177.761406995305, 182.974167697266,
		188.339789892592, 193.862756164982, 199.547680547569, 205.399312377606,
		211.422540264186, 217.622396172319, 224.00405962677, 230.572862039168,
		237.334291162016, 244.293995673299, 251.457789895542, 258.831658653253,
		266.421762272802, 274.23444172892, 282.276223942122, 290.55382723147,
		299.074166927232, 307.844361148136, 316.871736748036, 326.16383543696,
		335.728420081655, 345.573481190894, 355.707243590959, 366.138173296881,
		376.874984585173, 387.926647273968, 399.302394216647, 411.011729015209,
		423.064433959833, 435.470578201262, 448.240526162845, 461.384946199247,
		474.914819509078, 488.841449308881, 503.17647027614, 517.931858269193,
		533.119940332195, 548.753404993447, 564.845312865737, 581.409107557524,
		598.458626904083, 616.008114528006, 634.072231738697, 652.666069780829,
		671.805162441964, 691.505499029896, 711.783537730546, 732.656219357569,
		754.140981505155, 776.255773115868, 799.019069475666, 822.449887648646,
		846.56780236441, 871.392962371313, 896.946107269263, 923.248584836137,
		950.322368862284, 978.190077508011, 1006.8749921994, 1036.40107707824,
		1066.79299902227, 1098.07614825262, 1130.27665954539, 1163.42143406542,
		1197.53816184017, 1232.65534489275, 1268.80232105327, 1306.0092884684,
		1344.30733082978, 1383.72844334208, 1424.30555945272, 1466.0725783653,
		1509.06439335992, 1553.31692094395, 1598.86713085763, 1645.7530769596,
		1694.01392901807, 1743.69000543432, 1794.82280692576, 1847.45505119672,
		1901.63070862596, 1957.39503900072, 2014.79462932791, 2073.87743275419,
		2134.69280862726, 2197.29156373198, 2261.72599473576, 2328.0499318785,
		2396.31878394389, 2466.58958454936, 2538.92103979343, 2613.37357730044,
		2690.00939670333, 2768.89252160687, 2850.08885307474, 2933.66622468498,
		3019.69445919998, 3108.24542689828, 3199.39310561694, 3293.21364255455,
		3389.78541788666, 3489.18911024665, 3591.50776412681, 3696.82685925595,
		3805.23438201139, 3916.82089892518, 4031.67963234573, 4149.90653831832,
		4271.60038674925, 4396.86284392094, 4525.79855742656, 4658.5152435955,
		4795.12377748239, 4935.73828549514, 5080.47624073912, 5229.45856115735,
		5382.80971054859, 5540.65780254767, 5703.13470765507, 5870.37616340502,
		6042.52188776427, 6219.71569585619, 6402.10562010778, 6589.84403391985,
		6783.08777896388, 6981.99829621169, 7186.74176080753, 7397.48922089525,
		7614.41674051642, 7837.705546699, 8067.54218085915, 8304.11865464295,
		8547.63261033799, 8798.28748598908, 9056.29268535572, 9321.86375285365,
		9595.22255362638, 9876.59745889721, 10166.2235367566, 10464.3427485446,
		10771.2041509912, 11087.0641042856, 11412.186486246, 11746.8429127701,
		12091.3129647506, 12445.8844216441, 12810.85350189, 13186.5251103792,
		13573.2130931798, 13971.2404997323, 14380.939852734, 14802.6534259371,
		15236.7335300933, 15683.5428072837, 16143.4545338796, 16616.8529323876,
		17104.1334924391, 17605.7033011931, 18121.9813834276, 18653.399051604,
		19200.400266197, 19763.4420065911, 20342.9946528535, 20939.5423787015,
		21553.583555995, 22185.6311710889, 22836.213253396, 23505.8733165167,
		24195.1708123047, 24904.6815982479, 25634.9984185554, 26386.7313993514,
		27160.5085583912, 27956.9763297236, 28776.8001037393, 29620.6647830559,
		30489.2753547035, 31383.3574790898, 32303.6580962362, 33250.9460497912,
		34226.012729343, 35229.6727315675, 36262.7645407638, 37326.1512293466,
		38420.7211788797, 39547.3888222538, 40707.0954076278, 41900.809784773,
		43129.5292144756, 44394.2802016752, 45696.1193530341, 47036.134259655,
		48415.4444056833, 49835.202103554, 51296.5934566645, 52800.8393502764,
		54349.1964714763, 55942.9583590447, 57583.4564841139, 59272.0613625136,
		61010.1836997366, 62799.2755694798, 64640.831626745, 66536.390356514,
		68487.5353590394, 70495.8966728266, 72563.1521364116, 74691.0287900718,
		76881.3043186414, 79135.8085366368, 81456.4249169323, 83845.0921642637,
		86303.8058348733, 88834.6200036517, 91439.6489801656, 94121.0690750094,
		96881.120417952, 99722.1088294016, 102646.407746749, 105656.460207202,
		108754.78088876, 111943.958211047, 115226.656497745, 118605.61820244,
		122083.666199741, 125663.706143586
};
