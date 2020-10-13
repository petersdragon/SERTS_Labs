
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
enum commands{
  ListFiles,
  SendComplete,
  SendFiles
};

// State Machine definitions
enum state{
  NoState,
  Idle,
  List,
};

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

void Process_Event(uint16_t event) {
	static uint16_t Current_State = NoState; // Current state of the SM
	switch(Current_State){
	case NoState:
		// This is an initialization state
		// so the system will go into the initial state
		// Next State
		Current_State = Idle;
		// NoState Exit actions
		// Transition actions
		// Idle state entry actions
		LED_On(LED_Red);
		break;

	case Idle:
		if(event == ListFiles){
			// Next State
			Current_State = List;
			// Idle state Exit actions
			LED_Off(LED_Red);
			// Transition actions
			// List state entry actions
			LED_On(LED_Blue);
			osMessagePut (mid_FSQueue, SendFiles, osWaitForever);
		}
		break;

	case List:
		// Next State
		Current_State = Idle;
		// List state Exit actions
		LED_Off(LED_Blue);
		// Transition actions
		// Idle state entry actions
		LED_On(LED_Red);
		break;
	}
}


void Init_Thread (void) {

	osThreadId id; // holds the returned thread create ID
	
	LED_Initialize(); // Initialize the LEDs
	UART_Init(); // Initialize the UART

	// Create queues
	mid_CMDQueue = osMessageCreate (osMessageQ(CMDQueue), NULL);  // create msg queue
	if (!mid_CMDQueue)return; // Message Queue object not created, handle failure
	mid_FSQueue = osMessageCreate (osMessageQ(FSQueue), NULL);  // create msg queue
	if (!mid_FSQueue)return; // Message Queue object not created, handle failure

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
         osMessagePut (mid_CMDQueue, ListFiles, osWaitForever);
      }

   }
} // end Rx_Command

// Thread function
void FS_Thread(void const *arg){

	osEvent evt; // Receive message object
	usbStatus ustatus; // USB driver status variable
	uint8_t drivenum = 0; // Using U0: drive number
	char *drive_name = "U0:"; // USB drive name
	fsStatus fstatus; // file system status variable
	fsFileInfo drive_info;
	drive_info.fileID = 0;

	LED_On(LED_Green);
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

	   while(1){
		   evt = osMessageGet (mid_FSQueue, osWaitForever); // receive command
		   if (evt.status == osEventMessage) { // check for valid message
			   while (ffind("U0:*.wav", &drive_info) == fsOK) {
					UART_send(drive_info.name, strlen(drive_info.name));
					UART_send("\n\r", 2);
			   }
			   osMessagePut (mid_CMDQueue, SendComplete, osWaitForever);
		   }
	   }

	} // end if USBH_Initialize
	LED_Off(LED_Green);

}


