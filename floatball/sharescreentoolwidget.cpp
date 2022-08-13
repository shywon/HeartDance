#include "sharescreentoolwidget.h"
#include "gui.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "screensharetimewidget.h"
#define ScreenShareTool_BaseWidget   "ScreenShareTool_BaseWidget"
#define ScreenShareTool_ExpandButton "ScreenShareTool_ExpandButton"
#define ScreenShareTool_ShrinkButton "ScreenShareTool_ShrinkButton"
#define ScreenShareTool_UpdateButton "ScreenShareTool_updateButton"

#define ScreenShareTool_SpaceLine          "ScreenShareTool_SpaceLine"
#define ScreenShareTool_ShareTimeWidget    "ScreenShareTool_ShareTimeWidget"
#define ScreenShareTool_OnlineStudentLabel "ScreenShareTool_OnlineStudentLabel"
#define ScreenShareTool_FinishShareButton  "ScreenShareTool_FinishShareButton"
#define ToolSize QSize(396,56)
#define baseRadius (gui::getScaleRatio() * 28)

ShareScreenToolWidget::ShareScreenToolWidget(QWidget *parent)
	: MoveableWidget(parent)
	, m_state(ToolState_Expading)
{
	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::ToolTip | Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setMoveWidget(this);


	initUi();
	initConnect();
}

void ShareScreenToolWidget::setOnlineStudent(int cur, int total)
{
    m_onlineStudentLbl->setText(QString("设置数量:%1/%2").arg(cur).arg(total));
}

void ShareScreenToolWidget::start()
{
	m_shareTimeWidget->startTimer();
}

void ShareScreenToolWidget::onClicked()
{
	QString objName = sender()->objectName();

	emit sigClick(objName);
}

void ShareScreenToolWidget::springback()
{
	m_state = m_state == ToolState_Expading ? ToolState_Shrink : ToolState_Expading;
	initSize();
}

void ShareScreenToolWidget::initUi()
{
    int fontSize = 14 * gui::getScaleRatio();

	this->setFixedSize(ToolSize);
	m_baseWidget = new QWidget(this);
	setMoveWidget(m_baseWidget);
	setSpringBackWidget(m_baseWidget);
	m_baseWidget->setObjectName(ScreenShareTool_BaseWidget);
	m_baseWidget->setFixedHeight(56);
	QHBoxLayout * layout = new QHBoxLayout(this);

	layout->setMargin(0);
	layout->setSpacing(0);
	layout->addWidget(m_baseWidget, 0, Qt::AlignLeft);

	//expand
	m_expandBtn = new QPushButton(m_baseWidget);
	setMoveWidget(m_expandBtn);
	m_expandBtn->setFixedSize(32, 32);
	m_expandBtn->setObjectName(ScreenShareTool_ExpandButton);
	m_expandBtn->setToolTip(QStringLiteral("展开"));
	//shrink
	m_shrinkBtn = new QPushButton(m_baseWidget);
	setMoveWidget(m_shrinkBtn);

	m_shrinkBtn->setFixedSize(32, 32);
	m_shrinkBtn->setObjectName(ScreenShareTool_ShrinkButton);
	m_shrinkBtn->setToolTip(QStringLiteral("收起"));
	//line
	m_line = new QLabel(m_baseWidget);
	m_line->setFixedSize(1, 16);
	m_line->setObjectName(ScreenShareTool_SpaceLine);

	//
    m_shareTimeWidget = new ScreenShareTimeWidget(m_baseWidget);
	m_shareTimeWidget->setObjectName(ScreenShareTool_ShareTimeWidget);

	//update
	m_updateBtn = new QPushButton(m_baseWidget);
	m_updateBtn->setFixedSize(32, 32);
	m_updateBtn->setObjectName(ScreenShareTool_UpdateButton);
	m_updateBtn->setToolTip(QStringLiteral("刷新"));
	//current online student
	m_onlineStudentLbl = new QLabel(m_baseWidget);
	//studentLabel
	m_onlineStudentLbl->setStyleSheet(QString("QLabel#%1{color:white;font-size:%2px;}").arg(ScreenShareTool_OnlineStudentLabel).arg(fontSize));
	m_onlineStudentLbl->setObjectName(ScreenShareTool_OnlineStudentLabel);
	//finish
	m_finishShareBtn = new QPushButton(m_baseWidget);
	m_finishShareBtn->setFixedSize(80, 28);
	m_finishShareBtn->setObjectName(ScreenShareTool_FinishShareButton);
	m_finishShareBtn->setText("结束共享");

	QHBoxLayout * baseLayout = new QHBoxLayout(m_baseWidget);
	baseLayout->setContentsMargins(12, 0, 12, 0);
	baseLayout->setSpacing(10);
	//收起 刷新 展开
	baseLayout->addWidget(m_expandBtn);
	baseLayout->addWidget(m_shrinkBtn);
	baseLayout->addWidget(m_updateBtn);
	baseLayout->addWidget(m_line);
	baseLayout->addWidget(m_shareTimeWidget);
	//计时器 在线学生
	//baseLayout->addWidget(m_shareTimeWidget);
	baseLayout->addWidget(m_onlineStudentLbl);
	//结束
	baseLayout->addWidget(m_finishShareBtn);

	QStringList qss;
	//basewidget
	qss.append(QString("QWidget#%1{border-radius:%2px;background-color:rgba(0,0,0,200);}").arg(ScreenShareTool_BaseWidget).arg(baseRadius - 1));
	//expand
    qss.append(QString("QPushButton#%1{border:none;border-image:url(:/img/image/expandN_2x.png);}").arg(ScreenShareTool_ExpandButton));
    qss.append(QString("QPushButton#%1:pressed{border:none;border-image:url(:/img/image/expandC_2x.png);}").arg(ScreenShareTool_ExpandButton));
	//shrink
    qss.append(QString("QPushButton#%1{border:none;border-image:url(:/img/image/shrinkN_2x.png);}").arg(ScreenShareTool_ShrinkButton));
    qss.append(QString("QPushButton#%1:pressed{border:none;border-image:url(:/img/image/shrinkC_2x.png);}").arg(ScreenShareTool_ShrinkButton));
	//update
    qss.append(QString("QPushButton#%1{border:none;border-image:url(:/img/image/updateN_2x.png);}").arg(ScreenShareTool_UpdateButton));
    qss.append(QString("QPushButton#%1:pressed{border:none;border-image:url(:/img/image/updateC_2x.png);}").arg(ScreenShareTool_UpdateButton));
	//finish
    qss.append(QString("QPushButton#%1{border:none;border-radius:%2px;font-size:%3px;color:white;background-color:#FF4444;}").arg(ScreenShareTool_FinishShareButton).arg(2 * gui::getScaleRatio()).arg(fontSize));
	qss.append(QString("QPushButton#%1:pressed{background-color:#E21717;}").arg(ScreenShareTool_FinishShareButton));
	//line
	qss.append(QString("QLabel#%1{background:rgba(255,255,255,51);}").arg("ScreenShareTool_SpaceLine"));

	setStyleSheet(qss.join(""));

	start();
	initSize();
    gui::scaleWidgetAndLayOut(this);

	//移动到左下角
	adjustSize();
}

void ShareScreenToolWidget::initConnect()
{
	//初始化学生数量
	setOnlineStudent(0, 0);

	connect(this, &ShareScreenToolWidget::sigClickLogo, this, &ShareScreenToolWidget::springback);
	connect(m_finishShareBtn, &QPushButton::clicked, this, &ShareScreenToolWidget::onClicked);

}

void ShareScreenToolWidget::initSize()
{
	switch (m_state)
	{
	case ShareScreenToolWidget::ToolState_Expading:
	{
		m_shrinkBtn->show();
		m_expandBtn->hide();

        m_updateBtn->show();
		m_line->show();
		m_shareTimeWidget->show();
		m_onlineStudentLbl->show();
		m_finishShareBtn->show();
	}
	break;
	case ShareScreenToolWidget::ToolState_Shrink:
	{
		m_shrinkBtn->hide();
		m_expandBtn->show();

		m_updateBtn->hide();
		m_line->hide();
		m_shareTimeWidget->hide();
		m_onlineStudentLbl->hide();
		m_finishShareBtn->hide();
	}
	break;
	default:
		break;
	}
	repaint();
}
