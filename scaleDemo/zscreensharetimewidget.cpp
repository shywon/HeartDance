#include "zscreensharetimewidget.h"
#include <QTimer>
#include <QLabel>
#include <QHBoxLayout>
#include "gui.h"
ZScreenShareTimeWidget::ZScreenShareTimeWidget(QWidget *parent)
    : QWidget(parent)
	, m_recordtimer(new QTimer(this))
	, m_allTime(0)
    , m_recordString("00:00:00")
    , m_shareState(ScreenShareState_finish)
{
	m_recordtimer->setInterval(500);
	m_timeRecoder.setHMS(0, 0, 0, 0);

	initUi();
	initConnect();
}

void ZScreenShareTimeWidget::stopTimer()
{
	m_recordtimer->stop();

	m_shareState = ScreenShareState_stop;
	m_allTime += m_startTime.msecsTo(QDateTime::currentDateTime());
}

void ZScreenShareTimeWidget::startTimer()
{
	m_recordtimer->start();

	switch (m_shareState)
	{
	case ScreenShareState_ing:
		break;
	case ScreenShareState_stop:
	case ScreenShareState_finish:
	{
		m_startTime = QDateTime::currentDateTime();
		m_shareState = ScreenShareState_ing;
	}
		break;
	default:
		break;
	}
}

void ZScreenShareTimeWidget::resetDistin()
{
	m_shareState = ScreenShareState_finish;
	m_allTime = 0;
	m_timeRecoder.setHMS(0, 0, 0, 0);
	m_recordString = "00:00:00";
	updateTime(m_recordString);
}

unsigned int ZScreenShareTimeWidget::getTime()
{
	return m_timeRecoder.hour() * 3600 + m_timeRecoder.minute() * 60 + m_timeRecoder.second();
}

void ZScreenShareTimeWidget::onUpdateTimer()
{
	qint64 offset = m_startTime.msecsTo(QDateTime::currentDateTime());
	offset += m_allTime;
	m_timeRecoder.setHMS(0, 0, 0, 0);
	m_timeRecoder = m_timeRecoder.addMSecs(offset);
	m_recordString = m_timeRecoder.toString("hh:mm:ss");

	updateTime(m_recordString);

}

void ZScreenShareTimeWidget::initUi()
{
    int fontSize = 14 * gui::getScaleRatio();
	m_label = new QLabel(this);
	m_label->setText(m_recordString);
	QHBoxLayout * layout = new QHBoxLayout;
	layout->setMargin(0);
	layout->setSpacing(0);
	layout->addWidget(m_label);

	m_label->setStyleSheet(QString("color:white;font-size:%1px;").arg(fontSize));
	setLayout(layout);
}

void ZScreenShareTimeWidget::initConnect()
{
	connect(m_recordtimer, &QTimer::timeout, this, &ZScreenShareTimeWidget::onUpdateTimer);
}

void ZScreenShareTimeWidget::updateTime(const QString&timeString)
{
	m_label->setText(timeString);
}
