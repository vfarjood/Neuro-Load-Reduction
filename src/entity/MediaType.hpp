//***************************************************************************//
//                                                                           //
// @File Name:     MediaType.hpp                                             //
// @Author:        Vahid Farjood Chafi                                       //
// @Version:       0.0.1                                                     //
// @Date:          13th December 2021                                        //
// @Description:   Media type entity that is used to wrap streams attributes.//
//                 It is a base class and its drived classes are Audio&Video.//
//                                                                           //
//***************************************************************************//

#pragma once

#include "../utils/Log.h"

class MediaType
{
protected:
	const char* m_format{"empty"};
	double m_duration{0};
    double m_time_base{0};

    int m_num_frames = 0;
	int m_stream_index = -1;

public:
	MediaType(){LOG_DEBUG("MediaType:     Constructor of the object: ", this);}
	~MediaType(){LOG_DEBUG("MediaType:     Destructor  of the object: ", this);}

	void setFormat(const char* format)  { m_format = format;}
	const char* getFormat()  { return m_format;}

	void setDuration(double const& time)  { m_duration = time;}
	double getDuration()  { return m_duration;}
	
	void setTimeBase(double const& time)  { m_time_base = time;}
	double getTimeBase()  { return m_time_base;}

	void setNumFrames(int const& num)  {m_num_frames = num;}
	int getNumFrames()  { return m_num_frames;}

	void setStreamIndex(int const& index)  { m_stream_index = index;}
	int getStreamIndex()  { return m_stream_index;}
	
};