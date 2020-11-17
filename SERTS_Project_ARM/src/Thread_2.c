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

#define Show_Files_char "1"
#define Read_Files_char "4"
#define Stop_Song_char "8"
#define Song_State_Change_char "9"

#define MAX 256
char fileName[MAX];

// pointer to file type for files on USB device
FILE *f;

// Buffer Declarations
#define NUM_CHAN	2 // number of audio channels
#define NUM_POINTS 1024 // number of points per channel
#define BUF_LEN NUM_CHAN*NUM_POINTS // length of the audio buffer
int16_t Audio_Buffer1[BUF_LEN];
int16_t Audio_Buffer2[BUF_LEN];

enum commands{
	SendComplete,
	SendFiles,
	StopSong,
	SongStateChange,
	PlaySong,
	ResumeSong,
	PauseSong
};

// State Machine definitions
enum state{
	NoState,
	Stopped,
	Playing,
	Paused
};

// WAVE file header format
typedef struct WAVHEADER {
	unsigned char riff[4];					// RIFF string
	uint32_t overall_size;					// overall size of file in bytes
	unsigned char wave[4];					// WAVE string
	unsigned char fmt_chunk_marker[4];		// fmt string with trailing null char
	uint32_t length_of_fmt;					// length of the format data
	uint16_t format_type;					// format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
	uint16_t channels;						// no.of channels
	uint32_t sample_rate;					// sampling rate (blocks per second)
	uint32_t byterate;						// SampleRate * NumChannels * BitsPerSample/8
	uint16_t block_align;					// NumChannels * BitsPerSample/8
	uint16_t bits_per_sample;				// bits per sample, 8- 8bits, 16- 16 bits etc
	unsigned char data_chunk_header [4];	// DATA string or FLLR string
	uint32_t data_size;						// NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
} WAVHEADER;

void Control (void const *argument); // thread function
osThreadId tid_Control; // thread id
osThreadDef (Control, osPriorityNormal, 1, 0); // thread object

// UART receive thread
void Rx_Command (void const *argument);  // thread function
osThreadId tid_RX_Command;  // thread id
osThreadDef (Rx_Command, osPriorityNormal, 1, 0); // thread object

void FS_Thread (void const *arg);                           // function prototype for FS_Thread
osThreadDef (FS_Thread, osPriorityNormal, 1, 0);            // define FS_Thread

// Command queue from Rx_Command to Controller
osMessageQId mid_CMDQueue; // message queue for commands to Thread
osMessageQDef (CMDQueue, 1, uint32_t); // message queue object

// Command queue from Controller to FS thread
osMessageQId mid_FSQueue; // message queue for commands to Thread
osMessageQDef (FSQueue, 1, uint32_t); // message queue object

osMessageQId buffer_MsgQueue; // message queue for commands to Thread
osMessageQDef (buffer_MsgQueue, 1, uint32_t); // message queue object

osSemaphoreDef(SEM0);
osSemaphoreId(SEM_id);

void Process_Event(uint16_t event) {
	static uint16_t Current_State = NoState; // Current state of the SM
	switch(Current_State){
	case NoState:
		// This is an initialization state
		// so the system will go into the initial state
		// Next State
		Current_State = Stopped;
		// Exit actions
		// Transition actions
		// entry actions
		LED_On(LED_Red);
		break;

	case Stopped:
		if(event == SendFiles){
			// Next State
			// Exit actions
			// Transition actions
			osMessagePut (mid_FSQueue, SendFiles, osWaitForever);
			// entry actions
		}
		if(event == SongStateChange){
			// Next State
			Current_State = Playing;
			// Exit actions
			LED_Off(LED_Red);
			// Transition actions
			// entry actions
			LED_On(LED_Green);
			osMessagePut (mid_FSQueue, PlaySong, osWaitForever);
		}
		break;

	case Playing:
		if(event == SendFiles){
			// Next State
			// Exit actions
			// Transition actions
			osMessagePut (mid_FSQueue, SendFiles, osWaitForever);
			// entry actions
		}
		if(event == SongStateChange){
			// Next State
			Current_State = Paused;
			// Exit actions
			LED_Off(LED_Green);
			// Transition actions
			// entry actions
			LED_On(LED_Blue);
			osMessagePut (mid_FSQueue, PauseSong, osWaitForever);

		}
		if(event == StopSong){
			// Next State
			Current_State = Stopped;
			// Exit actions
			LED_Off(LED_Green);
			// Transition actions
			// entry actions
			LED_On(LED_Red);
			osMessagePut (mid_FSQueue, StopSong, osWaitForever);
		}
		break;
	case Paused:
		if(event == SendFiles){
			// Next State
			// Exit actions
			// Transition actions
			osMessagePut (mid_FSQueue, SendFiles, osWaitForever);
			// entry actions
		}
		if(event == SongStateChange){
			// Next State
			Current_State = Playing;
			// Exit actions
			LED_Off(LED_Blue);
			// Transition actions
			// entry actions
			LED_On(LED_Green);
			osMessagePut (mid_FSQueue, ResumeSong, osWaitForever);
		}
		if(event == StopSong){
			// Next State
			Current_State = Stopped;
			// Exit actions
			LED_Off(LED_Blue);
			// Transition actions
			// entry actions
			LED_On(LED_Red);
		}
		break;
	default:
		break;
	}
}

void Init_Thread (void) {

	osThreadId id; // holds the returned thread create ID
	
	LED_Initialize(); // Initialize the LEDs
	UART_Init(); // Initialize the UART

	SEM_id = osSemaphoreCreate(osSemaphore(SEM0),0);

	// Create queues
	mid_CMDQueue = osMessageCreate (osMessageQ(CMDQueue), NULL);  // create msg queue
	if (!mid_CMDQueue)return; // Message Queue object not created, handle failure
	mid_FSQueue = osMessageCreate (osMessageQ(FSQueue), NULL);  // create msg queue
	if (!mid_FSQueue)return; // Message Queue object not created, handle failure
	buffer_MsgQueue = osMessageCreate (osMessageQ(buffer_MsgQueue), NULL);  // create msg queue
	if (!buffer_MsgQueue)return; // Message Queue object not created, handle failure

	// Create threads
	tid_RX_Command = osThreadCreate (osThread(Rx_Command), NULL);
	if (!tid_RX_Command) return;
	tid_Control = osThreadCreate (osThread(Control), NULL);
	if (!tid_Control) return;

	id = osThreadCreate (osThread (FS_Thread), NULL);         // create the thread
	if (id == NULL) {                                        // handle thread creation
		// Failed to create a thread
	};
}

void Control(void const *arg){
  osEvent evt; // Receive message object
  Process_Event(0); // Initialize the State Machine
   while(1){
	   evt = osMessageGet (mid_CMDQueue, osWaitForever); // receive command
	   if (evt.status == osEventMessage) { // check for valid message
		   Process_Event(evt.value.v); // Process event
	   }
   }
}

void Rx_Command (void const *argument){
   char rx_char[2]={0,0};
   while(1){
      UART_receive(rx_char, 1); // Wait for command from PC GUI
      // Check for the type of character received
      if(!strcmp(rx_char, Show_Files_char)){
         // Show_Files received
         osMessagePut (mid_CMDQueue, SendFiles, osWaitForever);
      }
      else if(!strcmp(rx_char, Read_Files_char)){
    	  UART_receivestring(fileName, MAX); // Read file name from GUI
    	  int i = 0;
      }
      else if(!strcmp(rx_char, Stop_Song_char)){
    	  // Send Stop Command to State Machine
          osMessagePut (mid_CMDQueue, StopSong, osWaitForever);
      }
      else if(!strcmp(rx_char, Song_State_Change_char)){
    	  // Send Song_State_Change Command to State Machine
    	  osMessagePut (mid_CMDQueue, SongStateChange, osWaitForever);
      }

   }
} // end Rx_Command

// Thread function
void FS_Thread(void const *arg){

	usbStatus ustatus; // USB driver status variable
	uint8_t drivenum = 0; // Using U0: drive number
	char *drive_name = "U0:"; // USB drive name
	fsStatus fstatus; // file system status variable

	WAVHEADER header;
	size_t rd;
	static uint8_t rtrn = 0; // return variable
	float32_t Fs; // sample frequency

	fsFileInfo drive_info;
	drive_info.fileID = 0;
	osEvent evt; // Receive message object

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
			return;	// handle the error, finit didn't work
		} // end if
		// Mount the drive
		fstatus = fmount (drive_name);
		if (fstatus != fsOK){
			return;	// handle the error, fmount didn't work
		} // end if
		// file system and drive are good to go
		rtrn = BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, 0x46, 44100);

		while(1){
			evt = osMessageGet (mid_FSQueue, osWaitForever); // receive command
			if (evt.status == osEventMessage) { // check for valid message

				switch(evt.value.v){
					case SendFiles:
						while (ffind("U0:*.wav", &drive_info) == fsOK) {
							UART_send(drive_info.name, strlen(drive_info.name));
							UART_send("\n", 1);
						}
						osMessagePut (mid_CMDQueue, SendComplete, osWaitForever);
						break;
					case PlaySong:
						f = fopen(fileName, "r");
						if(f != NULL){
							fread((void *)&header, sizeof(header), 1, f);
							if (rtrn != AUDIO_OK)return;

							if(fread(&Audio_Buffer1, sizeof(uint16_t), BUF_LEN, f) > 0){
								osMessagePut (buffer_MsgQueue, 2, osWaitForever);
							}

							// Start the audio player, size is number of bytes so mult. by 2
							BSP_AUDIO_OUT_Play((uint16_t *)Audio_Buffer1, BUF_LEN*2);
						} // @suppress("No break at end of case")
					case ResumeSong:
						if(evt.value.v == ResumeSong){
							BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_OFF);
							//The audio buffer has data when the song is paused, why does it not play?
							BSP_AUDIO_OUT_ChangeBuffer((uint16_t*)Audio_Buffer1, BUF_LEN);
						}
						bool keepPlaying = true;
						//The problem is that after this thing resumes, it does not go ba into the loop
						while(fread(&Audio_Buffer2, sizeof(uint16_t), BUF_LEN, f) > 0 & keepPlaying){ // read data from file into Audio_Buffer2 while not end of file
							osMessagePut (buffer_MsgQueue, 2, osWaitForever); // Send Message
							osSemaphoreWait(SEM_id, osWaitForever);

							if (fread(&Audio_Buffer1, sizeof(uint16_t), BUF_LEN, f) > 0){// read data from file into Audio_Buffer while not end of file
								osMessagePut (buffer_MsgQueue, 1, osWaitForever); // Send Message
								osSemaphoreWait(SEM_id, osWaitForever);
							}
							evt = osMessageGet (mid_FSQueue, 10); // receive command
							if (evt.status == osEventMessage){
								if (evt.value.v == PauseSong){
									BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_ON);
									break; // This is not good programming but works
								}
								else if (evt.value.v == StopSong){
									keepPlaying = false;
								}
							}
						}
						BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_ON);
						//fclose(f);
						break;
					case PauseSong:
						BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_ON);
						break;
					case StopSong:
						BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_ON);
						fclose(f);
						break;
				}
			}
		}// end while(1)
	} // end if USBH_Initialize
}

/* User Callbacks: user has to implement these functions if they are needed. */
/* This function is called when the requested data has been completely transferred. */
void    BSP_AUDIO_OUT_TransferComplete_CallBack(void){
	uint16_t msg;
		msg = osMessageGet (buffer_MsgQueue, 0).value.v;
		if(msg == 2){
			BSP_AUDIO_OUT_ChangeBuffer((uint16_t*)Audio_Buffer2, BUF_LEN);
		}
		else if(msg == 1){
			BSP_AUDIO_OUT_ChangeBuffer((uint16_t*)Audio_Buffer1, BUF_LEN);
		}
		osSemaphoreRelease(SEM_id);
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
