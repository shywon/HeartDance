#include "sharescreenobj.h"
#include "sharescreentoolwidget.h"
#define safe_delete(x) {if(x){delete x;x = nullptr;}}
ShareScreenObj::ShareScreenObj(QObject *parent)
	: QObject(parent)
	, m_toolwidget(nullptr)
{
	initConnect();
}

ShareScreenObj::~ShareScreenObj()
{
	safe_delete(m_toolwidget);
}

void ShareScreenObj::showTool()
{
	if (m_toolwidget == nullptr) {
        m_toolwidget = new ShareScreenToolWidget;
	}
	m_toolwidget->show();
}

void ShareScreenObj::initConnect()
{

}
