#include "heartdancewidget.h"
#include "NavigateListModel.h"
#include "NavigateListDelegate.h"
#include "ui_heartdance.h"
#include <QDebug>
HeartDanceWidget::HeartDanceWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HeartDance)
{
    ui->setupUi(this);

    NavigateListModel * navigateModel = new NavigateListModel(this);
    NavigateListDelegate * pDelegate = new NavigateListDelegate(this);
    navigateModel->readConfig(":/image/navigatebar/navigation.xml");
    ui->listView->setModel(navigateModel);
    ui->listView->setItemDelegate(pDelegate);

    connect(ui->listView, &QListView::clicked, this, [=](const QModelIndex& index)
    {
        NavigateListModel::TreeNode* pTreeNode
                = static_cast<NavigateListModel::TreeNode*>(index.data(Qt::UserRole).value<void*>());
        //处理展开关闭
        navigateModel->Collapse(index);

        qDebug() <<pTreeNode->labelName;

    });

    QModelIndex indexFromList = navigateModel->index(1, 0);
    ui->listView->clicked(indexFromList);
    ui->listView->setCurrentIndex(indexFromList);


}
