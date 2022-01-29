//***************************************************************************//
//                                                                           //
// @File Name:     MediaManager.h                                            //
// @Author:        Vahid Farjood Chafi                                       //
// @Version:       0.0.1                                                     //
// @Date:          13th December 2021                                        //
// @Description:   This is the core of application which controls and manages//
//				   all the process and services.                             //
//                                                                           //
//***************************************************************************//

#pragma once

#include <thread>

#include "../entity/MediaBuilder.hpp"
#include "../utils/Timer.hpp"


namespace vfc
{

class MediaManager
{

private:
	Media* media{nullptr};
	FileLoader* file{nullptr};
	VideoEditor* video{nullptr};
	AudioEditor* audio{nullptr};

protected:
    MediaManager(const MediaManager& obj)= delete;
    void operator=(const MediaManager& obj)= delete;

public:
	//----------------------------------------//
				/*** Constructors ***/
	//----------------------------------------//

	MediaManager(); // Default Constructor:

	MediaManager(std::string const& path); // Init Constructor

	~MediaManager(); // Destructor:



	//----------------------------------------//
				/*** Methods ***/
	//----------------------------------------//
	void loadMedia();
	void loadMedia(std::string const& filePath); // In case if you didn't provide the path at first.
	

	void decodeMedia();
	
	void decodeVideo();
	
	void decodeAudio();
	
	void playMedia();
	
	void playVideo();
	
	void playAudio();
	
	void exportFrames(std::string const& path);
	
	void splitVideo(std::string const& outputDir, int const& num);
	
	void splitAuto(std::string const& outputDir, int const& detector_threshold);
	
	void reduceSence(std::string const& option, int const& detector_threshold, int const& reduce_level);
	
	void freeMediaContext();

	void statusMedia();



	void syncMedia(); // Not implemented yet.

	void saveMedia(); // Not implemented yet.

	void saveVideo(std::string const& outputDir);

	void saveAudio(); // Not implemented yet.

};

}
