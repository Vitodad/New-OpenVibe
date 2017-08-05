/*
* Receives data from OpenViBE's VRPN boxes
*
* See here: http://openvibe.inria.fr/vrpn-tutorial-sending-data-from-openvibe-to-an-external-application/
*
*/

#include <iostream>

#include <vrpn_Button.h>
#include <vrpn_Analog.h>

void VRPN_CALLBACK vrpn_rows_callback(void* user_data, vrpn_BUTTONCB button)
{
	std::cout << "Row: " << button.button << std::endl;

}
void VRPN_CALLBACK vrpn_columns_callback(void* user_data, vrpn_BUTTONCB button)
{
	std::cout << "Column: " << button.button << std::endl;

}

void VRPN_CALLBACK vrpn_test_callback(void* user_data, vrpn_BUTTONCB button)
{
	std::cout << "Test: " << button.button << std::endl;

}


int main(int argc, char** argv)
{

	const char *rows = "openvibe_vrpn_rows@localhost";
	const char *columns = "openvibe_vrpn_columns@localhost";
	const char *test = "openvibe_vrpn_test@localhost";
	//const char *analogDevice = "openvibe-vrpn-imagery-task@localhost";


	std::cout << "Polling these VRPN devices\n  Rows: " << rows << "\n  Columns: " << columns << "\n";

	/* flag used to stop the program execution */
	bool running = true;

	/* VRPN Button object */
	vrpn_Button_Remote *VRPNRows;
	vrpn_Button_Remote *VRPNColumns;
	vrpn_Button_Remote *VRPNTest;

	/* Binding of the VRPN Button to a callback */
	VRPNRows = new vrpn_Button_Remote(rows);
	VRPNColumns = new vrpn_Button_Remote(columns);
	VRPNTest = new vrpn_Button_Remote(test);
	VRPNRows->register_change_handler(&running, vrpn_rows_callback);
	VRPNColumns->register_change_handler(&running, vrpn_columns_callback);
	VRPNTest->register_change_handler(&running, vrpn_test_callback);

	/* The main loop of the program, each VRPN object must be called in order to process data */
	while (running)
	{
		VRPNRows->mainloop();
		VRPNColumns->mainloop();
		VRPNTest->mainloop();
	}

	VRPNRows->unregister_change_handler(&running, vrpn_rows_callback);
	VRPNColumns->unregister_change_handler(&running, vrpn_columns_callback);
	VRPNTest->unregister_change_handler(&running, vrpn_test_callback);

	delete VRPNRows;
	delete VRPNColumns;
	delete VRPNTest;

	return 0;
}
