/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*\
** 文件名：	my_videocodec.cpp
** 作者：	Alex.zhang
** 日期：	9/21/周三  22:19:00
** 功能：	音视频文件编解码
** 修改记录：
** 备注：
\*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C"	{
	#include "include/libavformat/avformat.h"
}

#pragma comment(lib,"lib/avformat.lib")
#pragma comment(lib,"lib/avcodec.lib")
#pragma comment(lib,"lib/avutil.lib")

#define INPUT_FILE  "D:\\share\\video\\1.avi"
#define OUTPUT_FILE "D:\\H264_aac.mkv"
#define VideoEncoderID CODEC_ID_H264
#define AudioEncoderID CODEC_ID_AAC
#define ISDEBUG 1
bool isHaveAudio = true;

int main(int argc, char** argv)
{
	int ret;

    const char *input_file_name = INPUT_FILE;

    /* initialize libavcodec, and register all codecs and formats */
	//avcodec_init();
	//avcodec_register_all();
	av_register_all();


/*=========================================================================================*\
**	解码部分
\*=========================================================================================*/
    AVFormatContext *pIFmtCtx	= av_alloc_format_context();
	AVCodecContext *pCodecCtxVD = avcodec_alloc_context();
	AVCodecContext *pCodecCtxAD = avcodec_alloc_context();
	AVCodec *pCodecVD			= NULL;
	AVCodec *pCodecAD			= NULL;

	/* get info from the input media file */
	ret = av_open_input_file(&pIFmtCtx,input_file_name,NULL,0,NULL); if(ret){printf("can't open the file %s\n",input_file_name);return -1;}

	ret = av_find_stream_info(pIFmtCtx); if(ret < 0){printf("can't find suitable codec parameters\n");return -1;}

	dump_format(pIFmtCtx,0,input_file_name,0);


	/* find the stream index of video & audio */
	int videoindex=-1,audioindex=-1;
    for(unsigned int i=0;i<pIFmtCtx->nb_streams;i++)
    {
        if(	pIFmtCtx->streams[i]->codec->codec_type	== CODEC_TYPE_VIDEO)
            videoindex=i;
        else if(pIFmtCtx->streams[i]->codec->codec_type== CODEC_TYPE_AUDIO)
            audioindex=i;
    }
    if(videoindex == -1) {printf("can't find video stream\n");return -1;}
    if(audioindex == -1) {printf("can't find audio stream\n");isHaveAudio = false;}
	printf("video index is %d\naudio index is %d\n",videoindex,audioindex);


    /* open the video decoder */
	pCodecCtxVD	= pIFmtCtx->streams[videoindex]->codec;
    pCodecVD	= avcodec_find_decoder(pCodecCtxVD->codec_id); if(!pCodecVD){printf("can't find suitable video decoder\n");return 1;}
	ret			= avcodec_open(pCodecCtxVD,pCodecVD); if(ret < 0){printf("can't open the video decoder\n");return 1;}

/** 打印出输入视频的帧率,大约为22.999 **/
//////////////////////////////////////////////////////////////////////////
#if 0
printf("pCodecCtxVD->time_base.num = %d\npCodecCtxVD->time_base.den = %d\n\n",
pCodecCtxVD->time_base.num,
pCodecCtxVD->time_base.den);
printf("pFmtCtxIn->streams[videoindex]->pts.val = %ld\npFmtCtxIn->streams[videoindex]->pts.num = %ld\npFmtCtxIn->streams[videoindex]->pts.den = %ld\n\n",
pIFmtCtx->streams[videoindex]->pts.val,
pIFmtCtx->streams[videoindex]->pts.num,
pIFmtCtx->streams[videoindex]->pts.den);
printf("pFmtCtxIn->streams[videoindex]->r_frame_rate.num = %d\npFmtCtxIn->streams[videoindex]->r_frame_rate.den = %d\npFmtCtxIn->streams[videoindex]->time_base.num = %d\npFmtCtxIn->streams[videoindex]->time_base.den = %d\n\n",
pIFmtCtx->streams[videoindex]->r_frame_rate.num,
pIFmtCtx->streams[videoindex]->r_frame_rate.den,
pIFmtCtx->streams[videoindex]->time_base.num,
pIFmtCtx->streams[videoindex]->time_base.den);
#endif
//////////////////////////////////////////////////////////////////////////

if (isHaveAudio)
{
    pCodecCtxAD	= pIFmtCtx->streams[audioindex]->codec;
    pCodecAD	= avcodec_find_decoder(pCodecCtxAD->codec_id); if(!pCodecAD){printf("can't find suitable audio decoder\n");return 1;}
	ret			= avcodec_open(pCodecCtxAD,pCodecAD); if(ret < 0){printf("can't open the audio decoder\n");return 1;}
}


puts("\n*****************************************************\n\
	Now,we have found suitable V/A Decoders!\n\
	Next,we will find suitable V/A Encoders!\n\
*****************************************************\n");
system("pause");



/*=========================================================================================*\
**	编码部分
\*=========================================================================================*/
	AVFormatContext *pOFmtCtx	= av_alloc_format_context();
	AVCodecContext *pCodecCtxVE	= avcodec_alloc_context();
	AVCodecContext *pCodecCtxAE	= avcodec_alloc_context();
	AVCodec  *pCodecVE			= NULL;
	AVCodec	 *pCodecAE			= NULL;
	AVStream *pOVideoStream		= NULL;
	AVStream *pOAudioStream		= NULL;

	/* get info from the output media file */
	pOFmtCtx->oformat = guess_format(NULL,OUTPUT_FILE,NULL); if(!pOFmtCtx->oformat){printf("could not deduce output format from outfile extension\n");return 0;}
    sprintf(pOFmtCtx->filename,"%s",OUTPUT_FILE);
#if ISDEBUG
	printf("pOFmtCtx->oformat->name is  %s \npOFmtCtx->oformat->long_name is  %s \npOFmtCtx->oformat->extensions is  %s \n",
		pOFmtCtx->oformat->name,
		pOFmtCtx->oformat->long_name,
		pOFmtCtx->oformat->extensions);
#endif

	/* new a video stream,in the next step(AVCodecContext) I will use it */
    pOVideoStream = av_new_stream(pOFmtCtx,0); if(!pOVideoStream){printf("could not alloc video stream\n");return 0;}
    pOVideoStream->r_frame_rate = pIFmtCtx->streams[videoindex]->r_frame_rate;		// keep the same framerate

/* printf the framerate */
//////////////////////////////////////////////////////////////////////////
#if ISDEBUG
	printf("pIFmtCtx->streams[videoindex]->r_frame_rate.num = %d\npIFmtCtx->streams[videoindex]->r_frame_rate.den = %d\n",
		pIFmtCtx->streams[videoindex]->r_frame_rate.num,
		pIFmtCtx->streams[videoindex]->r_frame_rate.den);
	printf("output file video stream's time_base.num =  %d \noutput file video stream's time_base.den =  %d \n",
		pOVideoStream->time_base.num,
		pOVideoStream->time_base.den);
#endif
//////////////////////////////////////////////////////////////////////////

/*********************************************************************\
**	encode param configure
\*********************************************************************/
    pCodecCtxVE					= pOVideoStream->codec;
    pCodecCtxVE->codec_id		= VideoEncoderID;
    pCodecCtxVE->codec_type		= CODEC_TYPE_VIDEO;
    pCodecCtxVE->width			= 320;
    pCodecCtxVE->height			= 240;
    pCodecCtxVE->time_base		= pCodecCtxVD->time_base;
    pCodecCtxVE->gop_size		= pCodecCtxVD->gop_size;
    pCodecCtxVE->pix_fmt		= PIX_FMT_YUV420P;
    pCodecCtxVE->max_b_frames	= pCodecCtxVD->max_b_frames;
    pCodecCtxVE->bit_rate		= 400000;

    if (pCodecCtxVE->codec_id == CODEC_ID_MPEG1VIDEO)
        pCodecCtxVE->mb_decision	= 2;
    if (pCodecCtxVE->codec_id == CODEC_ID_MPEG2VIDEO)
        pCodecCtxVE->max_b_frames	= 2;

	// some formats want stream headers to be seperate
    if( !strcmp(pOFmtCtx->oformat->name, "mp4") ||
		!strcmp(pOFmtCtx->oformat->name, "mov") ||
		!strcmp(pOFmtCtx->oformat->name, "3gp") )
        pOFmtCtx->flags	|= CODEC_FLAG_GLOBAL_HEADER;

if (isHaveAudio)
{
	// new a audio stream
    pOAudioStream			= av_new_stream(pOFmtCtx,1); if(!pOAudioStream){printf("could not alloc audio stream\n");return 0;}
    pCodecCtxAE				= pOAudioStream->codec;
    pCodecCtxAE->codec_id	= AudioEncoderID;
    pCodecCtxAE->codec_type	= CODEC_TYPE_AUDIO;
    pCodecCtxAE->bit_rate	= 64000;
    pCodecCtxAE->sample_rate= 44100;
    pCodecCtxAE->channels	= 2;
	pCodecCtxAE->profile	= FF_PROFILE_AAC_MAIN;
	//pCodecCtxAE->flags		|= PKT_FLAG_KEY;
	//pCodecCtxAE->block_align= pCodecCtx_AD->block_align;	// WAV 才使用的,用于设定:nBytes/packet
	if (pCodecCtxAE->block_align==1 && pCodecCtxAE->codec_id==CODEC_ID_MP3)
		pCodecCtxAE->block_align = 0;
}

    // set the output info
	strcpy(pOFmtCtx->title,pIFmtCtx->title);
    strcpy(pOFmtCtx->author,pIFmtCtx->author);
    strcpy(pOFmtCtx->copyright,pIFmtCtx->copyright);
    strcpy(pOFmtCtx->comment,pIFmtCtx->comment);
    strcpy(pOFmtCtx->album,pIFmtCtx->album);
    strcpy(pOFmtCtx->genre,pIFmtCtx->genre);
    strcpy(pOFmtCtx->title,"title") ;
    strcpy(pOFmtCtx->author,"author");
    pOFmtCtx->year	= pIFmtCtx->year;
    pOFmtCtx->track	= pIFmtCtx->track;

	// set the param above
	ret		= av_set_parameters(pOFmtCtx, NULL); if (ret < 0) {printf( "Invalid output format parameters\n");return 0;}

	/* display the output media info **/
	dump_format(pOFmtCtx,0,OUTPUT_FILE,1);

    // open the video encoder
	pCodecVE= avcodec_find_encoder(pCodecCtxVE->codec_id); if(!pCodecVE) {printf("can't find suitable video encoder\n");return 0;}
	ret		= avcodec_open(pCodecCtxVE,pCodecVE); if(ret < 0) {printf("can't open the output video codec\n");return 0;}


if (isHaveAudio)
{
	pCodecAE= avcodec_find_encoder(pCodecCtxAE->codec_id); if(!pCodecAE) {printf("can't find suitable audio encoder\n");return 0;}
	ret		= avcodec_open(pCodecCtxAE,pCodecAE); if(ret < 0) {printf("can't open the output audio codec");return 0;}
}

	// fopen the output media file
    if (!(pOFmtCtx->flags & AVFMT_NOFILE))
        if(url_fopen(&pOFmtCtx->pb,OUTPUT_FILE,URL_WRONLY)<0)
			{printf("can't open the output file %s\n",OUTPUT_FILE);return 0;}

	// fwrite the header to the output file
	ret		= av_write_header(pOFmtCtx); if(ret < 0) {fprintf(stderr, "Could not write header for output file\n");return 1;}


/*****************************************
**	the real job,this will use the codecContext
*****************************************/
    AVPacket packet;
    int isFinished = 0;
	int64_t	frame_index = 0;
	int nFrameEncoded	= 0;


	puts("\n*****************************************************\n\
Now,we will do the real job: the decode & encode cycle!\n\
*****************************************************\n");
	system("pause");

	// video
	AVFrame *pFrameDecoded	= avcodec_alloc_frame();	// store the frame that was decoded
	int video_outbuf_size	= 500000;
	uint8_t *pFrameEncoded	= (unsigned char *)av_malloc(video_outbuf_size);
	// audio
	short *pSamplesDecoded	= NULL;				// used for decoding
	int unsigned samples_size = 0;
	int audio_outbuf_size	= 10000;
	uint8_t *pSampleEncoded	= (unsigned char *)av_malloc(audio_outbuf_size);

	//packet 作为读取数据的单元
    while(av_read_frame(pIFmtCtx,&packet) >= 0)
    {
		// video

        if(packet.stream_index == videoindex)
        {
			ret = avcodec_decode_video(pCodecCtxVD,pFrameDecoded,&isFinished,packet.data,packet.size);
            if(ret < 0) {printf("Error while decoding\n");return 0;}

            // do something with the frame's data
            if(isFinished)
            {
                fflush(stdout);
#if 0
                // 下面这个函数是用来作为校准时间用的,用它来初始化结构体 pFrame 的一个成员 pts
				// 设置 frame 的pts 的意义在于 ????
                pFrameDecoded->pts = av_rescale(frame_index,AV_TIME_BASE*(int64_t)pCodecCtxVE->time_base.num,
										(int64_t)pCodecCtxVE->time_base.den);
#endif

				// fet data must be YUV420P
                nFrameEncoded = avcodec_encode_video(pCodecCtxVE, pFrameEncoded,
													video_outbuf_size, pFrameDecoded);
                if (nFrameEncoded > 0)
                {
                    AVPacket pkt;
                    av_init_packet(&pkt);
                    pkt.stream_index = pOVideoStream->index;
                    pkt.data		 = pFrameEncoded;
                    pkt.size		 = nFrameEncoded;
                    pkt.flags		 = packet.flags;
                    if(pCodecCtxVE->coded_frame->key_frame)
						pkt.flags	|= PKT_FLAG_KEY;

					// av_interleaved_write_frame
					ret = av_write_frame(pOFmtCtx, &pkt);	if(ret < 0) printf("while write video frame error\n");
					av_free_packet(&pkt);
                }
                frame_index++;
            }
        }			// video

if (isHaveAudio)
{
	if(packet.stream_index == audioindex)
	{
		uint8_t *ptr	= packet.data;
		int len			= packet.size,temp;

		while(len > 0)
		{
			if(&packet)
                pSamplesDecoded	 = (short *)av_fast_realloc(pSamplesDecoded,&samples_size,FFMAX(packet.size*sizeof(*pSamplesDecoded),AVCODEC_MAX_AUDIO_FRAME_SIZE));
			temp	 = samples_size;
			ret		 = avcodec_decode_audio2(pCodecCtxAD,pSamplesDecoded,&temp,packet.data,packet.size); if(ret < 0) {printf("while decode audio failure\n");return 0;}
			if(temp <= 0) continue;
			fflush(stdout);
			ptr		+= ret;
			len		-= ret;
			AVPacket pkt;
			av_init_packet(&pkt);
			pkt.size = avcodec_encode_audio(pCodecCtxAE, pSampleEncoded,audio_outbuf_size,pSamplesDecoded);

#if 0
			// 时间戳在没有视频的情况下，实际上没有设置的必要
			if (pCodecCtxAE->coded_frame && pCodecCtxAE->coded_frame->pts!=AV_NOPTS_VALUE)
                pkt.pts	 = av_rescale_q(pCodecCtxAE->coded_frame->pts,pCodecCtxAE->time_base,pOAudioStream->time_base);
#endif

			pkt.stream_index = audioindex;
			pkt.data		 = pSampleEncoded;
			if(pCodecCtxAE->coded_frame->key_frame)
				pkt.flags	|= PKT_FLAG_KEY;

			// av_interleaved_write_frame
			ret = av_write_frame(pOFmtCtx, &pkt); if(ret < 0) {fprintf(stderr, "Error while writing audio frame\n");return 1;}

			av_free_packet(&pkt);
		}
	}
}

        av_free_packet(&packet);
    }

    av_write_trailer(pOFmtCtx);

/****   do the clean up    ****/
    for(i = 0; i<pOFmtCtx->nb_streams; i++)
    {
        av_freep(&pOFmtCtx->streams[i]->codec);
        av_freep(&pOFmtCtx->streams[i]);
    }
    av_free(pOFmtCtx);
    av_free(pFrameDecoded);
    avcodec_close(pCodecCtxVD);
	if (isHaveAudio)
	avcodec_close(pCodecCtxAD);
    av_close_input_file(pIFmtCtx);

    return 0;
}
