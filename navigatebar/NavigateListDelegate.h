#pragma once
#include <qstyleditemdelegate.h>

class NavigateListDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
    explicit NavigateListDelegate(QObject* parent);
    virtual ~NavigateListDelegate();
public:
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void  paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void  SetPending(bool pending) { m_pending = pending; }

private:
	bool m_pending;
};

