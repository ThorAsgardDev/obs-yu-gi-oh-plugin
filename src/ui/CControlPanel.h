
#ifndef __CCONTROL_PANEL_H__
#define __CCONTROL_PANEL_H__

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include "ui/CWidgetPlayerImage.h"
#include "ui/CControlPanelModel.h"

class CControlPanel : public QMainWindow {
	private:
		QWidget *centralwidget;
		QVBoxLayout *verticalLayout;
		QGroupBox *OBSSources;
		QFormLayout *formLayout_2;
		QLabel *label_2;
		QComboBox *comboBoxMainSource;
		QLabel *label_3;
		QComboBox *comboBoxPlayer1LpSource;
		QLabel *label_4;
		QComboBox *comboBoxPlayer2LpSource;
		QPushButton *pushButtonRefresh;
		QGroupBox *Players;
		QVBoxLayout *verticalLayout_12;
		QPushButton *pushButtonInitializeLp;
		QPushButton *pushButtonClearLp;
		QHBoxLayout *horizontalLayout_2;
		QGroupBox *Player1;
		QFormLayout *formLayout_3;
		QLabel *label_6;
		QSpinBox *spinBoxPlayer1Lp;
		QHBoxLayout *horizontalLayout_7;
		QPushButton *pushButtonPlayer1Add;
		QPushButton *pushButtonPlayer1Remove;
		QPushButton *pushButtonPlayer1Set;
		QLabel *label_7;
		QLineEdit *lineEditPlayer1Name;
		QLabel *label_8;
		QPushButton *pushButtonPlayer1LoadImage;
		CWidgetPlayerImage *player1Image;
		QGroupBox *Player2;
		QFormLayout *formLayout;
		QLabel *label_5;
		QSpinBox *spinBoxPlayer2Lp;
		QLabel *label_9;
		QPushButton *pushButtonPlayer2LoadImage;
		CWidgetPlayerImage *player2Image;
		QHBoxLayout *horizontalLayout_6;
		QPushButton *pushButtonPlayer2Add;
		QPushButton *pushButtonPlayer2Remove;
		QPushButton *pushButtonPlayer2Set;
		QLabel *label_10;
		QLineEdit *lineEditPlayer2Name;
		QHBoxLayout *horizontalLayout;
		QGroupBox *Anim;
		QVBoxLayout *verticalLayout_4;
		QPushButton *pushButtonStart;
		QPushButton *pushButtonStop;

		CControlPanelModel *m_pModel;

		void refreshObsData();
		bool requestValue(const QString &title, int initValue, int *pValue);
		QString loadImage(CWidgetPlayerImage *pWidgetPlayerImage);
		void onPushButtonRefreshClicked(void);
		void onComboBoxMainSourceChanged(const QString &value);
		void onComboBoxPlayer1LpSourceChanged(const QString &value);
		void onComboBoxPlayer2LpSourceChanged(const QString &value);
		void onPushButtonInitializeLpClicked(void);
		void onPushButtonClearLpClicked(void);
		void onPushButtonPlayer1AddClicked(void);
		void onPushButtonPlayer1RemoveClicked(void);
		void onPushButtonPlayer1SetClicked(void);
		void onPushButtonPlayer2AddClicked(void);
		void onPushButtonPlayer2RemoveClicked(void);
		void onPushButtonPlayer2SetClicked(void);
		void onPushButtonStartClicked(void);
		void onPushButtonStopClicked(void);
		void onPushButtonPlayer1LoadImageClicked(void);
		void onPushButtonPlayer2LoadImageClicked(void);
		void onLineEditPlayer1NameTextChanged(const QString &text);
		void onLineEditPlayer2NameTextChanged(const QString &text);

	public:
		CControlPanel(QWidget *parent = NULL);
		virtual ~CControlPanel(void);

		void toggleShowHide();

};

#endif
