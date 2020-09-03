
#include "CAudioTrack.h"
#include <stdio.h>
#include <memory.h>

#define TRACK_OGG_NBITS_PER_SAMPLE 16

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

CAudioTrack::CAudioTrack(const char *pFileName, bool loop, size_t loopSample) {
	m_loop = loop;
	m_loopSample = loopSample;
	m_finished = false;

	ov_fopen(pFileName, &m_vorbisFile);
}

CAudioTrack::~CAudioTrack(void) {
	ov_clear(&m_vorbisFile);
}

void CAudioTrack::fill(void *pData, size_t sizeInBytes) {
	size_t nbRead;
	
	nbRead = 0;

	while(nbRead < sizeInBytes) {
		int sec;
		long ret;

		ret = ov_read(&m_vorbisFile, ((char *)(pData)) + nbRead, sizeInBytes - nbRead, 0, 2, 1, &sec);
		nbRead += ret;

		if(ret == 0 || (ov_pcm_tell(&m_vorbisFile) == ov_pcm_total(&m_vorbisFile, -1))) {
			if(!m_loop) {
				m_finished = true;
				break;
			} else {
				ov_pcm_seek(&m_vorbisFile, m_loopSample);
				
				ret = ov_read(&m_vorbisFile, ((char *)(pData)) + nbRead, sizeInBytes - nbRead, 0, 2, 1, &sec);
				nbRead += ret;

				if(ret == 0)
					break;
			}
		}
	}
}

bool CAudioTrack::isFinished(void) {
	return m_finished;
}
