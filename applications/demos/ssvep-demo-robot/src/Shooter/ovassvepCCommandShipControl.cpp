
#if defined(TARGET_HAS_ThirdPartyOgre3D)

#include "ovassvepCCommandShipControl.h"
#include "ovassvepCShooterApplication.h"


using namespace OpenViBESSVEP;
using namespace std;




CCommandShipControl::CCommandShipControl(CShooterApplication* poApplication, CCommandKeyBoard* m_poCommandKeyboard)
	: ICommandVRPNButton(poApplication, "SSVEP_VRPN_ShipControl")
{
	l_poCommandKeyBoard = m_poCommandKeyboard;
}


void CCommandShipControl::execute(int iButton, int iState)
{
	CShooterApplication* l_poShooterApplication = dynamic_cast<CShooterApplication*>(m_poApplication);

	InInterrupt = clock();

	switch (iButton)
	{
		case 0:
			if (InInterrupt - InCase0 > 4000){
				cout << "Recive 0: Cambio de eje" << endl;

				if (l_poCommandKeyBoard->getSelectAxis() == 0){
					if (l_poCommandKeyBoard->getSelectVertical() == 2){
						//cout << "Recive 0: Cambio de eje XXXXXXX" << endl;
						l_poShooterApplication->m_RobotStaubli->Robot_axis_MODE = ST_AXIS_X;
					}
					else if (l_poCommandKeyBoard->getSelectVertical() == 1){
						//cout << "Recive 0: Cambio de eje RRRRRRRRXXXXXXX" << endl;
						l_poShooterApplication->m_RobotStaubli->Robot_axis_MODE = ST_AXIS_RX;
					}
					l_poShooterApplication->m_RobotStaubli->Enable_Movement = false;
					l_poCommandKeyBoard->setSelectAxis(l_poCommandKeyBoard->getSelectAxis() + 1);
					l_poCommandKeyBoard->viewAxis = 0;

				}
				else if (l_poCommandKeyBoard->getSelectAxis() == 1){

					if (l_poCommandKeyBoard->getSelectVertical() == 2){
						//cout << "Recive 0: Cambio de eje YYYYYYY" << endl;
						l_poShooterApplication->m_RobotStaubli->Robot_axis_MODE = ST_AXIS_Y;
					}
					else if (l_poCommandKeyBoard->getSelectVertical() == 1){
						//cout << "Recive 0: Cambio de eje RRRRRYYYYY" << endl;
						l_poShooterApplication->m_RobotStaubli->Robot_axis_MODE = ST_AXIS_RY;
					}
					l_poShooterApplication->m_RobotStaubli->Enable_Movement = false;
					l_poCommandKeyBoard->setSelectAxis(l_poCommandKeyBoard->getSelectAxis()+1);
					l_poCommandKeyBoard->viewAxis = 1;
				}
				else if (l_poCommandKeyBoard->getSelectAxis() == 2){

					if (l_poCommandKeyBoard->getSelectVertical() == 2){
						//cout << "Recive 0: Cambio de eje ZZZZZZZ" << endl;
						l_poShooterApplication->m_RobotStaubli->Robot_axis_MODE = ST_AXIS_Z;
					}
					else if (l_poCommandKeyBoard->getSelectVertical() == 1){
						//cout << "Recive 0: Cambio de eje RRRRRZZZZZ" << endl;
						l_poShooterApplication->m_RobotStaubli->Robot_axis_MODE = ST_AXIS_RZ;
					}
					l_poShooterApplication->m_RobotStaubli->Enable_Movement = false;
					l_poCommandKeyBoard->setSelectAxis(0);
					l_poCommandKeyBoard->viewAxis = 2;
				}
				InCase0 = clock();
			}
			pthread_cond_signal(&l_poCommandKeyBoard->condition_ControlScreen);
			break;
		case 1:
			if (InInterrupt - InCase1> 100){
				cout << "Recive 1: Movimiento negativo" << endl;
				l_poShooterApplication->m_RobotStaubli->Robot_direction = ST_NEGATIVE_MOV;
				l_poShooterApplication->m_RobotStaubli->Enable_Movement = true;
				InCase1 = clock();
				
			}
			break;
		case 2:
			if (InInterrupt - InCase2 > 100){
				cout << "Recive 2: Movimiento positivo" << endl;
				l_poShooterApplication->m_RobotStaubli->Robot_direction = ST_POSITIVE_MOV;
				l_poShooterApplication->m_RobotStaubli->Enable_Movement = true;
				InCase2 = clock();
			}
			break;
	}
	m_poApplication = l_poShooterApplication;
}

#endif