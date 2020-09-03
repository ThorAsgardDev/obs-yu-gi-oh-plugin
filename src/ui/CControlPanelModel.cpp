
#include "CControlPanelModel.h"
#include "CMainSource.h"
#include "CLpSource.h"

#define CONFIG_SECTION_NAME "YU_GI_OH_MODULE_CONFIG"


CControlPanelModel::CControlPanelModel(void) {
	char *pConfigDir = obs_module_config_path("");
	os_mkdirs(pConfigDir);
	bfree(pConfigDir);

	m_pConfigFileName = obs_module_config_path("config.ini");
	
	m_mainSource.clear();
	m_player1LpSource.clear();
	m_player2LpSource.clear();
	m_player1ImageFileName.clear();
	m_player2ImageFileName.clear();
	m_player1Name.clear();
	m_player2Name.clear();
}

CControlPanelModel::~CControlPanelModel(void) {
	bfree(m_pConfigFileName);
}

void CControlPanelModel::setConfigString(config_t *pConfig, const char *pName, const QString &value) {
	if(value.isEmpty()) {
		config_set_string(pConfig, CONFIG_SECTION_NAME, pName, "");
	} else {
		config_set_string(pConfig, CONFIG_SECTION_NAME, pName, value.toUtf8().constData());
	}
}

void CControlPanelModel::saveConfig(void) {
	config_t *pConfig = NULL;
	int ret = config_open(&pConfig, m_pConfigFileName, CONFIG_OPEN_ALWAYS);
	if(ret == CONFIG_SUCCESS) {
		setConfigString(pConfig, "main_source", m_mainSource);
		setConfigString(pConfig, "player_1_source", m_player1LpSource);
		setConfigString(pConfig, "player_2_source", m_player2LpSource);
		setConfigString(pConfig, "player_1_image", m_player1ImageFileName);
		setConfigString(pConfig, "player_2_image", m_player2ImageFileName);
		setConfigString(pConfig, "player_1_name", m_player1Name);
		setConfigString(pConfig, "player_2_name", m_player2Name);

		config_save(pConfig);
		config_close(pConfig);
	}
}

void CControlPanelModel::loadConfig(void) {
	config_t *pConfig = NULL;
	if(config_open(&pConfig, m_pConfigFileName, CONFIG_OPEN_ALWAYS) == CONFIG_SUCCESS) {
		config_set_default_string(pConfig, CONFIG_SECTION_NAME, "main_source", "");
		config_set_default_string(pConfig, CONFIG_SECTION_NAME, "player_1_source", "");
		config_set_default_string(pConfig, CONFIG_SECTION_NAME, "player_2_source", "");
		config_set_default_string(pConfig, CONFIG_SECTION_NAME, "player_1_image", "");
		config_set_default_string(pConfig, CONFIG_SECTION_NAME, "player_2_image", "");
		config_set_default_string(pConfig, CONFIG_SECTION_NAME, "player_1_name", "");
		config_set_default_string(pConfig, CONFIG_SECTION_NAME, "player_2_name", "");

		if(config_has_user_value(pConfig, CONFIG_SECTION_NAME, "main_source")) {
			m_mainSource = config_get_string(pConfig, CONFIG_SECTION_NAME, "main_source");
		}
		if(config_has_user_value(pConfig, CONFIG_SECTION_NAME, "player_1_source")) {
			m_player1LpSource = config_get_string(pConfig, CONFIG_SECTION_NAME, "player_1_source");
		}
		if(config_has_user_value(pConfig, CONFIG_SECTION_NAME, "player_2_source")) {
			m_player2LpSource = config_get_string(pConfig, CONFIG_SECTION_NAME, "player_2_source");
		}
		if(config_has_user_value(pConfig, CONFIG_SECTION_NAME, "player_1_image")) {
			m_player1ImageFileName = config_get_string(pConfig, CONFIG_SECTION_NAME, "player_1_image");
		}
		if(config_has_user_value(pConfig, CONFIG_SECTION_NAME, "player_2_image")) {
			m_player2ImageFileName = config_get_string(pConfig, CONFIG_SECTION_NAME, "player_2_image");
		}
		if(config_has_user_value(pConfig, CONFIG_SECTION_NAME, "player_1_name")) {
			m_player1Name = config_get_string(pConfig, CONFIG_SECTION_NAME, "player_1_name");
		}
		if(config_has_user_value(pConfig, CONFIG_SECTION_NAME, "player_2_name")) {
			m_player2Name = config_get_string(pConfig, CONFIG_SECTION_NAME, "player_2_name");
		}

		config_close(pConfig);
	}
}

bool CControlPanelModel::staticEnumMainSourcesProc(void *pParam, obs_source_t *pSource) {
	QStringList *pStringList = (QStringList *)pParam;

	if(strcmp(obs_source_get_id(pSource), CMainSource::m_pSourceIdName) == 0) {
		pStringList->push_back(obs_source_get_name(pSource));
	}

	return true;
}

bool CControlPanelModel::staticEnumLpSourcesProc(void *pParam, obs_source_t *pSource) {
	QStringList *pStringList = (QStringList *)pParam;

	if(strcmp(obs_source_get_id(pSource), CLpSource::m_pSourceIdName) == 0) {
		pStringList->push_back(obs_source_get_name(pSource));
	}

	return true;
}

obs_source_t *CControlPanelModel::getSourceByName(const QString &sourceName) {
	if(sourceName.isEmpty()) {
		return NULL;
	}

	return obs_get_source_by_name(sourceName.toUtf8().constData());
}

QStringList CControlPanelModel::getMainSources(void) {
	QStringList stringList;

	obs_enum_sources(staticEnumMainSourcesProc, &stringList);

	return stringList;
}

void CControlPanelModel::setMainSource(const QString &mainSource) {
	m_mainSource = mainSource;
}

QString CControlPanelModel::getMainSource(void) {
	return m_mainSource;
}

QStringList CControlPanelModel::getLpSources(void) {
	QStringList stringList;

	obs_enum_sources(staticEnumLpSourcesProc, &stringList);

	return stringList;
}

void CControlPanelModel::setPlayer1LpSource(const QString &lpSource) {
	m_player1LpSource = lpSource;
}

QString CControlPanelModel::getPlayer1LpSource(void) {
	return m_player1LpSource;
}

void CControlPanelModel::setPlayer2LpSource(const QString &lpSource) {
	m_player2LpSource = lpSource;
}

QString CControlPanelModel::getPlayer2LpSource(void) {
	return m_player2LpSource;
}

void CControlPanelModel::setLpSourceLp(const QString &lpSource, int value) {
	obs_source_t *pSource = getSourceByName(lpSource);

	if(pSource) {
		obs_data_t *pSettings = obs_source_get_settings(pSource);

		obs_data_set_bool(pSettings, "init_mode", false);
		obs_data_set_int(pSettings, "life_points", value);
		obs_source_update(pSource, pSettings);

		obs_data_release(pSettings);
		obs_source_release(pSource);
	}
}

int CControlPanelModel::getLpSourceLp(const QString &lpSource) {
	int lp = -1;

	obs_source_t *pSource = getSourceByName(lpSource);

	if(pSource) {
		obs_data_t *pSettings = obs_source_get_settings(pSource);

		lp = (int)obs_data_get_int(pSettings, "life_points");
		
		obs_data_release(pSettings);
		obs_source_release(pSource);
	}
	return lp;
}

void CControlPanelModel::setPlayer1Lp(int value) {
	setLpSourceLp(m_player1LpSource, value);
}

int CControlPanelModel::getPlayer1Lp(void) {
	return getLpSourceLp(m_player1LpSource);
}

void CControlPanelModel::setPlayer2Lp(int value) {
	setLpSourceLp(m_player2LpSource, value);
}

int CControlPanelModel::getPlayer2Lp(void) {
	return getLpSourceLp(m_player2LpSource);
}

void CControlPanelModel::initializeLp(void) {
	obs_source_t *pPlayer1Source = getSourceByName(m_player1LpSource);
	obs_source_t *pPlayer2Source = getSourceByName(m_player2LpSource);

	if(pPlayer2Source) {
		obs_data_t *pSettings = obs_source_get_settings(pPlayer2Source);

		if(pPlayer1Source) {
			// If pPlayer1Source is not NULL then the init sound will be played by pPlayer1Source
			obs_data_set_bool(pSettings, "init_mute", true);
		} else {
			obs_data_set_bool(pSettings, "init_mute", false);
		}
		obs_data_set_bool(pSettings, "init_mode", true);
		obs_data_set_int(pSettings, "life_points", 8000);

		obs_source_update(pPlayer2Source, pSettings);
		
		obs_data_release(pSettings);
		obs_source_release(pPlayer2Source);
	}

	if(pPlayer1Source) {
		obs_data_t *pSettings = obs_source_get_settings(pPlayer1Source);

		obs_data_set_bool(pSettings, "init_mute", false);
		obs_data_set_bool(pSettings, "init_mode", true);
		obs_data_set_int(pSettings, "life_points", 8000);
		obs_source_update(pPlayer1Source, pSettings);

		obs_data_release(pSettings);
		obs_source_release(pPlayer1Source);
	}
}

void CControlPanelModel::clearLp(void) {
	obs_source_t *pPlayer1Source = getSourceByName(m_player1LpSource);
	obs_source_t *pPlayer2Source = getSourceByName(m_player2LpSource);

	if(pPlayer2Source) {
		obs_data_t *pSettings = obs_source_get_settings(pPlayer2Source);

		obs_data_set_int(pSettings, "life_points", -1);
		obs_source_update(pPlayer2Source, pSettings);
		
		obs_data_release(pSettings);
		obs_source_release(pPlayer2Source);
	}

	if(pPlayer1Source) {
		obs_data_t *pSettings = obs_source_get_settings(pPlayer1Source);

		obs_data_set_int(pSettings, "life_points", -1);
		obs_source_update(pPlayer1Source, pSettings);

		obs_data_release(pSettings);
		obs_source_release(pPlayer1Source);
	}
}

void CControlPanelModel::setPlayer1ImageFileName(const QString &fileName) {
	m_player1ImageFileName = fileName;
}

QString CControlPanelModel::getPlayer1ImageFileName(void) {
	return m_player1ImageFileName;
}

void CControlPanelModel::setPlayer2ImageFileName(const QString &fileName) {
	m_player2ImageFileName = fileName;
}

QString CControlPanelModel::getPlayer2ImageFileName(void) {
	return m_player2ImageFileName;
}

void CControlPanelModel::setPlayer1Name(const QString &name) {
	m_player1Name = name;
}

QString CControlPanelModel::getPlayer1Name(void) {
	return m_player1Name;
}

void CControlPanelModel::setPlayer2Name(const QString &name) {
	m_player2Name = name;
}

QString CControlPanelModel::getPlayer2Name(void) {
	return m_player2Name;
}

void CControlPanelModel::startAnim(void) {
	if(m_player1ImageFileName.isEmpty()
	|| m_player2ImageFileName.isEmpty()
	|| m_player1Name.isEmpty()
	|| m_player2Name.isEmpty()) {
		return;
	}

	obs_source_t *pSource = getSourceByName(m_mainSource);

	if(pSource) {
		obs_data_t *pSettings = obs_source_get_settings(pSource);

		obs_data_set_string(pSettings, "player_1_image_file_name", m_player1ImageFileName.toUtf8().constData());
		obs_data_set_string(pSettings, "player_2_image_file_name", m_player2ImageFileName.toUtf8().constData());
		obs_data_set_string(pSettings, "player_1_name", m_player1Name.toUtf8().constData());
		obs_data_set_string(pSettings, "player_2_name", m_player2Name.toUtf8().constData());
		obs_data_set_bool(pSettings, "play", true);
		obs_source_update(pSource, pSettings);

		obs_data_release(pSettings);
		obs_source_release(pSource);
	}
}

void CControlPanelModel::stopAnim(void) {
	obs_source_t *pSource = getSourceByName(m_mainSource);

	if(pSource) {
		obs_data_t *pSettings = obs_source_get_settings(pSource);

		obs_data_set_bool(pSettings, "play", false);
		obs_source_update(pSource, pSettings);

		obs_data_release(pSettings);
		obs_source_release(pSource);
	}
}
