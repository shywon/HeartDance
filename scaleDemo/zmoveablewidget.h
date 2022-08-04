#pragma once
#include "qwidget.h"
#include <QDateTime>
class ZMoveableWidget :	public QWidget
{
	Q_OBJECT
public:
	explicit ZMoveableWidget(QWidget * parent = nullptr);
	~ZMoveableWidget();

	void setMoveWidget(QWidget*);
	void setSpringBackWidget(QWidget*);

protected:
	bool eventFilter(QObject *, QEvent *)override;

signals:
	void sigClickLogo();

private:	
	void springBack(QObject *obj, QEvent *evt);//»Øµ¯
	void horizonSpringBack(QObject *obj, QEvent *evt);
	void log();
	QPoint m_mousePos;
	QPoint m_rectTopRight;
	bool m_bPressed{false};

	QDateTime m_pressedTime;
	QWidget * m_springbackWidget;
	QList<QWidget*> m_canMoveWidList;
};

