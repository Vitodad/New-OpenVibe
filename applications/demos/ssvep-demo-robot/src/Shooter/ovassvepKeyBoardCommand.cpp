
#if defined(TARGET_HAS_ThirdPartyOgre3D)

#include "ovassvepKeyBoardCommand.h"

#include "../ovassvepCApplication.h"
#include "ovassvepCShooterApplication.h"
#include "pthread.h"

using namespace OpenViBESSVEP;
using namespace OpenViBE::Kernel;
using namespace std;


struct args_Handle_KeyBoard{
	CShooterApplication *l_shooter;
	CCommandKeyBoard *l_command;
};


void wait_end_move(class communication::STAUBLI inp){
	bool bsy = true;
	int ret = 0;
	cout << "Waiting for the robot." << endl;
	do{
		cout << ".";
		ret = inp.is_arm_busy(&bsy);
	} while (bsy == true);
	cout << endl;
}
void *handle_ManualControl(void *ptr){
	int ret = 0;

	args_Handle_KeyBoard *l_args = (args_Handle_KeyBoard *)ptr;

	CShooterApplication *l_poThreadShooterApplication = (CShooterApplication *)l_args->l_shooter;
	CCommandKeyBoard *l_poCommandKey = (CCommandKeyBoard *)l_args->l_command;

	pthread_condattr_t cond_attr;
	pthread_mutexattr_t mutex_attr;

	pthread_mutexattr_init(&mutex_attr);
	pthread_condattr_init(&cond_attr);
	pthread_mutex_init(&l_poCommandKey->mutex_ManualControl, &mutex_attr);
	pthread_cond_init(&l_poCommandKey->condition_ManualControl, &cond_attr);

	while (true)
	{
		pthread_cond_wait(&l_poCommandKey->condition_ManualControl, &l_poCommandKey->mutex_ManualControl);

		if (l_poCommandKey->getOrder() == 10)
		{

			point_pos Point_Origen = { 520.0, 0, -80.0, 0.0, 180.0, 0.0 };

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->enable_movement(true);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_RELATIVE);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_move_increment(100);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_single_axis(ST_AXIS_Z, ST_POSITIVE_MOV);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_ABSOLUTE);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_multiple_axis(Point_Origen);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->enable_movement(false);
			l_poCommandKey->setOrder(0);

		}
		else if (l_poCommandKey->getOrder() == 9)
		{

			point_pos Point1_1 = { 500.0, 125.0, -120.0, 0.0, 180.0, 0.0 };


			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->enable_movement(true);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_RELATIVE);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_move_increment(100);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_single_axis(ST_AXIS_Z, ST_POSITIVE_MOV);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_ABSOLUTE);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_multiple_axis(Point1_1);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_RELATIVE);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_move_increment(100);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_single_axis(ST_AXIS_Z, ST_NEGATIVE_MOV);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->enable_movement(false);
			l_poCommandKey->setOrder(0);

		}
		else if (l_poCommandKey->getOrder() == 8)
		{

			point_pos Point1_2 = { 500.0, 0.0, -120.0, 0.0, 180.0, 0.0 };

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->enable_movement(true);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_RELATIVE);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_move_increment(100);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_single_axis(ST_AXIS_Z, ST_POSITIVE_MOV);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_ABSOLUTE);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_multiple_axis(Point1_2);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_RELATIVE);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_move_increment(100);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_single_axis(ST_AXIS_Z, ST_NEGATIVE_MOV);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->enable_movement(false);
			l_poCommandKey->setOrder(0);

		}
		else if (l_poCommandKey->getOrder() == 7)
		{
			point_pos Point1_3 = { 500.0, -125.0, -120.0, 0.0, 180.0, 0.0 };

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->enable_movement(true);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_RELATIVE);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_move_increment(100);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_single_axis(ST_AXIS_Z, ST_POSITIVE_MOV);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_ABSOLUTE);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_multiple_axis(Point1_3);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_RELATIVE);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_move_increment(100);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_single_axis(ST_AXIS_Z, ST_NEGATIVE_MOV);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->enable_movement(false);
			l_poCommandKey->setOrder(0);

		}
		else if (l_poCommandKey->getOrder() == 6)
		{
			point_pos Point2_1 = { 375.0, 125.0, -120.0, 0.0, 180.0, 0.0 };

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->enable_movement(true);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_RELATIVE);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_move_increment(100);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_single_axis(ST_AXIS_Z, ST_POSITIVE_MOV);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_ABSOLUTE);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_multiple_axis(Point2_1);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_RELATIVE);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_move_increment(100);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_single_axis(ST_AXIS_Z, ST_NEGATIVE_MOV);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->enable_movement(false);
			l_poCommandKey->setOrder(0);
		}
		else if (l_poCommandKey->getOrder() == 5)
		{
			point_pos Point2_2 = { 375.0, 0.0, -120.0, 0.0, 180.0, 0.0 };

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->enable_movement(true);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_RELATIVE);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_move_increment(100);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_single_axis(ST_AXIS_Z, ST_POSITIVE_MOV);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_ABSOLUTE);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_multiple_axis(Point2_2);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_RELATIVE);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_move_increment(100);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_single_axis(ST_AXIS_Z, ST_NEGATIVE_MOV);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->enable_movement(false);
			l_poCommandKey->setOrder(0);
		}
		else if (l_poCommandKey->getOrder() == 4)
		{
			point_pos Point2_3 = { 375.0, -125.0, -120.0, 0.0, 180.0, 0.0 };

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->enable_movement(true);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_RELATIVE);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_move_increment(100);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_single_axis(ST_AXIS_Z, ST_POSITIVE_MOV);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_ABSOLUTE);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_multiple_axis(Point2_3);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_RELATIVE);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_move_increment(100);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_single_axis(ST_AXIS_Z, ST_NEGATIVE_MOV);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->enable_movement(false);
			l_poCommandKey->setOrder(0);
		}
		else if (l_poCommandKey->getOrder() == 3)
		{
			point_pos Point3_1 = { 250.0, 125.0, -120.0, 0.0, 180.0, 0.0 };

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->enable_movement(true);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_RELATIVE);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_move_increment(100);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_single_axis(ST_AXIS_Z, ST_POSITIVE_MOV);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_ABSOLUTE);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_multiple_axis(Point3_1);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_RELATIVE);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_move_increment(100);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_single_axis(ST_AXIS_Z, ST_NEGATIVE_MOV);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->enable_movement(false);
			l_poCommandKey->setOrder(0);
		}
		else if (l_poCommandKey->getOrder() == 2)
		{
			point_pos Point3_2 = { 250.0, 0.0, -120.0, 0.0, 180.0, 0.0 };

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->enable_movement(true);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_RELATIVE);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_move_increment(100);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_single_axis(ST_AXIS_Z, ST_POSITIVE_MOV);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_ABSOLUTE);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_multiple_axis(Point3_2);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_RELATIVE);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_move_increment(100);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_single_axis(ST_AXIS_Z, ST_NEGATIVE_MOV);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->enable_movement(false);
			l_poCommandKey->setOrder(0);
		}
		else if (l_poCommandKey->getOrder() == 1)
		{
			point_pos Point3_3 = { 250.0, -125.0, -120.0, 0.0, 180.0, 0.0 };

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->enable_movement(true);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_RELATIVE);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_move_increment(100);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_single_axis(ST_AXIS_Z, ST_POSITIVE_MOV);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_ABSOLUTE);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_multiple_axis(Point3_3);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_system_reference(ST_RELATIVE);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->configure_move_increment(100);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->set_move_single_axis(ST_AXIS_Z, ST_NEGATIVE_MOV);
			l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->move_arm();
			wait_end_move(*l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60);

			ret = l_poThreadShooterApplication->m_RobotStaubli->Robot_TX60->enable_movement(false);
			l_poCommandKey->setOrder(0);
		}
		else{
			Sleep(500);
		}

	}
}

void *handle_Control_Screen(void *ptr){
	args_Handle_KeyBoard *l_args = (args_Handle_KeyBoard *)ptr;

	CShooterApplication *l_poThreadShooterApplication = (CShooterApplication *)l_args->l_shooter;
	CCommandKeyBoard *l_poCommandKey = (CCommandKeyBoard *)l_args->l_command;

	pthread_condattr_t cond_attr;
	pthread_mutexattr_t mutex_attr;

	pthread_mutexattr_init(&mutex_attr);
	pthread_condattr_init(&cond_attr);
	pthread_mutex_init(&l_poCommandKey->mutex_ControlScreen, &mutex_attr);
	pthread_cond_init(&l_poCommandKey->condition_ControlScreen, &cond_attr);

	int ret;
	while (true){
		pthread_cond_wait(&l_poCommandKey->condition_ControlScreen, &l_poCommandKey->mutex_ControlScreen);

		//cout << "selVertical:" << l_poCommandKey->getSelectVertical() << endl;
		//cout << "selHorizontal:" << l_poCommandKey->getSelectHorizontal() << endl;
		if (l_poThreadShooterApplication->m_RobotStaubli->Enable_Robot == true){
			l_poThreadShooterApplication->m_poTextActivo_G->setVisible(true);
			l_poThreadShooterApplication->m_poTextDesactivo_G->setVisible(false);
		}
		else{
			l_poThreadShooterApplication->m_poTextActivo_G->setVisible(false);
			l_poThreadShooterApplication->m_poTextDesactivo_G->setVisible(true);
		}

		if (l_poCommandKey->getSelectVertical() == 1){
			l_poThreadShooterApplication->m_poTextPosicion->setVisible(true);
			l_poThreadShooterApplication->m_poTextPosicion_G->setVisible(false);
			l_poThreadShooterApplication->m_poTextOrientacion->setVisible(false);
			l_poThreadShooterApplication->m_poTextOrientacion_G->setVisible(true);

			l_poThreadShooterApplication->m_poTextX->setVisible(false);
			l_poThreadShooterApplication->m_poTextX_G->setVisible(false);
			l_poThreadShooterApplication->m_poTextY->setVisible(false);
			l_poThreadShooterApplication->m_poTextY_G->setVisible(false);
			l_poThreadShooterApplication->m_poTextZ->setVisible(false);
			l_poThreadShooterApplication->m_poTextZ_G->setVisible(false);

			l_poThreadShooterApplication->m_poTextAlpha->setVisible(true);
			l_poThreadShooterApplication->m_poTextAlpha_G->setVisible(false);
			l_poThreadShooterApplication->m_poTextBeta->setVisible(true);
			l_poThreadShooterApplication->m_poTextBeta_G->setVisible(false);
			l_poThreadShooterApplication->m_poTextGamma->setVisible(true);
			l_poThreadShooterApplication->m_poTextGamma_G->setVisible(false);


			l_poThreadShooterApplication->m_poCoordinateSystem_X->setVisible(false);
			l_poThreadShooterApplication->m_poCoordinateSystem_Y->setVisible(false);
			l_poThreadShooterApplication->m_poCoordinateSystem_Z->setVisible(false);
			l_poThreadShooterApplication->m_poCoordinateSystem_alfa->setVisible(false);
			l_poThreadShooterApplication->m_poCoordinateSystem_beta->setVisible(false);
			l_poThreadShooterApplication->m_poCoordinateSystem_gamma->setVisible(false);

		}
		if (l_poCommandKey->getSelectVertical() == 2){

			l_poThreadShooterApplication->m_poTextPosicion->setVisible(false);
			l_poThreadShooterApplication->m_poTextPosicion_G->setVisible(true);
			l_poThreadShooterApplication->m_poTextOrientacion->setVisible(true);
			l_poThreadShooterApplication->m_poTextOrientacion_G->setVisible(false);

			l_poThreadShooterApplication->m_poTextX->setVisible(true);
			l_poThreadShooterApplication->m_poTextX_G->setVisible(false);
			l_poThreadShooterApplication->m_poTextY->setVisible(true);
			l_poThreadShooterApplication->m_poTextY_G->setVisible(false);
			l_poThreadShooterApplication->m_poTextZ->setVisible(true);
			l_poThreadShooterApplication->m_poTextZ_G->setVisible(false);

			l_poThreadShooterApplication->m_poTextAlpha->setVisible(false);
			l_poThreadShooterApplication->m_poTextAlpha_G->setVisible(false);
			l_poThreadShooterApplication->m_poTextBeta->setVisible(false);
			l_poThreadShooterApplication->m_poTextBeta_G->setVisible(false);
			l_poThreadShooterApplication->m_poTextGamma->setVisible(false);
			l_poThreadShooterApplication->m_poTextGamma_G->setVisible(false);

			l_poThreadShooterApplication->m_poCoordinateSystem_X->setVisible(false);
			l_poThreadShooterApplication->m_poCoordinateSystem_Y->setVisible(false);
			l_poThreadShooterApplication->m_poCoordinateSystem_Z->setVisible(false);
			l_poThreadShooterApplication->m_poCoordinateSystem_alfa->setVisible(false);
			l_poThreadShooterApplication->m_poCoordinateSystem_beta->setVisible(false);
			l_poThreadShooterApplication->m_poCoordinateSystem_gamma->setVisible(false);
		}

		if (l_poCommandKey->viewAxis == 0){
			if (l_poCommandKey->getSelectVertical() == 2){
				l_poThreadShooterApplication->m_poTextX->setVisible(false);
				l_poThreadShooterApplication->m_poTextX_G->setVisible(true);
				l_poThreadShooterApplication->m_poTextY->setVisible(true);
				l_poThreadShooterApplication->m_poTextY_G->setVisible(false);
				l_poThreadShooterApplication->m_poTextZ->setVisible(true);
				l_poThreadShooterApplication->m_poTextZ_G->setVisible(false);

				l_poThreadShooterApplication->m_poCoordinateSystem->setVisible(false);
				l_poThreadShooterApplication->m_poCoordinateSystem_X->setVisible(true);
				l_poThreadShooterApplication->m_poCoordinateSystem_Y->setVisible(false);
				l_poThreadShooterApplication->m_poCoordinateSystem_Z->setVisible(false);

			}
			else if (l_poCommandKey->getSelectVertical() == 1){
				l_poThreadShooterApplication->m_poTextAlpha->setVisible(false);
				l_poThreadShooterApplication->m_poTextAlpha_G->setVisible(true);
				l_poThreadShooterApplication->m_poTextBeta->setVisible(true);
				l_poThreadShooterApplication->m_poTextBeta_G->setVisible(false);
				l_poThreadShooterApplication->m_poTextGamma->setVisible(true);
				l_poThreadShooterApplication->m_poTextGamma_G->setVisible(false);

				l_poThreadShooterApplication->m_poCoordinateSystem->setVisible(false);
				l_poThreadShooterApplication->m_poCoordinateSystem_alfa->setVisible(true);
				l_poThreadShooterApplication->m_poCoordinateSystem_beta->setVisible(false);
				l_poThreadShooterApplication->m_poCoordinateSystem_gamma->setVisible(false);
			}
		}
		else if (l_poCommandKey->viewAxis == 1){
			if (l_poCommandKey->getSelectVertical() == 2){
				l_poThreadShooterApplication->m_poTextX->setVisible(true);
				l_poThreadShooterApplication->m_poTextX_G->setVisible(false);
				l_poThreadShooterApplication->m_poTextY->setVisible(false);
				l_poThreadShooterApplication->m_poTextY_G->setVisible(true);
				l_poThreadShooterApplication->m_poTextZ->setVisible(true);
				l_poThreadShooterApplication->m_poTextZ_G->setVisible(false);

				l_poThreadShooterApplication->m_poCoordinateSystem->setVisible(false);
				l_poThreadShooterApplication->m_poCoordinateSystem_X->setVisible(false);
				l_poThreadShooterApplication->m_poCoordinateSystem_Y->setVisible(true);
				l_poThreadShooterApplication->m_poCoordinateSystem_Z->setVisible(false);

			}
			else if (l_poCommandKey->getSelectVertical() == 1){
				l_poThreadShooterApplication->m_poTextAlpha->setVisible(true);
				l_poThreadShooterApplication->m_poTextAlpha_G->setVisible(false);
				l_poThreadShooterApplication->m_poTextBeta->setVisible(false);
				l_poThreadShooterApplication->m_poTextBeta_G->setVisible(true);
				l_poThreadShooterApplication->m_poTextGamma->setVisible(true);
				l_poThreadShooterApplication->m_poTextGamma_G->setVisible(false);

				l_poThreadShooterApplication->m_poCoordinateSystem->setVisible(false);
				l_poThreadShooterApplication->m_poCoordinateSystem_alfa->setVisible(false);
				l_poThreadShooterApplication->m_poCoordinateSystem_beta->setVisible(true);
				l_poThreadShooterApplication->m_poCoordinateSystem_gamma->setVisible(false);
			}
		}
		else if (l_poCommandKey->viewAxis == 2){
			if (l_poCommandKey->getSelectVertical() == 2){
				l_poThreadShooterApplication->m_poTextX->setVisible(true);
				l_poThreadShooterApplication->m_poTextX_G->setVisible(false);
				l_poThreadShooterApplication->m_poTextY->setVisible(true);
				l_poThreadShooterApplication->m_poTextY_G->setVisible(false);
				l_poThreadShooterApplication->m_poTextZ->setVisible(false);
				l_poThreadShooterApplication->m_poTextZ_G->setVisible(true);

				l_poThreadShooterApplication->m_poCoordinateSystem->setVisible(false);
				l_poThreadShooterApplication->m_poCoordinateSystem_X->setVisible(false);
				l_poThreadShooterApplication->m_poCoordinateSystem_Y->setVisible(false);
				l_poThreadShooterApplication->m_poCoordinateSystem_Z->setVisible(true);
			}
			else if (l_poCommandKey->getSelectVertical() == 1){
				l_poThreadShooterApplication->m_poTextAlpha->setVisible(true);
				l_poThreadShooterApplication->m_poTextAlpha_G->setVisible(false);
				l_poThreadShooterApplication->m_poTextBeta->setVisible(true);
				l_poThreadShooterApplication->m_poTextBeta_G->setVisible(false);
				l_poThreadShooterApplication->m_poTextGamma->setVisible(false);
				l_poThreadShooterApplication->m_poTextGamma_G->setVisible(true);

				l_poThreadShooterApplication->m_poCoordinateSystem->setVisible(false);
				l_poThreadShooterApplication->m_poCoordinateSystem_alfa->setVisible(false);
				l_poThreadShooterApplication->m_poCoordinateSystem_beta->setVisible(false);
				l_poThreadShooterApplication->m_poCoordinateSystem_gamma->setVisible(true);
			}
		}

		//---- Seleccionar precisión ----

		if (l_poCommandKey->getSelectHorizontal() == 1){
			l_poThreadShooterApplication->m_poText1->setVisible(true);
			l_poThreadShooterApplication->m_poText5->setVisible(false);
			l_poThreadShooterApplication->m_poText10->setVisible(false);
			l_poThreadShooterApplication->m_poText100->setVisible(false);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Increment = 1;
		}
		if (l_poCommandKey->getSelectHorizontal() == 2){
			l_poThreadShooterApplication->m_poText1->setVisible(false);
			l_poThreadShooterApplication->m_poText5->setVisible(true);
			l_poThreadShooterApplication->m_poText10->setVisible(false);
			l_poThreadShooterApplication->m_poText100->setVisible(false);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Increment = 5;

		}
		if (l_poCommandKey->getSelectHorizontal() == 3){
			l_poThreadShooterApplication->m_poText1->setVisible(false);
			l_poThreadShooterApplication->m_poText5->setVisible(false);
			l_poThreadShooterApplication->m_poText10->setVisible(true);
			l_poThreadShooterApplication->m_poText100->setVisible(false);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Increment = 10;
		}
		if (l_poCommandKey->getSelectHorizontal() == 4){
			l_poThreadShooterApplication->m_poText1->setVisible(false);
			l_poThreadShooterApplication->m_poText5->setVisible(false);
			l_poThreadShooterApplication->m_poText10->setVisible(false);
			l_poThreadShooterApplication->m_poText100->setVisible(true);
			ret = l_poThreadShooterApplication->m_RobotStaubli->Increment = 100;
		}

	}

	return 0;
}

CCommandKeyBoard::CCommandKeyBoard(CApplication* poApplication)
: ICommandOIS(poApplication)
{

	l_poShooterApplication = dynamic_cast<CShooterApplication*>(m_poApplication);
	args_Handle_KeyBoard *args = new args_Handle_KeyBoard();

	args->l_command = this;
	args->l_shooter = l_poShooterApplication;

	setOrder(0);
	setSelectHorizontal(1);
	setSelectVertical(2);

	pthread_t ManualControl, ControlScreen;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&ManualControl, &attr, handle_ManualControl, args);
	pthread_create(&ControlScreen, &attr, handle_Control_Screen, args);
}

CCommandKeyBoard::~CCommandKeyBoard()
{

}

void CCommandKeyBoard::receiveKeyPressedEvent(const OIS::KeyCode oKey)
{
	int ret = 0;
	int valV, valH;

	if (oKey == OIS::KC_A)
	{
		m_poApplication->getLogManager() << LogLevel_Info << "****** A\n";
		l_poShooterApplication->m_RobotStaubli->Enable_Robot = true;
		pthread_cond_signal(&condition_ControlScreen);
	}

	if (oKey == OIS::KC_S)
	{
		m_poApplication->getLogManager() << LogLevel_Info << "****** S\n";
		l_poShooterApplication->m_RobotStaubli->Enable_Robot = false;
		pthread_cond_signal(&condition_ControlScreen);
	}
	if (oKey == OIS::KC_NUMPADENTER)
	{
		setOrder(10);
		pthread_cond_signal(&condition_ManualControl);
	}
	if (oKey == OIS::KC_NUMPAD9)
	{
		setOrder(9);
		pthread_cond_signal(&condition_ManualControl);
	}

	if (oKey == OIS::KC_NUMPAD8)
	{
		setOrder(8);
		pthread_cond_signal(&condition_ManualControl);
	}

	if (oKey == OIS::KC_NUMPAD7)
	{
		setOrder(7);
		pthread_cond_signal(&condition_ManualControl);
	}

	if (oKey == OIS::KC_NUMPAD6)
	{
		setOrder(6);
		pthread_cond_signal(&condition_ManualControl);
	}

	if (oKey == OIS::KC_NUMPAD5)
	{
		setOrder(5);
		pthread_cond_signal(&condition_ManualControl);
	}

	if (oKey == OIS::KC_NUMPAD4)
	{
		setOrder(4);
		pthread_cond_signal(&condition_ManualControl);
	}

	if (oKey == OIS::KC_NUMPAD3)
	{
		setOrder(3);
		pthread_cond_signal(&condition_ManualControl);
	}

	if (oKey == OIS::KC_NUMPAD2)
	{
		setOrder(2);
		pthread_cond_signal(&condition_ManualControl);
	}

	if (oKey == OIS::KC_NUMPAD1)
	{
		setOrder(1);
		pthread_cond_signal(&condition_ManualControl);
	}

	if (oKey == OIS::KC_UP){

		m_poApplication->getLogManager() << LogLevel_Info << "****** UP\n";
		pthread_cond_signal(&condition_ControlScreen);
		valV = getSelectVertical();
		if (valV < 2){
			valV++;
			setSelectVertical(valV);
		}

		if (viewAxis == 0){
			if (getSelectVertical() == 2){
				//cout << "Recive 0: Cambio de eje XXXXXXX" << endl;
				l_poShooterApplication->m_RobotStaubli->Robot_axis_MODE = ST_AXIS_X;
			}
			else if (getSelectVertical() == 1){
				//cout << "Recive 0: Cambio de eje RRRRRRRRXXXXXXX" << endl;
				l_poShooterApplication->m_RobotStaubli->Robot_axis_MODE = ST_AXIS_RX;
			}
		}
		if (viewAxis == 1){
			if (getSelectVertical() == 2){
				//cout << "Recive 0: Cambio de eje YYYYYYY" << endl;
				l_poShooterApplication->m_RobotStaubli->Robot_axis_MODE = ST_AXIS_Y;
			}
			else if (getSelectVertical() == 1){
				//cout << "Recive 0: Cambio de eje RRRRRYYYYY" << endl;
				l_poShooterApplication->m_RobotStaubli->Robot_axis_MODE = ST_AXIS_RY;
			}
		}
		if (viewAxis == 2){
			if (getSelectVertical() == 2){
				//cout << "Recive 0: Cambio de eje ZZZZZZZ" << endl;
				l_poShooterApplication->m_RobotStaubli->Robot_axis_MODE = ST_AXIS_Z;
			}
			else if (getSelectVertical() == 1){
				//cout << "Recive 0: Cambio de eje RRRRRZZZZZ" << endl;
				l_poShooterApplication->m_RobotStaubli->Robot_axis_MODE = ST_AXIS_RZ;
			}
		}
		pthread_cond_signal(&condition_ControlScreen);

	}
	if (oKey == OIS::KC_DOWN){

		m_poApplication->getLogManager() << LogLevel_Info << "****** DOWN\n";
		pthread_cond_signal(&condition_ControlScreen);
		valV = getSelectVertical();
		if (valV > 1){
			valV--;
			setSelectVertical(valV);
		}
		if (viewAxis == 0){
			if (getSelectVertical() == 2){
				//cout << "Recive 0: Cambio de eje XXXXXXX" << endl;
				l_poShooterApplication->m_RobotStaubli->Robot_axis_MODE = ST_AXIS_X;
			}
			else if (getSelectVertical() == 1){
				//cout << "Recive 0: Cambio de eje RRRRRRRRXXXXXXX" << endl;
				l_poShooterApplication->m_RobotStaubli->Robot_axis_MODE = ST_AXIS_RX;
			}
		}
		if (viewAxis == 1){
			if (getSelectVertical() == 2){
				//cout << "Recive 0: Cambio de eje YYYYYYY" << endl;
				l_poShooterApplication->m_RobotStaubli->Robot_axis_MODE = ST_AXIS_Y;
			}
			else if (getSelectVertical() == 1){
				//cout << "Recive 0: Cambio de eje RRRRRYYYYY" << endl;
				l_poShooterApplication->m_RobotStaubli->Robot_axis_MODE = ST_AXIS_RY;
			}
		}
		if (viewAxis == 2){
			if (getSelectVertical() == 2){
				//cout << "Recive 0: Cambio de eje ZZZZZZZ" << endl;
				l_poShooterApplication->m_RobotStaubli->Robot_axis_MODE = ST_AXIS_Z;
			}
			else if (getSelectVertical() == 1){
				//cout << "Recive 0: Cambio de eje RRRRRZZZZZ" << endl;
				l_poShooterApplication->m_RobotStaubli->Robot_axis_MODE = ST_AXIS_RZ;
			}
		}
		pthread_cond_signal(&condition_ControlScreen);
	}
	if (oKey == OIS::KC_LEFT){

		m_poApplication->getLogManager() << LogLevel_Info << "****** LEFT\n";
		pthread_cond_signal(&condition_ControlScreen);
		valH = getSelectHorizontal();
		if (valH > 1){
			valH--;
			setSelectHorizontal(valH);
		}

	}
	if (oKey == OIS::KC_RIGHT){

		m_poApplication->getLogManager() << LogLevel_Info << "****** RIGHT\n";
		pthread_cond_signal(&condition_ControlScreen);
		valH = getSelectHorizontal();
		if (valH < 4){
			valH++;
			setSelectHorizontal(valH);
		}
	}

	if (oKey == OIS::KC_J){
		l_poShooterApplication->m_RobotStaubli->Robot_TX60->set_electrovalvule(0, true);
		m_poApplication->getLogManager() << LogLevel_Info << "Accionador 0: ON!\n";
	}
	if (oKey == OIS::KC_K){
		l_poShooterApplication->m_RobotStaubli->Robot_TX60->set_electrovalvule(0, false);
		m_poApplication->getLogManager() << LogLevel_Info << "Accionador 0: OFF!\n";
	}
}

void CCommandKeyBoard::receiveKeyReleasedEvent(const OIS::KeyCode oKey)
{
	if (oKey == OIS::KC_A)
	{
	}

}

#endif