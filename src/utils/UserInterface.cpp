//***************************************************************************//
//                                                                           //
// @File Name:     UserInterface.cpp                                         //
// @Author:        Vahid Farjood Chafi                                       //
// @Version:       0.0.1                                                     //
// @Date:          13th December 2021                                        //
// @Description:   Command-line user interface to interact with application. //
//                                                                           //
//***************************************************************************//

#include "UserInterface.h"
#include "../utils/Log.h"

vfc::UserInterface::UserInterface()
{
	// vfc::logger = vfc::Log::getInstance();
	UserInterface::welcome();
	LOG_DEBUG("UserInterface: Constructor of the object: ", this);
}

vfc::UserInterface::~UserInterface()
{
	LOG_DEBUG("UserInterface: Destructor  of the object: ", this);

}


void vfc::UserInterface::welcome()
{
		std::cout
			  << "\n			//*** Neuro-Load Reduction for Kids ***//\n\n"				
	 		  << "\nDESCRIPTION:\n"
	 		  << "	-This application is designed for reducing scene transition in video stream.\n"
	 		  << "	-It is written in c++ language with the help of FFmpeg and OpenCV packages.\n"
	 		  << "	-It is a command-line application which will provide you some specific operations in order to manipulate your media.\n"
	 		  << "	-It works either in automatic or manual way.\n"
	 		  << "	-In 'auto' mode, it will automatically reduce scene transition base on the number of sence per second.\n"

	 		  << "\nLIMITATIONs:\n"
	 		  << "	-Video stream need to be more than 20 seconds and less than 1 minute.\n"
	 		  << "	-You are required to specify your media location which differ on the type of operation you choose:\n"
	 		  << "	  1) In case if you want to use 'auto' you are expected to locate your media in 'media/Trailer/' directory in advance.\n"
	 		  << "	  2) If you don't want to choose 'auto' then you need to provide the location after you used 'load' operation.\n"

	 		  << "\nOPERATION:\n"
			  << "	[auto, load, decode, decode_video, decode_audio, export, split_manual, split_auto, reduce_scene, play, play_video, play_audio, free, help, exit] \n"
	 		  << "-------------------------------------" << std::endl;
}

void vfc::UserInterface::help()
{
		std::cout
	 		  << "Operations: \n"
			  << "	auto		\"Load, Decode and Play your media.\"\n"
			  << "	load		\"Load your media.\"\n"
		  	  << "	decode		\"Decode the media (video, audio).\"\n"
		  	  << "	decode_video	\"Decode the video only.\"\n"
		  	  << "	decode_audio	\"Decode the audio only.\"\n"
		  	  << "	export		\"Export all frames of the video stream.\"\n"
		  	  << "	split_manual	\"Split video manually.\"\n"
		  	  << "	split_auto	\"Split video manually.\"\n"
		  	  << "	reduce_scene	\"Split video manually.\"\n"
		   	  << "	play		\"Play the media on the screen.\"\n"
		   	  << "	play_video	\"Play the media on the screen.\"\n"
		   	  << "	play_audio	\"Play the media on the screen.\"\n"
		   	  << "	status		\"show the status of media in the application.\"\n"
		  	  << "	free		\"Free all media contexts.\"\n"
		   	  << "	log_config 	\"Change log gonfiguration.\"\n"
		   	  << "	help 		\"Shows available operations.\"\n"
		  	  << "	exit		\"Exit from the application.\"\n";

}

void vfc::UserInterface::log_help()
{
		std::cout
	 		  << "Operations:\n"
			  << "	mode:		\"[enable]: enables the loggin\"\n"
			  << "			\"[disable]: disable the loggin\"\n\n"

			  << "	type:		\"[console]: send the log to the console\"\n"
			  << "			\"[file]: send the log to the file\"\n"
			  << "			\"[all]: send the log to the both console and file\"\n\n"

			  << "	level:		\"[info]: send the information level log\"\n"
			  << "			\"[warning]: send the warning and below levels log\"\n"
			  << "			\"[error]: send the error and below levels log\"\n"
			  << "			\"[buffer]: send the buffer and below levels log\"\n"
			  << "			\"[trace]: send the trace and below levels log\"\n"
			  << "			\"[debug]: send the debug and below levels log\"\n\n"

			  << "	save:		\"save the configuration and go back to main program.\"\n"
			  << "	default: 	\"set the configuration to default config.\"\n"
			  << "	exit, quit:	\"go back to main program.\"\n\n";

		std::cout 
			  << "Example:\n"
			  << "			mode=enable\n"
			  << "			type=console\n"
			  << "			level=warning\n";

}

void vfc::UserInterface::logConfig()
{

		std::string userInput;
	label200:
		std::cout << "Operation->log_config:";
		std::cin >> userInput;

		if(userInput=="save")
		{
			goto label201;
		}
		if(userInput=="quit" || userInput=="exit")
		{
			goto label201;
		}

		if(userInput=="default")
		{
			LOG_UPDATE_CONFIGURE(vfc::ENABLE, vfc::ERROR, vfc::ALL);
			LOG_WARNING("UserInterface: Log Configuration has been set to: Default");
			LOG_WARNING("UserInterface: Mode:ENABLE - Type:FILE - Level:ERROR");

			goto label200;
		}

		if(userInput=="mode=enable")
		{
			LOG_UPDATE_MODE(vfc::ENABLE);
			LOG_WARNING("UserInterface: Log Mode has been set to: ENABLE");
			goto label200;
		}
		if(userInput=="mode=disable")
		{
			LOG_UPDATE_MODE(vfc::DISABLE);
			LOG_WARNING("UserInterface: Log Mode has been set to: DISABLE");
			goto label200;
		}
		if(userInput=="type=console")
		{
			LOG_UPDATE_TYPE(vfc::CONSOLE);
			LOG_WARNING("UserInterface: Log Type has been set to: CONSOLE");
			goto label200;
		}
		if(userInput=="type=file")
		{
			LOG_UPDATE_TYPE(vfc::FILE);
			LOG_WARNING("UserInterface: Log Type has been set to: FILE");
			goto label200;
		}
		if(userInput=="type=all")
		{
			LOG_UPDATE_TYPE(vfc::ALL);
			LOG_WARNING("UserInterface: Log Type has been set to: ALL");
			goto label200;
		}
		if(userInput=="level=info")
		{
			LOG_UPDATE_LEVEL(vfc::INFO);
			LOG_WARNING("UserInterface: Log Level has been set to: INFO");
			goto label200;
		}
		if(userInput=="level=warning")
		{
			LOG_UPDATE_LEVEL(vfc::WARNING);
			LOG_WARNING("UserInterface: Log Level has been set to: WARNING");
			goto label200;
		}
		if(userInput=="level=error")
		{
			LOG_UPDATE_LEVEL(vfc::ERROR);
			LOG_WARNING("UserInterface: Log Level has been set to: ERROR");
			goto label200;
		}
		if(userInput=="level=buffer")
		{
			LOG_UPDATE_LEVEL(vfc::BUFFER);
			LOG_WARNING("UserInterface: Log Level has been set to: BUFFER");
			goto label200;
		}
		if(userInput=="level=trace")
		{
			LOG_UPDATE_LEVEL(vfc::TRACE);
			LOG_WARNING("UserInterface: Log Level has been set to: TRACE");
			goto label200;
		}
		if(userInput=="level=debug")
		{
			LOG_UPDATE_LEVEL(vfc::DEBUG);
			LOG_WARNING("UserInterface: Log Level has been set to: DEBUG");
			goto label200;
		}
		if(userInput=="help" || userInput=="?")
		{
			log_help();
			goto label200;
		}
		if(userInput!="")
		{
			std::cout << "\aERROR: Invalid command!\n";

			goto label200;
		}
	label201:
		LOG_WARNING("UserInterface: Log Configuration has been changed.");

}

bool vfc::UserInterface::run()
{
	std::string userInput;
	std::string location;
	std::string option;
	int number;
	int threshold;
	int level;

	bool mainLoop = true;

label:
	while(mainLoop)
	{
		vfc::MediaManager video1;
	label0:
		std::cout << "Operation: ";
		std::cin >> userInput;

		if(userInput=="auto")
			goto label1;
		if(userInput=="load")
			goto label2;
		if(userInput=="decode")
			goto label3;
		if(userInput=="decode_video")
			goto label4;
		if(userInput=="decode_audio")
			goto label5;
		if(userInput=="export")
			goto label6;
		if(userInput=="split_manual")
			goto label7;
		if(userInput=="split_auto")
			goto label8;
		if(userInput=="reduce_scene")
			goto label9;
		if(userInput=="free")
			goto label10;
		if(userInput=="status")
			goto label11;
		if(userInput=="play")
			goto label12;
		if(userInput=="play_video")
			goto label13;
		if(userInput=="play_audio")
			goto label14;
		if(userInput=="save_video")
			goto label15;
		if(userInput=="log_config")
			goto label16;
		if(userInput=="help" || userInput=="?")
			goto label17;
		if(userInput=="exit")
			goto label18;
		if(userInput!="")
		{
			std::cout << "\aERROR: Invalid command!\n";
			goto label0;
		}


	label1:
		std::cout << "Enter file location: ";
		std::cin >> location;
		video1.loadMedia(location);
		video1.decodeMedia();
		video1.reduceSence("fade-last-to-next", 5, 2);
		video1.playMedia();
		// video1.saveVideo("../media/video/");
		video1.freeMediaContext();

		goto label;


	label2:
		std::cout << "Enter file location: ";
		std::cin >> location;
		video1.loadMedia(location);
		goto label0;


	label3:
		video1.decodeMedia();
		goto label0;


	label4:
		video1.decodeVideo();
		goto label0;


	label5:
		video1.decodeAudio();
		goto label0;


	label6:
		std::cout << "Location to save:";
		std::cin >> location;
		video1.exportFrames(location);
		goto label0;


	label7:
		std::cout << "Location to save:";
		std::cin >> location;

		std::cout << "How many parts:";
		std::cin >> number;

		video1.splitVideo(location, number);
		goto label0;

	label8:
		std::cout << "Location to save:";
		std::cin >> location;
		std::cin >> threshold;

		video1.splitAuto(location, threshold);
		goto label0;

	label9:
		std::cout << "replace option:\t [black, white, freeze, last-frame, next-frame, blure-last-to-next, fade, fade-last-to-next]\n";
		std::cout << ">option:";
		std::cin >> option;

		std::cout << "Enter the threshold for scene detector(default is 5):\n";
		std::cout << ">threshold:";
		std::cin >> threshold;

		std::cout << "Enter the reduce level to reduce the number of detected scene(default is 2 which means sences less than half of a second will be reduced):\n";
		std::cout << ">reduce-level:";
		std::cin >> level;

		video1.reduceSence(option, threshold, level);
		goto label0;

	label10:
		video1.freeMediaContext();
		goto label;

	label11:
		video1.statusMedia();
		goto label0;


	label12:
		video1.playMedia();
		goto label0;


	label13:
		video1.playVideo();
		goto label0;


	label14:
		video1.playAudio();
		goto label0;

	label15:			
		std::cout << "Location to save:";
		std::cin >> location;
		
		video1.saveVideo(location);
		goto label0;

	label16:			
		logConfig();
		goto label0;

	label17:
		help();
		goto label0;


	label18:
		mainLoop = false;

	}

	return false;
}