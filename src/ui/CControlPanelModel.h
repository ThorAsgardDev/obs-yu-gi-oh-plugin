
#ifndef __CCONTROL_PANEL_MODEL_H__
#define __CCONTROL_PANEL_MODEL_H__

#include <obs.h>
#include <util/config-file.h>
#include <QStringList>


class CControlPanelModel {
	private:
		
		static bool staticEnumMainSourcesProc(void *pParam, obs_source_t *pSource);
		static bool staticEnumLpSourcesProc(void *pParam, obs_source_t *pSource);

		char *m_pConfigFileName;
		QString m_mainSource;
		QString m_player1LpSource;
		QString m_player2LpSource;
		QString m_player1ImageFileName;
		QString m_player1Name;
		QString m_player2ImageFileName;
		QString m_player2Name;

		obs_source_t *getSourceByName(const QString &sourceName);
		void setLpSourceLp(const QString &lpSource, int value);
		int getLpSourceLp(const QString &lpSource);
		void setConfigString(config_t *pConfig, const char *pName, const QString &value);

	public:
		CControlPanelModel(void);
		virtual ~CControlPanelModel(void);

		void saveConfig(void);
		void loadConfig(void);
		virtual QStringList getMainSources(void);
		virtual void setMainSource(const QString &mainSource);
		virtual QString getMainSource(void);
		virtual QStringList getLpSources(void);
		virtual void setPlayer1LpSource(const QString &lpSource);
		virtual QString getPlayer1LpSource(void);
		virtual void setPlayer2LpSource(const QString &lpSource);
		virtual QString getPlayer2LpSource(void);
		virtual void setPlayer1Lp(int value);
		virtual int getPlayer1Lp(void);
		virtual void setPlayer2Lp(int value);
		virtual int getPlayer2Lp(void);
		virtual void initializeLp(void);
		virtual void clearLp(void);
		virtual void setPlayer1ImageFileName(const QString &fileName);
		virtual QString getPlayer1ImageFileName(void);
		virtual void setPlayer2ImageFileName(const QString &fileName);
		virtual QString getPlayer2ImageFileName(void);
		virtual void setPlayer1Name(const QString &name);
		virtual QString getPlayer1Name(void);
		virtual void setPlayer2Name(const QString &name);
		virtual QString getPlayer2Name(void);
		virtual void startAnim(void);
		virtual void stopAnim(void);
};

#endif
