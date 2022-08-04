#include "zsharescreenobj.h"
#include "zsharescreentoolwidget.h"
#define safe_delete(x) {if(x){delete x;x = nullptr;}}
ZShareScreenObj::ZShareScreenObj(QObject *parent)
	: QObject(parent)
	, m_toolwidget(nullptr)
{
	initConnect();
}

ZShareScreenObj::~ZShareScreenObj()
{
	safe_delete(m_toolwidget);
}

void ZShareScreenObj::showTool()
{
	if (m_toolwidget == nullptr) {
		m_toolwidget = new ZShareScreenToolWidget;
	}
	m_toolwidget->show();
}

void ZShareScreenObj::initConnect()
{

}
