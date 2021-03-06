
#if defined(TARGET_HAS_ThirdPartyOgre3D)

#include "ovassvepCTrainerApplication.h"

#include "../ovassvepCCommandStartStop.h"
#include "../ovassvepCCommandStimulatorControl.h"
#include "ovassvepCCommandReceiveTarget.h"

using namespace Ogre;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBESSVEP;

#if !((CEGUI_VERSION_MAJOR > 0) || (CEGUI_VERSION_MINOR >= 8))
namespace CEGUI
{
	typedef CEGUI::UVector2 USize;
};
#endif

CTrainerApplication::CTrainerApplication() :
CApplication(),
m_bActive(false),
m_poInstructionsReady(NULL)
{
}

bool CTrainerApplication::setup(OpenViBE::Kernel::IKernelContext* poKernelContext)
{
	CApplication::setup(poKernelContext);


	(*m_poLogManager) << LogLevel_Debug << "  * CTrainerApplication::setup()\n";


	IConfigurationManager* l_poConfigurationManager = &(m_poKernelContext->getConfigurationManager());

	CTrainerFlickeringObject::initialize(this);

	// paint targets

	OpenViBE::uint32 l_ui32TargetCount = (OpenViBE::uint32)(l_poConfigurationManager->expandAsUInteger("${SSVEP_TargetCount}"));

	for (OpenViBE::uint32 i = 0; i < l_ui32TargetCount; i++)
	{
		this->addObject(CTrainerFlickeringObject::createTrainerFlickeringObject(i));
	}


	// draw the initial text
	m_poInstructionsReady = m_poGUIWindowManager->createWindow("TaharezLook/StaticImage", "InstructionsReady");
	m_poInstructionsReady->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim(0.0f)));
	m_poInstructionsReady->setSize(CEGUI::USize(CEGUI::UDim(0.0f, 1280.f), CEGUI::UDim(0.0f, 120.f)));


#if (CEGUI_VERSION_MAJOR > 0) || (CEGUI_VERSION_MINOR >= 8)
	m_poSheet->addChild(m_poInstructionsReady);
	CEGUI::ImageManager::getSingleton().addFromImageFile("InstructionsReady", "InstructionText-Start-Black.png");
	m_poInstructionsReady->setProperty("Image", "InstructionsReady");
#else
	m_poSheet->addChildWindow(m_poInstructionsReady);
	CEGUI::ImagesetManager::getSingleton().createFromImageFile("InstructionsReady", "InstructionText-Start.png");
	m_poInstructionsReady->setProperty("Image", "set:InstructionsReady image:full_image");
#endif


	m_poInstructionsReady->setProperty("FrameEnabled", "False");
	m_poInstructionsReady->setProperty("BackgroundEnabled", "False");
	m_poInstructionsReady->setVisible(true);


	// initialize commands
	(*m_poLogManager) << LogLevel_Debug << "+ addCommand(new CCommandStartStop(...)\n";
	this->addCommand(new CCommandStartStop(this));

	(*m_poLogManager) << LogLevel_Debug << "+ addCommand(new CCommandStimulatorControl(...))\n";
	this->addCommand(new CCommandStimulatorControl(this));

	(*m_poLogManager) << LogLevel_Debug << "+ addCommand(new CC(...))\n";
	this->addCommand(new CCommandReceiveTarget(this));

	(*m_poLogManager) << LogLevel_Debug << "  * CTrainerApplication::setup() completed successfully\n";

	return true;
}


void CTrainerApplication::processFrame(OpenViBE::uint32 ui32CurrentFrame)
{
	CApplication::processFrame(ui32CurrentFrame);

	if (!m_bActive)
	{
		return;
	}

	for (OpenViBE::uint32 i = 0; i < m_oObjects.size(); i++)
	{
		m_oObjects[i]->processFrame();
	}
	// ---- Solo 1 descomentar:

	//m_oObjects[m_TrainTarget]->processFrame();
	
}

void CTrainerApplication::addObject(CTrainerFlickeringObject* poObject)
{
	m_oObjects.push_back(poObject);

	//poObject->setVisible(false);
	poObject->setVisible(true);
}

void CTrainerApplication::setTarget(OpenViBE::int32 i32Target)
{
	OpenViBE::uint32 l_ui32CurrentTime = (OpenViBE::int32)(time(NULL) - m_ttStartTime);
	(*m_poLogManager) << LogLevel_Info << l_ui32CurrentTime << "    > Target set to " << i32Target << "\n";
	m_TrainTarget = i32Target;
	for (OpenViBE::int32 i = 0; i < int(m_oObjects.size()); i++)
	{
		m_oObjects[i]->setTarget(i32Target == i);	
	}
}

void CTrainerApplication::startExperiment()
{
	CApplication::startExperiment();

	m_ttStartTime = time(NULL);

	this->stopFlickering();
	m_poInstructionsReady->setVisible(false);
}

void CTrainerApplication::startFlickering()
{
	OpenViBE::uint32 l_ui32CurrentTime = (OpenViBE::uint32)(time(NULL) - m_ttStartTime);
	(*m_poLogManager) << LogLevel_Info << l_ui32CurrentTime << "    > Starting Visual Stimulation\n";
	
	// ---- Solo 1 descomentar:
	/*
	for (OpenViBE::int32 i = 0; i < int(m_oObjects.size()); i++)
	{
		if (m_TrainTarget == i){
			m_oObjects[i]->setVisible(true);
		}
		else{
			m_oObjects[i]->setVisible(false);
		}
	}
	*/
	m_bActive = true;
}

void CTrainerApplication::stopFlickering()
{
	OpenViBE::uint32 l_ui32CurrentTime = (OpenViBE::uint32)(time(NULL) - m_ttStartTime);
	(*m_poLogManager) << LogLevel_Info << l_ui32CurrentTime << "    > Stopping Visual Stimulation\n";
	m_bActive = false;

	
	for (OpenViBE::uint32 i = 0; i < m_oObjects.size(); i++)
	{
		m_oObjects[i]->setVisible(true);
	}
	
	this->setTarget(-1);
}

#endif