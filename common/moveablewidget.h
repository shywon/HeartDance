#pragma once
#include "qwidget.h"
#include <QDateTime>
class MoveableWidget :	public QWidget
{
	Q_OBJECT
public:
    explicit MoveableWidget(QWidget * parent = nullptr);
    ~MoveableWidget();

	void setMoveWidget(QWidget*);
	void setSpringBackWidget(QWidget*);

protected:
	bool eventFilter(QObject *, QEvent *)override;

signals:
	void sigClickLogo();

private:	
    void springBack(QObject *obj, QEvent *evt);
	void horizonSpringBack(QObject *obj, QEvent *evt);
	QPoint m_mousePos;
	QPoint m_rectTopRight;
    bool m_bPressed;

	QDateTime m_pressedTime;
	QWidget * m_springbackWidget;
	QList<QWidget*> m_canMoveWidList;
};

