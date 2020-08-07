/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*\
** 文件名：	FFmpegDecoder.h
** 作者：	张垒
** 日期：	4/19/周四  10:20:37
** 功能：	FFmepg deocder
** 修改记录：
** 备注：	
\*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#ifndef FFmpegDecoder_H
#define FFmpegDecoder_H

#ifdef DLL_FILE
#   define FFMPEG_EXPORT _declspec(dllexport)
#else
#   define FFMPEG_EXPORT _declspec(dllimport)
#endif

extern "C"
{
#include "ffmpeg/include/avformat.h"
#include "ffmpeg/include/swscale.h"
}
#pragma comment(lib,"ffmpeg/lib/avformat.lib")
#pragma comment(lib,"ffmpeg/lib/avcodec.lib")
#pragma comment(lib,"ffmpeg/lib/avutil.lib")
///
/// @brief  The video decoder class for wrapping FFmpeg decoding API.
/// There may be several audio/video streams in the input file. However, only one stream is used here.
///
class FFMPEG_EXPORT FFmpegDecoder
{
	// structure
public:
	typedef struct FFmpegVideoParam
	{
		PixelFormat pixelFormat;    ///< The pixel format of the video
		int width;                  ///< The width of the video
		int height;                 ///< The height of the video
		int bitRate;                ///< The bit rate of the video
		int frameRate;              ///< The frame rate of the video
		std::string videoCodecName; ///< The name of the video codec
	} FFmpegVideoParam;
	
	typedef struct FFmpegAudioParam
	{
		int sampleRate;             ///< The sample rate of the audio
		int channels;               ///< The number of audio channels
		int bitRate;                ///< The bit rate of the audio
		std::string audioCodecName;	///< The name of the audio codec
	} FFmpegAudioParam;
//////////////////////////////////////////////////////////////////////////
    FFmpegVideoParam m_videoParam;        ///< The video parameters of the video to be decoded
    FFmpegAudioParam m_audioParam;        ///< The audio parameters of the audio to be decoded

	uint8_t *m_data[4];
	int m_linesize[4];
	int	m_iWidth;
	int m_iHeight;
//////////////////////////////////////////////////////////////////////////
public:
    FFmpegDecoder();
    virtual ~FFmpegDecoder();

public:
    void open(const char *fileName);
    void close();

public:
    int decodeFrame();
    const uint8_t *getVideoFrame() const;
    const uint8_t *getAudioFrame() const;
    int getVideoFrameSize() const;
    int getAudioFrameSize() const;

    double getPresentTimeStamp() const;
    double getDecodeTimeStamp() const;
//////////////////////////////////////////////////////////////////////////
private:
    bool m_bDecodeVideo;           ///< Whether video decoding is needed
    bool m_bDecodeAudio;           ///< Whether audio decoding is needed
    bool m_bOpened;                ///< Whether the FFmpegDecoder is opened yet

    AVFormatContext *m_inputFmtContext;		///< The input format context
    AVStream *m_videoStream;				///< The video output stream
    AVStream *m_audioStream;				///< The audio output stream
    AVPacket m_currentPacket;		///< the packet read from the input file currently
    uint8_t *m_videoFrameBuffer;	///< The buffer storing one output video frame data
    int      m_videoFrameSize;		///< The size of the output video frame
    int      m_videoBufferSize;		///< The total size of the video output buffer
    uint8_t *m_audioFrameBuffer;	///< The buffer storing one output audio frame data
    int      m_audioFrameSize;		///< The size of the output audio frame
    int      m_audioBufferSize;		///< The total size of the audio output buffer
    uint8_t *m_audioPacketData;		///< The remaining audio packet data to be decoded
    int      m_audioPacketSize;		///< The size of the remaining audio packet data to be decoded
    double m_currentPacketPts;		///< The presentation time stamp of the current packet
    double m_currentPacketDts;		///< The decompression time stamp of the current packet

private:
    int readFrame();

    void decodeVideoFrame();
    void decodeAudioFrame();
};

#endif//FFmpegDecoder_H
