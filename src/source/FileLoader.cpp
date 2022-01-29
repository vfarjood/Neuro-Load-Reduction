//***************************************************************************//
//                                                                           //
// @File Name:     FileLoader.cpp                                            //
// @Author:        Vahid Farjood Chafi                                       //
// @Version:       0.0.1                                                     //
// @Date:          13th December 2021                                        //
// @Description:   Used to load and decode streams from disk.                //
//                                                                           //
//***************************************************************************//

#include "../header/FileLoader.h"
#include "../utils/Log.h"

FileLoader::FileLoader()
{
	LOG_DEBUG("FileLoader:    Constructor of the object: ", this);
}

FileLoader::~FileLoader()
{ 
	closeFile();
	LOG_DEBUG("FileLoader:    Destructor  of the object: ", this);
}



void FileLoader::loadFile(Media* media)
{
	// ************************************************************ //
	// 						Load Format Context 					//
	// ************************************************************ //


	m_avFormatContext = avformat_alloc_context();
	avformat_open_input(&m_avFormatContext, media->getPath().data(), nullptr, nullptr);
	if (avformat_find_stream_info(m_avFormatContext,  nullptr) < 0)
		LOG_ERROR("FileLoader:    Could not get the stream info for video!");


	m_avAudioFormatContext = avformat_alloc_context();
	avformat_open_input(&m_avAudioFormatContext, media->getPath().data(), nullptr, nullptr);
	if (avformat_find_stream_info(m_avAudioFormatContext,  nullptr) < 0)
		LOG_ERROR("FileLoader:    Could not get the stream info for Audio!");


	for (int i = 0; i < m_avFormatContext->nb_streams; i++)
	{
   		AVCodecParameters* pLocalCodecParameters = m_avFormatContext->streams[i]->codecpar;
   		AVCodec* pLocalCodec = avcodec_find_decoder(pLocalCodecParameters->codec_id);

   		// If the codec is not found we just skip it
   		if (pLocalCodec==nullptr) 
   		{
			LOG_ERROR("FileLoader:    Unsupported codec!");
      		continue;
    	}

    	// when the stream is a video we store its index, codec parameters and codec
    	if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) 
    	{
    		if (video_stream_index == -1) 
    		{
    	    	video_stream_index = i;
    	    	pVideoCodec = pLocalCodec;
    	    	pVideoCodecParameters = pLocalCodecParameters;
    			width = pLocalCodecParameters->width;
				height = pLocalCodecParameters->height;
    	  	}
    	} 
    	// when the stream is an audio, we store its index, codec parameters and codec
    	else if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_AUDIO) 
    	{
    		if (audio_stream_index == -1) 
    		{
    	    	audio_stream_index = i;
    	    	pAudioCodec = pLocalCodec;
    	    	pAudioCodecParameters = pLocalCodecParameters;
    	    	channels = pLocalCodecParameters->channels;
    			sample_rate = pLocalCodecParameters->sample_rate;			
    	  	}
    	}
	}

	if (video_stream_index==-1)
	{
		LOG_ERROR("FileLoader:    Couldn't find video stream!");
	}
	if (audio_stream_index==-1)
	{
		LOG_ERROR("FileLoader:    Couldn't find audio stream!");
	}

	video_stream = m_avFormatContext->streams[video_stream_index];
	audio_stream = m_avFormatContext->streams[audio_stream_index];



	//*** Video Codec Context ***//

	m_avVideoCodecContext = avcodec_alloc_context3(pVideoCodec);
	if (!m_avVideoCodecContext)
		LOG_ERROR("FileLoader:    Couldn't create avVideoCodecContext!");

	if(avcodec_parameters_to_context(m_avVideoCodecContext,pVideoCodecParameters) < 0)
		LOG_ERROR("FileLoader:    Couldn't initialized avVideoCodecContext!");


	if (avcodec_open2(m_avVideoCodecContext, pVideoCodec, nullptr) < 0 )
		LOG_ERROR("FileLoader:    Couldn't open the Video codec!");

	m_avVideoFrame = av_frame_alloc();
	if(!m_avVideoFrame)
		LOG_ERROR("FileLoader:    Couldn't allocate Video AVFrame!");

	m_avVideoPacket = av_packet_alloc();
	if(!m_avVideoPacket)
		LOG_ERROR("FileLoader:    Couldn't allocate Video AVPacket!");

	media->video.setWidth(width);
	media->video.setHeight(height);
	media->video.setFrameSize(4);
	media->video.setNumFrames(video_stream->nb_frames);
	media->video.setFormat(m_avFormatContext->iformat->long_name);
	media->video.data.reserve(video_stream->nb_frames);

	media->video.setTimeBase(((double)video_stream->time_base.num / (double)video_stream->time_base.den));
	media->video.setDuration(((double)video_stream->duration * media->video.getTimeBase()));


	//*** Audio Codec Context ***//

	m_avAudioCodecContext = avcodec_alloc_context3(pAudioCodec);
	if (!m_avAudioCodecContext)
		LOG_ERROR("FileLoader:    Couldn't create avAudioCodecContext!");

	if(avcodec_parameters_to_context(m_avAudioCodecContext,audio_stream->codecpar) < 0)
		LOG_ERROR("FileLoader:    Couldn't initialized avAudioCodecContext!");

	if (avcodec_open2(m_avAudioCodecContext, pAudioCodec, nullptr) < 0 )
		LOG_ERROR("FileLoader:    Couldn't open the Audio codec!");

	m_avAudioFrame = av_frame_alloc();
	if(!m_avAudioFrame)
		LOG_ERROR("FileLoader:    Couldn't allocate Audio AVFrame!");

	m_avAudioPacket = av_packet_alloc();
	if(!m_avAudioPacket)
		LOG_ERROR("FileLoader:    Couldn't allocate Audio AVPacket!");


	media->audio.setTimeBase(((double)audio_stream->time_base.num / (double)audio_stream->time_base.den));
	media->audio.setDuration(((double)audio_stream->duration * media->audio.getTimeBase()));

	media->audio.setNumFrames(audio_stream->nb_frames);
    media->audio.setChannels(channels);
	media->audio.setSampleRate(sample_rate);

	// LOG_TRACE("FileLoader:    Audio \'sample rate\' is set to: ", sample_rate);


}

void FileLoader::readVideoStream(Video& video) 
{
	for (int i= 0 ; i < video.getNumFrames(); ++i)
	{	

		video.setFrame();
		int response = 0;
		while(av_read_frame(m_avFormatContext, m_avVideoPacket) == 0)
		{
			if(m_avVideoPacket->stream_index != video_stream_index)
			{
				av_packet_unref(m_avVideoPacket);
				continue;
			}
	
			response = avcodec_send_packet(m_avVideoCodecContext, m_avVideoPacket);
			if (response < 0) {
				LOG_ERROR("FileLoader:    Failed to decode video packet!");
				LOG_DEBUG("FileLoader:    Failed to decode video packet: ", av_err2str(response));
				break;
			}
	
			response = avcodec_receive_frame(m_avVideoCodecContext, m_avVideoFrame);
			if (response==AVERROR(EAGAIN) || response==AVERROR_EOF )
			{
				av_packet_unref(m_avVideoPacket);
				continue;
			}
	
			if (response < 0) {
				LOG_ERROR("FileLoader:    Failed to decode video packet!");
				LOG_DEBUG("FileLoader:    Failed to decode video packet: ", av_err2str(response));
				break;
			}
	
			av_packet_unref(m_avVideoPacket);
			break;
		}
	
		pts = m_avVideoFrame->pts;
		uint8_t* dest[4] = {video.getFrame(), nullptr, nullptr, nullptr};
    	int dest_linesize[4] = {width*4, 0, 0, 0};
		
		m_swsScalarContext = sws_getContext(width, height, m_avVideoCodecContext->pix_fmt,
											width, height, AV_PIX_FMT_RGB0,
											SWS_BILINEAR, nullptr, nullptr, nullptr);
		if(!m_swsScalarContext)
		{
			LOG_ERROR("FileLoader:    Couldn't initialized swsScalarContext!");
		}
	
		sws_scale(m_swsScalarContext, m_avVideoFrame->data, m_avVideoFrame->linesize, 0, height, dest, dest_linesize);
		video.data.emplace_back(video.getFrame());
		sws_freeContext(m_swsScalarContext);
	}
	video.setPTS(pts);
	// video.setPTS(pts*video.getTimeBase());
	video.setFPS(video.data.size()/(video.getDuration()));


}


void FileLoader::readAudioStream(AVAudioFifo** fifo)
{
	SwrContext* resampler = swr_alloc_set_opts(
		nullptr, pAudioCodecParameters->channel_layout, AV_SAMPLE_FMT_FLT,
	 	sample_rate, pAudioCodecParameters->channel_layout,
	 	(AVSampleFormat)pAudioCodecParameters->format, sample_rate, 0, nullptr);


	*fifo = av_audio_fifo_alloc(AV_SAMPLE_FMT_FLT, pAudioCodecParameters->channels, 1);

	//Decode one packet:
	int response = 0;
	bool first = true;
	while(av_read_frame(m_avAudioFormatContext, m_avAudioPacket) == 0)
	{
		if(m_avAudioPacket->stream_index != audio_stream_index)
		{
			av_packet_unref(m_avAudioPacket);
			continue;
		}

		if(first){ first = false; continue;} // skip the first audio packet since it logs out an error.
			
		response = avcodec_send_packet(m_avAudioCodecContext, m_avAudioPacket);

		if (response < 0) 
		{
			if (response==AVERROR(EAGAIN) || response==AVERROR_EOF )
			{
				av_packet_unref(m_avAudioPacket);
				LOG_ERROR("FileLoader:    Failed to decode audio packet!");
				LOG_DEBUG("FileLoader:    Failed to decode audio packet: ", av_err2str(response));
				continue;
			}
			av_packet_unref(m_avAudioPacket);
		}

		while((response = avcodec_receive_frame(m_avAudioCodecContext, m_avAudioFrame)) == 0)
		{
			AVFrame* resampled_frame = av_frame_alloc();
			resampled_frame->sample_rate = sample_rate;
			resampled_frame->channel_layout = pAudioCodecParameters->channel_layout;
			resampled_frame->channels = channels;
			resampled_frame->format = AV_SAMPLE_FMT_FLT;

			response = swr_convert_frame(resampler, resampled_frame, m_avAudioFrame);
			av_frame_unref(m_avAudioFrame);
			av_audio_fifo_write(*fifo, (void**)resampled_frame->data, resampled_frame->nb_samples);
			av_frame_free(&resampled_frame); 
		}
		av_packet_unref(m_avAudioPacket);
		av_frame_unref(m_avAudioFrame);
	}
	swr_free(&resampler);
}

void FileLoader::closeFile()
{
	av_frame_free(&m_avVideoFrame);
	m_avVideoFrame = nullptr;
	av_packet_free(&m_avVideoPacket);
	m_avVideoPacket = nullptr;
	avformat_close_input(&m_avFormatContext);
	avformat_free_context(m_avFormatContext);
	m_avFormatContext = nullptr;
	avcodec_free_context(&m_avVideoCodecContext);
	m_avVideoCodecContext = nullptr;

	av_frame_free(&m_avAudioFrame);
	m_avAudioFrame = nullptr;
	av_packet_free(&m_avAudioPacket);
	m_avAudioPacket = nullptr;
	avcodec_free_context(&m_avAudioCodecContext);
	m_avAudioCodecContext = nullptr;
	avformat_close_input(&m_avAudioFormatContext);
	avformat_free_context(m_avAudioFormatContext);
	m_avAudioFormatContext = nullptr;
}

