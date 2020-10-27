
#include "cmsis_os.h"  // CMSIS RTOS header file
#include "Board_LED.h"
#include "UART_driver.h"
#include "stdint.h"                     // data type definitions
#include "stdio.h"                      // file I/O functions
#include "rl_usb.h"                     // Keil.MDK-Pro::USB:CORE
#include "rl_fs.h"                      // Keil.MDK-Pro::File System:CORE
#include "stm32f4xx_hal.h"
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_audio.h"
#include "math.h"
#include "arm_math.h" // header for DSP library
#include <stdio.h>

// LED constants
#define LED_Green   0
#define LED_Orange  1
#define LED_Red     2
#define LED_Blue    3

void Thread_1 (void const *arg);                           // function prototype for Thread_1
osThreadDef (Thread_1, osPriorityNormal, 1, 0);            // define Thread_1

#define NUM_CHAN	2 // number of audio channels
#define NUM_POINTS 1024 // number of points per channel
#define BUF_LEN NUM_CHAN*NUM_POINTS // length of the audio buffer
/* buffer used for audio play */
int16_t Audio_Buffer[BUF_LEN];
int16_t Audio_Buffer2[BUF_LEN];

osSemaphoreDef (my_semaphore);    // Declare semaphore
osSemaphoreId  (my_semaphore_id); // Semaphore ID

#define MSGQUEUE_OBJECTS    1                 // number of Message Queue Objects

osMessageQId buffer_MsgQueue;                     // message queue id
osMessageQDef (buffer_MsgQueue, MSGQUEUE_OBJECTS, int32_t);    // message queue object

void Init_Thread (void) {

	osThreadId id; // holds the returned thread create ID
	
	LED_Initialize(); // Initialize the LEDs
	UART_Init(); // Initialize the UART
	my_semaphore_id = osSemaphoreCreate(osSemaphore(my_semaphore), 0);  // Create semaphore with 0 tokens
	buffer_MsgQueue = osMessageCreate (osMessageQ(buffer_MsgQueue), NULL);  // create msg queue

  id = osThreadCreate (osThread (Thread_1), NULL);         // create the thread
  if (id == NULL) {                                        // handle thread creation
    // Failed to create a thread
  };
}


// Thread function
void Thread_1 (void const *argument) {
	static uint8_t rtrn = 0;  // return variable
	float32_t Fs = 8000.0; // sample frequency
	float32_t freq = 200.0;//250.0; // signal frequency in Hz
	int16_t i; // loop counter
	float32_t tmp; // factor
	float32_t cnt = 0.0f; // time index

	tmp = 6.28f*freq/Fs; // only calc this factor once

	// initialize the audio output
	rtrn = BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, 0x46, 8000);
	if (rtrn != AUDIO_OK)return;

	// generate data for the audio buffer
 	for(i=0;i<NUM_POINTS;i++){
		Audio_Buffer[2*i] = arm_cos_f32(tmp*cnt)*32000.0f; // Left channel
		Audio_Buffer[2*i+1] = Audio_Buffer[2*i]; // Right channel
		cnt = cnt + 1.0f;
	}
	// Start the audio player, size is number of bytes so mult. by 2
  BSP_AUDIO_OUT_Play((uint16_t *)Audio_Buffer, BUF_LEN*2);

  while(1){
		// generate data for the audio buffer
	 	for(i=0;i<NUM_POINTS;i++){
			Audio_Buffer2[2*i] = arm_cos_f32(tmp*cnt)*32000.0f; // Left channel
			Audio_Buffer2[2*i+1] = Audio_Buffer2[2*i]; // Right channel
			cnt = cnt + 1.0f;
		}
		osMessagePut (buffer_MsgQueue, 2, osWaitForever); // Send Message
	 	osSemaphoreWait(my_semaphore_id, osWaitForever);

	 	// generate data for the audio buffer
	 	for(i=0;i<NUM_POINTS;i++){
			Audio_Buffer[2*i] = arm_cos_f32(tmp*cnt)*32000.0f; // Left channel
			Audio_Buffer[2*i+1] = Audio_Buffer[2*i]; // Right channel
			cnt = cnt + 1.0f;
		}
	 	osMessagePut (buffer_MsgQueue, 1, osWaitForever); // Send Message
	 	osSemaphoreWait(my_semaphore_id, osWaitForever);

  }
}

/* User Callbacks: user has to implement these functions if they are needed. */
/* This function is called when the requested data has been completely transferred. */
void    BSP_AUDIO_OUT_TransferComplete_CallBack(void){
    osEvent evt = osMessageGet (buffer_MsgQueue, 0);        // wait for message
    if (evt.status == osEventMessage){
    	osSemaphoreRelease(my_semaphore_id);

		if (evt.value.v == 1)
			BSP_AUDIO_OUT_ChangeBuffer((uint16_t*)Audio_Buffer, BUF_LEN);
		else
			BSP_AUDIO_OUT_ChangeBuffer((uint16_t*)Audio_Buffer2, BUF_LEN);
	}
}

/* This function is called when half of the requested buffer has been transferred. */
void    BSP_AUDIO_OUT_HalfTransfer_CallBack(void){
}

/* This function is called when an Interrupt due to transfer error or peripheral
   error occurs. */
void    BSP_AUDIO_OUT_Error_CallBack(void){
		while(1){
		}
}


