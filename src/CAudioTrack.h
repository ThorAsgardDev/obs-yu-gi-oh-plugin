
#ifndef __CAUDIO_TRACK_H__
#define __CAUDIO_TRACK_H__

class CAudioTrack {
	private:
		bool m_loop;
		size_t m_loopSample;
		size_t m_sizeInBytes;
		size_t m_sizeInSamples;
		short *m_pSamples;
		size_t m_currentSampleOffset;
		//double m_angle;
		//int f;

	public:
		CAudioTrack(const char *pFileName, bool loop, size_t loopSample);
		virtual ~CAudioTrack(void);

		void fill(short *pData, size_t size);
		bool isFinished(void);
};

#endif
