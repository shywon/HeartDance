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

    if (role == Qt::DisplayRole)      //显示文字
        return m_allNode[index.row()].labelName;
    else if (role == Qt::UserRole)    //用户定义起始位置
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

    QDomElement xmlRoot = docXml.documentElement(); //返回根节点
    QDomNode domNode = xmlRoot.firstChild(); //获取子节点，一级节点
    while (!domNode.isNull())
    {
        if (domNode.isElement())
        {
            QDomElement domElement = domNode.toElement();   //一级节点
            TreeNode* pTreeNode = new TreeNode;

            pTreeNode->labelName = domElement.attribute("label");//获取一级节点的lable
            pTreeNode->nLevel = 1;  //标志一级节点
            pTreeNode->collapse = static_cast<bool>(domElement.attribute("collapse").toInt());
            //pTreeNode->collapse = domElement.attribute("collapse").toInt(); //标志是否展开
            pTreeNode->nIndex = domElement.attribute("index").toInt();  //获取标志
            QDomNodeList list = domElement.childNodes();    //获取二级节点
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
        domNode = domNode.nextSibling();    //下一一级节点
    }

    xmlFile.close();
    refresh();  //刷新界面标题栏展示数据
    beginInsertRows(QModelIndex(), 0, m_allNode.size());    //插入所有节点
    endInsertRows(); //结束插入
}

void NavigateListModel::refresh()
{
    refreshList();
    beginResetModel();
    endResetModel();
}

void NavigateListModel::Collapse(const QModelIndex &index) // 节点展开，收缩曹函数
{
    TreeNode* pTreeNode = m_allNode[index.row()].pTreeNode; //获取当前点击节点
    if (pTreeNode->listChildren.size() == 0) //如果该节点没有子节点 则返回
        return;
    pTreeNode->collapse = !pTreeNode->collapse; //刷新是否展开标志
    if (!pTreeNode->collapse)    //如果是不展开，即为展开变成合并，移除合并的
    {
        qDebug() << "close current row :" << index.row() <<"children size:" << pTreeNode->listChildren.size();
        beginRemoveRows(QModelIndex(), index.row() + 1, index.row() + pTreeNode->listChildren.size()); //默认起始节点为最初节点
        endRemoveRows();
    }
    else {
        qDebug() << "open current row :" << index.row() <<"children size:" << pTreeNode->listChildren.size();
        beginInsertRows(QModelIndex(), index.row() + 1, index.row() + pTreeNode->listChildren.size());
        endInsertRows();
    }
    refreshList(); //更新界面显示节点数据
}

void NavigateListModel::refreshList()//刷新当前界面显示的节点
{
    m_allNode.clear();
    for (auto it = m_vecTreeNode.begin(); it != m_vecTreeNode.end(); it++)
    {
        //一级节点
        ListNode node;
        node.labelName = (*it)->labelName;
        node.pTreeNode = *it;
        m_allNode.append(node);

        if (!(*it)->collapse) //如果一级节点未展开，则插入下一一级节点
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
