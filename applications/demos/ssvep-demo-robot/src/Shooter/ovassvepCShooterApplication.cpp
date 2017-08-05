
#if defined(TARGET_HAS_ThirdPartyOgre3D)

#include "ovassvepCShooterApplication.h"

#include "pthread.h"
#include "../st_Communication.h" 
#include "../ovassvepCCommandStartStop.h"
#include "../ovassvepCCommandStimulatorControl.h"
#include "ovassvepCCommandShipControl.h"
#include "ovassvepKeyBoardCommand.h"
#include "ovassvepRobotCommand.h"


using namespace Ogre;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBESSVEP;

void wait_end_move(communication::STAUBLI* TX60){
	bool bsy = true;
	int ret = 0;
	std::cout << "Waiting for the robot." << std::endl;
	do{
		std::cout << ".";
		ret = TX60->is_arm_busy(&bsy);
	} while (bsy == true);
	std::cout << std::endl;
}

void *handle_Staubli_Move(void *ptr){

	arguments *l_args = (arguments *)ptr;

	std::cout << "Hilo de movimiento del Robot STAUBLI TX60! Estado: Activo" << std::endl;

	int ret = 0;
	int speed = 100;
	bool bussy = false;
	bool error = false;



	do{
		ret = l_args->Robot_TX60->init_remote_control(l_args->RobotHost.c_str(), l_args->RobotPort);
		std::cout << "ERROR: Connection with STAUBLI." << std::endl;
		Sleep(500);

	} while (ret != 0);

	ret = l_args->Robot_TX60->enable_movement(TRUE);
	ret = l_args->Robot_TX60->configure_move_increment(l_args->Increment);
	ret = l_args->Robot_TX60->configure_working_mode(ST_ASYNC);
	ret = l_args->Robot_TX60->configure_system_reference(ST_RELATIVE);

	if (ret == 0) {
		std::cout << "STAUBLI Ready." << std::endl;
	}
	else{
		std::cout << "ERROR: Set parameters STAUBLI." << std::endl;
	}

	while (true){

		// NO Esta testeado!!!!! 
		while (error){
			ret = l_args->Robot_TX60->init_remote_control(l_args->RobotHost.c_str(),l_args->RobotPort);
			std::cout << "ERROR: Try to Re-Connect STAUBLI." << std::endl;
			Sleep(500);
		}
		while (l_args->Enable_Robot == true && !error){
			if (l_args->Enable_Movement == true) {
				ret = l_args->Robot_TX60->configure_move_increment(l_args->Increment);
				if (ret == 0){
					ret = l_args->Robot_TX60->set_move_single_axis(l_args->Robot_axis_MODE, l_args->Robot_direction);
					l_args->Robot_TX60->move_arm();
					wait_end_move(l_args->Robot_TX60);
					printf("\n\n *** Movimiento de Robot: Dirección %d ***\n\n", l_args->Robot_direction);
					l_args->Enable_Movement = false;
				}
				else{
					printf("ERROR: Lost connection with STAUBLI\n");
					error = true;
				}
			}
			else{
				printf("Esperando Oden de Movimiento\n");
			}
			Sleep(500);
		}
		printf("Esperando Start Experimento\n");
		Sleep(500);
	}
	std::cout << "\n******************************************Fuera While***************\n" << std::endl;

	
	return 0;
}



#if !((CEGUI_VERSION_MAJOR > 0) || (CEGUI_VERSION_MINOR >= 8))
namespace CEGUI
{
	typedef CEGUI::UVector2 USize;
};
#endif

CShooterApplication::CShooterApplication()
	: CApplication(),
	  m_poRobot( NULL )
{
}

CShooterApplication::~CShooterApplication()
{
	(*m_poLogManager) << LogLevel_Debug << "- m_poRobot\n";
	//delete m_poShip;
	delete m_poRobot;
}

bool CShooterApplication::setup(OpenViBE::Kernel::IKernelContext* poKernelContext)
{
	CApplication::setup(poKernelContext);

	// Empieza el hilo Robot

	pthread_t MoveRobot;
	pthread_attr_t attr;

	TX60 = new communication::STAUBLI();
	m_RobotStaubli = new arguments();
	m_RobotStaubli->Robot_TX60 = TX60;
	m_RobotStaubli->Robot_axis_MODE = ST_AXIS_X;
	m_RobotStaubli->Increment = 5;
	m_RobotStaubli->Enable_Robot = false;
	m_RobotStaubli->RobotPort = getStaubliRobot_Port();
	//m_RobotStaubli->RobotHost = getStaubliRobot_Host();
	m_RobotStaubli->RobotHost = "192.168.100.33";
	pthread_attr_init(&attr);
	pthread_create(&MoveRobot, &attr, handle_Staubli_Move, m_RobotStaubli);

	//******** PROBLEMA
	//pthread_join(MoveRobot, NULL);


	// Create the StarShip object
	(*m_poLogManager) << LogLevel_Debug << "+ m_poRobot = new CRobotControl(...)\n";
	
	//m_poShip = new CStarShip( this, m_poSceneNode, 0.3f, &m_oFrequencies );
	m_poRobot = new CRobotControl(this, m_poSceneNode, 0.3f, &m_oFrequencies);
	
	// draw the initial text

	drawTextInApp();

	// Create commands
	(*m_poLogManager) << LogLevel_Debug << "+ addCommand(new CCommandStartStop(...)\n";
	this->addCommand(new CCommandStartStop( this ));

	m_poCommandKeyboard = new CCommandKeyBoard(this);

	(*m_poLogManager) << LogLevel_Debug << "+ addCommand(new CCommandKeyBoard(...)\n";
	this->addCommand(m_poCommandKeyboard);

	(*m_poLogManager) << LogLevel_Debug << "+ addCommand(new CCommandStimulatorControl(...))\n";
	this->addCommand(new CCommandStimulatorControl( this ));

	(*m_poLogManager) << LogLevel_Debug << "+ addCommand(new CCommandShipControl(...))\n";
	CCommandShipControl *ShipControl = new CCommandShipControl(this, m_poCommandKeyboard);
	this->addCommand(ShipControl);

	return true;
}

void CShooterApplication::processFrame(OpenViBE::uint32 ui32CurrentFrame)
{
	CApplication::processFrame(ui32CurrentFrame);
	
	m_poRobot->processFrame(ui32CurrentFrame);

}


void CShooterApplication::startExperiment()
{
	CApplication::startExperiment();
	
	m_poInstructionsReady->setVisible( false );
	
	m_RobotStaubli->Enable_Robot = true;
	
	//pthread_cond_signal(&condition_ControlScreen);
}

#endif

void CShooterApplication::drawTextInApp()
{

	m_poInstructionsReady = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "InstructionsReady");
	m_poInstructionsReady->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim(0.0f)));
	m_poInstructionsReady->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 1280.f), CEGUI::UDim(0.0f, 120.f)));

	m_poCoordinateSystem = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "CoordinateSystem");
	m_poCoordinateSystem->setPosition(CEGUI::UVector2(cegui_absdim(430.0f), cegui_absdim(455.0f)));
	m_poCoordinateSystem->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 438.5f), CEGUI::UDim(0.0f, 310.0f)));

	m_poCoordinateSystem_X = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "CoordinateSystem_X");
	m_poCoordinateSystem_X->setPosition(CEGUI::UVector2(cegui_absdim(430.0f), cegui_absdim(455.0f)));
	m_poCoordinateSystem_X->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 438.5f), CEGUI::UDim(0.0f, 310.0f)));

	m_poCoordinateSystem_Y = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "CoordinateSystem_Y");
	m_poCoordinateSystem_Y->setPosition(CEGUI::UVector2(cegui_absdim(430.0f), cegui_absdim(455.0f)));
	m_poCoordinateSystem_Y->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 438.5f), CEGUI::UDim(0.0f, 310.0f)));

	m_poCoordinateSystem_Z = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "CoordinateSystem_Z");
	m_poCoordinateSystem_Z->setPosition(CEGUI::UVector2(cegui_absdim(430.0f), cegui_absdim(455.0f)));
	m_poCoordinateSystem_Z->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 438.5f), CEGUI::UDim(0.0f, 310.0f)));

	m_poCoordinateSystem_alfa = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "CoordinateSystem_alfa");
	m_poCoordinateSystem_alfa->setPosition(CEGUI::UVector2(cegui_absdim(430.0f), cegui_absdim(455.0f)));
	m_poCoordinateSystem_alfa->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 438.5f), CEGUI::UDim(0.0f, 310.0f)));

	m_poCoordinateSystem_beta = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "CoordinateSystem_beta");
	m_poCoordinateSystem_beta->setPosition(CEGUI::UVector2(cegui_absdim(430.0f), cegui_absdim(455.0f)));
	m_poCoordinateSystem_beta->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 438.5f), CEGUI::UDim(0.0f, 310.0f)));

	m_poCoordinateSystem_gamma = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "CoordinateSystem_gamma");
	m_poCoordinateSystem_gamma->setPosition(CEGUI::UVector2(cegui_absdim(430.0f), cegui_absdim(455.0f)));
	m_poCoordinateSystem_gamma->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 438.5f), CEGUI::UDim(0.0f, 310.0f)));

	m_poUPV = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "UPV");
	m_poUPV->setPosition(CEGUI::UVector2(cegui_absdim(875.0f), cegui_absdim(10.0f)));
	m_poUPV->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 402.0f), CEGUI::UDim(0.0f, 142.0f)));
	//  ---- Inicialización variables de control de posición ----

	m_poTextX = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextX");
	m_poTextX->setPosition(CEGUI::UVector2(cegui_absdim(350.0f), cegui_absdim(10.0f)));
	m_poTextX->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	m_poTextX_G = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextX_G");
	m_poTextX_G->setPosition(CEGUI::UVector2(cegui_absdim(350.0f), cegui_absdim(10.0f)));
	m_poTextX_G->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	m_poTextY = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextY");
	m_poTextY->setPosition(CEGUI::UVector2(cegui_absdim(350.0f), cegui_absdim(60.0f)));
	m_poTextY->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	m_poTextY_G = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextY_G");
	m_poTextY_G->setPosition(CEGUI::UVector2(cegui_absdim(350.0f), cegui_absdim(60.0f)));
	m_poTextY_G->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	m_poTextZ = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextZ");
	m_poTextZ->setPosition(CEGUI::UVector2(cegui_absdim(350.0f), cegui_absdim(110.0f)));
	m_poTextZ->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	m_poTextZ_G = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextZ_G");
	m_poTextZ_G->setPosition(CEGUI::UVector2(cegui_absdim(350.0f), cegui_absdim(110.0f)));
	m_poTextZ_G->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	
	//  ---- Inicialización variables de control de orientación ----

	m_poTextAlpha = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextAlpha");
	m_poTextAlpha->setPosition(CEGUI::UVector2(cegui_absdim(350.0f), cegui_absdim(5.0f)));
	m_poTextAlpha->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));
	
	m_poTextAlpha_G = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextAlpha_G");
	m_poTextAlpha_G->setPosition(CEGUI::UVector2(cegui_absdim(350.0f), cegui_absdim(5.0f)));
	m_poTextAlpha_G->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	m_poTextBeta = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextBeta");
	m_poTextBeta->setPosition(CEGUI::UVector2(cegui_absdim(350.0f), cegui_absdim(55.0f)));
	m_poTextBeta->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	m_poTextBeta_G = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextBeta_G");
	m_poTextBeta_G->setPosition(CEGUI::UVector2(cegui_absdim(350.0f), cegui_absdim(55.0f)));
	m_poTextBeta_G->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	m_poTextGamma = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextGamma");
	m_poTextGamma->setPosition(CEGUI::UVector2(cegui_absdim(350.0f), cegui_absdim(105.0f)));
	m_poTextGamma->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	m_poTextGamma_G = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextGamma_G");
	m_poTextGamma_G->setPosition(CEGUI::UVector2(cegui_absdim(350.0f), cegui_absdim(105.0f)));
	m_poTextGamma_G->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	//  ---- Inicialización variables de selección ----

	m_poTextPosicion = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextPosicion");
	m_poTextPosicion->setPosition(CEGUI::UVector2(cegui_absdim(-20.0f), cegui_absdim(10.0f)));
	m_poTextPosicion->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	m_poTextPosicion_G = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextPosicion_G");
	m_poTextPosicion_G->setPosition(CEGUI::UVector2(cegui_absdim(-20.0f), cegui_absdim(10.0f)));
	m_poTextPosicion_G->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	m_poTextOrientacion= m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextOrientacion");
	m_poTextOrientacion->setPosition(CEGUI::UVector2(cegui_absdim(0.0f), cegui_absdim(60.0f)));
	m_poTextOrientacion->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	m_poTextOrientacion_G = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextOrientacion_G");
	m_poTextOrientacion_G->setPosition(CEGUI::UVector2(cegui_absdim(0.0f), cegui_absdim(60.0f)));
	m_poTextOrientacion_G->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	
	//  ---- Inicialización valores de precisión ----

	m_poText1= m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "Text1");
	m_poText1->setPosition(CEGUI::UVector2(cegui_absdim(150.0f), cegui_absdim(110.0f)));
	m_poText1->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	m_poText1_G= m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "Text1_G");
	m_poText1_G->setPosition(CEGUI::UVector2(cegui_absdim(150.0f), cegui_absdim(110.0f)));
	m_poText1_G->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	m_poText5= m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "Text5");
	m_poText5->setPosition(CEGUI::UVector2(cegui_absdim(150.0f), cegui_absdim(110.0f)));
	m_poText5->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	m_poText5_G= m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "Text5_G");
	m_poText5_G->setPosition(CEGUI::UVector2(cegui_absdim(150.0f), cegui_absdim(110.0f)));
	m_poText5_G->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	m_poText10= m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "Text10");
	m_poText10->setPosition(CEGUI::UVector2(cegui_absdim(150.0f), cegui_absdim(110.0f)));
	m_poText10->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	m_poText10_G= m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "Text10_G");
	m_poText10_G->setPosition(CEGUI::UVector2(cegui_absdim(150.0f), cegui_absdim(110.0f)));
	m_poText10_G->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));
	
	m_poText100= m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "Text100");
	m_poText100->setPosition(CEGUI::UVector2(cegui_absdim(150.0f), cegui_absdim(110.0f)));
	m_poText100->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	m_poText100_G= m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "Text100_G");
	m_poText100_G->setPosition(CEGUI::UVector2(cegui_absdim(150.0f), cegui_absdim(110.0f)));
	m_poText100_G->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	
	//  ---- Inicialización variable de informacion ----

	m_poTextActivo= m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextActivo");
	m_poTextActivo->setPosition(CEGUI::UVector2(cegui_absdim(600.0f), cegui_absdim(10.0f)));
	m_poTextActivo->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 472.f), CEGUI::UDim(0.0f, 118.f)));

	m_poTextActivo_G= m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextActivo_G");
	m_poTextActivo_G->setPosition(CEGUI::UVector2(cegui_absdim(600.0f), cegui_absdim(10.0f)));
	m_poTextActivo_G->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 472.f), CEGUI::UDim(0.0f, 118.f)));

	m_poTextDesactivo= m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextDesactivo");
	m_poTextDesactivo->setPosition(CEGUI::UVector2(cegui_absdim(550.0f), cegui_absdim(10.0f)));
	m_poTextDesactivo->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 472.f), CEGUI::UDim(0.0f, 118.f)));

	m_poTextDesactivo_G= m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextDesactivo_G");
	m_poTextDesactivo_G->setPosition(CEGUI::UVector2(cegui_absdim(550.0f), cegui_absdim(10.0f)));
	m_poTextDesactivo_G->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 472.f), CEGUI::UDim(0.0f, 118.f)));

	m_poTextError= m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextError");
	m_poTextError->setPosition(CEGUI::UVector2(cegui_absdim(600.0f), cegui_absdim(10.0f)));
	m_poTextError->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 472.f), CEGUI::UDim(0.0f, 118.f)));

	m_poTextError_G= m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextError_G");
	m_poTextError_G->setPosition(CEGUI::UVector2(cegui_absdim(600.0f), cegui_absdim(10.0f)));
	m_poTextError_G->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 472.f), CEGUI::UDim(0.0f, 118.f)));

	m_poTextPrecision= m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextPrecision");
	m_poTextPrecision->setPosition(CEGUI::UVector2(cegui_absdim(20.0f), cegui_absdim(110.0f)));
	m_poTextPrecision->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));

	m_poTextPrecision_G= m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "TextPrecision_G");
	m_poTextPrecision_G->setPosition(CEGUI::UVector2(cegui_absdim(20.0f), cegui_absdim(110.0f)));
	m_poTextPrecision_G->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 236.f), CEGUI::UDim(0.0f, 59.f)));


#if (CEGUI_VERSION_MAJOR > 0) || (CEGUI_VERSION_MINOR >= 8)
	m_poSheet->addChild(m_poInstructionsReady);
	CEGUI::ImageManager::getSingleton().addFromImageFile("InstructionsReady", "InstructionText-Start-Blac.png");
	m_poInstructionsReady->setProperty("Image", "InstructionsReady");

	m_poSheet->addChild(m_poUPV);
	CEGUI::ImageManager::getSingleton().addFromImageFile("UPV", "UPV.png");
	m_poUPV->setProperty("Image", "UPV");

	m_poSheet->addChild(m_poCoordinateSystem);
	CEGUI::ImageManager::getSingleton().addFromImageFile("CoordinateSystem", "ejes.png");
	m_poCoordinateSystem->setProperty("Image", "CoordinateSystem");

	m_poSheet->addChild(m_poCoordinateSystem_X);
	CEGUI::ImageManager::getSingleton().addFromImageFile("CoordinateSystem_X", "eje_x.png");
	m_poCoordinateSystem_X->setProperty("Image", "CoordinateSystem_X");

	m_poSheet->addChild(m_poCoordinateSystem_Y);
	CEGUI::ImageManager::getSingleton().addFromImageFile("CoordinateSystem_Y", "eje_y.png");
	m_poCoordinateSystem_Y->setProperty("Image", "CoordinateSystem_Y");

	m_poSheet->addChild(m_poCoordinateSystem_Z);
	CEGUI::ImageManager::getSingleton().addFromImageFile("CoordinateSystem_Z", "eje_z.png");
	m_poCoordinateSystem_Z->setProperty("Image", "CoordinateSystem_Z");

	m_poSheet->addChild(m_poCoordinateSystem_alfa);
	CEGUI::ImageManager::getSingleton().addFromImageFile("CoordinateSystem_alfa", "alfa.png");
	m_poCoordinateSystem_alfa->setProperty("Image", "CoordinateSystem_alfa");

	m_poSheet->addChild(m_poCoordinateSystem_beta);
	CEGUI::ImageManager::getSingleton().addFromImageFile("CoordinateSystem_beta", "beta.png");
	m_poCoordinateSystem_beta->setProperty("Image", "CoordinateSystem_beta");

	m_poSheet->addChild(m_poCoordinateSystem_gamma);
	CEGUI::ImageManager::getSingleton().addFromImageFile("CoordinateSystem_gamma", "gamma.png");
	m_poCoordinateSystem_gamma->setProperty("Image", "CoordinateSystem_gamma");

	m_poSheet->addChild(m_poTextX);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextX", "TextX.png");
	m_poTextX->setProperty("Image", "TextX");

	m_poSheet->addChild(m_poTextY);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextY", "TextY.png");
	m_poTextY->setProperty("Image", "TextY");

	m_poSheet->addChild(m_poTextZ);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextZ", "TextZ.png");
	m_poTextZ->setProperty("Image", "TextZ");
	
	m_poSheet->addChild(m_poTextX_G);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextX_G", "TextX_G.png");
	m_poTextX_G->setProperty("Image", "TextX_G");

	m_poSheet->addChild(m_poTextY_G);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextY_G", "TextY_G.png");
	m_poTextY_G->setProperty("Image", "TextY_G");

	m_poSheet->addChild(m_poTextZ_G);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextZ_G", "TextZ_G.png");
	m_poTextZ_G->setProperty("Image", "TextZ_G");

	m_poSheet->addChild(m_poTextAlpha);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextAlpha", "TextAlpha.png");
	m_poTextAlpha->setProperty("Image", "TextAlpha");
	
	m_poSheet->addChild(m_poTextBeta);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextBeta", "TextBeta.png");
	m_poTextBeta->setProperty("Image", "TextBeta");

	m_poSheet->addChild(m_poTextGamma);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextGamma", "TextGamma.png");
	m_poTextGamma->setProperty("Image", "TextGamma");

	m_poSheet->addChild(m_poTextAlpha_G);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextAlpha_G", "TextAlpha_G.png");
	m_poTextAlpha_G->setProperty("Image", "TextAlpha_G");

	m_poSheet->addChild(m_poTextBeta_G);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextBeta_G", "TextBeta_G.png");
	m_poTextBeta_G->setProperty("Image", "TextBeta_G");

	m_poSheet->addChild(m_poTextGamma_G);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextGamma_G", "TextGamma_G.png");
	m_poTextGamma_G->setProperty("Image", "TextGamma_G");
	
	m_poSheet->addChild(m_poText1);
	CEGUI::ImageManager::getSingleton().addFromImageFile("Text1", "Text1.png");
	m_poText1->setProperty("Image", "Text1");

	m_poSheet->addChild(m_poText1_G);
	CEGUI::ImageManager::getSingleton().addFromImageFile("Text1_G", "Text1_G.png");
	m_poText1_G->setProperty("Image", "Text1_G");

	m_poSheet->addChild(m_poText5);
	CEGUI::ImageManager::getSingleton().addFromImageFile("Text5", "Text5.png");
	m_poText5->setProperty("Image", "Text5");

	m_poSheet->addChild(m_poText5_G);
	CEGUI::ImageManager::getSingleton().addFromImageFile("Text5_G", "Text5_G.png");
	m_poText5_G->setProperty("Image", "Text5_G");

	m_poSheet->addChild(m_poText10);
	CEGUI::ImageManager::getSingleton().addFromImageFile("Text10", "Text10.png");
	m_poText10->setProperty("Image", "Text10");

	m_poSheet->addChild(m_poText10_G);
	CEGUI::ImageManager::getSingleton().addFromImageFile("Text10_G", "Text10_G.png");
	m_poText10_G->setProperty("Image", "Text10_G");

	m_poSheet->addChild(m_poText100);
	CEGUI::ImageManager::getSingleton().addFromImageFile("Text100", "Text100.png");
	m_poText100->setProperty("Image", "Text100");

	m_poSheet->addChild(m_poText100_G);
	CEGUI::ImageManager::getSingleton().addFromImageFile("Text100_G", "Text100_G.png");
	m_poText100_G->setProperty("Image", "Text100_G");

	// ----
	
	m_poSheet->addChild(m_poTextPosicion);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextPosicion", "TextPosicion.png");
	m_poTextPosicion->setProperty("Image", "TextPosicion");

	m_poSheet->addChild(m_poTextPosicion_G);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextPosicion_G", "TextPosicion_G.png");
	m_poTextPosicion_G->setProperty("Image", "TextPosicion_G");

	m_poSheet->addChild(m_poTextOrientacion);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextOrientacion", "TextOrientacion.png");
	m_poTextOrientacion->setProperty("Image", "TextOrientacion");

	m_poSheet->addChild(m_poTextOrientacion_G);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextOrientacion_G", "TextOrientacion_G.png");
	m_poTextOrientacion_G->setProperty("Image", "TextOrientacion_G");

	//---

	m_poSheet->addChild(m_poTextActivo);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextActivo", "TextActivo.png");
	m_poTextActivo->setProperty("Image", "TextActivo");

	m_poSheet->addChild(m_poTextActivo_G);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextActivo_G", "TextActivo_G.png");
	m_poTextActivo_G->setProperty("Image", "TextActivo_G");

	m_poSheet->addChild(m_poTextDesactivo);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextDesactivo", "TextDesactivo.png");
	m_poTextDesactivo->setProperty("Image", "TextDesactivo");

	m_poSheet->addChild(m_poTextDesactivo_G);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextDesactivo_G", "TextDesactivo_G.png");
	m_poTextDesactivo_G->setProperty("Image", "TextDesactivo_G");

	m_poSheet->addChild(m_poTextError);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextError", "TextError.png");
	m_poTextError->setProperty("Image", "TextError");

	m_poSheet->addChild(m_poTextError_G);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextError_G", "TextError_G.png");
	m_poTextError_G->setProperty("Image", "TextError_G");

	m_poSheet->addChild(m_poTextPrecision);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextPrecision", "TextPrecision.png");
	m_poTextPrecision->setProperty("Image", "TextPrecision");

	m_poSheet->addChild(m_poTextPrecision_G);
	CEGUI::ImageManager::getSingleton().addFromImageFile("TextPrecision_G", "TextPrecision_G.png");
	m_poTextPrecision_G->setProperty("Image", "TextPrecision_G");

#else
	m_poSheet->addChildWindow(m_poInstructionsReady);
	CEGUI::ImagesetManager::getSingleton().createFromImageFile("InstructionsReady", "InstructionText-Start.png");
	m_poInstructionsReady->setProperty("Image", "set:InstructionsReady image:full_image");
#endif

	m_poInstructionsReady->setProperty("FrameEnabled", "False");
	m_poInstructionsReady->setProperty("BackgroundEnabled", "False");
	m_poInstructionsReady->setVisible(false);
	
	m_poUPV->setProperty("FrameEnabled", "False");
	m_poUPV->setProperty("BackgroundEnabled", "False");
	m_poUPV->setVisible(true);

	m_poCoordinateSystem->setProperty("FrameEnabled", "False");
	m_poCoordinateSystem->setProperty("BackgroundEnabled", "False");
	m_poCoordinateSystem->setVisible(true);

	m_poCoordinateSystem_X->setProperty("FrameEnabled", "False");
	m_poCoordinateSystem_X->setProperty("BackgroundEnabled", "False");
	m_poCoordinateSystem_X->setVisible(false);

	m_poCoordinateSystem_Y->setProperty("FrameEnabled", "False");
	m_poCoordinateSystem_Y->setProperty("BackgroundEnabled", "False");
	m_poCoordinateSystem_Y->setVisible(false);

	m_poCoordinateSystem_Z->setProperty("FrameEnabled", "False");
	m_poCoordinateSystem_Z->setProperty("BackgroundEnabled", "False");
	m_poCoordinateSystem_Z->setVisible(false);

	m_poCoordinateSystem_alfa->setProperty("FrameEnabled", "False");
	m_poCoordinateSystem_alfa->setProperty("BackgroundEnabled", "False");
	m_poCoordinateSystem_alfa->setVisible(false);

	m_poCoordinateSystem_beta->setProperty("FrameEnabled", "False");
	m_poCoordinateSystem_beta->setProperty("BackgroundEnabled", "False");
	m_poCoordinateSystem_beta->setVisible(false);

	m_poCoordinateSystem_gamma->setProperty("FrameEnabled", "False");
	m_poCoordinateSystem_gamma->setProperty("BackgroundEnabled", "False");
	m_poCoordinateSystem_gamma->setVisible(false);

	m_poTextX->setProperty("FrameEnabled", "False");
	m_poTextX->setProperty("BackgroundEnabled", "False");
	m_poTextX->setVisible(false);

	m_poTextY->setProperty("FrameEnabled", "False");
	m_poTextY->setProperty("BackgroundEnabled", "False");
	m_poTextY->setVisible(true);

	m_poTextZ->setProperty("FrameEnabled", "False");
	m_poTextZ->setProperty("BackgroundEnabled", "False");
	m_poTextZ->setVisible(true);

	m_poTextX_G->setProperty("FrameEnabled", "False");
	m_poTextX_G->setProperty("BackgroundEnabled", "False");
	m_poTextX_G->setVisible(true);

	m_poTextY_G->setProperty("FrameEnabled", "False");
	m_poTextY_G->setProperty("BackgroundEnabled", "False");
	m_poTextY_G->setVisible(false);

	m_poTextZ_G->setProperty("FrameEnabled", "False");
	m_poTextZ_G->setProperty("BackgroundEnabled", "False");
	m_poTextZ_G->setVisible(false);

	m_poTextAlpha->setProperty("FrameEnabled", "False");
	m_poTextAlpha->setProperty("BackgroundEnabled", "False");
	m_poTextAlpha->setVisible(false);

	m_poTextBeta->setProperty("FrameEnabled", "False");
	m_poTextBeta->setProperty("BackgroundEnabled", "False");
	m_poTextBeta->setVisible(false);

	m_poTextGamma->setProperty("FrameEnabled", "False");
	m_poTextGamma->setProperty("BackgroundEnabled", "False");
	m_poTextGamma->setVisible(false);
	
	m_poTextAlpha_G->setProperty("FrameEnabled", "False");
	m_poTextAlpha_G->setProperty("BackgroundEnabled", "False");
	m_poTextAlpha_G->setVisible(false);

	m_poTextBeta_G->setProperty("FrameEnabled", "False");
	m_poTextBeta_G->setProperty("BackgroundEnabled", "False");
	m_poTextBeta_G->setVisible(false);

	m_poTextGamma_G->setProperty("FrameEnabled", "False");
	m_poTextGamma_G->setProperty("BackgroundEnabled", "False");
	m_poTextGamma_G->setVisible(false);
	
	m_poText1->setProperty("FrameEnabled", "False");
	m_poText1->setProperty("BackgroundEnabled", "False");
	m_poText1->setVisible(false);

	m_poText5->setProperty("FrameEnabled", "False");
	m_poText5->setProperty("BackgroundEnabled", "False");
	m_poText5->setVisible(true);

	m_poText10->setProperty("FrameEnabled", "False");
	m_poText10->setProperty("BackgroundEnabled", "False");
	m_poText10->setVisible(false);

	m_poText100->setProperty("FrameEnabled", "False");
	m_poText100->setProperty("BackgroundEnabled", "False");
	m_poText100->setVisible(false);

	m_poText1_G->setProperty("FrameEnabled", "False");
	m_poText1_G->setProperty("BackgroundEnabled", "False");
	m_poText1_G->setVisible(false);

	m_poText5_G->setProperty("FrameEnabled", "False");
	m_poText5_G->setProperty("BackgroundEnabled", "False");
	m_poText5_G->setVisible(false);

	m_poText10_G->setProperty("FrameEnabled", "False");
	m_poText10_G->setProperty("BackgroundEnabled", "False");
	m_poText10_G->setVisible(false);

	m_poText100_G->setProperty("FrameEnabled", "False");
	m_poText100_G->setProperty("BackgroundEnabled", "False");
	m_poText100_G->setVisible(false);

	m_poTextPosicion->setProperty("FrameEnabled", "False");
	m_poTextPosicion->setProperty("BackgroundEnabled", "False");
	m_poTextPosicion->setVisible(false);

	m_poTextPosicion_G->setProperty("FrameEnabled", "False");
	m_poTextPosicion_G->setProperty("BackgroundEnabled", "False");
	m_poTextPosicion_G->setVisible(true);

	m_poTextOrientacion->setProperty("FrameEnabled", "False");
	m_poTextOrientacion->setProperty("BackgroundEnabled", "False");
	m_poTextOrientacion->setVisible(true);

	m_poTextOrientacion_G->setProperty("FrameEnabled", "False");
	m_poTextOrientacion_G->setProperty("BackgroundEnabled", "False");
	m_poTextOrientacion_G->setVisible(false);

	m_poTextActivo->setProperty("FrameEnabled", "False");
	m_poTextActivo->setProperty("BackgroundEnabled", "False");
	m_poTextActivo->setVisible(false);

	m_poTextDesactivo->setProperty("FrameEnabled", "False");
	m_poTextDesactivo->setProperty("BackgroundEnabled", "False");
	m_poTextDesactivo->setVisible(false);

	m_poTextError->setProperty("FrameEnabled", "False");
	m_poTextError->setProperty("BackgroundEnabled", "False");
	m_poTextError->setVisible(false);

	m_poTextPrecision->setProperty("FrameEnabled", "False");
	m_poTextPrecision->setProperty("BackgroundEnabled", "False");
	m_poTextPrecision->setVisible(true);

	m_poTextActivo_G->setProperty("FrameEnabled", "False");
	m_poTextActivo_G->setProperty("BackgroundEnabled", "False");
	m_poTextActivo_G->setVisible(false);

	m_poTextDesactivo_G->setProperty("FrameEnabled", "False");
	m_poTextDesactivo_G->setProperty("BackgroundEnabled", "False");
	m_poTextDesactivo_G->setVisible(true);

	m_poTextError_G->setProperty("FrameEnabled", "False");
	m_poTextError_G->setProperty("BackgroundEnabled", "False");
	m_poTextError_G->setVisible(false);

	m_poTextPrecision_G->setProperty("FrameEnabled", "False");
	m_poTextPrecision_G->setProperty("BackgroundEnabled", "False");
	m_poTextPrecision_G->setVisible(false);
	

}
