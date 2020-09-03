
#ifndef __CAUDIO_ENGINE_H__
#define __CAUDIO_ENGINE_H__

#include <obs.h>
#include "CAudioTrack.h"

#define AUDIO_ENGINE_MAX_TRACKS 4
#define AUDIO_ENGINE_UNDEFINED_TRACK_ID (-1)

#define AUDIO_ENGINE_SAMPLE_RATE 48000
#define AUDIO_ENGINE_BUFFER_SIZE_IN_MS 50
#define AUDIO_ENGINE_BUFFER_SIZE_IN_NS (AUDIO_ENGINE_BUFFER_SIZE_IN_MS * 1000000)
#define AUDIO_ENGINE_BUFFER_SAMPLE_SIZE 2
#define AUDIO_ENGINE_BUFFER_SIZE_IN_SAMPLES (AUDIO_ENGINE_BUFFER_SIZE_IN_MS * AUDIO_ENGINE_SAMPLE_RATE / 1000)
#define AUDIO_ENGINE_BUFFER_SIZE_IN_BYTES (AUDIO_ENGINE_BUFFER_SIZE_IN_SAMPLES * AUDIO_ENGINE_BUFFER_SAMPLE_SIZE)
#define AUDIO_ENGINE_NB_BUFFERS 4


class CAudioEngine
{
	private:
		obs_source_t *m_pSource;
		uint64_t m_timestamp;
		uint64_t m_bufferTimestamps[AUDIO_ENGINE_NB_BUFFERS];
		CAudioTrack *m_pTracks[AUDIO_ENGINE_MAX_TRACKS];

		short m_trackBuffer[AUDIO_ENGINE_BUFFER_SIZE_IN_SAMPLES];
		short m_mixBuffer[AUDIO_ENGINE_BUFFER_SIZE_IN_SAMPLES];

		void sendAudioBufferToObs(short *pData, uint64_t timestamp);

	public:
		CAudioEngine(obs_source_t *pSource);
		virtual ~CAudioEngine(void);

		int play(const char *pFileName, bool loop, size_t loopSample);
		void stop(int trackId);
		void stopAll(void);
		void update(void);
};

#endif
