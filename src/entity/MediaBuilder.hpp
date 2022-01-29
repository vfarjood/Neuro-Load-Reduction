//***************************************************************************//
//                                                                           //
// @File Name:     MediaBuilder.hpp                                          //
// @Author:        Vahid Farjood Chafi                                       //
// @Version:       0.0.1                                                     //
// @Date:          13th December 2021                                        //
// @Description:   A factory class which is responsible to build entities    //
//                 based on demands.			                             //
//                                                                           //
//***************************************************************************//

#pragma once

#include "../entity/Media.hpp"
#include "../header/FileLoader.h"
#include "../header/VideoEditor.h"
#include "../header/AudioEditor.h"
#include "../header/Display.h"


struct MediaBuilder
{
	static Media* createMedia()
	{
		return new Media;
	}
	static void closeMedia(Media* ptr)
	{
		return delete ptr;
	}

	static Video* createVideo()
	{
		return new Video;
	}
	static void closeVideo(Video* ptr)
	{
		return delete ptr;
	}

	static Audio* createAudio()
	{
		return new Audio;
	}
	static void closeAudio(Audio* ptr)
	{
		return delete ptr;
	}

	static FileLoader* createFileLoader()
	{
		return new FileLoader;
	}	
	static void closeFileLoader(FileLoader* ptr)
	{
		return delete ptr;
	}
	
	static VideoEditor* createVideoEditor()
	{
		return new VideoEditor;
	}
	static void closeVideoEditor(VideoEditor* ptr)
	{
		return delete ptr;
	}

	static AudioEditor* createAudioEditor()
	{
		return new AudioEditor;
	}
	static void closeAudioEditor(AudioEditor* ptr)
	{
		return delete ptr;
	}

	static Display* createDisplay()
	{
		return new Display;
	}
	static void closeDisplay(Display* ptr)
	{
		return delete ptr;
	}

};
