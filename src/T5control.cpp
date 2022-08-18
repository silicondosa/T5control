// T5control.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <quickDAQ.h>
#include "..\include\T5control.h"

extern unsigned muscle_mtr_val[16][2];
extern unsigned muscle_mtr_en [16][2];
extern unsigned muscle_enc_org[ 8][2];
extern unsigned muscle_enc_alt[ 8][2];
extern unsigned muscle_enc_mtr[12][2];
extern unsigned muscle_enc_spl[12][2];
extern unsigned muscle_ld_cell[ 8][2];
extern int T5_quickDAQstatus;

muscle::muscle(unsigned muscleChannel, const encoderArrangement myEncoderArrangement = MUSCLE_MODULE)
{
	// Initialize muscle object
	unsigned channelID	= muscleChannel;
	this->myEncoderArrangement		= myEncoderArrangement;

	// Initialize quickDAQ library
	if (T5_quickDAQstatus == 0) {
		quickDAQinit();
		T5_quickDAQstatus = 1;
	}
	
	// Set pin modes based on channel ID and encoder arrangement
	int encChannelLimit = 0;
	int mtrChannelLimit = 0;
	encoder_opt_config = NULL;
	switch (this->myEncoderArrangement)
	{
	case RING_OF_FIRE:
		encChannelLimit = 8;
		mtrChannelLimit = 8;
		encoder_config = muscle_enc_org;
		break;

	case QUADRUPED:
		encChannelLimit = 8;
		mtrChannelLimit = 12;
		encoder_config = muscle_enc_alt;
		break;

	case MUSCLE_MODULE:
		encChannelLimit = 12;
		mtrChannelLimit = 12;
		encoder_config = muscle_enc_mtr;
		encoder_opt_config = muscle_enc_spl;
		break;
	default:
		fprintf(ERRSTREAM, "ERROR: T5 Control: Invalid encoder configuration/arrangement selected. Program will terminate.");
		quickDAQTerminate();
		exit(-1);
		break;
	}

	if (channelID < mtrChannelLimit && this->myEncoderArrangement != QUADRUPED) {
		pinMode((*motor_value_config)[0], ANALOG_OUT, (*motor_value_config)[1]); // MOTOR VALUE OUTPUT

		if ((*motor_enable_config)[0] == 2) {
			if ((*motor_enable_config)[1] < 8) {
				pinMode((*motor_enable_config)[0], DIGITAL_OUT, 0); // MOTOR ENABLE OUTPUT
			}
			else if ((*motor_enable_config)[1] > 15 && (*motor_enable_config)[1] < 24) {
				pinMode((*motor_enable_config)[1], ANALOG_OUT, 0); // MOTOR ENABLE OUTPUT
			}
		}


		pinMode((*loadcell_config)[0], ANALOG_IN, (*loadcell_config)[1]); // LOADCELL INPUT

		// ENCODER INPUT
	}
	// Setup DAQ sampling rate and trigger mode
}
