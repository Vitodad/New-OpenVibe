#ifndef __OpenViBEApplication_CShooterApplication_H__
#define __OpenViBEApplication_CShooterApplication_H__

#include <iostream>

#include <openvibe/ov_all.h>
#include <toolkit/ovtk_all.h>


#include "../st_Communication.h"
#include "../ovassvepCApplication.h"
#include "ovassvepRobotCommand.h"

struct arguments {

	communication::STAUBLI* Robot_TX60;
	int Robot_axis_MODE;
	int Robot_direction;
	bool Enable_Movement;
	bool Enable_Robot;
	int Increment;
	char *Origen;
	std::string RobotHost;
	int RobotPort;

};

namespace OpenViBESSVEP
{
	
	class CShooterApplication : public CApplication
	{
		public:

			CShooterApplication();
			~CShooterApplication();

			bool setup(OpenViBE::Kernel::IKernelContext* poKernelContext);

			void startExperiment();

			void drawTextInApp();

			arguments* m_RobotStaubli;
		
		    void CShooterApplication::setRobotProperties(void * ptr){
				m_RobotStaubli = (arguments *) ptr;
			}

			arguments* CShooterApplication::getRobotProperties(){
				return(m_RobotStaubli);
			}

			communication::STAUBLI* TX60;

			CEGUI::Window* m_poInstructionsReady, *m_poUPV;
			CEGUI::Window* m_poCoordinateSystem;
			CEGUI::Window* m_poCoordinateSystem_X, *m_poCoordinateSystem_Y, *m_poCoordinateSystem_Z;
			CEGUI::Window* m_poCoordinateSystem_alfa, *m_poCoordinateSystem_beta, *m_poCoordinateSystem_gamma;
			CEGUI::Window* m_poTextX, *m_poTextY, *m_poTextZ;
			CEGUI::Window* m_poTextX_G, *m_poTextY_G, *m_poTextZ_G;
			CEGUI::Window* m_poTextAlpha, *m_poTextBeta, *m_poTextGamma;
			CEGUI::Window* m_poTextAlpha_G, *m_poTextBeta_G, *m_poTextGamma_G;
			CEGUI::Window* m_poTextPosicion, *m_poTextPosicion_G, *m_poTextOrientacion, *m_poTextOrientacion_G;
			CEGUI::Window* m_poText1, *m_poText5, *m_poText10, *m_poText100;
			CEGUI::Window* m_poText1_G, *m_poText5_G, *m_poText10_G, *m_poText100_G;
			CEGUI::Window* m_poTextActivo, *m_poTextDesactivo, *m_poTextError, *m_poTextPrecision;
			CEGUI::Window* m_poTextActivo_G, *m_poTextDesactivo_G, *m_poTextError_G, *m_poTextPrecision_G;
	
		class CCommandKeyBoard* m_poCommandKeyboard;

		private:

			bool m_bActive;
			void processFrame(OpenViBE::uint32 ui32CurrentFrame);
			CRobotControl* m_poRobot;
			

	};
}

#endif // __OpenViBEApplication_CShooterApplication_H__
