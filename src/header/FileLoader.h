//***************************************************************************//
//                                                                           //
// @File Name:     FileLoader.h                                              //
// @Author:        Vahid Farjood Chafi                                       //
// @Version:       0.0.1                                                     //
// @Date:          13th December 2021                                        //
// @Description:   Used to load and decode streams from disk.                //
//                                                                           //
//***************************************************************************//

#pragma once

extern "C" 
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavutil/avutil.h"
    #include "libswscale/swscale.h"
    #include "libswresample/swresample.h"
    #include "libavutil/audio_fifo.h"
}

#include <iostream>
#include <string>
#include "../entity/Media.hpp"

class FileLoader
{
private:
    AVFormatContext* m_avFormatContext{nullptr};
    AVFormatContext* m_avAudioFormatContext{nullptr};

    // Video Variable declaration
    AVCodec* pVideoCodec = nullptr;
    AVCodecContext* m_avVideoCodecContext{nullptr};
    AVCodecParameters* pVideoCodecParameters =  nullptr;
    AVStream* video_stream{nullptr};
    AVPacket* m_avVideoPacket{nullptr};
    AVFrame* m_avVideoFrame{nullptr};
    SwsContext* m_swsScalarContext{nullptr};


    // Audio Variable declaration
    AVCodec* pAudioCodec = nullptr;
    AVCodecContext* m_avAudioCodecContext{nullptr};
    AVCodecParameters* pAudioCodecParameters = nullptr;
    AVStream* audio_stream{nullptr};
    AVPacket* m_avAudioPacket{nullptr};
    AVFrame* m_avAudioFrame{nullptr};    

    
protected:
    FileLoader(const FileLoader& obj)= delete;
    void operator=(const FileLoader& obj)= delete;

public:

	//Video properties:
    int64_t pts;
    int width;
    int height;
    const char* format{nullptr};
    int video_stream_index = -1;

    //Audio porperties:
    int channels;
    int sample_rate;
    int audio_stream_index = -1;
    int audio_buffer_size;

public:
	FileLoader();
    ~FileLoader();

    void loadFile(Media* media);
    void readFile();
	void readVideoStream(Video& video);
    void readAudioStream(AVAudioFifo** fifo);
	void closeFile();

};
