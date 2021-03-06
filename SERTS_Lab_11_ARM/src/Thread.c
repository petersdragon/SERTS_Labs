
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

#define BYTE 8

//void Thread_1 (void const *arg);                           // function prototype for Thread_1
//osThreadDef (Thread_1, osPriorityNormal, 1, 0);            // define Thread_1
void FS (void const *arg);                           // function prototype for Thread_1
osThreadDef (FS, osPriorityNormal, 1, 0);            // define Thread_1

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

//  id = osThreadCreate (osThread (Thread_1), NULL);         // create the thread
  id = osThreadCreate (osThread (FS), NULL);         // create the thread
  if (id == NULL) {                                        // handle thread creation
    // Failed to create a thread
  };
}

// WAVE file header format
typedef struct WAVHEADER {
	unsigned char riff[4];						// RIFF string
	uint32_t overall_size;				// overall size of file in bytes
	unsigned char wave[4];						// WAVE string
	unsigned char fmt_chunk_marker[4];		// fmt string with trailing null char
	uint32_t length_of_fmt;					// length of the format data
	uint16_t format_type;					// format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
	uint16_t channels;						// no.of channels
	uint32_t sample_rate;					// sampling rate (blocks per second)
	uint32_t byterate;						// SampleRate * NumChannels * BitsPerSample/8
	uint16_t block_align;					// NumChannels * BitsPerSample/8
	uint16_t bits_per_sample;				// bits per sample, 8- 8bits, 16- 16 bits etc
	unsigned char data_chunk_header [4];		// DATA string or FLLR string
	uint32_t data_size;						// NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
} WAVHEADER;

// pointer to file type for files on USB device
FILE *f;

void FS (void const *argument) {
	usbStatus ustatus; // USB driver status variable
	uint8_t drivenum = 0; // Using U0: drive number
	char *drive_name = "U0:"; // USB drive name
	fsStatus fstatus; // file system status variable
	WAVHEADER header;
	size_t rd;
	uint32_t i; // loop counter
	static uint8_t rtrn = 0;  // return variable
	uint8_t rdnum = 1; // read buffer number
	float32_t Fs;

	ustatus = USBH_Initialize (drivenum); // initialize the USB Host
	if (ustatus == usbOK){
		// loop until the device is OK, may be delay from Initialize
		ustatus = USBH_Device_GetStatus (drivenum); // get the status of the USB device
		while(ustatus != usbOK){
			ustatus = USBH_Device_GetStatus (drivenum); // get the status of the USB device
		}
		// initialize the drive
		fstatus = finit (drive_name);
		if (fstatus != fsOK){
			// handle the error, finit didn't work
		} // end if
		// Mount the drive
		fstatus = fmount (drive_name);
		if (fstatus != fsOK){
			// handle the error, fmount didn't work
		} // end if
		// file system and drive are good to go

		f = fopen ("Test.wav","r");// open a file on the USB device
		if (f != NULL) {
			fread((void *)&header, sizeof(header), 1, f);

			// initialize the audio output
			rtrn = BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, 0x46, header.sample_rate);
			if (rtrn != AUDIO_OK)return;

			if (fread(&Audio_Buffer, header.bits_per_sample/BYTE, BUF_LEN, f) > 0){// read data from file into Audio_Buffer while not end of file
				osMessagePut (buffer_MsgQueue, 1, osWaitForever); // Send Message
			}

			// Start the audio player, size is number of bytes so mult. by 2
			BSP_AUDIO_OUT_Play((uint16_t *)Audio_Buffer, BUF_LEN*2);

			while(fread(&Audio_Buffer2, header.bits_per_sample/BYTE, BUF_LEN, f) > 0){ // read data from file into Audio_Buffer2 while not end of file
				osMessagePut (buffer_MsgQueue, 2, osWaitForever); // Send Message
				osSemaphoreWait(my_semaphore_id, osWaitForever);

				if (fread(&Audio_Buffer, header.bits_per_sample/BYTE, BUF_LEN, f) > 0){// read data from file into Audio_Buffer while not end of file
					osMessagePut (buffer_MsgQueue, 1, osWaitForever); // Send Message
					osSemaphoreWait(my_semaphore_id, osWaitForever);
				}
			}
			BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_ON);
			fclose (f); // close the file
		} // end if file opened
	} // end if USBH_Initialize

} // end Thread

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


