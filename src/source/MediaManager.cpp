//***************************************************************************//
//                                                                           //
// @File Name:     MediaManager.cpp                                          //
// @Author:        Vahid Farjood Chafi                                       //
// @Version:       0.0.1                                                     //
// @Date:          13th December 2021                                        //
// @Description:   This is the core of application which controls and manages//
//				   all the process and services.                             //
//                                                                           //
//***************************************************************************//

#include "../header/MediaManager.h"
#include "../utils/Log.h"



vfc::MediaManager::MediaManager()
{ 
	media = MediaBuilder::createMedia();
	LOG_DEBUG("MediaManager:  Constructor of the object: ", this);
}

vfc::MediaManager::~MediaManager()
{
	LOG_DEBUG("MediaManager:  Destructor  of the object: ", this);
}

vfc::MediaManager::MediaManager(std::string const& filePath)
{
	media = MediaBuilder::createMedia();
	media->setPath(filePath);
	LOG_DEBUG("MediaManager:  Init Constructor of the object: ", this);
}

void vfc::MediaManager::loadMedia()
{
	if (media->getPath() != "empty")
	{
		file = MediaBuilder::createFileLoader();
		file->loadFile(media);
		if(media->video.getDuration() < 15 || media->video.getDuration() > 60)
		{
			LOG_ERROR("MediaManager:  Video lenght must be between 15-60 seconds.");
			LOG_INFO("MediaManager:  Please load a new media.");
			file->closeFile();
		}
		else
			LOG_TRACE("MediaManager:  Media has been loaded from: ", media->getPath());
	}
	else
		LOG_ERROR("MediaManager:  You must first provide file location!");
}

void vfc::MediaManager::loadMedia(std::string const& filePath) // In case if you didn't provide the path at first.
{

	media->setPath(filePath);

	file = MediaBuilder::createFileLoader();
	file->loadFile(media);

	if(media->video.getDuration() < 15 || media->video.getDuration() > 60)
	{
		LOG_ERROR("MediaManager:  Video lenght must be between 15-60 seconds.");
		LOG_INFO("MediaManager:  Please load a new media.");
		file->closeFile();
	}
	else
		LOG_TRACE("MediaManager:  Media has been loaded from: ", media->getPath());	
}

void vfc::MediaManager::statusMedia()
{

	std::cout << "VIDEO:"
			  << "\n  Format:	" << media->video.getFormat()
			  << "\n  Resolution:	" << media->video.getWidth() << "x" << media->video.getHeight()
			  << "\n  Frames:	" << media->video.getNumFrames()
			  << "\n  Duration:	" << media->video.getDuration()
			  << "\n  Buffer Size:	" << media->video.size() << std::endl;
	std::cout << "AUDIO:"
			  << "\n  Channels:	" << media->audio.getChannels()
			  << "\n  Rate:		" << media->audio.getSampleRate()
			  << "\n  Frames:	" << media->audio.getNumFrames()
			  << "\n  Buffer Size:	" << media->audio.size() << std::endl;
}

void vfc::MediaManager::decodeMedia()
{		
	if(file==nullptr)
		LOG_ERROR("MediaManager:  Please 'load' the media first.");
	else
	{
		vfc::MediaManager::decodeVideo();
		vfc::MediaManager::decodeAudio();
	}
}

void vfc::MediaManager::decodeVideo()
{
	if(file==nullptr)
		LOG_ERROR("MediaManager:  Please 'load' the media first.");
	else
	{
		file->readVideoStream(media->video);
		LOG_INFO("MediaManager:  Video stream has been decoded.");

		video = MediaBuilder::createVideoEditor();
		video->initialize(media->video.getWidth(), media->video.getHeight(), media->video.data, media->video.getFPS());
	}

}

void vfc::MediaManager::decodeAudio()
{
	if(file==nullptr)
		LOG_ERROR("MediaManager:  Please 'load' the media first.");
	else
	{
		file->readAudioStream(&media->audio.data);
		LOG_INFO("MediaManager:  Audio stream has been decoded.");
		
		audio = MediaBuilder::createAudioEditor();
		audio->initDevice(media->audio.data, media->audio.getChannels(), media->audio.getSampleRate(), media->audio.getDuration());
	}

}

void vfc::MediaManager::playMedia()
{
	LOG_INFO("MediaManager:  Media started to play.");
	std::thread worker(&MediaManager::playAudio, this);
	vfc::MediaManager::playVideo();
	worker.join();
	LOG_INFO("MediaManager:  Media ended to play.");
}

void vfc::MediaManager::playVideo()
{
	if(media->video.data[0] != nullptr)
	{
		media->video.setFrameTime(0.88/media->video.getFPS());
		Display* window = MediaBuilder::createDisplay();
		{
			// Timer play_time;
			for (auto frame : media->video.data)
			{	
				window->showMedia(media->video.getWidth(), media->video.getHeight(), &frame, media->video.getFrameTime());
			}
		}
		window->destroyWindow();
		window->~Display();
	}
	else if(media->video.getDuration()==0)
		LOG_ERROR("MediaManager:  Please 'load' the media first.");
	else
		LOG_ERROR("MediaManager:  Please 'decode' the video first.");
}

void vfc::MediaManager::playAudio()
{
	if(media->audio.data != nullptr)
	{
		// std::this_thread::sleep_for(std::chrono::milliseconds(200));
		{
			// Timer play_time;
			audio->play();
		}
		audio->close();

		// prepare for the next time playing:
		audio->~AudioEditor();

		file->loadFile(media);
		file->readAudioStream(&media->audio.data);
		audio = MediaBuilder::createAudioEditor();
		audio->initDevice(media->audio.data, media->audio.getChannels(), media->audio.getSampleRate(), media->audio.getDuration());
	}
	else if(media->audio.getDuration()==0)
		LOG_ERROR("MediaManager:  Please 'load' the media first.");
	else
		LOG_ERROR("MediaManager:  Please 'decode' the audio first.");
}

void vfc::MediaManager::exportFrames(std::string const& path)
{
	video->exportFrames(media->video.data, path);
}

void vfc::MediaManager::splitVideo(std::string const& path, int const& num = 0)
{
	video->splitVideoManual(media->video.data, path, num);
}

void vfc::MediaManager::splitAuto(std::string const& outputDir, int const& detector_threshold)
{
	video->splitVideoAuto(media->video.data, outputDir, detector_threshold);
}

void vfc::MediaManager::reduceSence(std::string const& option, int const& detector_threshold, int const& reduce_level)
{
	video->reduceScenceAuto(media->video.data, option, detector_threshold, reduce_level);
}

void vfc::MediaManager::syncMedia(){}

void vfc::MediaManager::saveMedia(){}

void vfc::MediaManager::saveVideo(std::string const& outputDir)
{
	video->saveVideo(media->video.data, outputDir);
}

void vfc::MediaManager::saveAudio(){}

void vfc::MediaManager::freeMediaContext()
{
	if(media!= nullptr){
		media->~Media();
		media = nullptr;
	}

	if(file!= nullptr){
		file->~FileLoader();
		file = nullptr;
	}

	if(video!= nullptr){
		video->~VideoEditor();
		video = nullptr;
	}

	if(audio!= nullptr){
		audio->~AudioEditor();
		audio = nullptr;
	}

}



