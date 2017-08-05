#ifndef __OpenViBEApplication_communication_H__
#define __OpenViBEApplication_communication_H__
/*
		FILENAME: "st_Communication.h".
		DESCRIPTION: Header for communication with a Staubli controller (CS8C) through a TCP/IP
		client port. The server port must be configurated and opened in the CS8C controller.
		AUTHOR: Pablo José Gijón Garzás.
		START DATE: Fall of december 2016.
		NOTE: Most of the functions returns 0 when succes and an Error code when a failure happens. 
*/
#include <stdint.h>
//Librery winsock for communication using TCP/IP protocol
#include <Winsock2.h>
#include <iostream>
//Standar definitions, and data types.
#include "st_Common.h"

#define ST_ERR_ID_MESSAGE			3000

//WORKING MODE MOVEMENT Flags
#define ST_AXIS_SINGLE				0
#define ST_AXIS_MULTIPLE			4
#define ST_JOINT_SINGLE				8
#define ST_JOINT_MULTIPLE			12
#define ST_NEW_MOVE					16
#define ST_GO_HOME					32
#define ST_ERASE_QUEUE				64

#ifdef __GNUC__
#define PACK(class_def) class_def __attribute__((__packed__,__gcc_struct__))
#else
#define PACK(class_def) __pragma( pack(push, 1) ) class_def __pragma( pack(pop) )
#endif

namespace communication
{
	class STAUBLI
	{
	public:

		//************************************************************INIT AND CLOSE COMUNICATION WITH THE CONTROLLLER**********************************************************************************
		/*
		init_remote_control(..): Starts the comunication with the simulator or the real robot. Simulator: ("127.0.0.1") Real ip: Check it in the control panel.
		Example of use for simulator (localhost CS8C emulator):
			init_remote_control("127.0.0.1");
		*/
		int init_remote_control(const char* ip,int port);
		//end_remote_control(..): It ends the remote control. It can be called at any time, and it'll stop the robot and erased all queued actions. No inputs required.
		int end_remote_control();

		//************************************************************BASIC CONFIGURATION FUNCTIONS*****************************************************************************************************
		//enable_movement(..):	Enable (TRUE) or disable (FALSE) the movement of the robot. It can be called at any moment. If disabled when moving, the robot stops and the movement's queue is erased.
		int enable_movement(bool inp);

		/*	configure_move_increment(..):	Function in order to stablish the increment of movement for a SINGLE axis or joint movement. This is only valid when working in RELATIVE MODE. See next fn().
			Default value of (1.0 mm / 1°) is configurated in the Staubli program. 
			Example of use 1 - Single Axis, ASYNC: The user wants to move 10mm in the positive direction of axis X.
				Set the system reference:	configure_system_reference(ST_RELATIVE)
				Configure async movement:	configure_working_mode(ST_ASYNC)
				Configure increment to 1mm: configure_move_increment(1)
				Configure type of movement:	set_move_single_axis(ST_AXIS_X, ST_POSITIVE_MOVE)
				Call 10 times move_arm_inc()
			Example of use 2: The same movement but with just one call of move_arm_inc(). That can be done by setting the increment to 100mm.
			Example of use 3 - Single Joint, SYNC: Moving joint1 -4'32° and after that joint2 12'28° (relative movement to the actual position).
				(..)
				Set the increment movement, let's say we want to make it in 1 time:	configure_move_increment(4.32);
				Configure type of movement: set_move_single_joint(ST_JOINT_1, ST_NEGATIVE_MOVE)
				Configure sync move: configure_working_mode(ST_SYNC)
				Move the arm:	move_arm_inc();
				wait until it has reached the position:
				do{
					is_arm_busy(&bsy)
				}while(bsy)
				Configure second move:	set_move_single_joint(ST_JOINT_2, ST_POSITIVE_MOVE); configure_move_increment(12.28);
				Move the arm:	move_arm_inc();
				wait until it has reached the position:
				do{
				is_arm_busy(&bsy)
				}while(bsy)
		
		*/
		int configure_move_increment(int inc);
		/*	configure_system_reference(..): Reference mode in order to set the movement reference system to RELATIVE or ABSOLUTE. It can only be changed when the robot is not busy.
			RELATIVE MODE-> Movements can be done single/multiple axis and simple/multiple joint. ref = ST_RELATIVE
			ABSOLUTE MODE-> Movements can only be done with multiple axis/joint functions.
			This is only valid for multiple axis or multiple joint movement. ref: ST_ABSOLUTE, ST_RELATIVE.
			This can only be changed when the robot movement is disabled. 
		*/
		int configure_system_reference(int ref);
		//configure_working_mode(..): Function in order to config the movement mode. Async (TRUE) or Sync (FALSE) movement. It can be changed only when the corntroller is not busy.
		int configure_working_mode(bool mode);
		//Function in order to config the speed according to the basic speed programmed withing the staubli program. Input (0 to 100)%
		int set_speed(int speed);

		//*******************************************************************CONFIGURATION OF MOVEMENT FUNCTIONS****************************************************************************************
		//Single axis movement. Input values. Axis: ST_AXIS_X, ST_AXIS_Y, ST_AXIS_Z, ST_AXIS_RX, ST_AXIS_RY, ST_AXIS_RZ. Direction: ST_POSITIVE_MOV, ST_NEGATIVE_MOV
		int set_move_single_axis(int axis, int direction);
		//Multiple axis movement. Input vector of movement. i.e; v_mov = (130, 325, -247, 0, 0, 0) where v_mov € (x, y, z, Rx, Ry, Rz). 1 unit input == 0.01mm.
		//Note: Rx Ry Rz should be a valid conbination. i.e. (-1, 1, 0)
		int set_move_multiple_axis(point_pos v_mov);
		//Single joint movement. Joint: <ST_JOINT_1, ST_JOINT_2, ST_JOINT_3, ST_JOINT_4, ST_JOINT_5, ST_JOINT_6> and direction: <ST_POSITIVE_MOVE, ST_NEGATIVE_MOVE>.
		int set_move_single_joint(int joint, int direction);
		//Multiple joint movement. Input vector of movement. i.e; v_mov = (-3000, 1200, -1500, 0, 300, -9000). This is equivalent to a relative movement of (-30, 12, -15, 0, 3, -90)degrees.
		int set_move_multiple_joint(joint_pos v_mov);
		
		//************************************************************************MOVEMENT FUNCTIONS****************************************************************************************************
		//Go home function (joint position defined within the staubli program in CS8C). Starts the movement. Return 0 on success. This function has priority, so it'll remove all queued ones.
		//Non blocking function, but controller won't allow any input movements until it has reached the home position. Check with is_arm_busy(..)
		int go_home();

		//move_arm(); sends a pulse in order to move into a relative/absolute position according to the config. movement (single axis/joint or multiple axis/joint) and system reference.
		//Note 1: If sync movement is configured, a new call of move_arm_inc can only be called when the arm is not busy. Check with is_arm_busy(..)
		//Note 2: If async movement is configurated, multiple calls of move_arm_inc can be stuck within the controller. WARNING: multiple action in a short period can saturate the queue.
		int move_arm();

		//******************************************************FUNCTION TO ERASE THE QUEUE OF MOVEMENTS. IN CASE OF SYNC, THE LAS INPUT.***************************************************************
		int erase_arm_movement_queue();

		//****************************************************************************OUTPUT CONTROL****************************************************************************************************
		//input electrovalvule id: <0, 1> state: <true (high), false (low)> 
		int set_electrovalvule(int id, bool state);
		//input digital output (currently just 4 mapped): id <0, 1, 2, 3> state: <true (high), false(low)>
		int set_digital_output(int id, bool state);

		//****************************************************************************READING FUNCTIONS*************************************************************************************************
		//returns true if the arm is busy or false if it is avaliable for a new instruction. This function is mainly for sync movement. It also allows the program
		int is_arm_busy(bool* bsy);
		//Returns a joint_vector showing the current position (j1, j2, j3, j4, j5, j6).
		int get_joint_pos(joint_pos* j_pos);
		//Returns a point_vector showing the current position in word coordinates (x, y , z, Rx, Ry, Rz)
		int get_axis_pos(point_pos* p_pos);


	private:

		PACK(struct DATA_CTRL{
			uint64_t ID_MSG;				//Identification number. Any number. Generated randomly.
			uint32_t RESERVED1;				//Set to 0
			uint8_t ACTION;					//ACTION IDENTIFICATOR
			uint16_t RESERVED2;				//Set to 0
			uint8_t ELECTROVALVULE;			//bits 0, 1 ID selection. Bit 7=0 on, bit 7=1 off.
			uint8_t DIGITAL_OUTPUT;			//ID of the output.	Bit 7=0 on, bit 7=1 off.
			uint8_t	DO_RESERVED;			//Reserved for expansion.
			uint8_t DISABLE_STOP;			//Must be set to 1 to enable movement.	128 to disable movement.
			uint8_t WORKING_MODE;			//Working mode. sync(bit 7 = 0) async(bit 7 = 1). Movement: axis (bit 2 = 1 and bit 3 = 0) joint (bit 2 = 0 and bit 3 = 1)
			uint8_t MOV_INC;				//increment movement. 1 == 1mm
			uint8_t SPEED;					//0 to 100% of nominal speed (The one configured in the Staubli program).
			uint32_t AXIS[6];				//components of the vector of movement
			uint8_t AXIS_SIGN[6];			//sign. Positive == 0, negative == 1
			uint32_t JOINT[6];				//components of the vector of movement 
			uint8_t JOINT_SIGN[6];			//sign. "" "".
		});	//82bytes

		PACK(struct DATA_STATE{
			uint64_t ID_MSG;				//Same ID as the petition
			uint32_t RESERVED1;				//Should be 0
			uint8_t ACTION;					//Action sent
			uint16_t STATUS;				//Current status. Busy == 12, 14. free == 0
			uint8_t ERROR_CODE;				//Should be 0, if it is different. Check Error code list.
			uint8_t WORKING_MODE;			//Working mode. sync(bit 7 = 0) async(bit 7 = 1). Movement: axis_single (00) axis_multiple(01) joint_single(10) joint multiple (11) (bit 2 and bit 3)
			uint8_t ELECTROVALVULE;			//bit 0 -> electrovalvule 0, bit 1-> electrovalvule 1
			uint8_t	DIGITAL_OUTPUT;			//bit 0 to 3, digital outputs 0 to 3.
			uint8_t	DO_RESERVED;			//Reserved for expansion of DO.
			uint8_t MOV_INC;				//Configured increment
			uint8_t SPEED;					//Configured Speed
			uint32_t ACTUAL_POS_AXIS[6];	//Actual position word absolute
			uint8_t RESERVED2[6];			//set to 0
			uint32_t ACTUAL_POS_JOINT[6];	//Actual position joint absolute
			uint8_t RESERVED3[6];			//set to 0
		}); //82bytes

		SOCKET st_Socket;
		DATA_CTRL st_msg_data;				//Sending data msg
		DATA_STATE st_rcv_data;				//Receiving data msg

		int init_memory();
		unsigned rand256();
		unsigned long long rand64bits();
		int get_data();
		int send_data();
	};
}


#endif
