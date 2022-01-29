//***************************************************************************//
//                                                                           //
// @File Name:     Audio.hpp                                                 //
// @Author:        Vahid Farjood Chafi                                       //
// @Version:       0.0.1                                                     //
// @Date:          13th December 2021                                        //
// @Description:   Audio entity class that is used to wrap audio attributes. //
//                 It is a derived class from MediaType base class.          //
//                                                                           //
//***************************************************************************//

#pragma once
#include "../utils/Log.h"
extern "C" 
{
    #include "libavutil/avutil.h"
    #include "libavutil/audio_fifo.h"
}


class Audio : public MediaType
{
private:
	unsigned int m_channels{0};
    unsigned int m_sample_rate{0};
    unsigned int m_num_sample{0};
    size_t m_buffer_size{0};

public:
	AVAudioFifo* data{nullptr};

public:
	Audio() {LOG_DEBUG("Audio:         Constructor of the object: ", this);}
	~Audio() 
	{
		if(data != nullptr)
		{
			av_audio_fifo_free(data);
		}
		LOG_DEBUG("Audio:         Destructor  of the object: ", this);
	}

	void setChannels(unsigned int const& channel) { m_channels = channel;}
	int getChannels() { return m_channels;}

	void setSampleRate(unsigned int const& sampleRate) { m_sample_rate = sampleRate;}
	int getSampleRate() { return m_sample_rate;}

	void setNumSample(unsigned int const& numSample) { m_num_sample = numSample;}
	int getNumSample() { return m_num_sample;}

	size_t size()
	{
		if(data != nullptr)
		{
			m_buffer_size = av_audio_fifo_size(data);
		}
		return m_buffer_size;
	}
};