#ifndef GUI_H
#define GUI_H

#include <QObject>
#include <QWidget>
#include <QSize>
#define Image_Base_Screen_Width         1920
#define Image_Base_Screen_Height        1080

class gui : public QObject
{
    Q_OBJECT
public:
    explicit gui(QObject *parent = nullptr);

    static void scaleWidgetAndLayOut(QWidget *pWidget);
    static QSize scale(int width,int height);

    static double getScaleRatio();
    static int getFontRatio();

    //
    static void scaleWidgetAndChildrenByScreen(QWidget *pWidget, Qt::FindChildOptions options = Qt::FindChildrenRecursively);
    static void scaleOneWidgetByScreen(QWidget *pWidget);
    static void scaleAllLayoutByScreen(QWidget *pWidget);
    static void scaleChildrenLabelFont(QWidget *pWidget);
    static void scaleChildrenBtnFont(QWidget *pWidget);

    // 查找label 批量设置字体大小
    static void dealSingleStyleSheet(QString& qsStyle);

    //分辨率宽
    static double screenResolutionRatioWidth(void);
    static double screenResolutionRatioHeight(void);
    static void scaleLayoutByScreen(QLayout *pLayout);
};

#endif // GUI_H
