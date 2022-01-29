//***************************************************************************//
//                                                                           //
// @File Name:     Video.hpp                                                 //
// @Author:        Vahid Farjood Chafi                                       //
// @Version:       0.0.1                                                     //
// @Date:          13th December 2021                                        //
// @Description:   Video entity class that is used to wrap video attributes. //
//                 It is a derived class from MediaType base class.          //
//                                                                           //
//***************************************************************************//

#pragma once

#include <vector>
#include "../utils/Log.h"

class Video : public MediaType
{
private:
    int64_t m_pts{0};
    int m_fps{30};
	double m_frame_time{0};
    unsigned int m_width{0};
    unsigned int m_height{0};
    unsigned int m_resolution{0};
    size_t m_frame_size = m_width*m_height;
	uint8_t* m_frame{nullptr};

public:
	std::vector<uint8_t*> data;

public:
	Video(){LOG_DEBUG("Video:         Constructor of the object: ", this);}
	~Video() 
	{	
		if(data.size() != 0)
		{
			for(auto frame = data.begin(); frame != data.end(); frame++)
			{
				delete[] *frame;
				*frame = nullptr;
			}
		}
		LOG_DEBUG("Video:         Destructor  of the object: ", this);

	}

	void setPTS(int64_t const& pts)  { m_pts = pts;}
	int64_t getPTS()  { return m_pts;}

	void setFPS(int const& time)  { m_fps = time;}
	int getFPS()  { return m_fps;}

	void setTimeBase(double const& time)  { m_time_base = time;}
	double getTimeBase()  { return m_time_base;}

	void setFrameTime(double const& time)  { m_frame_time = time;}
	double getFrameTime()  { return m_frame_time;}

	void setWidth(unsigned int const& width) { m_width = width;}
	unsigned int getWidth() { return m_width;}

	void setHeight(unsigned int const& height) { m_height = height;}
	unsigned int getHeight() { return m_height;}

	void setResolution(unsigned int const& width, unsigned int const& height) { m_resolution = width*height;}
	unsigned int getResolution() { return m_width*m_height;}

	void setFrameSize(size_t const& size)  { m_frame_size = m_width*m_height*size;}
	size_t getFrameSize()  { return m_frame_size;}

	void setFrame() { m_frame = new uint8_t[m_frame_size];}
	void setFrame(size_t frameSize) { m_frame = new uint8_t[frameSize];}
	uint8_t* getFrame() { return m_frame;}


	size_t size()
	{
		return data.size();
	}
};
