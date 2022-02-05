//***************************************************************************//
//                                                                           //
// @File Name:     VideoEditor.cpp                                           //
// @Author:        Vahid Farjood Chafi                                       //
// @Version:       0.0.1                                                     //
// @Date:          13th December 2021                                        //
// @Description:   Used to edit and manipulate a video stream.   			 //
//                                                                           //
//***************************************************************************//

#include "../header/VideoEditor.h"
#include "../utils/Log.h"


VideoEditor::VideoEditor()
{
	LOG_DEBUG("VideoEditor:   Constructor of the object: ", this);
}
VideoEditor::~VideoEditor()
{
	LOG_DEBUG("VideoEditor:   Destructor  of the object: ", this);
}

void VideoEditor::initialize(int const width, int const height, std::vector<uint8_t*> const& video, int const fps)
{
	m_framewidth = width;
  	m_frameheight = height;
  	m_size = video.size();
  	m_fps = fps;

	m_compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
	m_compression_params.push_back(100);

}

std::vector<cv::Mat> VideoEditor::convertToMAT(std::vector<uint8_t*> const& video)
{
	std::vector<cv::Mat> frameMAT;
	for (auto frame : video)
	{
		cv::Mat cv_frame(cv::Size(m_framewidth, m_frameheight), CV_8UC4, frame, cv::Mat::AUTO_STEP);
		cv::cvtColor( cv_frame, cv_frame, cv::COLOR_RGBA2BGR );
		frameMAT.emplace_back(cv_frame);
	}
	return frameMAT;
}

void VideoEditor::exportFrames(std::vector<uint8_t*> const& video, std::string const& outputDir)
{
	std::vector<cv::Mat> framesVector;
	framesVector.reserve(m_size);
	framesVector = convertToMAT(video);

	int counter = 0;
	for (auto frame: framesVector)
	{
		if (frame.empty())
			break;

	    std::string filePath = outputDir + std::to_string(counter)+ ".jpg";
	    cv::imwrite(filePath, frame, m_compression_params);
    	frame.release();
	    ++counter;

	}
	LOG_TRACE("VideoEditor:   Frames have been written to:", outputDir);

}

void VideoEditor::splitVideoManual(std::vector<uint8_t*> const& video, std::string const& outputDir, int const partNumber)
{
	std::vector<cv::Mat> framesVector;
	framesVector.reserve(m_size);
	framesVector = convertToMAT(video);

	int partSize = m_size / partNumber;
	int startPart = partSize;
	int endSize = partSize;

	for (int part = 0; part < partNumber; part++)
	{
		std::string filePath = outputDir + std::to_string(part+1)+ ".avi" ;
    	cv::VideoWriter output(filePath, cv::VideoWriter::fourcc('M','J','P','G'), 22, cv::Size(m_framewidth, m_frameheight));
    
    	for(int i = startPart*part; i < endSize*(part+1); i++)
		{
    		output.write(framesVector[i]);
    		framesVector[i].release();
		}
		std::cout << "Part: " << part+1 << " Done!" << std::endl;
	}

}

void VideoEditor::splitVideoAuto(std::vector<uint8_t*>& video, std::string const& outputDir, int const threshold)
{
	scene.sceneDetector(video, m_framewidth, m_frameheight, threshold);

	std::vector<cv::Mat> framesVector;
	framesVector.reserve(m_size);
	framesVector = convertToMAT(video);
	scene.sceneSplitter(framesVector, outputDir);
    for(int i = 0; i < m_size; i++)
	{
    	framesVector[i].release();
	}
}

void VideoEditor::reduceScenceAuto(std::vector<uint8_t*>& video, std::string const& option, int const threshold, int const reduce_level)
{
	scene.sceneDetector(video, m_framewidth, m_frameheight, threshold);
	scene.sceneReplacer(video, option, reduce_level, m_fps);	
}

void VideoEditor::saveVideo(std::vector<uint8_t*> const& video, std::string const& outputDir)
{
	std::vector<cv::Mat> framesVector;
	framesVector.reserve(m_size);
	framesVector = convertToMAT(video);

	std::string filePath = outputDir +  "output.avi" ;
    cv::VideoWriter output(filePath, cv::VideoWriter::fourcc('M','J','P','G'), 22, cv::Size(m_framewidth, m_frameheight));
   
    for(int i = 0; i < framesVector.size(); i++)
	{
    	output.write(framesVector[i]);
    	framesVector[i].release();
	}
	LOG_TRACE("VideoEditor:   video has been saved to:", outputDir);

}

size_t VideoEditor::size()
{
	return m_size;
}









