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
	void readConfig(const QString &path); //  ��ȡ�������ڵ�xml�ļ�
	void refresh();//ˢ��ģ��

public slots:
	void Collapse(const QModelIndex &index); // �ڵ�չ���������ܺ���

private:
	void refreshList();//ˢ�µ�ǰ������ʾ�Ľڵ�

private:
	QVector<TreeNode*> m_vecTreeNode; //���ڴ洢��Ӧ��ϵ
	QVector<ListNode> m_allNode; //�洢���еĽڵ�
};

