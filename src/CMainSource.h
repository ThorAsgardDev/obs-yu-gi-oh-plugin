
#ifndef __CMAIN_SOURCE_H__
#define __CMAIN_SOURCE_H__

#include <obs-module.h>
#include <util/platform.h>
#include "CAudioEngine.h"

enum class EMAIN_SOURCE_SUB_SOURCE_TYPES {
	VIDEO = 0,
	PLAYER1_IMAGE,
	PLAYER1_NAME,
	PLAYER2_IMAGE,
	PLAYER2_NAME,
	
	N
};
#define CEMAIN_SOURCE_SUB_SOURCE_TYPES (int)EMAIN_SOURCE_SUB_SOURCE_TYPES

enum class EMAIN_SOURCE_ANIM_STATE {
	STOPPED = 0,
	WAITING_FOR_PLAYERS,
	SHOWING_PLAYERS,
	WAITING_FOR_FADE_OUT,
	FADING_OUT,
	WAITING_FOR_END,
	ENDING,

	N
};
#define CEMAIN_SOURCE_ANIM_STATE (int)EMAIN_SOURCE_ANIM_STATE

typedef struct {
	obs_source_t *m_pSource;
	bool m_visible;
	int m_x;
} MAIN_SOURCE_SUB_SOURCE;

class CMainSource {
	private:
		
		static const char *m_pImageFilter;

		obs_source_t *m_pSource;
		bool m_mute;
		char *m_pPlayer1ImageFileName;
		char *m_pPlayer1Name;
		char *m_pPlayer2ImageFileName;
		char *m_pPlayer2Name;
		EMAIN_SOURCE_ANIM_STATE m_animState;
		uint64_t m_startAnimTime;
		MAIN_SOURCE_SUB_SOURCE m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::N];
		CAudioEngine *m_pAudioEngine;
		uint32_t m_colorFilter;

		int playSound(const char *pFileName, bool loop, size_t loopSample);
		void copyString(char **pDest, const char *pSrc);
		void start(void);
		void stop(void);
		void setAnimState(EMAIN_SOURCE_ANIM_STATE animState);
		void updateAnimStateStopped(void);
		void updateAnimStateWaitingForPlayers(void);
		void updateAnimStateShowingPlayers(void);
		void updateAnimStateWaitingForFadeOut();
		void updateAnimStateFadingOut();
		void updateAnimStateWaitingForEnd(void);
		void updateAnimStateEnding(void);
		void updatePlayerImageX(MAIN_SOURCE_SUB_SOURCE *pSubSource, int64_t dt);
		void updatePlayerNameX(MAIN_SOURCE_SUB_SOURCE *pSubSource, int64_t dt);

		obs_source_t *createVideoSource(void);
		void setVideoSourceFileName(obs_source_t *pVideoSource, const char *pFileName);
		void renderVideoSource(obs_source_t *pVideoSource);
		obs_source_t *createImageSource(void);
		void setImageSourceFileName(obs_source_t *pImageSource, const char *pFileName);
		void renderImageSource(obs_source_t *pImageSource, int x, bool mirror);
		void computeImageSize(obs_source_t *pImageSource, uint32_t *pWidth, uint32_t *pHeight, float *pScale);
		obs_source_t *createTextSource(void);
		void setTextSourceText(obs_source_t *pTextSource, const char *pText);
		void renderTextSource(obs_source_t *pTextSource, int x, bool mirror);

		void update(obs_data_t *pSettings);
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
		CMainSource(obs_data_t *pSettings, obs_source_t *pSource);
		virtual ~CMainSource(void);

		static const char *m_pSourceIdName;

		static void sGetSourceInfo(obs_source_info *pSourceInfo);
};

#endif
