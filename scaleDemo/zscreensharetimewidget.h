#ifndef ZSCREENSHARETIMEWIDGET_H
#define ZSCREENSHARETIMEWIDGET_H

#include <QWidget>
#include <QTime>
#include <QDateTime>

class QLabel;
class QTimer;
enum ScreenShareState
{
	ScreenShareState_ing = 0,
	ScreenShareState_stop,
	ScreenShareState_finish,
};
class ZScreenShareTimeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZScreenShareTimeWidget(QWidget *parent = nullptr);
	void stopTimer();	 //��ֹ��ʱ��
	void startTimer();   //������ʱ��
	void resetDistin();  //�����ʱ
	unsigned int  getTime();

signals:
	void sigUpdateTime(const QString&);

public slots:
	void onUpdateTimer();

private:
	void initUi();
	void initConnect();
	void updateTime(const QString&);
	QLabel * m_label;

	QTimer * m_recordtimer;
	QTime    m_timeRecoder;
	QDateTime  m_startTime;
	qint64 m_allTime;//��ͣʱҲ�ᱣ��
	QString m_recordString;

	ScreenShareState m_shareState;
};

#endif // ZSCREENSHARETIMEWIDGET_H
