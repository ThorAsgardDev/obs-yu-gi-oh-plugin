
#include "CControlPanel.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QFileDialog>
#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>

CControlPanel::CControlPanel(QWidget *parent) : QMainWindow(parent) {
	m_pModel = new CControlPanelModel();

	if(objectName().isEmpty()) {
		setObjectName(QString::fromUtf8("ControlPanel"));
	}

	resize(688, 803);
	centralwidget = new QWidget(this);
	centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
	verticalLayout = new QVBoxLayout(centralwidget);
	verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
	OBSSources = new QGroupBox(centralwidget);
	OBSSources->setObjectName(QString::fromUtf8("OBSSources"));
	formLayout_2 = new QFormLayout(OBSSources);
	formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
	label_2 = new QLabel(OBSSources);
	label_2->setObjectName(QString::fromUtf8("label_2"));

	formLayout_2->setWidget(0, QFormLayout::LabelRole, label_2);

	comboBoxMainSource = new QComboBox(OBSSources);
	comboBoxMainSource->setObjectName(QString::fromUtf8("comboBoxMainSource"));

	formLayout_2->setWidget(0, QFormLayout::FieldRole, comboBoxMainSource);

	label_3 = new QLabel(OBSSources);
	label_3->setObjectName(QString::fromUtf8("label_3"));

	formLayout_2->setWidget(1, QFormLayout::LabelRole, label_3);

	comboBoxPlayer1LpSource = new QComboBox(OBSSources);
	comboBoxPlayer1LpSource->setObjectName(QString::fromUtf8("comboBoxPlayer1LpSource"));

	formLayout_2->setWidget(1, QFormLayout::FieldRole, comboBoxPlayer1LpSource);

	label_4 = new QLabel(OBSSources);
	label_4->setObjectName(QString::fromUtf8("label_4"));

	formLayout_2->setWidget(2, QFormLayout::LabelRole, label_4);

	comboBoxPlayer2LpSource = new QComboBox(OBSSources);
	comboBoxPlayer2LpSource->setObjectName(QString::fromUtf8("comboBoxPlayer2LpSource"));

	formLayout_2->setWidget(2, QFormLayout::FieldRole, comboBoxPlayer2LpSource);

	pushButtonRefresh = new QPushButton(OBSSources);
	pushButtonRefresh->setObjectName(QString::fromUtf8("pushButtonRefresh"));

	formLayout_2->setWidget(3, QFormLayout::SpanningRole, pushButtonRefresh);


	verticalLayout->addWidget(OBSSources);

	Players = new QGroupBox(centralwidget);
	Players->setObjectName(QString::fromUtf8("Players"));
	verticalLayout_12 = new QVBoxLayout(Players);
	verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
	pushButtonInitializeLp = new QPushButton(Players);
	pushButtonInitializeLp->setObjectName(QString::fromUtf8("pushButtonInitializeLp"));

	verticalLayout_12->addWidget(pushButtonInitializeLp);

	pushButtonClearLp = new QPushButton(Players);
	pushButtonClearLp->setObjectName(QString::fromUtf8("pushButtonClearLp"));

	verticalLayout_12->addWidget(pushButtonClearLp);

	horizontalLayout_2 = new QHBoxLayout();
	horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
	Player1 = new QGroupBox(Players);
	Player1->setObjectName(QString::fromUtf8("Player1"));
	formLayout_3 = new QFormLayout(Player1);
	formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
	label_6 = new QLabel(Player1);
	label_6->setObjectName(QString::fromUtf8("label_6"));

	formLayout_3->setWidget(0, QFormLayout::LabelRole, label_6);

	spinBoxPlayer1Lp = new QSpinBox(Player1);
	spinBoxPlayer1Lp->setObjectName(QString::fromUtf8("spinBoxPlayer1Lp"));
	spinBoxPlayer1Lp->setEnabled(false);
	QFont font;
	font.setBold(true);
	font.setWeight(75);
	spinBoxPlayer1Lp->setFont(font);
	spinBoxPlayer1Lp->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
	spinBoxPlayer1Lp->setReadOnly(true);
	spinBoxPlayer1Lp->setButtonSymbols(QAbstractSpinBox::NoButtons);
	spinBoxPlayer1Lp->setMinimum(-1);
	spinBoxPlayer1Lp->setMaximum(99999);
	spinBoxPlayer1Lp->setValue(-1);

	formLayout_3->setWidget(0, QFormLayout::FieldRole, spinBoxPlayer1Lp);

	horizontalLayout_7 = new QHBoxLayout();
	horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
	pushButtonPlayer1Add = new QPushButton(Player1);
	pushButtonPlayer1Add->setObjectName(QString::fromUtf8("pushButtonPlayer1Add"));

	horizontalLayout_7->addWidget(pushButtonPlayer1Add);

	pushButtonPlayer1Remove = new QPushButton(Player1);
	pushButtonPlayer1Remove->setObjectName(QString::fromUtf8("pushButtonPlayer1Remove"));

	horizontalLayout_7->addWidget(pushButtonPlayer1Remove);

	pushButtonPlayer1Set = new QPushButton(Player1);
	pushButtonPlayer1Set->setObjectName(QString::fromUtf8("pushButtonPlayer1Set"));

	horizontalLayout_7->addWidget(pushButtonPlayer1Set);


	formLayout_3->setLayout(1, QFormLayout::FieldRole, horizontalLayout_7);

	label_7 = new QLabel(Player1);
	label_7->setObjectName(QString::fromUtf8("label_7"));

	formLayout_3->setWidget(2, QFormLayout::LabelRole, label_7);

	lineEditPlayer1Name = new QLineEdit(Player1);
	lineEditPlayer1Name->setObjectName(QString::fromUtf8("lineEditPlayer1Name"));

	formLayout_3->setWidget(2, QFormLayout::FieldRole, lineEditPlayer1Name);

	label_8 = new QLabel(Player1);
	label_8->setObjectName(QString::fromUtf8("label_8"));

	formLayout_3->setWidget(3, QFormLayout::LabelRole, label_8);

	pushButtonPlayer1LoadImage = new QPushButton(Player1);
	pushButtonPlayer1LoadImage->setObjectName(QString::fromUtf8("pushButtonPlayer1LoadImage"));

	formLayout_3->setWidget(3, QFormLayout::FieldRole, pushButtonPlayer1LoadImage);

	player1Image = new CWidgetPlayerImage(Player1);
	player1Image->setObjectName(QString::fromUtf8("player1Image"));
	player1Image->setMinimumSize(QSize(400, 300));
	player1Image->setMaximumSize(QSize(16777215, 300));
	player1Image->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));

	formLayout_3->setWidget(4, QFormLayout::FieldRole, player1Image);


	horizontalLayout_2->addWidget(Player1);

	Player2 = new QGroupBox(Players);
	Player2->setObjectName(QString::fromUtf8("Player2"));
	Player2->setLayoutDirection(Qt::LeftToRight);
	formLayout = new QFormLayout(Player2);
	formLayout->setObjectName(QString::fromUtf8("formLayout"));
	label_5 = new QLabel(Player2);
	label_5->setObjectName(QString::fromUtf8("label_5"));

	formLayout->setWidget(0, QFormLayout::LabelRole, label_5);

	spinBoxPlayer2Lp = new QSpinBox(Player2);
	spinBoxPlayer2Lp->setObjectName(QString::fromUtf8("spinBoxPlayer2Lp"));
	spinBoxPlayer2Lp->setEnabled(false);
	spinBoxPlayer2Lp->setFont(font);
	spinBoxPlayer2Lp->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
	spinBoxPlayer2Lp->setReadOnly(true);
	spinBoxPlayer2Lp->setButtonSymbols(QAbstractSpinBox::NoButtons);
	spinBoxPlayer2Lp->setMinimum(-1);
	spinBoxPlayer2Lp->setMaximum(99999);
	spinBoxPlayer2Lp->setValue(-1);

	formLayout->setWidget(0, QFormLayout::FieldRole, spinBoxPlayer2Lp);

	label_9 = new QLabel(Player2);
	label_9->setObjectName(QString::fromUtf8("label_9"));

	formLayout->setWidget(3, QFormLayout::LabelRole, label_9);

	pushButtonPlayer2LoadImage = new QPushButton(Player2);
	pushButtonPlayer2LoadImage->setObjectName(QString::fromUtf8("pushButtonPlayer2LoadImage"));

	formLayout->setWidget(3, QFormLayout::FieldRole, pushButtonPlayer2LoadImage);

	player2Image = new CWidgetPlayerImage(Player2);
	player2Image->setObjectName(QString::fromUtf8("player2Image"));
	player2Image->setMinimumSize(QSize(400, 300));
	player2Image->setMaximumSize(QSize(16777215, 300));
	player2Image->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));

	formLayout->setWidget(4, QFormLayout::FieldRole, player2Image);

	horizontalLayout_6 = new QHBoxLayout();
	horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
	pushButtonPlayer2Add = new QPushButton(Player2);
	pushButtonPlayer2Add->setObjectName(QString::fromUtf8("pushButtonPlayer2Add"));

	horizontalLayout_6->addWidget(pushButtonPlayer2Add);

	pushButtonPlayer2Remove = new QPushButton(Player2);
	pushButtonPlayer2Remove->setObjectName(QString::fromUtf8("pushButtonPlayer2Remove"));

	horizontalLayout_6->addWidget(pushButtonPlayer2Remove);

	pushButtonPlayer2Set = new QPushButton(Player2);
	pushButtonPlayer2Set->setObjectName(QString::fromUtf8("pushButtonPlayer2Set"));

	horizontalLayout_6->addWidget(pushButtonPlayer2Set);


	formLayout->setLayout(1, QFormLayout::FieldRole, horizontalLayout_6);

	label_10 = new QLabel(Player2);
	label_10->setObjectName(QString::fromUtf8("label_10"));

	formLayout->setWidget(2, QFormLayout::LabelRole, label_10);

	lineEditPlayer2Name = new QLineEdit(Player2);
	lineEditPlayer2Name->setObjectName(QString::fromUtf8("lineEditPlayer2Name"));

	formLayout->setWidget(2, QFormLayout::FieldRole, lineEditPlayer2Name);


	horizontalLayout_2->addWidget(Player2);


	verticalLayout_12->addLayout(horizontalLayout_2);


	verticalLayout->addWidget(Players);

	horizontalLayout = new QHBoxLayout();
	horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

	verticalLayout->addLayout(horizontalLayout);

	Anim = new QGroupBox(centralwidget);
	Anim->setObjectName(QString::fromUtf8("Anim"));
	verticalLayout_4 = new QVBoxLayout(Anim);
	verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
	pushButtonStart = new QPushButton(Anim);
	pushButtonStart->setObjectName(QString::fromUtf8("pushButtonStart"));

	verticalLayout_4->addWidget(pushButtonStart);

	pushButtonStop = new QPushButton(Anim);
	pushButtonStop->setObjectName(QString::fromUtf8("pushButtonStop"));

	verticalLayout_4->addWidget(pushButtonStop);


	verticalLayout->addWidget(Anim);

	setCentralWidget(centralwidget);

	setWindowTitle("Yu-Gi-Oh Control Panel");
	OBSSources->setTitle("OBS Sources");
	label_2->setText("YuGiOhMainSource:");
	label_3->setText("Player 1 YuGiOhLpSource:");
	label_4->setText("Player 2 YuGiOhLpSource:");
	pushButtonRefresh->setText("Refresh");
	Players->setTitle("Players");
	pushButtonInitializeLp->setText("Initialize LP");
	pushButtonClearLp->setText("Clear LP");
	Player1->setTitle("Player 1");
	label_6->setText("LP:");
	pushButtonPlayer1Add->setText("Add");
	pushButtonPlayer1Remove->setText("Remove");
	pushButtonPlayer1Set->setText("Set");
	label_7->setText("Name:");
	label_8->setText("Image:");
	pushButtonPlayer1LoadImage->setText("Load...");
	player1Image->setText(QString());
	Player2->setTitle("Player 2");
	label_5->setText("LP:");
	label_9->setText("Image:");
	pushButtonPlayer2LoadImage->setText("Load...");
	player2Image->setText(QString());
	pushButtonPlayer2Add->setText("Add");
	pushButtonPlayer2Remove->setText("Remove");
	pushButtonPlayer2Set->setText("Set");
	label_10->setText("Name:");
	Anim->setTitle("Animation \"Versus\"");
	pushButtonStart->setText("Start");
	pushButtonStop->setText("Stop");

	QMetaObject::connectSlotsByName(this);



	connect(pushButtonRefresh, &QPushButton::clicked, this, &CControlPanel::onPushButtonRefreshClicked);
    connect(comboBoxMainSource, &QComboBox::currentTextChanged, this, &CControlPanel::onComboBoxMainSourceChanged);
    connect(comboBoxPlayer1LpSource, &QComboBox::currentTextChanged, this, &CControlPanel::onComboBoxPlayer1LpSourceChanged);
    connect(comboBoxPlayer2LpSource, &QComboBox::currentTextChanged, this, &CControlPanel::onComboBoxPlayer2LpSourceChanged);

    connect(pushButtonInitializeLp, &QPushButton::clicked, this, &CControlPanel::onPushButtonInitializeLpClicked);
    connect(pushButtonClearLp, &QPushButton::clicked, this, &CControlPanel::onPushButtonClearLpClicked);

    connect(pushButtonPlayer1Add, &QPushButton::clicked, this, &CControlPanel::onPushButtonPlayer1AddClicked);
    connect(pushButtonPlayer1Remove, &QPushButton::clicked, this, &CControlPanel::onPushButtonPlayer1RemoveClicked);
    connect(pushButtonPlayer1Set, &QPushButton::clicked, this, &CControlPanel::onPushButtonPlayer1SetClicked);
    connect(pushButtonPlayer2Add, &QPushButton::clicked, this, &CControlPanel::onPushButtonPlayer2AddClicked);
    connect(pushButtonPlayer2Remove, &QPushButton::clicked, this, &CControlPanel::onPushButtonPlayer2RemoveClicked);
    connect(pushButtonPlayer2Set, &QPushButton::clicked, this, &CControlPanel::onPushButtonPlayer2SetClicked);

    connect(pushButtonStart, &QPushButton::clicked, this, &CControlPanel::onPushButtonStartClicked);
    connect(pushButtonStop, &QPushButton::clicked, this, &CControlPanel::onPushButtonStopClicked);

    connect(pushButtonPlayer1LoadImage, &QPushButton::clicked, this, &CControlPanel::onPushButtonPlayer1LoadImageClicked);
    connect(pushButtonPlayer2LoadImage, &QPushButton::clicked, this, &CControlPanel::onPushButtonPlayer2LoadImageClicked);

	connect(lineEditPlayer1Name, &QLineEdit::textChanged, this, &CControlPanel::onLineEditPlayer1NameTextChanged);
    connect(lineEditPlayer2Name, &QLineEdit::textChanged, this, &CControlPanel::onLineEditPlayer2NameTextChanged);

    refreshObsData();

	m_pModel->loadConfig();

    player2Image->setMirror(true);

	comboBoxMainSource->setCurrentText(m_pModel->getMainSource());
	comboBoxPlayer1LpSource->setCurrentText(m_pModel->getPlayer1LpSource());
	comboBoxPlayer2LpSource->setCurrentText(m_pModel->getPlayer2LpSource());
	lineEditPlayer1Name->setText(m_pModel->getPlayer1Name());
	lineEditPlayer2Name->setText(m_pModel->getPlayer2Name());

	QPixmap pixmapPlayer1(m_pModel->getPlayer1ImageFileName());
	QPixmap pixmapPlayer2(m_pModel->getPlayer2ImageFileName());
	player1Image->setPixmap(pixmapPlayer1);
	player2Image->setPixmap(pixmapPlayer2);

	adjustSize();
	setGeometry(
		QStyle::alignedRect(
			Qt::LeftToRight,
			Qt::AlignCenter,
			size(),
			QGuiApplication::screens().at(0)->geometry()
		)
	);
}

CControlPanel::~CControlPanel(void) {
	m_pModel->saveConfig();

	delete player1Image;
	delete player2Image;

	delete m_pModel;
}

void CControlPanel::toggleShowHide() {
	if(!isVisible()) {
		setVisible(true);
	} else {
		setVisible(false);
	}
}

void CControlPanel::refreshObsData() {
	comboBoxMainSource->clear();
	comboBoxMainSource->addItems(m_pModel->getMainSources());
	comboBoxPlayer1LpSource->clear();
	comboBoxPlayer1LpSource->addItems(m_pModel->getLpSources());
	comboBoxPlayer2LpSource->clear();
	comboBoxPlayer2LpSource->addItems(m_pModel->getLpSources());
}

void CControlPanel::onPushButtonRefreshClicked(void) {
	refreshObsData();
}

void CControlPanel::onComboBoxMainSourceChanged(const QString &value) {
	m_pModel->setMainSource(value);
}

void CControlPanel::onComboBoxPlayer1LpSourceChanged(const QString &value) {
	m_pModel->setPlayer1LpSource(value);
	spinBoxPlayer1Lp->setValue(m_pModel->getPlayer1Lp());
}

void CControlPanel::onComboBoxPlayer2LpSourceChanged(const QString &value) {
	m_pModel->setPlayer2LpSource(value);
	spinBoxPlayer2Lp->setValue(m_pModel->getPlayer2Lp());
}

bool CControlPanel::requestValue(const QString &title, int initValue, int *pValue) {
	bool ok;

	*pValue = QInputDialog::getInt(
				this,
				title,
				"Value:",
				initValue,
				0,
				99999,
				1,
				&ok);

	return ok;
}

void CControlPanel::onPushButtonInitializeLpClicked(void) {
	m_pModel->initializeLp();
	spinBoxPlayer1Lp->setValue(m_pModel->getPlayer1Lp());
	spinBoxPlayer2Lp->setValue(m_pModel->getPlayer2Lp());
}

void CControlPanel::onPushButtonClearLpClicked(void) {
	m_pModel->clearLp();
	spinBoxPlayer1Lp->setValue(m_pModel->getPlayer1Lp());
	spinBoxPlayer2Lp->setValue(m_pModel->getPlayer2Lp());
}

void CControlPanel::onPushButtonPlayer1AddClicked(void) {
	int value;
	if(requestValue("Add LP", 0, &value)) {
		int currentValue = m_pModel->getPlayer1Lp();
		m_pModel->setPlayer1Lp(currentValue + value);
		spinBoxPlayer1Lp->setValue(m_pModel->getPlayer1Lp());
	}
}

void CControlPanel::onPushButtonPlayer1RemoveClicked(void) {
	int value;
	if(requestValue("Remove LP", 0, &value)) {
		int currentValue = m_pModel->getPlayer1Lp();
		if(currentValue - value < 0) {
			m_pModel->setPlayer1Lp(0);
		} else {
			m_pModel->setPlayer1Lp(currentValue - value);
		}
		spinBoxPlayer1Lp->setValue(m_pModel->getPlayer1Lp());
	}
}

void CControlPanel::onPushButtonPlayer1SetClicked(void) {
	int value;
	if(requestValue("Set LP", m_pModel->getPlayer1Lp(), &value)) {
		m_pModel->setPlayer1Lp(value);
		spinBoxPlayer1Lp->setValue(m_pModel->getPlayer1Lp());
	}
}

void CControlPanel::onPushButtonPlayer2AddClicked(void) {
	int value;
	if(requestValue("Add LP", 0, &value)) {
		int currentValue = m_pModel->getPlayer2Lp();
		m_pModel->setPlayer2Lp(currentValue + value);
		spinBoxPlayer2Lp->setValue(m_pModel->getPlayer2Lp());
	}
}

void CControlPanel::onPushButtonPlayer2RemoveClicked(void) {
	int value;
	if(requestValue("Remove LP", 0, &value)) {
		int currentValue = m_pModel->getPlayer2Lp();
		if(currentValue - value < 0) {
			m_pModel->setPlayer2Lp(0);
		} else {
			m_pModel->setPlayer2Lp(currentValue - value);
		}
		spinBoxPlayer2Lp->setValue(m_pModel->getPlayer2Lp());
	}
}

void CControlPanel::onPushButtonPlayer2SetClicked(void) {
	int value;
	if(requestValue("Set LP", m_pModel->getPlayer2Lp(), &value)) {
		m_pModel->setPlayer2Lp(value);
		spinBoxPlayer2Lp->setValue(m_pModel->getPlayer2Lp());
	}
}

void CControlPanel::onPushButtonStartClicked(void) {
	m_pModel->startAnim();
}

void CControlPanel::onPushButtonStopClicked(void) {
	m_pModel->stopAnim();
}

QString CControlPanel::loadImage(CWidgetPlayerImage *pWidgetPlayerImage) {
	QString fileName = QFileDialog::getOpenFileName(
				this,
				"Open Image",
				"",
				"Image Files (*.png *.jpg *.bmp)");

	if (!fileName.isNull()) {
		pWidgetPlayerImage->setPixmap(QPixmap(fileName));
	}

return fileName;
}

void CControlPanel::onPushButtonPlayer1LoadImageClicked(void) {
	QString fileName = loadImage(player1Image);
	m_pModel->setPlayer1ImageFileName(fileName);
}

void CControlPanel::onPushButtonPlayer2LoadImageClicked(void) {
	QString fileName = loadImage(player2Image);
	m_pModel->setPlayer2ImageFileName(fileName);
}

void CControlPanel::onLineEditPlayer1NameTextChanged(const QString &text) {
	m_pModel->setPlayer1Name(text);
}

void CControlPanel::onLineEditPlayer2NameTextChanged(const QString &text) {
	m_pModel->setPlayer2Name(text);
}
