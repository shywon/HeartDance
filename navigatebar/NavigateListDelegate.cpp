#include "NavigateListDelegate.h"
#include "NavigateListModel.h"
#include <QPainter>
#include <QColor>


NavigateListDelegate::NavigateListDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
    , m_pending(false)
{

}

NavigateListDelegate::~NavigateListDelegate()
{

}


QSize NavigateListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index)const
{
    Q_UNUSED(option)
    NavigateListModel::TreeNode * pNode = static_cast<NavigateListModel::TreeNode *>(index.data(Qt::UserRole).value<void*>());
    if (pNode->nLevel == 1) {
        return QSize(50, 45); //主要设置高度
    }
    else {
        return QSize(50, 28);
    }
}

void NavigateListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index)const
{
    NavigateListModel::TreeNode * pNode = static_cast<NavigateListModel::TreeNode *>(index.data(Qt::UserRole).value<void*>());

    painter->setRenderHint(QPainter::Antialiasing);
    QColor normalBgColor("#FFFFFF");
    QColor selectBgColor("#fdf4f4");
    QColor hoverBgColor("#F3F3F3");
    QColor textNormalColor("#999999");
    QColor textHoverColor("#333333");
    QColor textSelectColor("#FF0033");

    QColor underlineColor(QColor("#F3F4F5"));

    const QColor selectFlagColor("#FF0033");
    if (pNode->nLevel == 1) {
        normalBgColor =   QColor("#ffffff");
        selectBgColor = normalBgColor;
        hoverBgColor = normalBgColor;
        textNormalColor = QColor("#000000");
        textHoverColor = textNormalColor;
        textSelectColor = textHoverColor;
    }

    if (option.state & QStyle::State_Selected)  {
        painter->fillRect(option.rect, selectBgColor);
        if (pNode->nLevel == 2) {
            painter->save();
            QRect rect = option.rect;
            rect.setWidth(2);
            painter->fillRect(rect,selectFlagColor);
            painter->restore();
        }
    }
    else if (option.state & QStyle::State_MouseOver) {
        painter->fillRect(option.rect, hoverBgColor);
    }
    else  {
        painter->fillRect(option.rect, normalBgColor);
    }


    if (pNode->listChildren.size() != 0) {
        QString qsImagePath;
        if (!pNode->collapse) {
            qsImagePath = ":/image/navigatebar/unexpand_normal.png";
        }
        else {
            qsImagePath = ":/image/navigatebar/expand_normal.png";
        }

        QPixmap img(qsImagePath);
        QRect targetRect = option.rect;
        targetRect.setWidth(16);
        targetRect.setHeight(16);

        QPoint c = option.rect.center();
        c.setX(8);
        targetRect.moveCenter(c);

        painter->drawPixmap(targetRect, qsImagePath, img.rect());
    }

    QPen textPen(option.state & QStyle::State_Selected ? textSelectColor :textNormalColor);
    painter->setPen(textPen);

    int margin = 25;

    if (pNode->nLevel == 2)
        margin = 45;

    QRect rect = option.rect;
    rect.setWidth(rect.width() - margin);
    rect.setX(rect.x() + margin);

    QFont normalFont("Microsoft Yahei", 9);
    painter->setFont(normalFont);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, index.data(Qt::DisplayRole).toString());

    QPen linePen(underlineColor);
    linePen.setWidth(1);
    painter->setPen(linePen);

    if (pNode->nLevel == 1)	{
        painter->drawLine(
                    QPointF(option.rect.x(), option.rect.y() + option.rect.height() - 1),
                    QPointF(option.rect.x() + option.rect.width(), option.rect.y() + option.rect.height() - 1));
        if (pNode->nIndex == 0) {
            painter->drawLine(
                        QPointF(option.rect.x(), option.rect.y() + 1),
                        QPointF(option.rect.x() + option.rect.width(), option.rect.y() +1));
        }
    }
}
