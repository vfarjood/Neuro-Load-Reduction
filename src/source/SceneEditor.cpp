//***************************************************************************//
//                                                                           //
// @File Name:     SceneEditor.cpp                                           //
// @Author:        Vahid Farjood Chafi                                       //
// @Version:       0.0.1                                                     //
// @Date:          13th December 2021                                        //
// @Description:   Used to detect and manipulate scenes in a video stream.   //
//                                                                           //
//***************************************************************************//

#include "../header/SceneEditor.h"
#include "../utils/Log.h"

SceneEditor::SceneEditor()
{
	LOG_DEBUG("SceneEditor:   Constructor of the object: ", this);
}

SceneEditor::~SceneEditor()
{
	LOG_DEBUG("SceneEditor:   Destructor  of the object: ", this);
}
	

uint8_t* SceneEditor::updateFrame2(uint8_t* frame1, uint8_t* frame2, int const& step)
{	

	for( int x = 1; x < m_height; x+=step ) {
		for( int y = 0; y < m_width; y+=step ) {
        	for( int c = 0; c < 3; c++ ) {
				frame1[x*m_width*4 + y*4 + c] = frame2[x*m_width*4 + y*4 + c];
			}
        }
    }
	return frame1;
}
uint8_t* SceneEditor::updateFrame1(uint8_t* frame1, uint8_t* frame2)
{	
		int size = m_width*m_height*4; // 3.686.400
		for(int i=4; i < size-4; i+=4){
			int b_avg = (frame2[i-4] + frame2[i]   + frame2[i+4])/3;   // blue
			int g_avg = (frame2[i-3] + frame2[i+1] + frame2[i+5])/3;	// green
			int r_avg = (frame2[i-2] + frame2[i+2] + frame2[i+6])/3;	// red
			frame1[i] = b_avg;
			frame1[i+1] = g_avg;
			frame1[i+2] = r_avg;
		}
	return frame1;
}
uint8_t* SceneEditor::updateFrame(uint8_t* frame, double const& alpha, int const& beta)
{	
	for( int x = 1; x < m_height; x++ ) {
		for( int y = 0; y < m_width; y++ ) {
        	for( int c = 0; c < 3; c++ ) {
				frame[x*m_width*4 + y*4 + c] = frame[x*m_width*4 + y*4 + c] * alpha + beta;
			}
        }
    }
	return frame;
}

double SceneEditor::findFrameAverage(uint8_t* frame)
{	
	double sum = 0;
	for( int x = 1; x < m_height;  x++ ) {
		for( int y = 0; y < m_width; y++ ) {
        	for( int c = 0; c < 3;    c++ ) {
        		sum += frame[x*m_width*4 + y*4 + c];
        	}
		}
	}
	double frame_avg = sum/m_frame_size; // frame size = 1280*720*3
	return frame_avg;

}

void SceneEditor::frameAverage(std::vector<uint8_t*>& video, std::vector<double> &framesAverage)
{
	for (auto frame: video)
	{
		double frameAVG = findFrameAverage(frame);
		framesAverage.emplace_back(frameAVG);
	}
}

void SceneEditor::sceneDetector(std::vector<uint8_t*>& video, int const& width, int const& height, int const& threshold)
{
	m_width = width;
    m_height = height;
    m_frame_size = width*height*3;
	std::vector<double> framesAverage;
	frameAverage(video, framesAverage);

    for(int i = 5; i < framesAverage.size(); i++)
	{

		int diff = std::abs(framesAverage[i-1] - framesAverage[i]);
		if(diff > threshold)
		{
    		scene_indexes.emplace_back(i);
    	}
	}
	LOG_TRACE("SceneEditor:   Number of scenes detected: ", scene_indexes.size());
} 

void SceneEditor::sceneSplitter(std::vector<cv::Mat> const& framesVector, std::string const& outputDir)
{
	int startPart = 0;
	for (int part = 0; part < scene_indexes.size(); part++)
	{
		std::string filePath = outputDir + std::to_string(part+1)+ ".avi" ;
    	cv::VideoWriter output(filePath, cv::VideoWriter::fourcc('M','J','P','G'), 22, cv::Size(1280, 720));
        
        for(int i = startPart; i < scene_indexes[part]; i++)
		{
    		output.write(framesVector[i]);
		}
		startPart = scene_indexes[part];
	}

}

void SceneEditor::sceneReplacer(std::vector<uint8_t*>& video, std::string const& option, int const& reduce_level, int const& fps)
{	
	int num_of_scene_reduced=0;
	bool previous_scene_was_reduced = false;
	for (int curr_scene = 2; curr_scene < scene_indexes.size(); curr_scene++)
	{
        int last_scene_frame = scene_indexes[curr_scene-1]-1 ;
        int num_of_frame_in_scene = std::abs(scene_indexes[curr_scene-1] - scene_indexes[curr_scene]);

    	// if((num_of_frame_in_scene < (fps/reduce_level)+reduce_level)) // && (curr_scene%reduce_level==0))
    	if((num_of_frame_in_scene < fps) && (!previous_scene_was_reduced))
    	{
    		previous_scene_was_reduced = true;
    		++num_of_scene_reduced;
			// fill with black frame:
			if(option=="black")
			{
				for(int i = scene_indexes[curr_scene-1]; i < scene_indexes[curr_scene]; ++i)
				{
					video[i] = updateFrame(video[i], 0, 0);
				}
			}
			// fill with white frame:
			if(option=="white")
			{
				for(int i = scene_indexes[curr_scene-1]; i < scene_indexes[curr_scene]; ++i)
				{
					video[i] = updateFrame(video[i], 0, 255);
				}
			}

			// freeze the removed frame:
			if(option=="freeze")
			{
				for(int i = scene_indexes[curr_scene-1]; i < scene_indexes[curr_scene]; ++i)
				{
					video[i] = video[scene_indexes[curr_scene-1]];
				}
			}

			// show the last frame of the scene before the remove one:
			if(option=="last-frame")
			{
				for(int i = scene_indexes[curr_scene-1]; i < scene_indexes[curr_scene]; ++i)
				{
					video[i] = video[last_scene_frame];
					last_scene_frame = i;
				}
			}

			// show the next frame of the scene after the remove one:
			if(option=="next-frame")
			{
				for(int i = scene_indexes[curr_scene-1]; i < scene_indexes[curr_scene]; ++i)
				{
					video[i] = video[scene_indexes [curr_scene]];
				}
			}

			// show the blured last frame of the scene before the remove one and show the blured of the next frame after removed one:
			if(option=="blure-last-to-next")
			{
				for(int i = scene_indexes[curr_scene-1]; i < scene_indexes[curr_scene]; ++i)
				{
					if(i < ((scene_indexes[curr_scene-1] + scene_indexes[curr_scene])/2))
						video[i] = updateFrame1(video[i], video[last_scene_frame] );
					else
						video[i] = updateFrame1(video[i], video[scene_indexes[curr_scene]]);

					last_scene_frame = i;
				}
			}

			// fade the removed scene to the next scene:
			if(option=="fade")
			{
				for(int i = scene_indexes[curr_scene-1]; i < scene_indexes[curr_scene]; ++i)
				{
					video[i] = updateFrame2(video[i], video[scene_indexes[curr_scene]], num_of_frame_in_scene);
					if(num_of_frame_in_scene < 1)
						continue;
					--num_of_frame_in_scene;
				}
			}

			// fade the last frame to the next scene:
			if(option=="fade-last-to-next")
			{
				for(int i = scene_indexes[curr_scene-1]; i < scene_indexes[curr_scene]; ++i)
				{
					video[i] = updateFrame1(video[i], video[last_scene_frame] );
					video[i] = updateFrame2(video[i], video[scene_indexes[curr_scene]], num_of_frame_in_scene);
					if(num_of_frame_in_scene < 1)
						continue;
					--num_of_frame_in_scene;
					last_scene_frame = i;
				}
			}
		}
		else
    		previous_scene_was_reduced = false;

	}
	LOG_TRACE("SceneEditor:   Number of scenes that have been reduced: ", num_of_scene_reduced);
}

void SceneEditor::sceneRemover(std::vector<cv::Mat> const& inVector, std::vector<cv::Mat>* const& outVector)
{

}

