/**
 ******************************************************************************
 * @file    vario.c
 * @author  JK
 * @version V1.1
 * @date    06-April-2018
 * @brief   Tells you whether you are climbing or sinking.
 ******************************************************************************
 */
// ***** Includes *****
#include "vario.h"

//****** Variables ******
float temp_climb;

datafusion_T* p_ipc_v_df_data;
T_command vario_command;

//uint8_t  volu 	= 0;
uint16_t freque = 0;
uint8_t  peri 	= 0;
uint8_t state_sinktone = 2;
uint8_t state_0climbtone = 1;

//uint8_t vario_tone = 0;

// ***** Function Definitions *****

void vario_tone_climb(float climb);
void vario_tone_0climb(float climb);
void vario_tone_sink(float climb);
void vario_tone_mute();

// ***** Functions *****
/*
 * Register everything relevant for IPC
 */
void vario_register_ipc(void)
{
	// register ipc queue
	ipc_register_queue(20 * sizeof(T_command), did_VARIO);
};

/*
 * Get everything relevant for IPC
 */
void vario_get_ipc(void)
{
	// get the ipc pointer addresses for the needed data
	p_ipc_v_df_data = ipc_memory_get(did_DATAFUSION);
};


void vario_task(void)
{

	/*
Top:
peri = 20;
Sound = 2000;

Bottom:
peri = 100
sound = 500;

	 */

	// Handle Queue Commands
	T_command IPC_cmd;
	while(ipc_get_queue_bytes(did_VARIO))
	{
		ipc_queue_get(did_VARIO, 10, &IPC_cmd); 	// get new command
		switch(IPC_cmd.cmd)					// switch for pad number
		{
		case cmd_vario_set_sinktone:
			state_sinktone = 1;
			break;

		case cmd_vario_clear_sinktone:
			state_sinktone = 0;
			break;

		case cmd_vario_toggle_sinktone:
			state_sinktone++;
			if(state_sinktone > 3) state_sinktone = 0;

			//state_sinktone ^= 1;
			break;

		default:
			break;
		}
	}

	// Sound creation
	temp_climb = p_ipc_v_df_data->climbrate_filt;

	if(temp_climb > 5)
		temp_climb = 5;

	if(temp_climb < -4)
		temp_climb = -4;


	if((temp_climb > 0.2) && (state_sinktone > 0))								// Climbing
	{
		vario_tone_climb(temp_climb);
	}
	else if((temp_climb <= 0.2) && (temp_climb > -1) && (state_sinktone > 2))	// 0C limb
	{
		vario_tone_0climb(temp_climb);
	}
	else if((temp_climb <= -2) &&  (state_sinktone > 1)) 		// Huge Sink  --> Turn on Thermals
	{
		vario_tone_sink(temp_climb);
	}
	else
	{
		vario_tone_mute();
	}
}

void vario_tone_climb(float climb)
{
	// Set Unmute
	vario_command.cmd 	= cmd_sound_set_unmute;
	vario_command.data	= 0;

	ipc_queue_push(&vario_command, 10, did_SOUND);

	// Set Beep
	vario_command.cmd 	= cmd_sound_set_beep;
	vario_command.data	= 0;

	ipc_queue_push(&vario_command, 10, did_SOUND);


	// Set Frequency
	freque = (uint16_t)((temp_climb + frequ_srate_base_climb) * frequ_rate_climb + frequ_base_climb);
	//volu = 50;

	vario_command.cmd 	= cmd_sound_set_frequ;
	vario_command.data	= (uint32_t)freque;

	ipc_queue_push(&vario_command, 10, did_SOUND);


	// Set Period
	if(temp_climb < 1)
		temp_climb = 1;
	peri = (uint8_t)(100 - (temp_climb / 5 * 80));

	vario_command.cmd 	= cmd_sound_set_period;
	vario_command.data	= (uint32_t)peri;

	ipc_queue_push(&vario_command, 10, did_SOUND);
}


void vario_tone_0climb(float climb)
{
	// Set Unmute
	vario_command.cmd 	= cmd_sound_set_unmute;
	vario_command.data	= 0;

	ipc_queue_push(&vario_command, 10, did_SOUND);

	// Set Beep
	vario_command.cmd 	= cmd_sound_set_beep_short;
	vario_command.data	= 0;

	ipc_queue_push(&vario_command, 10, did_SOUND);


	// Set Frequency
	freque = (uint16_t)((temp_climb + frequ_srate_base_0climb) * frequ_rate_0climb + frequ_base_0climb);
	//volu = 50;

	vario_command.cmd 	= cmd_sound_set_frequ;
	vario_command.data	= (uint32_t)freque;

	ipc_queue_push(&vario_command, 10, did_SOUND);


	// Set Period
	if(temp_climb < 1)
		temp_climb = 1;
	peri = (uint8_t)(100 - (temp_climb / 5 * 80));

	vario_command.cmd 	= cmd_sound_set_period;
	vario_command.data	= (uint32_t)peri;

	ipc_queue_push(&vario_command, 10, did_SOUND);
}

void vario_tone_sink(float climb)
{
	// Set Unmute
	vario_command.cmd 	= cmd_sound_set_unmute;
	vario_command.data	= 0;

	ipc_queue_push(&vario_command, 10, did_SOUND);

	// Set Continous
	vario_command.cmd 	= cmd_sound_set_cont;
	vario_command.data	= 0;

	ipc_queue_push(&vario_command, 10, did_SOUND);


	// Set Frequency
	freque = (uint16_t)((temp_climb + frequ_base_0climb) * frequ_rate_sink + frequ_base_sink);
	//volu = 100;

	vario_command.cmd 	= cmd_sound_set_frequ;
	vario_command.data	= (uint32_t)freque;

	ipc_queue_push(&vario_command, 10, did_SOUND);
}

void vario_tone_mute()
{
	vario_command.cmd 	= cmd_sound_set_mute;
	vario_command.data	= 0;
	ipc_queue_push(&vario_command, 10, did_SOUND);
}


// Function to calculate an estimate for the distance between two points
float vario_wgs84_distance(float lat1, float lon1, float lat2, float lon2)
{
	float dist = 0;

	float u_1 = 2 * pi * r_earth * arm_cos_f32(lat1 / 360 * pi);				// earth radius on lat1
	float u_2 = 2 * pi * r_earth * arm_cos_f32(lat2 / 360 * pi);				// earth radius on lat2

	float d_lon_12 = (lon1 - lon2) / 360 * (u_1 + u_2) / 2;			// lon distance between p1 and p2
	float d_lat_12 = 2 * pi * r_earth * (lat1 - lat2) / 360;		// lat distance between p1 and p2


	arm_sqrt_f32((d_lon_12 * d_lon_12 + d_lat_12 * d_lat_12),&dist);	// pythagoras between these distances

	return dist; //[m]
}

