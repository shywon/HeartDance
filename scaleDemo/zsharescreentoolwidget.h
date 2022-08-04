#pragma once
#include "ZMoveableWidget.h"
class QPushButton;
class QLabel;
class ZScreenShareTimeWidget;
class ZShareScreenToolWidget : public ZMoveableWidget
{
	Q_OBJECT
public:
	explicit ZShareScreenToolWidget(QWidget *parent = nullptr);

	enum ToolState
	{
		ToolState_Expading = 0,
		ToolState_Shrink,
	};

	void setOnlineStudent(int cur, int total);
	void start();

signals:
	void sigClick(const QString&objName);

public slots:
	void onClicked();
	void springback();

private:
	void initUi();
	void initConnect();
	void initSize();
	QWidget * m_baseWidget;   //main widget
	QPushButton * m_expandBtn; //伸展
	QPushButton * m_shrinkBtn; //收缩
	QPushButton * m_updateBtn;//刷新
	QLabel * m_line;//间隔线
	ZScreenShareTimeWidget*m_shareTimeWidget;//共享时间
	QLabel * m_onlineStudentLbl;//在线学生
	QPushButton * m_finishShareBtn;//结束共享

	ToolState m_state;


};

