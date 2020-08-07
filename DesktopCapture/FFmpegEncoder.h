#ifndef FFmpegEncoder_H
#define FFmpegEncoder_H

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

#include <string>
using namespace std;

///
/// @brief  The video/audio encoder class for wrapping FFmpeg encoding API.
///
class FFMPEG_EXPORT FFmpegEncoder
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
	FFmpegVideoParam m_videoParam;        ///< The video parameters of the video to be encoded
	FFmpegAudioParam m_audioParam;        ///< The audio parameters of the audio to be encoded

public:
	FFmpegEncoder();
	virtual ~FFmpegEncoder();

	// [in] width          The width of the video frame, must be greater than 0
	// [in] height         The height of the video frame, must be greater than 0
	// [in] pixelFormat    PixelFormat enum representing the pixel format of the source video frame
	// [in] bitRate        The target bit rate of the target video stream, must be greater than 0
	// [in] frameRate      The frame rate of the target video, must be greater than 0
	// [in] videoCodecName The name of the video codec which is going to be used in encoding/decoding
	void setVideoParam(int width, int height, PixelFormat pixelFormat, int bitRate, int frameRate, std::string videoCodecName = "");
	
	// [in] sampleRate     The sample rate of the audio, must be greater than 0
	// [in] channels       The number of channels in the audio, must be greater than 0
	// [in] bitRate        The target bit rate of the target audio stream, must be greater than 0
	// [in] audioCodecName The name of the audio codec which is going to be used in encoding/decoding
	void setAudioParam(int sampleRate, int channels, int bitRate, std::string audioCodecName = "");

// operations
public:
	const uint8_t *getVideoEncodedBuffer() const;
	const uint8_t *getAudioEncodedBuffer() const;

    int getVideoFrameSize() const;
	int getAudioFrameSize() const;

	double getVideoTimeStamp() const;
	double getAudioTimeStamp() const;
// methods
public:
	void open(const char *fileName = NULL);
    void close();

	int encodeVideoFrame(const uint8_t *frameData);
    int encodeAudioFrame(const uint8_t *frameData, int dataSize = 0);

    int writeVideoFrame(const uint8_t *frameData);
    int writeAudioFrame(const uint8_t *frameData, int dataSize = 0);

//////////////////////////////////////////////////////////////////////////
// private implemention
private:
	bool m_bEncodeVideo;   ///< Whether video encoding is needed
	bool m_bEncodeAudio;   ///< Whether audio encoding is needed
	bool m_bHasOutput;     ///< Whether there is a output file for encoding
	bool m_bOpened;        ///< Whether the FFmpegEncoder is opened yet
	AVFormatContext *m_outputFmtContext;     ///< The output format context
	AVStream *m_videoStream;              ///< The video output stream
	AVStream *m_audioStream;              ///< The audio output stream
    AVPicture *m_videoFrame;              ///< The temporary video frame for pixel format conversion
	uint8_t *m_videoBuffer;       ///< The video output buffer
	int     m_videoBufferSize;    ///< The size of video output buffer
	uint8_t *m_audioBuffer;       ///< The audio output buffer
	int     m_audioBufferSize;    ///< The size of audio output buffer

private:
    int convertPixFmt(AVPicture *srcPic, AVPicture *dstPic, const FFmpegVideoParam *srcParam, const AVCodecContext *dstContext);

	int encodeVideoData(AVPicture *picture);
	int encodeAudioData(short *frameData, int dataSize);

	void writeVideoData(uint8_t *packetData, int packetSize);
	void writeAudioData(uint8_t *packetData, int packetSize);
};

#endif//FFmpegEncoder_H
