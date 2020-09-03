
#include "CLpSource.h"

#define WIDTH 807
#define HEIGHT 512

#define LP_RATE 91

#define SOURCE_ID "Yu_Gi_Oh_Lp"

const char *CLpSource::m_pSourceIdName = SOURCE_ID"_v1";

CLpSource::CLpSource(obs_data_t *pSettings, obs_source_t *pSource) {
	m_pSource = pSource;

	m_mute = false;
	m_lp = -1;
	m_displayedLp = m_lp;
	m_pAudioEngine = NULL;
	m_audioLoopId = AUDIO_ENGINE_UNDEFINED_TRACK_ID;
	
	memset(m_pSubSources, 0, CELP_SOURCE_SUB_SOURCE_TYPES::N * sizeof(obs_source_t *));

	m_pSubSources[CELP_SOURCE_SUB_SOURCE_TYPES::BACKGROUND_IMAGE] = createImageSource();
	m_pSubSources[CELP_SOURCE_SUB_SOURCE_TYPES::LP_TEXT] = createTextSource();

	m_pAudioEngine = new CAudioEngine(m_pSource);

	update(pSettings, true);

	char path[_MAX_PATH];
	snprintf(path, _MAX_PATH, "%s/lp-background.png", obs_get_module_data_path(obs_current_module()));
	setImageSourceFileName(m_pSubSources[CELP_SOURCE_SUB_SOURCE_TYPES::BACKGROUND_IMAGE], path);
}

CLpSource::~CLpSource(void) {
	if(m_pAudioEngine) {
		delete m_pAudioEngine;
		m_pAudioEngine = NULL;
	}

	for(int i = 0; i < CELP_SOURCE_SUB_SOURCE_TYPES::N; i++) {
		if(m_pSubSources[i]) {
			obs_source_release(m_pSubSources[i]);
			m_pSubSources[i] = NULL;
		}
	}
}

int CLpSource::playSound(const char *pFileName, bool loop, size_t loopSample) {
	if(!m_mute) {
		return m_pAudioEngine->play(pFileName, loop, loopSample);
	}
	return AUDIO_ENGINE_UNDEFINED_TRACK_ID;
}

void CLpSource::updateLpText(void) {
	if(m_displayedLp == -1) {
		setTextSourceText(m_pSubSources[CELP_SOURCE_SUB_SOURCE_TYPES::LP_TEXT], "");
	} else {
		char text[6]; // 6 because: Max value is "99999" + '\0'
		snprintf(text, 6, "%d", m_displayedLp);
		setTextSourceText(m_pSubSources[CELP_SOURCE_SUB_SOURCE_TYPES::LP_TEXT], text);
	}
}

obs_source_t *CLpSource::createImageSource(void) {
	obs_data_t *pSettings = obs_data_create();

	obs_data_set_bool(pSettings, "unload", false);
	obs_source_t *pSource = obs_source_create_private("image_source", NULL, pSettings);

	obs_data_release(pSettings);
	return pSource;
}

void CLpSource::setImageSourceFileName(obs_source_t *pImageSource, const char *pFileName) {
	if(pImageSource == NULL) {
		return;
	}

	obs_data_t *pSettings = obs_source_get_settings(pImageSource);

	obs_data_set_string(pSettings, "file", pFileName);
	obs_source_update(pImageSource, pSettings);

	obs_data_release(pSettings);
}

void CLpSource::renderImageSource(obs_source_t *pImageSource) {
	if(pImageSource == NULL) {
		return;
	}

	uint32_t width = obs_source_get_width(m_pSource);
	uint32_t height = obs_source_get_height(m_pSource);
	uint32_t imageSourceWidth = obs_source_get_width(pImageSource);
	uint32_t imageSourceHeight = obs_source_get_height(pImageSource);
	
	if(!imageSourceWidth || !imageSourceHeight) {
		return;
	}

	float scaleX = (float)width / imageSourceWidth;
	float scaleY = (float)height / imageSourceHeight;

	gs_matrix_push();
	gs_matrix_scale3f(scaleX, scaleY, 1.0f);
	
	obs_source_video_render(pImageSource);
	gs_matrix_pop();
}

obs_source_t *CLpSource::createTextSource(void) {
	obs_data_t *pSettings = obs_data_create();
	obs_data_t *pFont = obs_data_create();

	obs_data_set_string(pFont, "face", "Cambria");
	obs_data_set_int(pFont, "flags", OBS_FONT_BOLD | OBS_FONT_ITALIC);

	obs_data_set_int(pFont, "size", 320);
	obs_data_set_obj(pSettings, "font", pFont);

	// Color A B G R
	obs_data_set_int(pSettings, "color", 0xff12d0eb);
	obs_data_set_bool(pSettings, "outline", true);
	obs_data_set_int(pSettings, "outline_size", 10);
	obs_data_set_int(pSettings, "outline_color", 0xff000000);
	
	obs_source_t *pSource = obs_source_create_private("text_gdiplus", NULL, pSettings);

	obs_data_release(pFont);
	obs_data_release(pSettings);
	return pSource;
}

void CLpSource::setTextSourceText(obs_source_t *pTextSource, const char *pText) {
	if(pTextSource == NULL) {
		return;
	}

	obs_data_t *pSettings = obs_source_get_settings(pTextSource);

	obs_data_set_string(pSettings, "text", pText);
	obs_source_update(pTextSource, pSettings);

	obs_data_release(pSettings);
}

void CLpSource::renderTextSource(obs_source_t *pTextSource) {
	if(pTextSource == NULL) {
		return;
	}

	uint32_t width = obs_source_get_width(m_pSource);
	uint32_t height = obs_source_get_height(m_pSource);
	uint32_t textSourceWidth = obs_source_get_width(pTextSource);
	uint32_t textSourceHeight = obs_source_get_height(pTextSource);

	if(!textSourceWidth || !textSourceHeight) {
		return;
	}

	int deltaX = (width - textSourceWidth) / 2;
	int deltaY = (height - textSourceHeight) / 2;

	gs_matrix_push();
	gs_matrix_translate3f((float)deltaX, (float)deltaY, 0.0f);

	obs_source_video_render(pTextSource);
	gs_matrix_pop();
}

void CLpSource::update(obs_data_t *pSettings, bool fromCtor) {
	m_lp = (int)obs_data_get_int(pSettings, "life_points");

	if(m_lp < -1) {
		m_lp = -1;
	} else if(m_lp >= 99999) {
		m_lp = 99999;
	}

	m_mute = obs_data_get_bool(pSettings, "mute");
	bool initMute = obs_data_get_bool(pSettings, "init_mute");
	bool initMode = obs_data_get_bool(pSettings, "init_mode");

	m_pAudioEngine->stopAll();

	if(fromCtor || m_lp == -1) {
		m_displayedLp = m_lp;
	} else if(initMode) {
		m_displayedLp = m_lp;
		if(!initMute) {
			char path[_MAX_PATH];
			snprintf(path, _MAX_PATH, "%s/lp-init.raw", obs_get_module_data_path(obs_current_module()));
			playSound(path, false, 0);
		}
	} else if(m_lp != m_displayedLp) {
		char path[_MAX_PATH];
		snprintf(path, _MAX_PATH, "%s/lp-loop.raw", obs_get_module_data_path(obs_current_module()));
		m_audioLoopId = playSound(path, true, 6554);
	}

	updateLpText();
}

uint32_t CLpSource::getWidth(void) {
	return WIDTH;
}

uint32_t CLpSource::getHeight(void) {
	return HEIGHT;
}

void CLpSource::videoRender(gs_effect_t *pEffect) {
	renderImageSource(m_pSubSources[CELP_SOURCE_SUB_SOURCE_TYPES::BACKGROUND_IMAGE]);
	renderTextSource(m_pSubSources[CELP_SOURCE_SUB_SOURCE_TYPES::LP_TEXT]);
}

obs_properties_t *CLpSource::getProperties(void) {
	obs_properties_t *pProperties = obs_properties_create();

	obs_properties_add_int(pProperties, "life_points", obs_module_text("Life points"), -1, 99999, 1);
	obs_properties_add_bool(pProperties, "mute", obs_module_text("Mute"));
	obs_properties_add_bool(pProperties, "init_mute", obs_module_text("Init mute"));
	obs_properties_add_bool(pProperties, "init_mode", obs_module_text("Init mode"));

	return pProperties;
}

void CLpSource::videoTick(float seconds) {
	if(m_pAudioEngine) {
		m_pAudioEngine->update();
	}
	if(m_displayedLp != m_lp) {
		if(m_displayedLp < m_lp) {
			if(m_displayedLp + LP_RATE >= m_lp) {
				m_displayedLp = m_lp;
			} else {
				m_displayedLp += LP_RATE;
			}
		} else {
			if(m_displayedLp - LP_RATE < m_lp) {
				m_displayedLp = m_lp;
			} else {
				m_displayedLp -= LP_RATE;
			}
		}

		if(m_displayedLp == m_lp) {
			if(m_pAudioEngine) {
				m_pAudioEngine->stop(m_audioLoopId);
				m_audioLoopId = AUDIO_ENGINE_UNDEFINED_TRACK_ID;

				if(m_lp == 0) {
					char path[_MAX_PATH];
					snprintf(path, _MAX_PATH, "%s/lp-zero.raw", obs_get_module_data_path(obs_current_module()));
					playSound(path, false, 0);
				} else {
					char path[_MAX_PATH];
					snprintf(path, _MAX_PATH, "%s/lp-target-reached.raw", obs_get_module_data_path(obs_current_module()));
					playSound(path, false, 0);
				}
			}
		}

		updateLpText();
	}
}

void *CLpSource::sCreate(obs_data_t *pSettings, obs_source_t *pSource) {
	return new CLpSource(pSettings, pSource);
}

void CLpSource::sDestroy(void *pData) {
	CLpSource *pLpSource = (CLpSource *)pData;
	if(pLpSource) {
		delete pLpSource;
	}
}

void CLpSource::sUpdate(void *pData, obs_data_t *pSettings) {
	CLpSource *pLpSource = (CLpSource *)pData;
	if(pLpSource) {
		pLpSource->update(pSettings, false);
	}
}

const char *CLpSource::sGetName(void *pTypeData) {
	return obs_module_text("Yu-Gi-Oh LP");
}

void CLpSource::sDefaults(obs_data_t *pSettings) {
	obs_data_set_default_int(pSettings, "life_points", -1);
	obs_data_set_default_bool(pSettings, "mute", false);
	obs_data_set_default_bool(pSettings, "init_mute", false);
	obs_data_set_default_bool(pSettings, "init_mode", true);
}

uint32_t CLpSource::sGetWidth(void *pData) {
	CLpSource *pLpSource = (CLpSource *)pData;
	if(pLpSource) {
		return pLpSource->getWidth();
	}
	return 0;
}

uint32_t CLpSource::sGetHeight(void *pData) {
	CLpSource *pLpSource = (CLpSource *)pData;
	if(pLpSource) {
		return pLpSource->getHeight();
	}
	return 0;
}

void CLpSource::sVideoRender(void *pData, gs_effect_t *pEffect) {
	CLpSource *pLpSource = (CLpSource *)pData;
	if(pLpSource) {
		pLpSource->videoRender(pEffect);
	}
}

obs_properties_t *CLpSource::sGetProperties(void *pData) {
	CLpSource *pLpSource = (CLpSource *)pData;
	if(pLpSource) {
		return pLpSource->getProperties();
	}
	return NULL;
}

void CLpSource::sVideoTick(void *pData, float seconds) {
	CLpSource *pLpSource = (CLpSource *)pData;
	if(pLpSource) {
		pLpSource->videoTick(seconds);
	}
}

void CLpSource::sGetSourceInfo(obs_source_info *pSourceInfo) {
	memset(pSourceInfo, 0, sizeof(obs_source_info));

	pSourceInfo->id = SOURCE_ID;
	pSourceInfo->version = 1;
	pSourceInfo->type = OBS_SOURCE_TYPE_INPUT;
	pSourceInfo->output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_CUSTOM_DRAW | OBS_SOURCE_AUDIO;
	pSourceInfo->create = sCreate;
	pSourceInfo->destroy = sDestroy;
	pSourceInfo->update = sUpdate;
	pSourceInfo->get_name = sGetName;
	pSourceInfo->get_defaults = sDefaults;
	pSourceInfo->get_width = sGetWidth;
	pSourceInfo->get_height = sGetHeight;
	pSourceInfo->video_render = sVideoRender;
	pSourceInfo->get_properties = sGetProperties;
	pSourceInfo->icon_type = OBS_ICON_TYPE_COLOR;
	pSourceInfo->video_tick = sVideoTick;
}
