
#include "CAudioTrack.h"
#include <stdio.h>
#include <memory.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

//#define M_PI_X2 (3.1415926535897932384626433832795 * 2)


CAudioTrack::CAudioTrack(const char *pFileName, bool loop, size_t loopSample) {
	m_loop = loop;
	m_loopSample = loopSample;
	m_sizeInBytes = 0;
	m_sizeInSamples = 0;

	m_currentSampleOffset = 0;
	m_pSamples = NULL;

	FILE *f;
	fopen_s(&f, pFileName, "rb");
	if(f) {
		fseek(f, 0, SEEK_END);
		m_sizeInBytes = ftell(f);
		m_sizeInSamples = m_sizeInBytes / 2;
		m_pSamples = new short[m_sizeInSamples];
		fseek(f, 0, SEEK_SET);
		fread(m_pSamples, 2, m_sizeInSamples, f);
		fclose(f);
	}

	//m_angle = 0;
}

CAudioTrack::~CAudioTrack(void) {
	if(m_pSamples) {
		delete[] m_pSamples;
		m_pSamples = NULL;
	}
}

void CAudioTrack::fill(short *pData, size_t size) {
	size_t totalSamplesWritten = 0;

	while(m_currentSampleOffset < m_sizeInSamples && totalSamplesWritten < size) {
		size_t missingSamples = size - totalSamplesWritten;
		size_t nbSamplesWritable = m_sizeInSamples - m_currentSampleOffset;
		size_t nbSampleWritten = MIN(
			size - totalSamplesWritten, // missing samples
			m_sizeInSamples - m_currentSampleOffset // available samples
		);

		memcpy(&pData[totalSamplesWritten], &m_pSamples[m_currentSampleOffset], nbSampleWritten * 2);

		totalSamplesWritten += nbSampleWritten;
		m_currentSampleOffset += nbSampleWritten;

		if(m_currentSampleOffset >= m_sizeInSamples && m_loop) {
			m_currentSampleOffset = m_loopSample;
		}
	}

	/*for (size_t i = 0; i < size; i++) {
		pData[i] = (short)(sin(m_angle) * 32767.0);

		m_angle += 0.05;
		if (m_angle > M_PI_X2)
			m_angle -= M_PI_X2;

		//int u = (double)rand() / (RAND_MAX + 1) * (65535 - 0) + 0;
		//pData[i] = (short)(u - 32768);
	}*/
}

bool CAudioTrack::isFinished(void) {
	if(m_currentSampleOffset >= m_sizeInSamples) {
		return true;
	}
	return false;
}
