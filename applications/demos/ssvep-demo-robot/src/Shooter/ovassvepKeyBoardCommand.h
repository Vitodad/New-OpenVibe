#ifndef __OpenViBEApplication_KeyBoardCommand_H__
#define __OpenViBEApplication_KeyBoardCommand_H__

#include "../ovassvepICommandOIS.h"
#include "pthread.h"

namespace OpenViBESSVEP
{

	class CCommandKeyBoard : public ICommandOIS
	{
	public:
		CCommandKeyBoard(CApplication* poApplication);
		~CCommandKeyBoard();


		void receiveKeyPressedEvent(const OIS::KeyCode oKey);
		void receiveKeyReleasedEvent(const OIS::KeyCode oKey);
		
		int getOrder(void){
			return order;
		}

		void setOrder(int value){
			order = value;
		}
		int getSelectHorizontal(void){
			return selHorizontal;
		}
		void setSelectHorizontal(int value){
			selHorizontal = value;
		}
		int getSelectVertical(void){
			return selVertical;
		}
		void setSelectVertical(int value){
			selVertical = value;
		}

		void setSelectAxis(int value){
			selAxis = value;
		}
		int getSelectAxis(void){
			return selAxis;
		}

		pthread_cond_t condition_ManualControl;
		pthread_mutex_t mutex_ManualControl;

		pthread_cond_t condition_ControlScreen ;
		pthread_mutex_t mutex_ControlScreen;
		int viewAxis = 0;

	private:

		class CShooterApplication* l_poShooterApplication;
		int order = 0;
		int selAxis = 0;
		int selHorizontal = 2;
		int selVertical = 2;

	};
}


#endif // __OpenViBEApplication_KeyBoardCommand_H__