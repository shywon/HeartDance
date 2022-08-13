#ifndef HEARTDANCEWIDGET_H
#define HEARTDANCEWIDGET_H

#include <QWidget>
namespace Ui {
    class HeartDance;
}
class HeartDanceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HeartDanceWidget(QWidget *parent = nullptr);

private:
    Ui::HeartDance *ui;
};

#endif // HEARTDANCEWIDGET_H
