#pragma once

#include <QObject>

class ZShareScreenToolWidget;
class ZShareScreenObj : public QObject
{
    Q_OBJECT
public:
    explicit ZShareScreenObj(QObject *parent = nullptr);
	~ZShareScreenObj();

	void showTool();
 
private:
	void initConnect();
	ZShareScreenToolWidget * m_toolwidget;
};

