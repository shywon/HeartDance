#pragma once
#include <QObject>
#include <QListView>
class NavigateListView : public QListView
{
	Q_OBJECT

public:
    explicit NavigateListView(QWidget * parent = nullptr);
    virtual ~NavigateListView();
};

