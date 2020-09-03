
#include "CAudioEngine.h"
#include <util/platform.h>

CAudioEngine::CAudioEngine(obs_source_t *pSource) {
	m_pSource = pSource;
	
	memset(m_trackBuffer, 0, AUDIO_ENGINE_BUFFER_SIZE_IN_BYTES);
	memset(m_mixBuffer, 0, AUDIO_ENGINE_BUFFER_SIZE_IN_BYTES);
	memset(m_bufferTimestamps, 0, AUDIO_ENGINE_NB_BUFFERS * sizeof(uint64_t));
	memset(m_pTracks, 0, AUDIO_ENGINE_MAX_TRACKS * sizeof(CAudioTrack *));

	m_timestamp = os_gettime_ns();
	for(int i = 0; i < AUDIO_ENGINE_NB_BUFFERS; i++) {
		memset(m_mixBuffer, 0, AUDIO_ENGINE_BUFFER_SIZE_IN_BYTES);
		m_bufferTimestamps[i] = m_timestamp;
		sendAudioBufferToObs(m_mixBuffer, m_timestamp);
		m_timestamp += AUDIO_ENGINE_BUFFER_SIZE_IN_NS;
	}
}

CAudioEngine::~CAudioEngine(void) {
	for(int i = 0; i < AUDIO_ENGINE_MAX_TRACKS; i++) {
		if(m_pTracks[i]) {
			delete m_pTracks[i];
			m_pTracks[i] = NULL;
		}
	}
}

void CAudioEngine::sendAudioBufferToObs(short *pData, uint64_t timestamp) {
	struct obs_source_audio audio;
	audio.data[0] = (unsigned char *)pData;
	audio.frames = AUDIO_ENGINE_BUFFER_SIZE_IN_SAMPLES;
	audio.speakers = SPEAKERS_MONO;
	audio.samples_per_sec = AUDIO_ENGINE_SAMPLE_RATE;
	audio.timestamp = timestamp;
	audio.format = AUDIO_FORMAT_16BIT;
	obs_source_output_audio(m_pSource, &audio);
}

int CAudioEngine::play(const char *pFileName, bool loop, size_t loopSample) {
	for(int i = 0; i < AUDIO_ENGINE_MAX_TRACKS; i++) {
		if(m_pTracks[i] == NULL) {
			m_pTracks[i] = new CAudioTrack(pFileName, loop, loopSample);
			return i;
		}
	}

	return AUDIO_ENGINE_UNDEFINED_TRACK_ID;
}

void CAudioEngine::stop(int trackId) {
	if(trackId == AUDIO_ENGINE_UNDEFINED_TRACK_ID) {
		return;
	}
	if(m_pTracks[trackId]) {
		delete m_pTracks[trackId];
		m_pTracks[trackId] = NULL;
	}
}

void CAudioEngine::stopAll(void) {
	for(int i = 0; i < AUDIO_ENGINE_MAX_TRACKS; i++) {
		if(m_pTracks[i]) {
			delete m_pTracks[i];
			m_pTracks[i] = NULL;
		}
	}
}

void CAudioEngine::update(void) {
	uint64_t timestamp = os_gettime_ns();

	for(int i = 0; i < AUDIO_ENGINE_NB_BUFFERS; i++) {
		// If buffer played
		if(timestamp >= m_bufferTimestamps[i] + AUDIO_ENGINE_BUFFER_SIZE_IN_NS) {
			memset(m_mixBuffer, 0, AUDIO_ENGINE_BUFFER_SIZE_IN_BYTES);

			for(int j = 0; j < AUDIO_ENGINE_MAX_TRACKS; j++) {
				if(m_pTracks[j]) {
					memset(m_trackBuffer, 0, AUDIO_ENGINE_BUFFER_SIZE_IN_BYTES);

					m_pTracks[j]->fill(m_trackBuffer, AUDIO_ENGINE_BUFFER_SIZE_IN_SAMPLES);
					
					for(int k = 0; k < AUDIO_ENGINE_BUFFER_SIZE_IN_SAMPLES; k++) {
						long mix = m_mixBuffer[k] + m_trackBuffer[k];
						if(mix < -32768)
							mix = -32768;
						else if(mix > 32767)
							mix = 32767;
						m_mixBuffer[k] = (short)mix;
					}
					
					if(m_pTracks[j]->isFinished()) {
						delete m_pTracks[j];
						m_pTracks[j] = NULL;
					}
				}
			}

			m_bufferTimestamps[i] = m_timestamp;
			sendAudioBufferToObs(m_mixBuffer, m_timestamp);
			m_timestamp += AUDIO_ENGINE_BUFFER_SIZE_IN_NS;
		}
	}
}
