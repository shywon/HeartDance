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
class ScreenShareTimeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScreenShareTimeWidget(QWidget *parent = nullptr);
    void stopTimer();
    void startTimer();
    void resetDistin();
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
    qint64 m_allTime;
    QString m_recordString;

    ScreenShareState m_shareState;
};

#endif // ZSCREENSHARETIMEWIDGET_H
