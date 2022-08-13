#include "NavigateListModel.h"
#include <QFile>
#include <QDomDocument>
#include <QDebug>
NavigateListModel::NavigateListModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

NavigateListModel::~NavigateListModel()
{
    QVector<TreeNode*>  m_vecTreeNode;
    QVector<ListNode>   m_allNode;
    for (QVector<TreeNode*>::iterator it = m_vecTreeNode.begin(); it != m_vecTreeNode.end(); it++)
    {
        if ((*it)->listChildren.size())
        {
            for (QList<TreeNode*>::iterator itChild = (*it)->listChildren.begin();
                 itChild != (*it)->listChildren.end();
                 it++)
                delete (*itChild);
        }
        delete (*it);
    }

    m_allNode.clear();
    m_vecTreeNode.clear();
}

int NavigateListModel::rowCount(const QModelIndex & parent) const
{
    return m_allNode.size();
}

QVariant NavigateListModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_allNode.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)      //��ʾ����
        return m_allNode[index.row()].labelName;
    else if (role == Qt::UserRole)    //�û�������ʼλ��
        return QVariant::fromValue((void*)m_allNode[index.row()].pTreeNode);

    return QVariant();
}

void NavigateListModel::readConfig(const QString &path)
{
    QFile xmlFile(path);
    if (!xmlFile.open(QFile::ReadOnly | QFile::Text))
        return;

    QDomDocument docXml;
    QString error;
    if (!docXml.setContent(&xmlFile, false, &error))
    {
        qDebug() << error;
        xmlFile.close();
        return;
    }

    QDomElement xmlRoot = docXml.documentElement(); //���ظ��ڵ�
    QDomNode domNode = xmlRoot.firstChild(); //��ȡ�ӽڵ㣬һ���ڵ�
    while (!domNode.isNull())
    {
        if (domNode.isElement())
        {
            QDomElement domElement = domNode.toElement();   //һ���ڵ�
            TreeNode* pTreeNode = new TreeNode;

            pTreeNode->labelName = domElement.attribute("label");//��ȡһ���ڵ��lable
            pTreeNode->nLevel = 1;  //��־һ���ڵ�
            pTreeNode->collapse = static_cast<bool>(domElement.attribute("collapse").toInt());
            //pTreeNode->collapse = domElement.attribute("collapse").toInt(); //��־�Ƿ�չ��
            pTreeNode->nIndex = domElement.attribute("index").toInt();  //��ȡ��־
            QDomNodeList list = domElement.childNodes();    //��ȡ�����ڵ�
            for (int i = 0; i < list.count(); i++)
            {
                QDomElement secNodeInfo = list.at(i).toElement();
                TreeNode* pSecNode = new TreeNode;
                pSecNode->labelName = secNodeInfo.attribute("label");
                pSecNode->nLevel = 2;
                pSecNode->nIndex = secNodeInfo.attribute("index").toInt();
                pSecNode->collapse = false;
                pTreeNode->listChildren.push_back(pSecNode);
            }
            m_vecTreeNode.push_back(pTreeNode);
        }
        domNode = domNode.nextSibling();    //��һһ���ڵ�
    }

    xmlFile.close();
    refresh();  //ˢ�½��������չʾ����
    beginInsertRows(QModelIndex(), 0, m_allNode.size());    //�������нڵ�
    endInsertRows(); //��������
}

void NavigateListModel::refresh()
{
    refreshList();
    beginResetModel();
    endResetModel();
}

void NavigateListModel::Collapse(const QModelIndex &index) // �ڵ�չ���������ܺ���
{
    TreeNode* pTreeNode = m_allNode[index.row()].pTreeNode; //��ȡ��ǰ����ڵ�
    if (pTreeNode->listChildren.size() == 0) //����ýڵ�û���ӽڵ� �򷵻�
        return;
    pTreeNode->collapse = !pTreeNode->collapse; //ˢ���Ƿ�չ����־
    if (!pTreeNode->collapse)    //����ǲ�չ������Ϊչ����ɺϲ����Ƴ��ϲ���
    {
        qDebug() << "close current row :" << index.row() <<"children size:" << pTreeNode->listChildren.size();
        beginRemoveRows(QModelIndex(), index.row() + 1, index.row() + pTreeNode->listChildren.size()); //Ĭ����ʼ�ڵ�Ϊ����ڵ�
        endRemoveRows();
    }
    else {
        qDebug() << "open current row :" << index.row() <<"children size:" << pTreeNode->listChildren.size();
        beginInsertRows(QModelIndex(), index.row() + 1, index.row() + pTreeNode->listChildren.size());
        endInsertRows();
    }
    refreshList(); //���½�����ʾ�ڵ�����
}

void NavigateListModel::refreshList()//ˢ�µ�ǰ������ʾ�Ľڵ�
{
    m_allNode.clear();
    for (auto it = m_vecTreeNode.begin(); it != m_vecTreeNode.end(); it++)
    {
        //һ���ڵ�
        ListNode node;
        node.labelName = (*it)->labelName;
        node.pTreeNode = *it;
        m_allNode.append(node);

        if (!(*it)->collapse) //���һ���ڵ�δչ�����������һһ���ڵ�
            continue;

        for (QList<TreeNode*>::iterator child = (*it)->listChildren.begin(); child != (*it)->listChildren.end(); child++)
        {
            ListNode node;
            node.labelName = (*child)->labelName;
            node.pTreeNode = *child;
            m_allNode.push_back(node);
        }
    }
}
