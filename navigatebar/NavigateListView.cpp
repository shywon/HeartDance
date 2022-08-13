#include "NavigateListView.h"

NavigateListView::NavigateListView(QWidget * parent)
	: QListView(parent)
{
	setStyleSheet("QListView{background-color:#FFFFFF;border:none;border-right:1px solid #F3F4F5;}");
}

NavigateListView::~NavigateListView()
{

}