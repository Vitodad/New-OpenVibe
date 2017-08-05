
#ifndef __GenericVRPNServer__
#define __GenericVRPNServer__

#include <Ogre.h>

#include <openvibe/ov_all.h>
#include <toolkit/ovtk_all.h>

namespace OpenViBESSVEP
{
	class CApplication;
	class CSSVEPFlickeringObject;

	class CRobotControl
	{
	public:
		CRobotControl(CApplication* poApplication, Ogre::SceneNode* poParentNode, Ogre::Real rRadius, std::vector<std::pair<OpenViBE::uint32, OpenViBE::uint32> >* pFrequencies);
		void processFrame(OpenViBE::uint32 ui32CurrentFrame);

		
	private:
		Ogre::SceneNode* m_poRobotNode;

		CSSVEPFlickeringObject* m_poChangeAxes;
		CSSVEPFlickeringObject* m_poPositiveMovement;
		CSSVEPFlickeringObject* m_poNegativeMovement;

		bool m_bIsShooting;
		


	};
}

#endif
