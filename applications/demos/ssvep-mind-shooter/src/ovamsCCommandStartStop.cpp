
#if defined(TARGET_HAS_ThirdPartyOgre3DTerrain)

#include "ovamsCCommandStartStop.h"
#include "ovamsCApplication.h"
#include "ovamsCVRPNServer.h"

using namespace OpenViBESSVEPMindShooter;
using namespace OpenViBE::Kernel;

CCommandStartStop::CCommandStartStop(CApplication* poApplication)
	: ICommandOIS(poApplication)
{
	m_poVRPNServer = CVRPNServer::getInstance(poApplication);
	m_poVRPNServer->addButton("SSVEP_VRPN_StartStop", 2);
}

CCommandStartStop::~CCommandStartStop()
{
	m_poApplication->getLogManager() << LogLevel_Info << "End message sent\n";
	m_poVRPNServer->changeButtonState("SSVEP_VRPN_StartStop", 1, 1);
	m_poVRPNServer->processFrame();
}

void CCommandStartStop::processFrame()
{
	ICommandOIS::processFrame();
	m_poVRPNServer->processFrame();
}

void CCommandStartStop::receiveKeyPressedEvent( const OIS::KeyCode oKey )
{
	if (oKey == OIS::KC_SPACE)
	{
		m_poApplication->getLogManager() << LogLevel_Info << "Start message sent\n";

		m_poVRPNServer->changeButtonState("SSVEP_VRPN_StartStop", 0, 1);
	}	

	if (oKey == OIS::KC_ESCAPE)
	{
		m_poApplication->exit();
	}

	if (oKey == OIS::KC_INSERT)
	{
		m_poApplication->getWindow()->writeContentsToFile("screenshot.png");
	}
}

void CCommandStartStop::receiveKeyReleasedEvent( const OIS::KeyCode oKey )
{
	if (oKey == OIS::KC_SPACE)
	{
		m_poVRPNServer->changeButtonState("SSVEP_VRPN_StartStop", 0, 0);
	}	
}

#endif