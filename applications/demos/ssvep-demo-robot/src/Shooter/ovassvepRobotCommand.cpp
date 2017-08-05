
#if defined(TARGET_HAS_ThirdPartyOgre3D)

#include "ovassvepRobotCommand.h"

#include "../ovassvepCApplication.h"
#include "../ovassvepCSSVEPFlickeringObject.h"


#define SIGN(x) ( x / abs(x) )

using namespace OpenViBESSVEP;
using namespace Ogre;

CRobotControl::CRobotControl(CApplication* poApplication, Ogre::SceneNode* poParentNode, Ogre::Real rRadius, std::vector<std::pair<OpenViBE::uint32, OpenViBE::uint32> >* pFrequencies) :
m_bIsShooting(false)
{
	// get configuration and controlling objects
	OpenViBE::Kernel::IConfigurationManager* l_poConfigurationManager = poApplication->getConfigurationManager();
	CBasicPainter* l_poPainter = poApplication->getPainter();
	
	ColourValue l_oLightColour = ColourValue(
		(float)(l_poConfigurationManager->expandAsFloat("${SSVEP_TargetLightColourRed}", 1.0f)),
		(float)(l_poConfigurationManager->expandAsFloat("${SSVEP_TargetLightColourGreen}", 1.0f)),
		(float)(l_poConfigurationManager->expandAsFloat("${SSVEP_TargetLightColourBlue}", 1.0f)));

	ColourValue l_oDarkColour = ColourValue(
		(float)(l_poConfigurationManager->expandAsFloat("${SSVEP_TargetDarkColourRed}", 0.0f)),
		(float)(l_poConfigurationManager->expandAsFloat("${SSVEP_TargetDarkColourGreen}", 0.0f)),
		(float)(l_poConfigurationManager->expandAsFloat("${SSVEP_TargetDarkColourBlue}", 0.0f)));


	// draw the robot control scene objects
	m_poRobotNode = poParentNode->createChildSceneNode();


	SceneNode* l_poDrawnObjectNode;
	MovableObject* l_poDrawnObject;
	
	Ogre::RealRect l_oWingRectangle = { -rRadius , rRadius , rRadius, -rRadius};
	Ogre::RealRect l_oWingRectangle2 = { -rRadius*0.75f, rRadius*0.75f, rRadius*0.75f, -rRadius*0.75f };
	

	// paint the change of axes

	l_poDrawnObjectNode = m_poRobotNode->createChildSceneNode();

	l_poDrawnObject = l_poPainter->paintRectangle(l_oWingRectangle2, l_oLightColour);
	l_poDrawnObject->setVisible(true);
	l_poDrawnObjectNode->attachObject(l_poDrawnObject);

	l_poDrawnObject = l_poPainter->paintRectangle(l_oWingRectangle2, l_oDarkColour);
	l_poDrawnObject->setVisible(false);
	l_poDrawnObjectNode->attachObject(l_poDrawnObject);

	l_poDrawnObjectNode->setPosition(0.0f, 0.25f, 0.0f);

	m_poChangeAxes = new CSSVEPFlickeringObject(l_poDrawnObjectNode, (*pFrequencies)[1].first, (*pFrequencies)[1].second);

	
	// paint the left wing

	l_poDrawnObjectNode = m_poRobotNode->createChildSceneNode();

	l_poDrawnObject = l_poPainter->paintRectangle(l_oWingRectangle, l_oLightColour);
	l_poDrawnObject->setVisible(true);
	l_poDrawnObjectNode->attachObject(l_poDrawnObject);

	l_poDrawnObject = l_poPainter->paintRectangle(l_oWingRectangle, l_oDarkColour);
	l_poDrawnObject->setVisible(false);
	l_poDrawnObjectNode->attachObject(l_poDrawnObject);

	l_poDrawnObjectNode->setPosition(-0.75f, -0.6f, 0.0f);

	m_poPositiveMovement = new CSSVEPFlickeringObject(l_poDrawnObjectNode, (*pFrequencies)[2].first, (*pFrequencies)[2].second);

	// paint the right wing

	l_poDrawnObjectNode = m_poRobotNode->createChildSceneNode();

	l_poDrawnObject = l_poPainter->paintRectangle(l_oWingRectangle, l_oLightColour);
	l_poDrawnObject->setVisible(true);
	l_poDrawnObjectNode->attachObject(l_poDrawnObject);

	l_poDrawnObject = l_poPainter->paintRectangle(l_oWingRectangle, l_oDarkColour);
	l_poDrawnObject->setVisible(false);
	l_poDrawnObjectNode->attachObject(l_poDrawnObject);

	l_poDrawnObjectNode->setPosition(0.75f, -0.6f, 0.0f);

	m_poNegativeMovement = new CSSVEPFlickeringObject(l_poDrawnObjectNode, (*pFrequencies)[3].first, (*pFrequencies)[3].second);


	//paint separate line

	Ogre::RealRect l_oSeparateLine(-0.95, 0.0005, 0.95, -0.0005 );

	l_poDrawnObjectNode = m_poRobotNode->createChildSceneNode();

	l_poDrawnObject = l_poPainter->paintRectangle(l_oSeparateLine, l_oLightColour);
	l_poDrawnObject->setVisible(true);
	l_poDrawnObjectNode->attachObject(l_poDrawnObject);

	l_poDrawnObject = l_poPainter->paintRectangle(l_oSeparateLine, l_oDarkColour);
	l_poDrawnObject->setVisible(false);
	l_poDrawnObjectNode->attachObject(l_poDrawnObject);

	l_poDrawnObjectNode->setPosition(0.0f, 0.65f, 0.0f);
}

void CRobotControl::processFrame(OpenViBE::uint32 ui32CurrentFrame)
{
	m_poChangeAxes->processFrame();
	m_poPositiveMovement->processFrame();
	m_poNegativeMovement->processFrame();

	

}


#endif