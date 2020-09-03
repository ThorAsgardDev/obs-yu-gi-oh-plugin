
#ifndef __CLP_SOURCE_H__
#define __CLP_SOURCE_H__

#include <obs-module.h>
#include <util/platform.h>
#include "CAudioEngine.h"

enum class ELP_SOURCE_SUB_SOURCE_TYPES {
	BACKGROUND_IMAGE,
	LP_TEXT,
	
	N
};
#define CELP_SOURCE_SUB_SOURCE_TYPES (int)ELP_SOURCE_SUB_SOURCE_TYPES

class CLpSource {
	private:
		
		obs_source_t *m_pSource;
		int m_lp;
		bool m_mute;
		obs_source_t *m_pSubSources[CELP_SOURCE_SUB_SOURCE_TYPES::N];
		CAudioEngine *m_pAudioEngine;
		int m_displayedLp;
		int m_audioLoopId;

		int playSound(const char *pFileName, bool loop, size_t loopSample);
		void updateLpText(void);

		obs_source_t *createImageSource(void);
		void setImageSourceFileName(obs_source_t *pImageSource, const char *pFileName);
		void renderImageSource(obs_source_t *pImageSource);
		obs_source_t *createTextSource(void);
		void setTextSourceText(obs_source_t *pTextSource, const char *pText);
		void renderTextSource(obs_source_t *pTextSource);

		void update(obs_data_t *pSettings, bool fromCtor);
		uint32_t getWidth(void);
		uint32_t getHeight(void);
		void videoRender(gs_effect_t *pEffect);
		obs_properties_t *getProperties(void);
		void videoTick(float seconds);

		static void *sCreate(obs_data_t *pSettings, obs_source_t *pSource);
		static void sDestroy(void *pData);
		static void sUpdate(void *pData, obs_data_t *pSettings);
		static const char *sGetName(void *pTypeData);
		static void sDefaults(obs_data_t *pSettings);
		static uint32_t sGetWidth(void *pData);
		static uint32_t sGetHeight(void *pData);
		static void sVideoRender(void *pData, gs_effect_t *pEffect);
		static obs_properties_t *sGetProperties(void *pData);
		static void sVideoTick(void *pData, float seconds);

	public:
		CLpSource(obs_data_t *pSettings, obs_source_t *pSource);
		virtual ~CLpSource(void);

		static const char *m_pSourceIdName;

		static void sGetSourceInfo(obs_source_info *pSourceInfo);
};

#endif
