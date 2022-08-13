#pragma once
#include <QObject>

class ShareScreenToolWidget;
class ShareScreenObj : public QObject
{
    Q_OBJECT
public:
    explicit ShareScreenObj(QObject *parent = nullptr);
    ~ShareScreenObj();

	void showTool();
 
private:
	void initConnect();
    ShareScreenToolWidget * m_toolwidget;
};

