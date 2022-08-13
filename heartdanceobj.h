#ifndef HEARTDANCEOBJ_H
#define HEARTDANCEOBJ_H

#include <QObject>
class HeartDanceWidget;
class HeartDanceObj : public QObject
{
    Q_OBJECT
public:
    explicit HeartDanceObj(QObject *parent = nullptr);
    ~HeartDanceObj();

    void showMainWidget();
private:
    HeartDanceWidget * m_widget;
};

#endif // HEARTDANCEOBJ_H
