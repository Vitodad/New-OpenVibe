/*
FILENAME: st_Communication.cpp
DESCRIPTION: Source code implementation for communication with a Staubli controller (CS8C) through a TCP/IP
client port. That server port must be configurated and opened in the CS8C controller.
AUTHOR: Pablo José Gijón Garzás
START DATE: December 2016
*/
#include "st_Communication.h"
#include <cmath>
using namespace std;
namespace communication
{
	int STAUBLI::init_remote_control(const char* ip,int port){
		WSADATA wsa;
		struct sockaddr_in gw_addr;
		int n_ret = 0;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)return(PRG_ERR_WINSOCK);
		st_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (st_Socket < 0) return(PRG_ERR_SOCKET);
		gw_addr.sin_family = AF_INET;
		gw_addr.sin_port = htons(port);
		gw_addr.sin_addr.s_addr = inet_addr(ip);
		n_ret = connect(st_Socket, (struct sockaddr *)&gw_addr, sizeof(gw_addr));
		if (n_ret != 0) return(PRG_ERR_CONNECTION);
		init_memory();
		return(PRG_ERR_NONE);
	}
	int STAUBLI::end_remote_control(){
		int ret = 0;
		st_msg_data.ACTION = ST_END_CONTROL;
		ret = send_data();
		ret = ret + get_data();
		if (ret == PRG_ERR_NONE)closesocket(st_Socket);
		return(PRG_ERR_NONE);
	}

	int STAUBLI::enable_movement(bool inp){
		int ret = 0;
		if (inp)st_msg_data.DISABLE_STOP = ST_ENABLE_ROBOT;
		else st_msg_data.DISABLE_STOP = ST_DISABLE_ROBOT;
		st_msg_data.ACTION = ST_WRITE_DATA;
		ret = send_data();
		ret = ret + get_data();
		if (ret != 0) st_msg_data.DISABLE_STOP = inp * 127 + 1;
		return(ret);
	}

	int STAUBLI::configure_move_increment(int inc){
		int ret = 0;
		int dummy = st_msg_data.MOV_INC;
		inc = abs(inc);
		if (inc <= ST_MAX_INC_VALUE && inc >= ST_MIN_INC_VALUE)st_msg_data.MOV_INC = inc;
		else if (inc < ST_MIN_INC_VALUE)return PRG_ERR_MIN_INC;
		else return PRG_ERR_MAX_INC;
		st_msg_data.ACTION = ST_WRITE_DATA;
		ret = send_data();
		ret = ret + get_data();
		if (ret != 0) st_msg_data.MOV_INC = dummy;
		return(ret);
	}

	int STAUBLI::configure_system_reference(int ref){
		int ret = 0;
		int dummy = st_msg_data.WORKING_MODE;
		st_msg_data.ACTION = ST_WRITE_DATA;
		st_msg_data.WORKING_MODE = st_msg_data.WORKING_MODE & 0xFE;
		if (ref == ST_ABSOLUTE) st_msg_data.WORKING_MODE = st_msg_data.WORKING_MODE | 1;
		else if (ref == ST_RELATIVE);
		else return PRG_ERR_INCORRECT_INPUT;
		ret = send_data();
		ret = ret + get_data();
		if (ret != 0) st_msg_data.WORKING_MODE = dummy;
		return(ret);
	}

	int STAUBLI::configure_working_mode(bool mode){
		int ret = 0;
		int dummy = st_msg_data.WORKING_MODE;
		if (mode)st_msg_data.WORKING_MODE = st_msg_data.WORKING_MODE & 127;
		else st_msg_data.WORKING_MODE = st_msg_data.WORKING_MODE | 128;
		st_msg_data.ACTION = ST_WRITE_DATA;
		ret = send_data();
		ret = ret + get_data();
		if (ret != 0) st_msg_data.WORKING_MODE = dummy;
		return(ret);
	}

	int STAUBLI::set_speed(int speed){
		int ret = 0;
		int dummy = st_msg_data.SPEED;
		if (speed >= ST_MIN_SPEED && speed <= ST_MAX_SPEED)st_msg_data.SPEED = speed;
		else return PRG_ERR_INCORRECT_INPUT;
		st_msg_data.ACTION = ST_WRITE_DATA;
		ret = send_data();
		ret = ret + get_data();
		if (ret != 0) st_msg_data.SPEED = dummy;
		return(ret);
	}

	int STAUBLI::set_move_single_axis(int axis, int direction){
		int ret;
		point_pos v_axis = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		switch (axis){
		case ST_AXIS_X:
			if (direction == ST_POSITIVE_MOV)v_axis.x = 1.0;
			else if (direction == ST_NEGATIVE_MOV)v_axis.x = -1.0;
			else return PRG_ERR_DIR_SEL;
			break;
		case ST_AXIS_Y:
			if (direction == ST_POSITIVE_MOV)v_axis.y = 1.0;
			else if (direction == ST_NEGATIVE_MOV)v_axis.y = -1.0;
			else return PRG_ERR_DIR_SEL;
			break;
		case ST_AXIS_Z:
			if (direction == ST_POSITIVE_MOV)v_axis.z = 1.0;
			else if (direction == ST_NEGATIVE_MOV)v_axis.z = -1.0;
			else return PRG_ERR_DIR_SEL;
			break;
		case ST_AXIS_RX:
			if (direction == ST_POSITIVE_MOV)v_axis.rx = 1.0;
			else if (direction == ST_NEGATIVE_MOV)v_axis.rx = -1.0;
			else return PRG_ERR_DIR_SEL;
			break;
		case ST_AXIS_RY:
			if (direction == ST_POSITIVE_MOV)v_axis.ry = 1.0;
			else if (direction == ST_NEGATIVE_MOV)v_axis.ry = -1.0;
			else return PRG_ERR_DIR_SEL;
			break;
		case ST_AXIS_RZ:
			if (direction == ST_POSITIVE_MOV)v_axis.rz = 1.0;
			else if (direction == ST_NEGATIVE_MOV)v_axis.rz = -1.0;
			else return PRG_ERR_DIR_SEL;
			break;
		default:
			return PRG_ERR_AXIS_SEL;
			break;
		}
		ret = set_move_multiple_axis(v_axis);
		return(ret);
	}

	int STAUBLI::set_move_multiple_axis(point_pos v_mov){
		int ret = 0;
		int dummy = st_msg_data.WORKING_MODE;
		float aux = 0.0;

		if (v_mov.x >= 0.0)st_msg_data.AXIS_SIGN[0] = 0;
		else st_msg_data.AXIS_SIGN[0] = 1;
		if (v_mov.y >= 0.0)st_msg_data.AXIS_SIGN[1] = 0;
		else st_msg_data.AXIS_SIGN[1] = 1;
		if (v_mov.z >= 0.0)st_msg_data.AXIS_SIGN[2] = 0;
		else st_msg_data.AXIS_SIGN[2] = 1;
		if (v_mov.rx >= 0.0)st_msg_data.AXIS_SIGN[3] = 0;
		else st_msg_data.AXIS_SIGN[3] = 1;
		if (v_mov.ry >= 0.0)st_msg_data.AXIS_SIGN[4] = 0;
		else st_msg_data.AXIS_SIGN[4] = 1;
		if (v_mov.rz >= 0.0)st_msg_data.AXIS_SIGN[5] = 0;
		else st_msg_data.AXIS_SIGN[5] = 1;

		st_msg_data.AXIS[0] = (uint32_t)round(abs(v_mov.x * 100));
		st_msg_data.AXIS[1] = (uint32_t)round(abs(v_mov.y * 100));
		st_msg_data.AXIS[2] = (uint32_t)round(abs(v_mov.z * 100));
		st_msg_data.AXIS[3] = (uint32_t)round(abs(v_mov.rx * 100));
		st_msg_data.AXIS[4] = (uint32_t)round(abs(v_mov.ry * 100));
		st_msg_data.AXIS[5] = (uint32_t)round(abs(v_mov.rz * 100));
		aux = sqrt(pow(v_mov.x, 2) + pow(v_mov.y, 2) + pow(v_mov.z, 2) + pow(v_mov.rx, 2) + pow(v_mov.ry, 2) + pow(v_mov.rz, 2));
		//bits 2 and 3 set to 0
		st_msg_data.WORKING_MODE = st_msg_data.WORKING_MODE & 0xF3;
		if (aux == 1.0)st_msg_data.WORKING_MODE = st_msg_data.WORKING_MODE | ST_AXIS_SINGLE;
		else st_msg_data.WORKING_MODE = st_msg_data.WORKING_MODE | ST_AXIS_MULTIPLE;
		st_msg_data.ACTION = ST_WRITE_DATA;
		ret = send_data();
		ret = ret + get_data();
		if (ret != 0) st_msg_data.WORKING_MODE = dummy;
		return(ret);
	}

	int STAUBLI::set_move_single_joint(int joint, int direction){
		int ret = 0;
		joint_pos v_mov = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		switch (joint){
		case ST_JOINT_1:
			if (direction == ST_POSITIVE_MOV)v_mov.j1 = 1.0;
			else if (direction == ST_NEGATIVE_MOV)v_mov.j1 = -1.0;
			else return PRG_ERR_DIR_SEL;
			break;
		case ST_JOINT_2:
			if (direction == ST_POSITIVE_MOV)v_mov.j2 = 1.0;
			else if (direction == ST_NEGATIVE_MOV)v_mov.j2 = -1.0;
			else return PRG_ERR_DIR_SEL;
			break;
		case ST_JOINT_3:
			if (direction == ST_POSITIVE_MOV)v_mov.j3 = 1.0;
			else if (direction == ST_NEGATIVE_MOV)v_mov.j3 = -1.0;
			else return PRG_ERR_DIR_SEL;
			break;
		case ST_JOINT_4:
			if (direction == ST_POSITIVE_MOV)v_mov.j4 = 1.0;
			else if (direction == ST_NEGATIVE_MOV)v_mov.j4 = -1.0;
			else return PRG_ERR_DIR_SEL;
			break;
		case ST_JOINT_5:
			if (direction == ST_POSITIVE_MOV)v_mov.j5 = 1.0;
			else if (direction == ST_NEGATIVE_MOV)v_mov.j5 = -1.0;
			else return PRG_ERR_DIR_SEL;
			break;
		case ST_JOINT_6:
			if (direction == ST_POSITIVE_MOV)v_mov.j6 = 1.0;
			else if (direction == ST_NEGATIVE_MOV)v_mov.j6 = -1.0;
			else return PRG_ERR_DIR_SEL;
			break;
		default:
			return PRG_ERR_AXIS_SEL;
			break;
		}
		ret = set_move_multiple_joint(v_mov);
		return(ret);
	}

	int STAUBLI::set_move_multiple_joint(joint_pos v_mov){
		int ret = 0;
		int dummy = st_msg_data.WORKING_MODE;
		int aux = 0;
		
		if (v_mov.j1 >= 0.0)st_msg_data.JOINT_SIGN[0] = 0;
		else st_msg_data.JOINT_SIGN[0] = 1;
		if (v_mov.j2 >= 0.0)st_msg_data.JOINT_SIGN[1] = 0;
		else st_msg_data.JOINT_SIGN[1] = 1;
		if (v_mov.j3 >= 0.0)st_msg_data.JOINT_SIGN[2] = 0;
		else st_msg_data.JOINT_SIGN[2] = 1;
		if (v_mov.j4 >= 0.0)st_msg_data.JOINT_SIGN[3] = 0;
		else st_msg_data.JOINT_SIGN[3] = 1;
		if (v_mov.j5 >= 0.0)st_msg_data.JOINT_SIGN[4] = 0;
		else st_msg_data.JOINT_SIGN[4] = 1;
		if (v_mov.j6 >= 0.0)st_msg_data.JOINT_SIGN[5] = 0;
		else st_msg_data.JOINT_SIGN[5] = 1;

		st_msg_data.JOINT[0] = (uint32_t)round(abs(v_mov.j1 * 100));
		st_msg_data.JOINT[1] = (uint32_t)round(abs(v_mov.j2 * 100));
		st_msg_data.JOINT[2] = (uint32_t)round(abs(v_mov.j3 * 100));
		st_msg_data.JOINT[3] = (uint32_t)round(abs(v_mov.j4 * 100));
		st_msg_data.JOINT[4] = (uint32_t)round(abs(v_mov.j5 * 100));
		st_msg_data.JOINT[5] = (uint32_t)round(abs(v_mov.j6 * 100));
		aux = sqrt(pow(v_mov.j1,2) + pow(v_mov.j2,2) + pow(v_mov.j3,2) + pow(v_mov.j4,2) + pow(v_mov.j5,2) + pow(v_mov.j6,2));
		//set to 0 bits; 2, 3
		st_msg_data.WORKING_MODE = st_msg_data.WORKING_MODE & 0xF3;
		if (aux == 1.0)st_msg_data.WORKING_MODE = st_msg_data.WORKING_MODE | ST_JOINT_SINGLE;
		else st_msg_data.WORKING_MODE = st_msg_data.WORKING_MODE | ST_JOINT_MULTIPLE;
		st_msg_data.ACTION = ST_WRITE_DATA;
		ret = send_data();
		ret = ret + get_data();
		if (ret != 0) st_msg_data.WORKING_MODE = dummy;
		return(ret);
	}

	int STAUBLI::go_home(){
		int ret = 0;
		int dummy = st_msg_data.WORKING_MODE;
		//bit 5
		st_msg_data.WORKING_MODE = st_msg_data.WORKING_MODE | ST_GO_HOME;
		st_msg_data.ACTION = ST_WRITE_DATA;
		ret = send_data();
		ret = ret + get_data();
		if (st_rcv_data.STATUS != 32)cout << "homing failed." << endl;
		st_msg_data.WORKING_MODE = st_msg_data.WORKING_MODE & 0xDF;
		if (ret != 0)st_msg_data.WORKING_MODE = dummy;
		return(ret);
	}

	int STAUBLI::move_arm(){
		int ret = 0;
		int dummy = st_msg_data.WORKING_MODE;
		st_msg_data.WORKING_MODE = st_msg_data.WORKING_MODE & 0xEF;
		st_msg_data.WORKING_MODE = st_msg_data.WORKING_MODE | ST_NEW_MOVE;
		st_msg_data.ACTION = ST_START_MOVE;
		ret = send_data();
		ret = ret + get_data();
		st_msg_data.WORKING_MODE = st_msg_data.WORKING_MODE & 0xEF;

		if (ret != 0)st_msg_data.WORKING_MODE = dummy;
		return(ret);
	}

	int STAUBLI::erase_arm_movement_queue(){
		int ret = 0;
		int dummy = st_msg_data.WORKING_MODE;
		st_msg_data.WORKING_MODE = st_msg_data.WORKING_MODE | ST_ERASE_QUEUE;
		st_msg_data.ACTION = ST_WRITE_DATA;
		ret = send_data();
		ret = ret + get_data();
		st_msg_data.WORKING_MODE = st_msg_data.WORKING_MODE & 0xBF;

		if (ret != 0)st_msg_data.WORKING_MODE = dummy;
		return(ret);
	}

	int STAUBLI::set_electrovalvule(int id, bool state){
		int ret = 0;
		if (id > ST_MAX_EV_NUMBER -1) return PRG_ERR_INCORRECT_INPUT;
		st_msg_data.ACTION = ST_SET_OUTPUT;
		if (state)st_msg_data.ELECTROVALVULE = id+1;
		else st_msg_data.ELECTROVALVULE = id+1 + 128;
		ret = send_data();
		ret = ret + get_data();
		st_msg_data.ELECTROVALVULE = 0;
		return(ret);		
	}

	int STAUBLI::set_digital_output(int id, bool state){
		int ret = 0;
		if (id > ST_MAX_DO_NUMBER) return PRG_ERR_INCORRECT_INPUT;
		st_msg_data.ACTION = ST_SET_OUTPUT;
		if (state)st_msg_data.DIGITAL_OUTPUT = id;
		else st_msg_data.DIGITAL_OUTPUT = id + 128;
		ret = send_data();
		ret = ret + get_data();
		st_msg_data.DIGITAL_OUTPUT = 0;
		return(ret);
	}

	int STAUBLI::is_arm_busy(bool* bsy){
		int ret = 0;
		st_msg_data.ACTION = ST_READ_DATA;
		ret = send_data();
		ret = ret + get_data();
		if (st_rcv_data.STATUS == 0 && ret == 0){
			*bsy = false;
		}
		return(ret);
	}
	int STAUBLI::get_joint_pos(joint_pos* j_pos){
		int ret = 0;
		st_msg_data.ACTION = ST_READ_DATA;
		ret = send_data();
		ret = ret + get_data();
		if (ret == 0){
			j_pos->j1 = (1.0 -2.0 * st_rcv_data.RESERVED3[0])*float(st_rcv_data.ACTUAL_POS_JOINT[0]) / 100.0;
			j_pos->j2 = (1.0 -2.0 * st_rcv_data.RESERVED3[1])*float(st_rcv_data.ACTUAL_POS_JOINT[1]) / 100.0;
			j_pos->j3 = (1.0 -2.0 * st_rcv_data.RESERVED3[2])*float(st_rcv_data.ACTUAL_POS_JOINT[2]) / 100.0;
			j_pos->j4 = (1.0 -2.0 * st_rcv_data.RESERVED3[3])*float(st_rcv_data.ACTUAL_POS_JOINT[3]) / 100.0;
			j_pos->j5 = (1.0 -2.0 * st_rcv_data.RESERVED3[4])*float(st_rcv_data.ACTUAL_POS_JOINT[4]) / 100.0;
			j_pos->j6 = (1.0 -2.0 * st_rcv_data.RESERVED3[5])*float(st_rcv_data.ACTUAL_POS_JOINT[5]) / 100.0;
		}
		return(ret);
	}
	int STAUBLI::get_axis_pos(point_pos* p_pos){
		int ret = 0;
		st_msg_data.ACTION = ST_READ_DATA;
		ret = send_data();
		ret = ret + get_data();
		if (ret == 0){
			p_pos->x = (1 -2 * st_rcv_data.RESERVED2[0])*float(st_rcv_data.ACTUAL_POS_AXIS[0]) / 100.0;
			p_pos->y = (1 -2 * st_rcv_data.RESERVED2[1])*float(st_rcv_data.ACTUAL_POS_AXIS[1]) / 100.0;
			p_pos->z = (1 -2 * st_rcv_data.RESERVED2[2])*float(st_rcv_data.ACTUAL_POS_AXIS[2]) / 100.0;
			p_pos->rx = (1 -2 * st_rcv_data.RESERVED2[3])*float(st_rcv_data.ACTUAL_POS_AXIS[3]) / 100.0;
			p_pos->ry = (1 -2 * st_rcv_data.RESERVED2[4])*float(st_rcv_data.ACTUAL_POS_AXIS[4]) / 100.0;
			p_pos->rz = (1 -2 * st_rcv_data.RESERVED2[5])*float(st_rcv_data.ACTUAL_POS_AXIS[5]) / 100.0;
		}
		return(ret);
	}

	int STAUBLI::init_memory(){
		st_msg_data.ID_MSG = 0;
		st_msg_data.DISABLE_STOP = ST_DISABLE_ROBOT;
		st_msg_data.MOV_INC = ST_MIN_INC_VALUE;
		st_msg_data.ACTION = ST_NULL_ACTION;
		st_msg_data.DIGITAL_OUTPUT = 0;
		st_msg_data.DO_RESERVED = 0;
		st_msg_data.ELECTROVALVULE = 0;
		st_msg_data.RESERVED1 = 0;
		st_msg_data.RESERVED2 = 0;
		st_msg_data.ACTION = 0;
		for (int i = 0; i < 6; i++){
			st_msg_data.JOINT[i] = 0;
			st_msg_data.AXIS[i] = 0;
			st_msg_data.AXIS_SIGN[i] = 0;
			st_msg_data.JOINT_SIGN[i] = 0;
		}
		st_msg_data.SPEED = ST_DEFAULT_SPEED;
		st_msg_data.WORKING_MODE = 0;

		st_rcv_data.ACTION = 0;
		st_rcv_data.DIGITAL_OUTPUT = 0;
		st_rcv_data.DO_RESERVED = 0;
		st_rcv_data.ELECTROVALVULE = 0;
		st_rcv_data.ERROR_CODE = 0;
		st_rcv_data.MOV_INC = 0;
		st_rcv_data.RESERVED1 = 0;
		st_rcv_data.SPEED = 0;
		st_rcv_data.WORKING_MODE = 0;
		st_rcv_data.STATUS = 0;
		st_rcv_data.ID_MSG = 0;
		for (int i = 0; i < 6; i++){
			st_rcv_data.ACTUAL_POS_AXIS[i] = 0;
			st_rcv_data.ACTUAL_POS_JOINT[i] = 0;
			st_rcv_data.RESERVED2[i] = 0;
			st_rcv_data.RESERVED3[i] = 0;
		}

		return(PRG_ERR_NONE);
	}
	unsigned STAUBLI::rand256(){
		static unsigned const limit = RAND_MAX - RAND_MAX % 256;
		unsigned result = rand();
		while (result >= limit) {
			result = rand();
		}
		return result % 256;
	}
	unsigned long long STAUBLI::rand64bits(){
		unsigned long long resultado = 0ULL;
		for (int count = 8; count > 0; --count){
			resultado = 256U * resultado + rand256();
		}
		return resultado;
	}
	int STAUBLI::get_data(){
		int bytes_recv = 0;
		bytes_recv = recv(st_Socket, (char*)&st_rcv_data, sizeof(st_rcv_data), 0);
		if (bytes_recv > 0){
			if (st_rcv_data.ID_MSG != st_msg_data.ID_MSG) return ST_ERR_ID_MESSAGE;
			if (st_rcv_data.ERROR_CODE != PRG_ERR_NONE) return st_rcv_data.ERROR_CODE;
		}
		else return PRG_ERR_SEND;
		return(PRG_ERR_NONE);
	}
	int STAUBLI::send_data(){
		int bytes_send = 0;
		st_msg_data.ID_MSG = (uint64_t)rand64bits();
		bytes_send = send(st_Socket, (const char*)&st_msg_data, 82, 0);
		if(bytes_send > 0) return(PRG_ERR_NONE);
		else return PRG_ERR_SEND;
	}
}