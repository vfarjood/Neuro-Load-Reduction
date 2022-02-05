//***************************************************************************//
//                                                                           //
// @File Name:     VideoEditor.h                                             //
// @Author:        Vahid Farjood Chafi                                       //
// @Version:       0.0.1                                                     //
// @Date:          13th December 2021                                        //
// @Description:   Used to edit and manipulate a video stream.   			 //
//                                                                           //
//***************************************************************************//

#pragma once

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "SceneEditor.h"


class VideoEditor
{	
private:
	size_t m_framewidth = 0;
    size_t m_frameheight = 0;
    size_t m_size = 0;
    double m_fps = 30;
    std::vector<int> m_compression_params;

protected:
    VideoEditor(const VideoEditor& obj)= delete;
    void operator=(const VideoEditor& obj)= delete;

public:
	std::vector<cv::Mat> frameVector;
	SceneEditor scene;
	
public:
	//----------------------------------------//
				/*** Constructors ***/
	//----------------------------------------//
	VideoEditor();
	~VideoEditor();
	VideoEditor(std::string const& FilePath);


	//----------------------------------------//
				/*** Methods ***/
	//----------------------------------------//
	void initialize(int const width, int const height, std::vector<uint8_t*> const& video, int const fps);
	
	std::vector<cv::Mat> convertToMAT(std::vector<uint8_t*> const& video);
	
	void extractFrames();

	void exportFrames(std::vector<uint8_t*> const& video, std::string const& outputDir);

	void saveVideo(std::vector<uint8_t*> const& video, std::string const& outputDir);

	void splitVideoManual(std::vector<uint8_t*> const& video, std::string const& outputDir, int const partNumber);
	
	void splitVideoAuto(std::vector<uint8_t*>& video, std::string const& outputDir, int const threshold);

	void reduceScenceAuto(std::vector<uint8_t*>& video, std::string const& option, int const threshold, int const reduce_level);

	size_t size();

};

