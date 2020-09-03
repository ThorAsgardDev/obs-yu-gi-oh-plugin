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

// TODO
// OK - scale des image des players
// OK - affichage LP doit etre transparent tant que pas initialisé
// OK - le son "chtong" quand le compteur de lp atteind sa destination c'est normal ou pas ?
// OK - Pourquoi Ratio Fixe taille de la source ?
// OK - ajouter un bouton clear pour les life point qui metteront a -1
// OK - Qt
// OK - ajouter un bouton pour stop anim
// OK - init valeur des spin box quand on attache une source dessus
// OK - Bug quand valeur LP trop grande !!
// OK - save des valeurs selectionnées pour les sources
// OK - tester comportement du combobox si au load de la conf on a un nom qui n'est pas dans la liste du combo
// OK - tester avec filename pas valides
// OK - tester avec filename valide a la save mais plus valide au load
// OK - regarder sources de text source pour width et height
// OK - nom des fichiers ressources en dur dans ce projet et le projet Qt
// OK - Raccourcis clavier ?
// fichier ogg
// icone des sources dans obs
// Faire un projet cmake ?
