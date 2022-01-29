//***************************************************************************//
//                                                                           //
// @File Name:     AudioEditor.cpp                                           //
// @Author:        Vahid Farjood Chafi                                       //
// @Version:       0.0.1                                                     //
// @Date:          13th December 2021                                        //
// @Description:   This file is used to control and manage audios files      //
//                 specially for playing back.                               //
//                                                                           //
//***************************************************************************//

#define MINIAUDIO_IMPLEMENTATION
#include "../header/AudioEditor.h"
#include "../utils/Log.h"

AudioEditor::AudioEditor()
{
	deviceConfig = ma_device_config_init(ma_device_type_playback);
    LOG_DEBUG("AudioEditor:   Constructor of the object: ", this);
}
AudioEditor::~AudioEditor()
{
    LOG_DEBUG("AudioEditor:   Destructor  of the object: ", this);
}

void AudioEditor::data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    AVAudioFifo* fifo = reinterpret_cast<AVAudioFifo*>(pDevice->pUserData);
    av_audio_fifo_read(fifo, &pOutput, frameCount);

    (void)pInput;
}

void AudioEditor::initDevice(const AVAudioFifo* const fifo, int const& channels, int const& sample_rate, double const& duration)
{
	deviceConfig.playback.format   = ma_format_f32; //decoder.outputFormat;
    deviceConfig.playback.channels = channels;
    deviceConfig.sampleRate        = sample_rate;
    deviceConfig.dataCallback      = data_callback;
    deviceConfig.pUserData         = (void*)fifo;
    wait = duration;


    if (ma_device_init(nullptr, &deviceConfig, &device) != MA_SUCCESS)
    {
        LOG_ERROR("AudioEditor:   Failed to open playback device!");
    }
}

void AudioEditor::play()
{
    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
        LOG_ERROR("AudioEditor:   Failed to open playback device!");
    }
    if (ma_device_start(&device) != MA_SUCCESS) 
    {
        LOG_ERROR("AudioEditor:   Failed to start playback device!");
    }
    LOG_INFO("AudioEditor:   Playing..!");
    sleep(wait+1);
}


void AudioEditor::close()
{
	ma_device_uninit(&device);
}

