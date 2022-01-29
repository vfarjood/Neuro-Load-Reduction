//***************************************************************************//
//                                                                           //
// @File Name:     UserInterface.h                                           //
// @Author:        Vahid Farjood Chafi                                       //
// @Version:       0.0.1                                                     //
// @Date:          13th December 2021                                        //
// @Description:   Command-line user interface to interact with application. //
//                                                                           //
//***************************************************************************//

#pragma once

#include "../header/MediaManager.h"

namespace vfc
{
	class UserInterface
	{
	public:
		UserInterface();
		~UserInterface();
	
	public:
		void welcome();
		void help();
		void log_help();
		void logConfig();
		bool run();
	};
}
