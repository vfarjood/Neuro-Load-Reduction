//***************************************************************************//
//                                                                           //
// @File Name:     SceneEditor.h                                             //
// @Author:        Vahid Farjood Chafi                                       //
// @Version:       0.0.1                                                     //
// @Date:          13th December 2021                                        //
// @Description:   Used to detect and manipulate scenes in a video stream.   //
//                                                                           //
//***************************************************************************//

#pragma once

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <memory>

class SceneEditor
{
private:
	int m_width = 0;
    int m_height = 0;
    int m_frame_size = 0;

protected:
    SceneEditor(const SceneEditor& obj)= delete;
    void operator=(const SceneEditor& obj)= delete;
    
public:
	std::vector<cv::Mat*> sceneVector;
	// std::vector<std::unique_ptr<cv::Mat>> sceneVector;

	std::vector<int> scene_indexes{0};
	std::vector<int> scene_indexes_to_reduce{0};
	size_t size = 0; // Number of scene

public:
	SceneEditor();
	~SceneEditor();
	
	uint8_t* updateFrame(uint8_t* frame, double const& alpha, int const& beta);
	uint8_t* updateFrame1(uint8_t* frame1, uint8_t* frame2);
	uint8_t* updateFrame2(uint8_t* frame1, uint8_t* frame2, int const& step);

	double findFrameAverage(uint8_t* frame);

	void frameAverage(std::vector<uint8_t*>& video, std::vector<double> &framesAverage);

	void sceneDetector(std::vector<uint8_t*>& video, int const& width, int const& height, int const& threshold);

	void sceneSplitter(std::vector<cv::Mat> const& framesVector, std::string const& outputDir);

	void sceneReplacer(std::vector<uint8_t*>& video, std::string const& option, int const& reduce_level, int const& fps);

	void sceneRemover(std::vector<cv::Mat> const& inVector, std::vector<cv::Mat>* const& outPtrVector);

};
