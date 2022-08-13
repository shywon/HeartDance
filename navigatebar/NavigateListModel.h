#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QVector>

class NavigateListModel : public QAbstractListModel
{
	Q_OBJECT

public:
	struct TreeNode
	{
		QString labelName;//4
		QList<TreeNode*> listChildren;//4
		int nLevel;    //4
		int nIndex;    //4
		bool collapse; //1
	};

	struct ListNode
	{
		QString    labelName;
		TreeNode * pTreeNode;
	};

public:
    explicit NavigateListModel(QObject *parent = nullptr);
    virtual ~NavigateListModel();

	int rowCount(const QModelIndex &parent = QModelIndex())const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole)const override;

public:
	void readConfig(const QString &path); //  读取导航栏节点xml文件
	void refresh();//刷新模型

public slots:
	void Collapse(const QModelIndex &index); // 节点展开，收缩曹函数

private:
	void refreshList();//刷新当前界面显示的节点

private:
	QVector<TreeNode*> m_vecTreeNode; //用于存储对应关系
	QVector<ListNode> m_allNode; //存储所有的节点
};

