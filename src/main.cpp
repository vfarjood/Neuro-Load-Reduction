#include "utils/UserInterface.h"
#include "utils/Log.h"
int main()
{
	LOG_UPDATE_CONFIGURE(vfc::ENABLE, vfc::DEBUG, vfc::ALL);
	LOG_INFO("Main:          Program has started.");

	{
		vfc::UserInterface interface;
		interface.run();
	}

	LOG_INFO("Main:          Exit the program.");
    return 0;
}