#include "ZMoveableWidget.h"
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>

ZMoveableWidget::ZMoveableWidget(QWidget * parent)
	: QWidget(parent)
	, m_pressedTime(QDateTime::currentDateTime())
	, m_springbackWidget(this)
{
	setMoveWidget(this);
	setMouseTracking(true);
}

ZMoveableWidget::~ZMoveableWidget()
{
}

void ZMoveableWidget::setMoveWidget(QWidget* wid)
{
	if (!m_canMoveWidList.contains(wid))
	{
		m_canMoveWidList.append(wid);
		wid->installEventFilter(this);
	}
}

void ZMoveableWidget::setSpringBackWidget(QWidget*widget)
{
	m_springbackWidget = widget;
}

bool ZMoveableWidget::eventFilter(QObject *obj, QEvent *evt)
{
	if (m_canMoveWidList.contains((QWidget*)obj))
	{
		switch (evt->type())
		{
		case QEvent::MouseButtonDblClick:
		case QEvent::MouseButtonPress:
		{
			QMouseEvent *event = static_cast<QMouseEvent *>(evt);

			if (event->button() == Qt::LeftButton)
			{
				m_bPressed = true;
				m_pressedTime = QDateTime::currentDateTime();
				m_mousePos = event->globalPos() - pos();
				m_rectTopRight = QPoint(pos().x() + rect().width(), pos().y());
				return true;
			}
		}break;
		case QEvent::MouseButtonRelease:
		{
			if (m_bPressed)
			{
				m_bPressed = false;
				QPoint curTopRight = QPoint(pos().x() + rect().width(), pos().y());
				int deltaTime = m_pressedTime.msecsTo(QDateTime::currentDateTime());
				//右上角坐标没变且点击小于600毫秒
				if (curTopRight == m_rectTopRight && deltaTime < 600
					&& (obj->objectName() == "ScreenShareTool_ExpandButton"
						|| obj->objectName() == "ScreenShareTool_ShrinkButton"))
				{
					emit sigClickLogo();
				}
				else {
					springBack(obj, evt);//回弹
				}//拖拽
				return true;
			}//pressed

		}break;
		case QEvent::MouseMove:
		{
			QMouseEvent *event = static_cast<QMouseEvent *>(evt);
			if (m_bPressed && (event->buttons() & Qt::LeftButton))
			{
				move(event->globalPos() - m_mousePos);
				return true;
			}
		}break;
		case QEvent::Resize:
		{
			if (obj->objectName() == "ScreenShareTool_BaseWidget")
			{
				horizonSpringBack(obj, evt);
			}
		}
		break;
		default:
			break;
		}
	}
	return QWidget::eventFilter(obj, evt);
}


void ZMoveableWidget::springBack(QObject *, QEvent *)
{
	QRect  curScreenRect = QApplication::desktop()->screenGeometry(QCursor::pos());
	//左右回弹 鼠标松开时
	bool isAlignLeft = true;
	if (isAlignLeft) {
		QPoint baseWidgetTopLeft = pos() + QPoint(1, 1);
		QPoint baseWidgetTopRight = this == m_springbackWidget ? QPoint(pos().x() + rect().width() - 1, pos().y() + 1)
			: QPoint(pos().x() + m_springbackWidget->rect().width(), pos().y());
		QPoint baseWidgetBottomLeft(baseWidgetTopLeft.x() + 1, baseWidgetTopLeft.y() + rect().height() - 1);
		QPoint baseWidgetBottomRight(baseWidgetTopRight.x() - 1, baseWidgetTopRight.y() + rect().height() - 1);

		if (curScreenRect.contains(QRect(baseWidgetTopLeft, QSize(m_springbackWidget->rect().width(), rect().height()))))
			return;
		if (!curScreenRect.contains(baseWidgetTopRight) && !curScreenRect.contains(baseWidgetBottomRight))
		{
			QPoint poi(curScreenRect.topLeft().x() + curScreenRect.width() - m_springbackWidget->rect().width(), pos().y());
			move(poi);
		}
		if ((!curScreenRect.contains(baseWidgetTopLeft) && !curScreenRect.contains(baseWidgetBottomLeft)))
		{
			QPoint poi(curScreenRect.topLeft().x(), pos().y());
			move(poi);
		}
		if ((!curScreenRect.contains(baseWidgetTopRight) && !curScreenRect.contains(baseWidgetTopLeft)))
		{
			QPoint poi(pos().x(), curScreenRect.topLeft().y());
			move(poi);
		}
		if (!curScreenRect.contains(baseWidgetBottomRight) && !curScreenRect.contains(baseWidgetBottomLeft))
		{
			QPoint poi(pos().x(), curScreenRect.topLeft().y() + curScreenRect.height() - rect().height());
			move(poi);
		}
	}
	else {

		QPoint baseWidgetTopLeft = this == m_springbackWidget ? pos() + QPoint(1, 1) : mapToGlobal(m_springbackWidget->pos()) + QPoint(1, 1);
		QPoint baseWidgetTopRight(pos().x() + rect().width() - 1, pos().y() + 1);
		QPoint baseWidgetBottomLeft(baseWidgetTopLeft.x() + 1, baseWidgetTopLeft.y() + rect().height() - 1);
		QPoint baseWidgetBottomRight(baseWidgetTopRight.x() - 1, baseWidgetTopRight.y() + rect().height() - 1);

		if (curScreenRect.contains(QRect(baseWidgetTopLeft, QSize(m_springbackWidget->rect().width(), rect().height()))))
			return;

		if (!curScreenRect.contains(baseWidgetTopLeft) && !curScreenRect.contains(baseWidgetBottomLeft))
		{
			QPoint poi(curScreenRect.topLeft().x() - (rect().width() - m_springbackWidget->rect().width()), pos().y());
			move(poi);
		}
		if ((!curScreenRect.contains(baseWidgetTopRight) && !curScreenRect.contains(baseWidgetBottomRight)))
		{
			QPoint poi(curScreenRect.topLeft().x() + curScreenRect.width() - rect().width(), pos().y());
			move(poi);
		}
		if ((!curScreenRect.contains(baseWidgetTopRight) && !curScreenRect.contains(baseWidgetTopLeft)))
		{
			QPoint poi(pos().x(), curScreenRect.topLeft().y());
			move(poi);
		}
		if (!curScreenRect.contains(baseWidgetBottomRight) && !curScreenRect.contains(baseWidgetBottomLeft))
		{
			QPoint poi(pos().x(), curScreenRect.topLeft().y() + curScreenRect.height() - rect().height());
			move(poi);
		}
	}
}

void ZMoveableWidget::horizonSpringBack(QObject *, QEvent *)
{
	QRect  curScreenRect = QApplication::desktop()->screenGeometry(QCursor::pos());

	bool isAlignLeft = true;
	if (isAlignLeft) {
		QPoint widTopRight(pos().x() + m_springbackWidget->rect().width() - 1, pos().y() + 1);
		QPoint widBottomRight(pos().x() + m_springbackWidget->rect().width() - 1, pos().y() + this->rect().height() - 1);
		QPoint widTopLeft(pos().x() + 1, pos().y() + 1);
		QPoint widBottomLeft(pos().x() +  1, pos().y() + rect().height() - 1);

		bool containLeft = curScreenRect.contains(widTopLeft) || curScreenRect.contains(widBottomLeft);
		bool containRight = curScreenRect.contains(widTopRight) || curScreenRect.contains(widBottomRight);
		if (containLeft && !containRight)
		{
			QPoint poi(curScreenRect.topLeft().x() + curScreenRect.width() - m_springbackWidget->rect().width(), pos().y());
			move(poi);
		}
		if (containRight && !containLeft)
		{
			QPoint poi(curScreenRect.topLeft().x(), pos().y());
			move(poi);
		}
	}
	else{
		QPoint widTopRight(pos().x() + this->rect().width() - 1, pos().y() + 1);
		QPoint widBottomRight(pos().x() + this->rect().width() - 1, pos().y() + this->rect().height() - 1);
		QPoint widTopLeft(pos().x() + rect().width() - m_springbackWidget->rect().width() + 1, pos().y() + 1);
		QPoint widBottomLeft(pos().x() + rect().width() - m_springbackWidget->rect().width() + 1, pos().y() + rect().height() - 1);
		bool containLeft = curScreenRect.contains(widTopLeft) || curScreenRect.contains(widBottomLeft);
		bool containRight = curScreenRect.contains(widTopRight) || curScreenRect.contains(widBottomRight);
		if (containLeft && !containRight)
		{
			QPoint poi(curScreenRect.topLeft().x() + curScreenRect.width() - rect().width(), pos().y());
			move(poi);
		}
		if (containRight && !containLeft)
		{
			QPoint poi(curScreenRect.topLeft().x(), pos().y());
			move(poi);
		}
	}
}

void ZMoveableWidget::log()
{
	QPoint widTopRight(this->rect().width(), pos().y());
	QDesktopWidget * deskTop = QApplication::desktop();
	int nScreenCount = deskTop->screenCount(), curScreenIndex = -1;
	for (int i = 0; i < nScreenCount; i++) {
		QRect rect = deskTop->screenGeometry(i);
		if (rect.contains(widTopRight))
		{
			curScreenIndex = i;
			break;
		}
	}
}

