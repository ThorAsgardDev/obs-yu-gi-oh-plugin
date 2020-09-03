
#include "CMainSource.h"

#define OS_MAX_PATH 260

#define WIDTH 1920
#define HEIGHT 1080
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) >= (y) ? (x) : (y))
#define SHOWING_PLAYERS_TIMESTAMP 4000
#define SHOWING_PLAYERS_DURATION 200
#define FADING_OUT_TIMESTAMP 9500
#define FADING_OUT_DURATION 200

#define SOURCE_ID "Yu_Gi_Oh_Main"

const char *CMainSource::m_pSourceIdName = SOURCE_ID"_v1";

const char *CMainSource::m_pImageFilter =
	"All formats (*.bmp *.tga *.png *.jpeg *.jpg *.gif *.psd);;"
	"BMP Files (*.bmp);;"
	"Targa Files (*.tga);;"
	"PNG Files (*.png);;"
	"JPEG Files (*.jpeg *.jpg);;"
	"GIF Files (*.gif);;"
	"PSD Files (*.psd);;"
	"All Files (*.*)";

CMainSource::CMainSource(obs_data_t *pSettings, obs_source_t *pSource) {
	m_pSource = pSource;

	m_mute = false;
	m_pPlayer1ImageFileName = NULL;
	m_pPlayer1Name = NULL;
	m_pPlayer2ImageFileName = NULL;
	m_pPlayer2Name = NULL;

	setAnimState(EMAIN_SOURCE_ANIM_STATE::STOPPED);

	m_colorFilter = 0;
	m_pAudioEngine = NULL;

	memset(m_pSubSources, 0, CEMAIN_SOURCE_SUB_SOURCE_TYPES::N * sizeof(MAIN_SOURCE_SUB_SOURCE));

	m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::VIDEO].m_pSource = createVideoSource();
	m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER1_IMAGE].m_pSource = createImageSource();
	m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER1_NAME].m_pSource = createTextSource();
	m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER2_IMAGE].m_pSource = createImageSource();
	m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER2_NAME].m_pSource = createTextSource();
	
	update(pSettings);
}

CMainSource::~CMainSource(void) {
	if(m_pAudioEngine) {
		delete m_pAudioEngine;
		m_pAudioEngine = NULL;
	}

	for(int i = 0; i < CEMAIN_SOURCE_SUB_SOURCE_TYPES::N; i++) {
		if(m_pSubSources[i].m_pSource) {
			obs_source_release(m_pSubSources[i].m_pSource);
			m_pSubSources[i].m_pSource = NULL;
		}
	}

	if(m_pPlayer1ImageFileName) {
		delete[] m_pPlayer1ImageFileName;
		m_pPlayer1ImageFileName = NULL;
	}

	if(m_pPlayer1Name) {
		delete[] m_pPlayer1Name;
		m_pPlayer1Name = NULL;
	}

	if(m_pPlayer2ImageFileName) {
		delete[] m_pPlayer2ImageFileName;
		m_pPlayer2ImageFileName = NULL;
	}

	if(m_pPlayer2Name) {
		delete[] m_pPlayer2Name;
		m_pPlayer2Name = NULL;
	}
}

int CMainSource::playSound(const char *pFileName, bool loop, size_t loopSample) {
	if(!m_mute) {
		return m_pAudioEngine->play(pFileName, loop, loopSample);
	}
	return AUDIO_ENGINE_UNDEFINED_TRACK_ID;
}

void CMainSource::setAnimState(EMAIN_SOURCE_ANIM_STATE animState) {
	m_animState = animState;
}

obs_source_t *CMainSource::createVideoSource(void) {
	obs_data_t *pSettings = obs_data_create();
	
	obs_data_set_bool(pSettings, "restart_on_activate", false);
	obs_source_t *pSource = obs_source_create_private("ffmpeg_source", "video", pSettings);
	//obs_source_t *pSource = obs_source_create("ffmpeg_source", NULL, pSettings, NULL);

	obs_data_release(pSettings);
	return pSource;
}

void CMainSource::setVideoSourceFileName(obs_source_t *pVideoSource, const char *pFileName) {
	if(pVideoSource == NULL) {
		return;
	}

	obs_data_t *pSettings = obs_source_get_settings(pVideoSource);

	obs_data_set_string(pSettings, "local_file", pFileName);
	obs_source_update(pVideoSource, pSettings);

	obs_data_release(pSettings);
}

void CMainSource::renderVideoSource(obs_source_t *pVideoSource)
{
	if(pVideoSource == NULL) {
		return;
	}

	uint32_t width = obs_source_get_width(m_pSource);
	uint32_t height = obs_source_get_height(m_pSource);
	uint32_t videoSourceWidth = obs_source_get_width(pVideoSource);
	uint32_t videoSourceHeight = obs_source_get_height(pVideoSource);

	if(!videoSourceWidth || !videoSourceHeight) {
		return;
	}

	gs_matrix_push();
	gs_matrix_scale3f(
		(float)width / (float)videoSourceWidth, 
		(float)height / (float)videoSourceHeight, 
		1.0f);

	obs_source_video_render(pVideoSource);
	gs_matrix_pop();
}

obs_source_t *CMainSource::createImageSource(void) {
	obs_data_t *pSettings = obs_data_create();

	obs_data_set_bool(pSettings, "unload", false);
	obs_source_t *pSource = obs_source_create_private("image_source", NULL, pSettings);

	obs_data_release(pSettings);
	return pSource;
}

void CMainSource::setImageSourceFileName(obs_source_t *pImageSource, const char *pFileName) {
	if(pImageSource == NULL) {
		return;
	}

	obs_data_t *pSettings = obs_source_get_settings(pImageSource);

	obs_data_set_string(pSettings, "file", pFileName);
	obs_source_update(pImageSource, pSettings);

	obs_data_release(pSettings);
}

void CMainSource::renderImageSource(obs_source_t *pImageSource, int x, bool mirror) {
	if(pImageSource == NULL) {
		return;
	}

	uint32_t width = obs_source_get_width(m_pSource);
	uint32_t height = obs_source_get_height(m_pSource);
	uint32_t imageSourceWidth;
	uint32_t imageSourceHeight;
	float scale;

	computeImageSize(pImageSource, &imageSourceWidth, &imageSourceHeight, &scale);

	if(!imageSourceWidth || !imageSourceHeight) {
		return;
	}

	int deltaX;
	if(mirror) {
		deltaX = width - x;
	} else {
		deltaX = x;
	}
	int deltaY = height - imageSourceHeight;

	gs_matrix_push();
	gs_matrix_translate3f((float)deltaX, (float)deltaY, 0.0f);
	if(mirror) {
		gs_matrix_scale3f(-scale, scale, 1.0f);
	} else {
		gs_matrix_scale3f(scale, scale, 1.0f);
	}
	obs_source_video_render(pImageSource);
	gs_matrix_pop();
}

void CMainSource::computeImageSize(obs_source_t *pImageSource, uint32_t *pWidth, uint32_t *pHeight, float *pScale) {
	float targetWidth = obs_source_get_width(m_pSource) / 2.2f;
	float targetHeight = obs_source_get_height(m_pSource) - 30.0f;
	uint32_t imageSourceWidth = obs_source_get_width(pImageSource);
	uint32_t imageSourceHeight = obs_source_get_height(pImageSource);

	float scale = MIN(
		(float)targetWidth / imageSourceWidth, // scaleX
		(float)targetHeight / imageSourceHeight // scaleY
	);
	
	if(pWidth) {
		*pWidth = (uint32_t)(imageSourceWidth * scale);
	}
	if(pHeight) {
		*pHeight = (uint32_t)(imageSourceHeight * scale);
	}
	if(pScale) {
		*pScale = scale;
	}
}

obs_source_t *CMainSource::createTextSource(void) {
	obs_data_t *pSettings = obs_data_create();
	obs_data_t *pFont = obs_data_create();

	obs_data_set_string(pFont, "face", "Arial");
	obs_data_set_int(pFont, "flags", OBS_FONT_BOLD);

	obs_data_set_int(pFont, "size", 128);
	obs_data_set_obj(pSettings, "font", pFont);

	// Color A B G R
	obs_data_set_int(pSettings, "color", 0xffffffff);
	obs_data_set_bool(pSettings, "outline", true);
	obs_data_set_int(pSettings, "outline_size", 20);
	obs_data_set_int(pSettings, "outline_color", 0xff000000);
	
	obs_source_t *pSource = obs_source_create_private("text_gdiplus", NULL, pSettings);

	obs_data_release(pFont);
	obs_data_release(pSettings);
	return pSource;
}

void CMainSource::setTextSourceText(obs_source_t *pTextSource, const char *pText) {
	if(pTextSource == NULL) {
		return;
	}

	obs_data_t *pSettings = obs_source_get_settings(pTextSource);

	obs_data_set_string(pSettings, "text", pText);
	obs_source_update(pTextSource, pSettings);

	obs_data_release(pSettings);
}

void CMainSource::renderTextSource(obs_source_t *pTextSource, int x, bool mirror) {
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

	int deltaX;
	if(mirror) {
		deltaX = width - textSourceWidth - x;
	} else {
		deltaX = x;
	}
	int deltaY = height - textSourceHeight - 75;

	gs_matrix_push();
	gs_matrix_translate3f((float)deltaX, (float)deltaY, 0.0f);

	obs_source_video_render(pTextSource);
	gs_matrix_pop();
}

void CMainSource::copyString(char **pDest, const char *pSrc) {
	if(*pDest) {
		delete[] (*pDest);
	}
	size_t length = strlen(pSrc) + 1;
	*pDest = new char[length];
	memcpy(*pDest, pSrc, length);
}

void CMainSource::update(obs_data_t *pSettings) {
	m_mute = obs_data_get_bool(pSettings, "mute");
	copyString(&m_pPlayer1ImageFileName, obs_data_get_string(pSettings, "player_1_image_file_name"));
	copyString(&m_pPlayer1Name, obs_data_get_string(pSettings, "player_1_name"));
	copyString(&m_pPlayer2ImageFileName, obs_data_get_string(pSettings, "player_2_image_file_name"));
	copyString(&m_pPlayer2Name, obs_data_get_string(pSettings, "player_2_name"));

	bool play = obs_data_get_bool(pSettings, "play");
	if(play && m_animState == EMAIN_SOURCE_ANIM_STATE::STOPPED) {
		start();
	} else {
		stop();
	}
}

uint32_t CMainSource::getWidth(void) {
	return WIDTH;
}

uint32_t CMainSource::getHeight(void) {
	return HEIGHT;
}

void CMainSource::videoRender(gs_effect_t *pEffect) {
	if(m_animState == EMAIN_SOURCE_ANIM_STATE::STOPPED) {
		return;
	}

	renderVideoSource(m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::VIDEO].m_pSource);
	
	MAIN_SOURCE_SUB_SOURCE *pSubSource;
	uint32_t height = obs_source_get_height(m_pSource);

	pSubSource = &m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER1_IMAGE];
	if(pSubSource->m_visible) {
		renderImageSource(pSubSource->m_pSource, pSubSource->m_x, false);
	}

	pSubSource = &m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER2_IMAGE];
	if(pSubSource->m_visible) {
		renderImageSource(pSubSource->m_pSource, pSubSource->m_x, true);
	}

	pSubSource = &m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER1_NAME];
	if(pSubSource->m_visible) {
		renderTextSource(pSubSource->m_pSource, pSubSource->m_x, false);
	}

	pSubSource = &m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER2_NAME];
	if(pSubSource->m_visible) {
		renderTextSource(pSubSource->m_pSource, pSubSource->m_x, true);
	}
	
	gs_effect_t *solid = obs_get_base_effect(OBS_EFFECT_SOLID);
	gs_eparam_t *color = gs_effect_get_param_by_name(solid, "color");
	gs_technique_t *tech = gs_effect_get_technique(solid, "Solid");
	
	struct vec4 colorVal;
	vec4_from_rgba(&colorVal, m_colorFilter);
	gs_effect_set_vec4(color, &colorVal);
	
	gs_technique_begin(tech);
	gs_technique_begin_pass(tech, 0);
	
	gs_draw_sprite(0, 0, WIDTH, HEIGHT);
	
	gs_technique_end_pass(tech);
	gs_technique_end(tech);
}

obs_properties_t *CMainSource::getProperties(void) {
	obs_properties_t *pProperties = obs_properties_create();

	obs_properties_add_bool(pProperties, "mute", obs_module_text("Mute"));
	obs_properties_add_path(
		pProperties,
		"player_1_image_file_name",
		obs_module_text("Player 1 image file"),
		OBS_PATH_FILE,
		m_pImageFilter,
		NULL);
	obs_properties_add_text(pProperties, "player_1_name", obs_module_text("Player 1 name"), OBS_TEXT_DEFAULT);

	obs_properties_add_path(
		pProperties,
		"player_2_image_file_name",
		obs_module_text("Player 2 image file"),
		OBS_PATH_FILE,
		m_pImageFilter,
		NULL);
	obs_properties_add_text(pProperties, "player_2_name", obs_module_text("Player 2 name"), OBS_TEXT_DEFAULT);

	obs_properties_add_bool(pProperties, "play", obs_module_text("Play"));

	return pProperties;
}

void CMainSource::videoTick(float seconds) {
	if(m_pAudioEngine) {
		m_pAudioEngine->update();
	}

	switch(m_animState) {
		case EMAIN_SOURCE_ANIM_STATE::STOPPED:
			updateAnimStateStopped();
			break;
		case EMAIN_SOURCE_ANIM_STATE::WAITING_FOR_PLAYERS:
			updateAnimStateWaitingForPlayers();
			break;
		case EMAIN_SOURCE_ANIM_STATE::SHOWING_PLAYERS:
			updateAnimStateShowingPlayers();
			break;
		case EMAIN_SOURCE_ANIM_STATE::WAITING_FOR_FADE_OUT:
			updateAnimStateWaitingForFadeOut();
			break;
		case EMAIN_SOURCE_ANIM_STATE::FADING_OUT:
			updateAnimStateFadingOut();
			break;
		case EMAIN_SOURCE_ANIM_STATE::WAITING_FOR_END:
			updateAnimStateWaitingForEnd();
			break;
		case EMAIN_SOURCE_ANIM_STATE::ENDING:
			updateAnimStateEnding();
			break;
	}
}

void CMainSource::start(void) {
	m_colorFilter = 0;

	m_pAudioEngine = new CAudioEngine(m_pSource);

	char path[OS_MAX_PATH];
	snprintf(path, OS_MAX_PATH, "%s/main-animation.webm", obs_get_module_data_path(obs_current_module()));
	setVideoSourceFileName(m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::VIDEO].m_pSource, path);

	MAIN_SOURCE_SUB_SOURCE *pSubSource;

	pSubSource = &m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER1_IMAGE];
	setImageSourceFileName(pSubSource->m_pSource, m_pPlayer1ImageFileName);
	pSubSource->m_visible = false;
	
	pSubSource = &m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER1_NAME];
	setTextSourceText(pSubSource->m_pSource, m_pPlayer1Name);
	pSubSource->m_visible = false;
	
	pSubSource = &m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER2_IMAGE];
	setImageSourceFileName(pSubSource->m_pSource, m_pPlayer2ImageFileName);
	pSubSource->m_visible = false;

	pSubSource = &m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER2_NAME];
	setTextSourceText(pSubSource->m_pSource, m_pPlayer2Name);
	pSubSource->m_visible = false;

	snprintf(path, OS_MAX_PATH, "%s/main-animation.raw", obs_get_module_data_path(obs_current_module()));
	playSound(path, false, 0);

	m_startAnimTime = os_gettime_ns();
	setAnimState(EMAIN_SOURCE_ANIM_STATE::WAITING_FOR_PLAYERS);
}

void CMainSource::stop(void) {
	obs_source_media_stop(m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::VIDEO].m_pSource);
	setVideoSourceFileName(m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::VIDEO].m_pSource, NULL);
	obs_source_release(m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::VIDEO].m_pSource);
	m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::VIDEO].m_pSource = createVideoSource();
	setImageSourceFileName(m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER1_IMAGE].m_pSource, NULL);
	m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER1_IMAGE].m_visible = false;
	setTextSourceText(m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER1_NAME].m_pSource, NULL);
	m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER1_NAME].m_visible = false;
	setImageSourceFileName(m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER2_IMAGE].m_pSource, NULL);
	m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER2_IMAGE].m_visible = false;
	setTextSourceText(m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER2_NAME].m_pSource, NULL);
	m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER2_NAME].m_visible = false;
	if(m_pAudioEngine) {
		m_pAudioEngine->stopAll();
		delete m_pAudioEngine;
		m_pAudioEngine = NULL;
	}

	setAnimState(EMAIN_SOURCE_ANIM_STATE::STOPPED);
}

void CMainSource::updateAnimStateStopped(void) {
	
}

void CMainSource::updateAnimStateWaitingForPlayers(void) {
	int64_t currentVideoTime = obs_source_media_get_time(m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::VIDEO].m_pSource);
	if(currentVideoTime >= SHOWING_PLAYERS_TIMESTAMP) {
		char path[OS_MAX_PATH];
		snprintf(path, OS_MAX_PATH, "%s/main-players-show.raw", obs_get_module_data_path(obs_current_module()));
		playSound(path, false, 0);
		setAnimState(EMAIN_SOURCE_ANIM_STATE::SHOWING_PLAYERS);
	}
}

void CMainSource::updatePlayerImageX(MAIN_SOURCE_SUB_SOURCE *pSubSource, int64_t dt) {
	if(dt >= SHOWING_PLAYERS_DURATION) {
		pSubSource->m_x = 0;
	} else {
		uint32_t subSourceWidth;
		computeImageSize(pSubSource->m_pSource, &subSourceWidth, NULL, NULL);
		pSubSource->m_x = (int)((dt * subSourceWidth / SHOWING_PLAYERS_DURATION) - subSourceWidth);
	}
}

void CMainSource::updatePlayerNameX(MAIN_SOURCE_SUB_SOURCE *pSubSource, int64_t dt) {
	uint32_t width = obs_source_get_width(m_pSource);
	uint32_t subSourceWidth = obs_source_get_width(pSubSource->m_pSource);
	uint32_t target = MAX(0, (width / 5) - (subSourceWidth / 2));
	
	if(dt >= SHOWING_PLAYERS_DURATION) {
		pSubSource->m_x = target;
	} else {
		pSubSource->m_x = (int)((dt * ((int64_t)subSourceWidth + target) / SHOWING_PLAYERS_DURATION) - subSourceWidth);
	}
}

void CMainSource::updateAnimStateShowingPlayers(void) {
	int64_t currentVideoTime = obs_source_media_get_time(m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::VIDEO].m_pSource);
	int64_t dt = currentVideoTime - SHOWING_PLAYERS_TIMESTAMP;

	m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER1_IMAGE].m_visible = true;
	m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER1_NAME].m_visible = true;
	m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER2_IMAGE].m_visible = true;
	m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER2_NAME].m_visible = true;

	updatePlayerImageX(&m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER1_IMAGE], dt);
	updatePlayerNameX(&m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER1_NAME], dt);
	updatePlayerImageX(&m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER2_IMAGE], dt);
	updatePlayerNameX(&m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::PLAYER2_NAME], dt);

	if(dt >= SHOWING_PLAYERS_DURATION) {
		setAnimState(EMAIN_SOURCE_ANIM_STATE::WAITING_FOR_FADE_OUT);
	}
}

void CMainSource::updateAnimStateWaitingForFadeOut(void) {
	int64_t currentVideoTime = obs_source_media_get_time(m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::VIDEO].m_pSource);
	if(currentVideoTime >= FADING_OUT_TIMESTAMP) {
		setAnimState(EMAIN_SOURCE_ANIM_STATE::FADING_OUT);
	}
}

void CMainSource::updateAnimStateFadingOut(void) {
	int64_t currentVideoTime = obs_source_media_get_time(m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::VIDEO].m_pSource);
	int64_t dt = currentVideoTime - FADING_OUT_TIMESTAMP;

	if(dt >= FADING_OUT_DURATION) {
		m_colorFilter = 0xff000000;
		setAnimState(EMAIN_SOURCE_ANIM_STATE::WAITING_FOR_END);
	} else {
		unsigned char alpha = (unsigned char)(dt * 0xff / FADING_OUT_DURATION);
		m_colorFilter = alpha << 24;
	}
}

void CMainSource::updateAnimStateWaitingForEnd(void) {
	obs_media_state mediaState = obs_source_media_get_state(m_pSubSources[CEMAIN_SOURCE_SUB_SOURCE_TYPES::VIDEO].m_pSource);
	if(mediaState == OBS_MEDIA_STATE_ENDED) {
		setAnimState(EMAIN_SOURCE_ANIM_STATE::ENDING);
	}
}

void CMainSource::updateAnimStateEnding(void) {
	stop();
	obs_data_t *pSettings = obs_source_get_settings(m_pSource);

	obs_data_set_bool(pSettings, "play", false);
	obs_source_update(m_pSource, pSettings);

	obs_data_release(pSettings);
}

void *CMainSource::sCreate(obs_data_t *pSettings, obs_source_t *pSource) {
	return new CMainSource(pSettings, pSource);
}

void CMainSource::sDestroy(void *pData) {
	CMainSource *pMainSource = (CMainSource *)pData;
	if(pMainSource) {
		delete pMainSource;
	}
}

void CMainSource::sUpdate(void *pData, obs_data_t *pSettings) {
	CMainSource *pMainSource = (CMainSource *)pData;
	if(pMainSource) {
		pMainSource->update(pSettings);
	}
}

const char *CMainSource::sGetName(void *pTypeData) {
	return obs_module_text("Yu-Gi-Oh Main");
}

void CMainSource::sDefaults(obs_data_t *pSettings) {
	obs_data_set_default_bool(pSettings, "mute", false);
	obs_data_set_default_string(pSettings, "player_1_image_file_name", "");
	obs_data_set_default_string(pSettings, "player_1_name", "");
	obs_data_set_default_string(pSettings, "player_2_image_file_name", "");
	obs_data_set_default_string(pSettings, "player_2_name", "");
	obs_data_set_default_bool(pSettings, "play", false);
}

uint32_t CMainSource::sGetWidth(void *pData) {
	CMainSource *pMainSource = (CMainSource *)pData;
	if(pMainSource) {
		return pMainSource->getWidth();
	}
	return 0;
}

uint32_t CMainSource::sGetHeight(void *pData) {
	CMainSource *pMainSource = (CMainSource *)pData;
	if(pMainSource) {
		return pMainSource->getHeight();
	}
	return 0;
}

void CMainSource::sVideoRender(void *pData, gs_effect_t *pEffect) {
	CMainSource *pMainSource = (CMainSource *)pData;
	if(pMainSource) {
		pMainSource->videoRender(pEffect);
	}
}

obs_properties_t *CMainSource::sGetProperties(void *pData) {
	CMainSource *pMainSource = (CMainSource *)pData;
	if(pMainSource) {
		return pMainSource->getProperties();
	}
	return NULL;
}

void CMainSource::sVideoTick(void *pData, float seconds) {
	CMainSource *pMainSource = (CMainSource *)pData;
	if(pMainSource) {
		pMainSource->videoTick(seconds);
	}
}

void CMainSource::sGetSourceInfo(obs_source_info *pSourceInfo) {
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
