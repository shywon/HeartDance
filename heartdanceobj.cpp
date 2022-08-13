#include "heartdanceobj.h"
#include "heartdancewidget.h"
HeartDanceObj::HeartDanceObj(QObject *parent)
    : QObject(parent)
    , m_widget(nullptr)
{

}

HeartDanceObj::~HeartDanceObj()
{
    if(m_widget){
        delete m_widget;
        m_widget = nullptr;
    }
}

void HeartDanceObj::showMainWidget()
{
    if(m_widget == nullptr){
        m_widget = new HeartDanceWidget;
    }
    m_widget->show();
}
