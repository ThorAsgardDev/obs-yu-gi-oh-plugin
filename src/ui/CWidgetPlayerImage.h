#ifndef __CWIDGET_PLAYER_IMAGE_H__
#define __CWIDGET_PLAYER_IMAGE_H__

#include <QLabel>
#include <QResizeEvent>

class CWidgetPlayerImage : public QLabel {

private:
	QPixmap m_pixmap;
	bool m_mirror;

	QPixmap resizedPixmap(const QPixmap &pixmap, const QSize &size);

public:
	CWidgetPlayerImage(QWidget *parent = NULL);
	virtual ~CWidgetPlayerImage(void);

	void setPixmap(const QPixmap &pixmap);
	void resizeEvent(QResizeEvent *event);
	void setMirror(bool mirror);
};


#endif
