#ifndef __OpenViBEApplication_CCommandShipControl_H__
#define __OpenViBEApplication_CCommandShipControl_H__

#include "../ovassvepICommandVRPNButton.h"
#include "../st_Communication.h"
#include "ovassvepKeyBoardCommand.h"
#include <time.h>

namespace OpenViBESSVEP
{
	class CShooterApplication;

	class CCommandShipControl : public ICommandVRPNButton
	{
		public:
			CCommandShipControl(CShooterApplication* poApplication, CCommandKeyBoard* m_poCommandKeyboard);
			~CCommandShipControl() {};

			void execute(int iButton, int iState);
			

		private:

			int ret = 0;
			
			CCommandKeyBoard* l_poCommandKeyBoard;

			clock_t InInterrupt, InCase0 = 0;
			clock_t InCase1 = 0;
			clock_t InCase2 = 0;
			
	};
}


#endif // __OpenViBEApplication_CCommandShipControl_H__
