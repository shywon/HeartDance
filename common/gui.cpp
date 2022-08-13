#include "gui.h"
#include <QPushButton>
#include <QLabel>
#include <QVariant>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

gui::gui(QObject *parent)
    : QObject(parent)
{

}

void gui::scaleWidgetAndLayOut(QWidget *pWidget)
{

    if (nullptr == pWidget)
    {
        return;
    }
    scaleWidgetAndChildrenByScreen(pWidget);
    scaleAllLayoutByScreen(pWidget);
}

QSize gui::scale(int width, int height)
{
    QSize size(width, height);
    QSize screenSize = QGuiApplication::primaryScreen()->size();
    double ratio = qMin(screenSize.width() / Image_Base_Screen_Width,screenSize.height() / Image_Base_Screen_Height);
    double nDestWidth = width * ratio;
    double nDestHeight = height * ratio;
    size.scale(static_cast<int>(nDestWidth),static_cast<int>(nDestHeight),Qt::KeepAspectRatio);
    return size;
}

void gui::scaleWidgetAndChildrenByScreen(QWidget *pWidget, Qt::FindChildOptions options)
{
    if (nullptr == pWidget)
    {
        return;
    }

    QList<QWidget*> widgetList = pWidget->findChildren<QWidget*>(QString(), options);
    widgetList << pWidget;

    int nEnd = widgetList.size();

    for (int i = 0; i < nEnd; i++)
    {
        QWidget *pTmpWidget = widgetList[i];
        scaleOneWidgetByScreen(pTmpWidget);
    }
}

void gui::scaleOneWidgetByScreen(QWidget *pWidget)
{

    if (nullptr == pWidget || pWidget->property("FIX").toInt() == 1)
    {
        return;
    }
    int nMaxWidth = pWidget->maximumWidth();
    int nMinWidth = pWidget->minimumWidth();
    int nMaxHeight = pWidget->maximumHeight();
    int nMinHeight = pWidget->minimumHeight();

    if (pWidget->property("scaleWH").toInt() == 1)
    {
        nMaxWidth = pWidget->property("MAX_W").toInt();
        nMinWidth = pWidget->property("MIN_W").toInt();
        nMaxHeight = pWidget->property("MAX_H").toInt();
        nMinHeight = pWidget->property("MIN_H").toInt();
    }
    else
    {
        pWidget->setProperty("MAX_W", nMaxWidth);
        pWidget->setProperty("MAX_H", nMaxHeight);
        pWidget->setProperty("MIN_W", nMinWidth);
        pWidget->setProperty("MIN_H", nMinHeight);
    }
    double dbWidhtRatio = screenResolutionRatioWidth() / Image_Base_Screen_Width;
    double dbHeightRatio = screenResolutionRatioHeight() / Image_Base_Screen_Height;

    dbWidhtRatio = qMin(dbWidhtRatio, dbHeightRatio);
    dbHeightRatio = dbWidhtRatio;
    pWidget->setProperty("scaleWH", 1);
    if (nMaxHeight == nMinHeight)
    {
        pWidget->setFixedHeight(static_cast<int>(dbHeightRatio*nMaxHeight));
    }
    if (nMaxWidth == nMinWidth)
    {
        pWidget->setFixedWidth(static_cast<int>(dbWidhtRatio*nMaxWidth));
    }

    if (nMinWidth != 0)
    {
        double widthD = dbWidhtRatio*nMaxWidth;
        bool bLine = 0 < widthD && widthD < 1;
        int wid = bLine ? 1 : static_cast<int>(dbWidhtRatio*nMaxWidth + 1);
        pWidget->setMinimumWidth(wid);

    }
    if (nMinHeight != 0)
    {
        double widthD = dbHeightRatio*nMaxHeight;
        bool bLine = 0 < widthD && widthD < 1;
        int rheight = bLine ? 1 : static_cast<int>(dbHeightRatio*nMaxHeight + 1);
        pWidget->setMinimumHeight(rheight);
    }
    //分辨率是按照1920*1080来的，所以单屏最大不超过2000，判断设置的超过2000就不处理了，认为是扩展的
    if (nMaxWidth < 2000)
    {
        pWidget->setMaximumWidth(static_cast<int>(dbWidhtRatio*nMaxWidth));
    }
    if (nMaxHeight < 2000)
    {
        pWidget->setMaximumHeight(static_cast<int>(dbHeightRatio*nMaxHeight));
    }
}

void gui::scaleAllLayoutByScreen(QWidget *pWidget)
{
    if (pWidget == nullptr)
    {
        return;
    }

    QList<QLayout*> list = pWidget->findChildren<QLayout*>();

    int nEnd = list.size();
    for (int i = 0; i < nEnd; i++)
    {
        if (nullptr != list[i])
        {
            scaleLayoutByScreen(list[i]);
        }
    }
}

void gui::scaleChildrenLabelFont(QWidget *pWidget)
{
    if (nullptr == pWidget)
    {
        return;
    }

    QList<QLabel*> listLabels = pWidget->findChildren<QLabel*>();

    double dbFontRatio = screenResolutionRatioWidth() / Image_Base_Screen_Width;
    dbFontRatio = screenResolutionRatioHeight() / Image_Base_Screen_Height;

    int nEnd = listLabels.size();
    for (int i = 0; i < nEnd; i++)
    {
        QFont font = listLabels[i]->font();
        font.setPixelSize(static_cast<int>(font.pixelSize()*dbFontRatio));
        QString qsStyleSheet = listLabels[i]->styleSheet();

        if (!qsStyleSheet.isEmpty())
        {
            dealSingleStyleSheet(qsStyleSheet);
            listLabels[i]->setStyleSheet(qsStyleSheet);
        }
        else
        {
            listLabels[i]->setFont(font);
        }
    }
}

void gui::scaleChildrenBtnFont(QWidget *pWidget)
{
    if (nullptr == pWidget)
    {
        return;
    }

    QList<QPushButton*> listBtn = pWidget->findChildren<QPushButton*>();
    double dbFontRatio = screenResolutionRatioWidth() / Image_Base_Screen_Width;
    dbFontRatio = screenResolutionRatioHeight() / Image_Base_Screen_Height;
    int nEnd = listBtn.size();
    for (int i = 0; i < nEnd; i++)
    {
        QFont font = listBtn[i]->font();
        font.setPixelSize(static_cast<int>(font.pixelSize()*dbFontRatio));
        QString qsStyleSheet = listBtn[i]->styleSheet();

        if (!qsStyleSheet.isEmpty())
        {
            dealSingleStyleSheet(qsStyleSheet);
            listBtn[i]->setStyleSheet(qsStyleSheet);
        }
        else
        {
            listBtn[i]->setFont(font);
        }
    }
}

double gui::getScaleRatio()
{
    double dbWidhtRatio = screenResolutionRatioWidth() / Image_Base_Screen_Width;
    double dbHeightRatio = screenResolutionRatioHeight() / Image_Base_Screen_Height;
    return qMin(dbWidhtRatio, dbHeightRatio);
}

int gui::getFontRatio()
{
    double dbFontRatio = screenResolutionRatioWidth() / Image_Base_Screen_Width;
    dbFontRatio = qMin(screenResolutionRatioHeight() / Image_Base_Screen_Height,dbFontRatio);
    return static_cast<int>(dbFontRatio);
}

void gui::dealSingleStyleSheet(QString &qsStyle)
{
    if (qsStyle.isEmpty())
    {
        return;
    }
    // 找样式表中的字体部分
    QString qsRxp("font.{0,30};");
    QRegExp regExp(qsRxp);
    double dbFontRatio = screenResolutionRatioWidth() / Image_Base_Screen_Width;
    dbFontRatio = screenResolutionRatioHeight() / Image_Base_Screen_Height;
    // 找出子串
    int nIndex = regExp.indexIn(qsStyle);
    while (nIndex > 0 && nIndex < qsStyle.length())
    {
        QString qsStyleCapTured = regExp.capturedTexts().at(0);

        QString qsTemp = qsStyleCapTured;
        int nPxPos = qsStyleCapTured.indexOf("px");
        if (nPxPos > 2 && qsStyle[nIndex - 1] != QChar('<'))  // <font style = ...  会被放大2次
        {
            // 找出像素大小
            QString fontSize = qsTemp.mid(nPxPos - 2, 2);
            int nFontSize = fontSize.toInt();

            // 放大
            nFontSize = static_cast<int>(dbFontRatio*nFontSize);
            qsTemp.replace(fontSize + "px", QString::number(nFontSize) + "px");

            // 替换放大后的字符串
            qsStyle.replace(nIndex, qsStyleCapTured.length(), qsTemp);
        }

        nIndex = regExp.indexIn(qsStyle, nIndex + 1);
    }
}

double gui::screenResolutionRatioWidth()
{
    double dbRes = 0;
    QScreen *pPrimaryScreen = QApplication::primaryScreen();

    if (nullptr != pPrimaryScreen)
    {
        dbRes = pPrimaryScreen->geometry().width();
    }
    return dbRes;
}

double gui::screenResolutionRatioHeight()
{
    double dbRes = 0;
    QScreen *pPrimaryScreen = QApplication::primaryScreen();

    if (nullptr != pPrimaryScreen)
    {
        dbRes = pPrimaryScreen->geometry().height();
    }
    return dbRes;
}

void gui::scaleLayoutByScreen(QLayout *pLayout)
{
    if (nullptr == pLayout || pLayout->property("FIX").toInt() == 1)
    {
        return;
    }
    double dbWidhtRatio = screenResolutionRatioWidth() / Image_Base_Screen_Width;
    double dbHeightRatio = screenResolutionRatioHeight() / Image_Base_Screen_Height;
    dbWidhtRatio = qMin(dbWidhtRatio, dbHeightRatio);
    dbHeightRatio = dbWidhtRatio;
    QGridLayout *pGridLayout = qobject_cast<QGridLayout *>(pLayout);
    if (pGridLayout != nullptr)
    { // gridLayout单独处理

        int nHoriSpace = pGridLayout->horizontalSpacing();
        int nVerticalSpac = pGridLayout->verticalSpacing();
        auto contentMargin = pGridLayout->contentsMargins();
        int nMarginRight = contentMargin.right();
        int nMarginLeft = contentMargin.left();
        int nMarginTop = contentMargin.top();
        int nMarginBottom = contentMargin.bottom();
        if (pGridLayout->property("SCALED").toInt() == 0)
        {
            pGridLayout->setProperty("HoriSpace", nHoriSpace);
            pGridLayout->setProperty("VerticalSpac", nVerticalSpac);
            pGridLayout->setProperty("nMarginRight", nMarginRight);
            pGridLayout->setProperty("nMarginLeft", nMarginLeft);
            pGridLayout->setProperty("nMarginTop", nMarginTop);
            pGridLayout->setProperty("nMarginBottom", nMarginBottom);
        }
        else
        {
            nHoriSpace = pGridLayout->property("HoriSpace").toInt();
            nVerticalSpac = pGridLayout->property("VerticalSpac").toInt();
            nMarginRight = pGridLayout->property("nMarginRight").toInt();
            nMarginLeft = pGridLayout->property("nMarginLeft").toInt();
            nMarginTop = pGridLayout->property("nMarginTop").toInt();
            nMarginBottom = pGridLayout->property("nMarginBottom").toInt();
        }

        double dbHorizontalSpacing = dbWidhtRatio*nHoriSpace;
        if (dbHorizontalSpacing > 0 && dbHorizontalSpacing < 1)
        {
            dbHorizontalSpacing = 1;
        }
        pGridLayout->setHorizontalSpacing(static_cast<int>(dbHorizontalSpacing));

        double dbVerticalSpacing = dbHeightRatio*nVerticalSpac;
        if (dbVerticalSpacing > 0 && dbVerticalSpacing < 1)
        {
            dbVerticalSpacing = 1;
        }
        pGridLayout->setVerticalSpacing(static_cast<int>(dbVerticalSpacing));

        double dbMarginRight = dbWidhtRatio*nMarginRight;
        if (dbMarginRight > 0 && dbMarginRight < 1)
        {
            dbMarginRight = 1;
        }
        contentMargin.setRight(static_cast<int>(dbMarginRight));

        double dbMarginLeft = dbWidhtRatio*nMarginLeft;
        if (dbMarginLeft > 0 && dbMarginLeft < 1)
        {
            dbMarginLeft = 1;
        }
        contentMargin.setLeft(static_cast<int>(dbMarginLeft));

        double dbMarginTop = dbHeightRatio*nMarginTop;
        if (dbMarginTop > 0 && dbMarginTop < 1)
        {
            dbMarginTop = 1;
        }
        contentMargin.setTop(static_cast<int>(dbMarginTop));

        double dbMarginBottom = dbHeightRatio*nMarginBottom;
        if (dbMarginBottom > 0 && dbMarginBottom < 1)
        {
            dbMarginBottom = 1;
        }
        contentMargin.setBottom(static_cast<int>(dbMarginBottom));
        pGridLayout->setContentsMargins(contentMargin);

    }
    else
    {
        QHBoxLayout * pHLayOut = qobject_cast<QHBoxLayout *>(pLayout);
        auto contentMargin = pLayout->contentsMargins();
        int nMarginRight = contentMargin.right();
        int nMarginLeft = contentMargin.left();
        int nMarginTop = contentMargin.top();
        int nMarginBottom = contentMargin.bottom();
        int nSpacing = pLayout->spacing();
        if (pLayout->property("SCALED").toInt() == 0)
        {
            pLayout->setProperty("nMarginRight", nMarginRight);
            pLayout->setProperty("nMarginLeft", nMarginLeft);
            pLayout->setProperty("nMarginTop", nMarginTop);
            pLayout->setProperty("nMarginBottom", nMarginBottom);
            pLayout->setProperty("nSpacing", nSpacing);
        }
        else
        {
            nMarginRight = pLayout->property("nMarginRight").toInt();
            nMarginLeft = pLayout->property("nMarginLeft").toInt();
            nMarginTop = pLayout->property("nMarginTop").toInt();
            nMarginBottom = pLayout->property("nMarginBottom").toInt();
            nSpacing = pLayout->property("nSpacing").toInt();
        }


        double dbMarginRight = dbWidhtRatio*nMarginRight;
        if (dbMarginRight > 0 && dbMarginRight < 1)
        {
            dbMarginRight = 1;
        }
        contentMargin.setRight(static_cast<int>(dbMarginRight));

        double dbMarginLeft = dbWidhtRatio*nMarginLeft;
        if (dbMarginLeft > 0 && dbMarginLeft < 1)
        {
            dbMarginLeft = 1;
        }
        contentMargin.setLeft(static_cast<int>(dbMarginLeft));

        double dbMarginTop = dbHeightRatio*nMarginTop;
        if (dbMarginTop > 0 && dbMarginTop < 1)
        {
            dbMarginTop = 1;
        }
        contentMargin.setTop(static_cast<int>(dbMarginTop));

        double dbMarginBottom = dbHeightRatio*nMarginBottom;
        if (dbMarginBottom > 0 && dbMarginBottom < 1)
        {
            dbMarginBottom = 1;
        }
        contentMargin.setBottom(static_cast<int>(dbMarginBottom));
        pLayout->setContentsMargins(contentMargin);

        if (pHLayOut != nullptr)
        {
            double dbSpace = dbWidhtRatio*nSpacing;
            if (dbSpace > 0 && dbSpace < 1)
            {
                dbSpace = 1;
            }
            pLayout->setSpacing(static_cast<int>(dbSpace));
        }
        else
        {
            double dbSpace = dbHeightRatio*nSpacing;
            if (dbSpace > 0 && dbSpace < 1)
            {
                dbSpace = 1;
            }
            pLayout->setSpacing(static_cast<int>(dbSpace));
        }
    }
    pLayout->setProperty("SCALED", 1);
}
