
#ifndef __CAUDIO_TRACK_H__
#define __CAUDIO_TRACK_H__

#include <stdlib.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

class CAudioTrack {
	private:
		bool m_loop;
		size_t m_loopSample;
		bool m_finished;

		OggVorbis_File m_vorbisFile;

	public:
		CAudioTrack(const char *pFileName, bool loop, size_t loopSample);
		virtual ~CAudioTrack(void);

		void fill(void *pData, size_t sizeInBytes);
		bool isFinished(void);
};

#endif
