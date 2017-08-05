/*
	FILENAME: st_Common.h
	DESCRIPTION: Header with basic definitions
	AUTHOR: Pablo José Gijón Garzás
	START DATE: December 2016
*/

//The port stablished here must be opened in the Staubli's robot controller as a server.


#define ST_DISABLE_ROBOT			128
#define ST_ENABLE_ROBOT				1
#define ST_MAX_INC_VALUE			255
#define ST_MIN_INC_VALUE			1
#define ST_MAX_EV_NUMBER			2
#define ST_MAX_DO_NUMBER			4
#define ST_DEFAULT_SPEED			10
#define ST_MIN_SPEED				1
#define ST_MAX_SPEED				100

//REFERENCE SYSTEM
#define ST_RELATIVE					0
#define ST_ABSOLUTE					1

//MODE SYNC OR ASYNC
#define ST_ASYNC					TRUE
#define ST_SYNC						FALSE

//ACTION
#define ST_NULL_ACTION				13
#define ST_READ_DATA				16
#define ST_WRITE_DATA				17
#define ST_SET_OUTPUT				18
#define ST_START_MOVE				19
#define ST_END_CONTROL				66

//DIRECTION SELECTION
#define ST_POSITIVE_MOV				1
#define ST_NEGATIVE_MOV				0

//AXIS SELECTION

#define ST_AXIS_X					1
#define ST_AXIS_Y					2
#define ST_AXIS_Z					4
#define ST_AXIS_RX					8
#define ST_AXIS_RY					16
#define ST_AXIS_RZ					32

//JOINT SELECTION
#define ST_JOINT_1					1
#define ST_JOINT_2					2
#define ST_JOINT_3					4
#define	ST_JOINT_4					8
#define ST_JOINT_5					16
#define ST_JOINT_6					32

//ERROR CODES FOR PROGRAM
#define PRG_ERR_NONE				0

#define PRG_ERR_WINSOCK				1000
#define PRG_ERR_SOCKET				1001
#define PRG_ERR_CONNECTION			1002
#define PRG_ERR_AXIS_SEL			1003
#define PRG_ERR_DIR_SEL				1004
#define PRG_ERR_SEND				1005
#define PRG_ERR_MAX_INC				1006
#define PRG_ERR_MIN_INC				1007

#define PRG_ERR_INCORRECT_INPUT		2000


//ERROR CODES FOR COMMUNICATION WITH STAUBLI
#define ST_ERR_NONE					0			//None error
#define ST_ERR_WRONG_VALUE			1			//Wrong input
#define ST_ERR_MOVEMENT				2			//Invalid movement.
#define ST_ERR_MOV_CONFIG			3			//Wrong input for configuration movement.
#define ST_ERR_SYNC					4			//Robot is busy (sync) and can't queue a new movement.
#define ST_ERR_UNABLE				5			//Unable to modify a variable at a certain moment. (Busy).
#define ST_ERR_UNDEFINED_CASE		13			//Undefined staubli error.

//Types
//Movement vector definition for multiple axis action.
struct point_pos {
	float x, y, z, rx, ry, rz;
};
struct joint_pos {
	float j1, j2, j3, j4, j5, j6;
};


//Movement vector definition for multiple joint action.