/*
Plugin Name
Copyright (C) <Year> <Developer> <Email Address>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include <obs-module.h>

#include "plugin-macros.generated.h"

#include <obs-frontend-api.h>
#include <QtWidgets/QAction>
#include "ui/CControlPanel.h"
#include "CMainSource.h"
#include "CLpSource.h"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

CControlPanel *pControlPanel = NULL;

static void onMenu() {
	if(!pControlPanel) {
		pControlPanel = new CControlPanel();
	}
	if(pControlPanel) {
		pControlPanel->toggleShowHide();
	}
}

bool obs_module_load(void) {
	blog(LOG_INFO, "plugin loaded successfully (version %s)", PLUGIN_VERSION);
	
	struct obs_source_info sourceInfo;

	CMainSource::sGetSourceInfo(&sourceInfo);
	obs_register_source(&sourceInfo);

	CLpSource::sGetSourceInfo(&sourceInfo);
	obs_register_source(&sourceInfo);
	
	const char* menuActionText = obs_module_text("Yu-Gi-Oh Control Panel");
	QAction* menuAction = (QAction*)obs_frontend_add_tools_menu_qaction(menuActionText);
	QObject::connect(menuAction, &QAction::triggered, onMenu);
	
	return true;
}

void obs_module_unload() {
	if(pControlPanel) {
		delete pControlPanel;
		pControlPanel = NULL;
	}
	
	blog(LOG_INFO, "plugin unloaded");
}
