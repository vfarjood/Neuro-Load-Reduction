//***************************************************************************//
//                                                                           //
// @File Name:     AudioEditor.h                                             //
// @Author:        Vahid Farjood Chafi                                       //
// @Version:       0.0.1                                                     //
// @Date:          13th December 2021                                        //
// @Description:   This file is used to control and manage audios files      //
//                 specially for playing back.                               //
//                                                                           //
//***************************************************************************//

#pragma once

#define __STDC_CONSTANT_MACROS
extern "C" 
{
    #include "libavutil/avutil.h"
    #include "libavutil/audio_fifo.h"
}
#include "../../lib/miniaudio/miniaudio.h"


class AudioEditor
{
private:
	ma_device_config deviceConfig;
    ma_device device;

public:
	double wait;

private:
	static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);


public:

	AudioEditor();
	~AudioEditor();
	
	void initDevice(const AVAudioFifo* const fifo, int const& channels, int const& sample_rate, double const& duration);
	void play();
	void close();
};
