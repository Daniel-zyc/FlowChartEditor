#include "inspector.h"
#include "ddiaitem.h"
#include "drectitem.h"

#include <QToolBar>
#include <QCloseEvent>
#include <QLineEdit>

Inspector::Inspector(QWidget *parent, DScene* scene, DView *view)
    : QWidget(parent), scene(scene), view(view) {

    errorListWidget = new QListWidget(this);

    // 创建工具栏
    QToolBar *toolBar = new QToolBar(this);
    QAction *clearAllAction = new QAction("清空所有项", this);
    QAction *showErrorAction = new QAction("仅显示错误", this);
    QAction *showGraphErrorsAction = new QAction("仅显示图形错误", this);  // 新的按钮
    QAction *refreshAction = new QAction("刷新", this);  // 刷新按钮
    QAction *closeAction = new QAction("关闭", this);    // 关闭按钮

    toolBar->addAction(clearAllAction);
    toolBar->addAction(showErrorAction);
    toolBar->addAction(showGraphErrorsAction);  // 添加图形错误按钮
    toolBar->addSeparator();  // 分隔符
    toolBar->addAction(refreshAction);  // 添加刷新按钮
    toolBar->addAction(closeAction);    // 添加关闭按钮

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(toolBar);  // 添加工具栏到布局中
    layout->addWidget(errorListWidget);

    setLayout(layout);

    // 连接信号和槽
    connect(errorListWidget, &QListWidget::itemClicked, this, &Inspector::onItemClicked);
    connect(clearAllAction, &QAction::triggered, this, &Inspector::clearAllItems);
    connect(showErrorAction, &QAction::triggered, this, &Inspector::showErrorsOnly);
    connect(showGraphErrorsAction, &QAction::triggered, this, &Inspector::showDErrorsOnly);
    connect(refreshAction, &QAction::triggered, this, &Inspector::checkAll);
    connect(closeAction, &QAction::triggered, this, &Inspector::setHidden);

    updateErrorList();
}

Inspector::~Inspector() {
}

/**
 * @brief Inspector::checkAll
 * 菱形判定至少有一个输入两个输出
 * 矩形判定至少有一个输入一个输出
 * 平行四边形判定一个输入一个输出
 */
void Inspector::checkAll(){
    qDebug() << "check";
    if(scene == nullptr || view == nullptr) return;
    originalCentrer = view->mapToScene(view->viewport()->rect().center());
    originalTransform = view->transform();
    QList<QGraphicsItem * > items = scene->items();
    checkItems(items);
    updateErrorList();
}
void Inspector::checkItems(QList<QGraphicsItem *> items){
    for(QGraphicsItem * item : items) checkItem(item);
}

void Inspector::checkItem(QGraphicsItem *item){
    DAbstractBase *abstractItem = dynamic_cast<DAbstractBase *>(item);
    if(abstractItem->type() > 200 && abstractItem->type() < 300)    checkDItem(abstractItem);
}

void Inspector::checkDItem(QGraphicsItem *item){
    DAbstractBase *abstractItem = dynamic_cast<DAbstractBase *>(item);
    auto result = abstractItem->getLinedArrowType();
    int in,out,none;
    in = std::get<0>(result);
    out = std::get<1>(result);
    none = std::get<2>(result);
    if(!in && !out && !none){
        errorMessage.append({NoLinkedItem,tr("图形无连接"),abstractItem});
        return;
    }
    switch(abstractItem->type()){
    case DDiaItem::Type:
    {
        if(in < 1) errorMessage.append({DDiaIn,tr("判定图形需要至少1个输入，但是输入为%1").arg(in),item});
        if(out < 2) errorMessage.append({DDiaOut,tr("判定图形需要至少2个输出，但是输入为%2").arg(out),item});
        if(!none) errorMessage.append({DDiaNoTypeArrow,tr("存在%1个没有指定输入输出类型的连线").arg(none),item});
        break;
    }
    case DRectItem::Type:
    {
        if(in < 1) errorMessage.append({DRectIn,tr("矩形需要至少1个输入，但是输入为%1").arg(in),item});
        if(out < 1) errorMessage.append({DRectOut,tr("矩形需要至少2个输出，但是输入为%2").arg(out),item});
        if(!none) errorMessage.append({DRectNoTypeArrow,tr("存在%1个没有指定输入输出类型的连线").arg(none),item});
        break;
    }
    }
}

void Inspector::restoreView(){
    if(view == nullptr) return;
    view->setTransform(originalTransform);
    view->centerOn(originalCentrer);
}

void Inspector::updateErrorList() {
    clearAllItems();

    for (const auto &error : errorMessage) {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(error.message );
        item->setTextAlignment(Qt::AlignCenter);  // 设置文本居中

        if (DTool::getErrorLevel(error.errorType) == ERROR) {
            // item->setBackground(QColor(Qt::red));
            item->setIcon(QIcon(":/icon/error.png"));
        } else if (DTool::getErrorLevel(error.errorType) == WARNING) {
            // item->setBackground(QColor(Qt::yellow));
            item->setIcon(QIcon(":/icon/warning.png"));
        }
        item->setData(Qt::UserRole, QVariant::fromValue(static_cast<void*>(error.item)));
        item->setData(Qt::UserRole + 1, QVariant(error.errorType));
        errorListWidget->addItem(item);

        QListWidgetItem *separator = new QListWidgetItem();
        separator->setSizeHint(QSize(0, 1));
        separator->setBackground(Qt::gray);
        errorListWidget->addItem(separator);
    }
}


void Inspector::onItemClicked(QListWidgetItem *item){
    QVariant itemData = item->data(Qt::UserRole);
    QGraphicsItem *graphicsItem = static_cast<QGraphicsItem*>(itemData.value<void*>());
    view->centerOn(graphicsItem);
}

void Inspector::showAllType(){
    for(int i = 0; i < errorListWidget->count(); ++i)
        errorListWidget->item(i)->setHidden(false);
}

void Inspector::clearAllItems() {
    errorListWidget->clear();
    errorMessage.clear();
}



void Inspector::showErrorsOnly() {
    for (int i = 0; i < errorListWidget->count(); ++i) {
        QListWidgetItem *item = errorListWidget->item(i);
        if (DTool::getErrorLevel(item->data(Qt::UserRole + 1).toInt()) != ERROR) {
            item->setHidden(true);
        } else {
            item->setHidden(false);
        }
    }
}

void Inspector::onshowErrorActionClicked(){
    if(ifShowErrorOnly) ifShowErrorOnly = false;
    else ifShowErrorOnly = true;
    if(ifShowErrorOnly) showErrorsOnly();
    else showAllType();
}

void Inspector::showDErrorsOnly(){

}
