#include "CWidgetPlayerImage.h"

CWidgetPlayerImage::CWidgetPlayerImage(QWidget *parent) : QLabel(parent) {
	m_mirror = false;
	setScaledContents(false);
	setAlignment(Qt::AlignCenter);
}

CWidgetPlayerImage::~CWidgetPlayerImage(void) {

}

void CWidgetPlayerImage::setMirror(bool mirror) {
	m_mirror = mirror;
}

QPixmap CWidgetPlayerImage::resizedPixmap(const QPixmap &pixmap, const QSize &size) {
	return pixmap.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void CWidgetPlayerImage::setPixmap(const QPixmap &pixmap) {
	if (m_mirror) {
		m_pixmap = pixmap.transformed(QTransform().scale(-1, 1));
	} else {
		m_pixmap = pixmap;
	}

	QLabel::setPixmap(resizedPixmap(m_pixmap, size()));
}

void CWidgetPlayerImage::resizeEvent(QResizeEvent *event) {
	if (!m_pixmap.isNull()) {
		QLabel::setPixmap(resizedPixmap(m_pixmap, event->size()));
	}
}
