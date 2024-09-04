#include "inspector.h"
#include <QToolBar>
#include <QCloseEvent>
#include <QLineEdit>

Inspector::Inspector(QWidget *parent, DScene* scene, DView *view)
    : QWidget(parent), scene(scene), view(view) {

    errorListWidget = new QListWidget(this);

    // 创建工具栏
    QToolBar *toolBar = new QToolBar(this);
    QAction *clearAllAction = new QAction("清空所有项", this);
    QAction *showErrorAction = new QAction("屏蔽警告", this);
    QAction *showFlowChartErrorsAction = new QAction("仅显示流程图图形错误", this);  // 新的按钮
    QAction *refreshAction = new QAction("刷新", this);
    QAction *closeAction = new QAction("关闭", this);

    toolBar->addAction(clearAllAction);
    toolBar->addAction(showErrorAction);
    toolBar->addAction(showFlowChartErrorsAction);
    toolBar->addSeparator();
    toolBar->addAction(refreshAction);
    toolBar->addAction(closeAction);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(toolBar);
    layout->addWidget(errorListWidget);

    setLayout(layout);

    connect(errorListWidget, &QListWidget::itemClicked, this, &Inspector::onItemClicked);
    connect(clearAllAction, &QAction::triggered, this, &Inspector::clearAllErrors);
    connect(showErrorAction, &QAction::triggered, this, &Inspector::showErrorsOnly);
    connect(showFlowChartErrorsAction, &QAction::triggered, this, &Inspector::showFlowChartErrorsOnly);
    connect(refreshAction, &QAction::triggered, this, &Inspector::checkAll);
    connect(closeAction, &QAction::triggered, this, &Inspector::onCloseActionClicked);

    updateErrorList();
}

Inspector::~Inspector() {
}

void Inspector::checkAll(){
    clearAllErrors();
    if(scene == nullptr || view == nullptr) return;
    originalCentrer = view->mapToScene(view->viewport()->rect().center());
    originalTransform = view->transform();
    QList<QGraphicsItem * > items = scene->items();
    checkItems(items);
    updateErrorList();
}
void Inspector::checkItems(QList<QGraphicsItem *> items){
    DTool::filterRootBases(items);      // 线+一般图形+流程图图形+文本框
    for(QGraphicsItem * item : items)
        checkItem(item);
}

void Inspector::checkItem(QGraphicsItem *item){
    DAbstractBase *abstractItem = dynamic_cast<DAbstractBase *>(item);
    if(abstractItem == nullptr) return;
    if(DTool::isLine(abstractItem->type())) checkLineItem(abstractItem);                    // 检查线
    if(DTool::isFlowChartShape(abstractItem->type()))  checkChartFlowItem(abstractItem);    // 检查流程图
    else checkOtherItem(abstractItem);                                                      // 检查一般图形
}

void Inspector::checkTextItem(QGraphicsItem *item){
    DTextItem *textItem = dynamic_cast<DTextItem * >(item);
    if(textItem == nullptr) return;
    if(textItem->isTextEmpty())
        errorMessage.append({EmptyText,tr("未填充文字的文本框"),textItem});
    if(scene->ifCollision(item))
        errorMessage.append({CollisionItem,tr("线条发生碰撞"),textItem});
}

void Inspector::checkChartFlowItem(QGraphicsItem *item){
    DShapeBase *shapeBase = dynamic_cast<DShapeBase *>(item);
    if(shapeBase == nullptr) return;
    auto result = shapeBase->getLinedArrowType();
    int in,out,none;
    in = std::get<0>(result);
    out = std::get<1>(result);
    none = std::get<2>(result);
    qDebug() << in << out << none;
    if(!in && !out && !none)
        errorMessage
            .append({ChartFlowNoLinedItem,tr("无连接的孤立流程图图形图形"),shapeBase});
    if(none)
        errorMessage
            .append({ChartFlowHasNoTypeArrow,tr("存在未指定箭头类型的流程图图形"),shapeBase});
    if(scene->ifCollision(shapeBase))
        errorMessage.append({CollisionItem,tr("流程图图形发生碰撞"),shapeBase});
    switch(shapeBase->type()){
    case DFProcessItem::Type:
        if(in < 1)
            errorMessage
                .append({DFProcessShouldHasMoreThanOneIn,tr("过程图形需要至少一个输入"),shapeBase});
        if(out < 1)
            errorMessage
                .append({DFProcessShouldHasMoreThanOneOut,tr("过程图形需要至少一个输出"),shapeBase});
        if(shapeBase->isTextEmpty())
            errorMessage
                .append({DFProcessNoName,tr("过程图形未指定文本"),shapeBase});
        break;
    case DFNodeItem::Type:
        if(shapeBase->isTextEmpty())
            errorMessage
                .append({DFNodeItemNoName,tr("连接符号未指定文本"),shapeBase});
        break;
    case DFDataItem::Type:
        if((in == 0 && out != 1)
            || (in != 1 && out == 0))
            errorMessage
                .append({DFDataItemShouldOnlyInOrOut,
                                 tr("数据图形应该只有一个输入或只有一个输出,但是当前%1个输入%2个输出").arg(in).arg(out),shapeBase});
        break;
    case DFManualOperateItem::Type:
        if(out < 1)
            errorMessage
                .append({DFManualOperateItemShouldHasOneOut,tr("手动操作符至少有一个输出,但是当前%1个").arg(out),shapeBase});
        if(shapeBase->isTextEmpty())
            errorMessage
                .append({DFManualOperateItemNoName,tr("手动操作符未指定文本"),shapeBase});
        break;
    case DFConditionItem::Type:
        if(in < 1)
            errorMessage
                .append({DFConditionItemShouldHasOneIn,tr("决策图形需要至少一个输入"),shapeBase});
        if(out < 1)
            errorMessage
                .append({DFConditionItemShouldHasMoreThanTwoOut,tr("决策图形需要至少一个输出"),shapeBase});
        if(shapeBase->isTextEmpty())
            errorMessage
                .append({DFConditionItemNoName,tr("决策图形未指定文本"),shapeBase});
        break;
    case DFDocumentItem::Type:
        if(shapeBase->isTextEmpty())
            errorMessage
                .append({DFDocumentItemNoName,tr("文档图形未指定文本"),shapeBase});
        break;
    case DFPredefineItem::Type:
        if(in < 1)
            errorMessage
                .append({DFPredefineItemShouldHasMoreThanOneIn,tr("预定义图形需要至少一个输入"),shapeBase});
        if(out < 1)
            errorMessage
                .append({DFPredefineItemShouldHasMoreThanOneOut,tr("预定义图形需要至少一个输出"),shapeBase});
        if(shapeBase->isTextEmpty())
            errorMessage
                .append({DFPredefineItemNoName,tr("预定义图形未指定文本"),shapeBase});
        break;
    case DFStartEndItem::Type:
        if((in == 0 && out != 1)
            || (in != 1 && out == 0))
            errorMessage
                .append({DFEndItemShouldOnlyInOrOut,tr("启止图形应该只有一个输入或只有一个输出,但是当前%1个输入%2个输出").arg(in).arg(out),shapeBase});
        break;
    case DFPrepareItem::Type:
        if(shapeBase->isTextEmpty())
            errorMessage
                .append({DFPrepareItemNoName,tr("准备图形未指定文本"),shapeBase});
        break;
    case DFInternalStoreItem::Type:
        if(shapeBase->isTextEmpty())
            errorMessage
                .append({DFInternalStoreItemNoName,tr("内部存储图形图形未指定文本"),shapeBase});
        break;
    }
}

void Inspector::checkLineItem(QGraphicsItem * item){
    DLineBase * lineBase = dynamic_cast<DLineBase *>(item);
    if(lineBase == nullptr) return;
    if(lineBase->ifHasRound()) errorMessage.append({RoundLine,tr("回环连线"),lineBase});
    if(scene->ifCollision(lineBase)) errorMessage.append({CollisionItem,tr("连线碰撞"),lineBase});
}

void Inspector::checkOtherItem(QGraphicsItem * item){
    DShapeBase *shapeBase = dynamic_cast<DShapeBase *>(item);
    if(shapeBase == nullptr) return;
    auto result = shapeBase->getLinedArrowType();
    int in,out,none;
    in = std::get<0>(result);
    out = std::get<1>(result);
    none = std::get<2>(result);
    if(!in && !out && !none){
        errorMessage.append({NomalItemNoLinedItem,tr("无连接的孤立一般图形"),shapeBase});
        return;
    }
}
void Inspector::restoreView(){
    if(view == nullptr) return;
    view->setTransform(originalTransform);
    view->centerOn(originalCentrer);
}

void Inspector::updateErrorList() {

    for (const auto &error : errorMessage) {
        qDebug() << "add";
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(error.message);
        item->setTextAlignment(Qt::AlignCenter);  // 设置文本居中

        if (DTool::getErrorLevel(error.errorType) == ERROR) {
            // item->setBackground(QColor(202,89,99, 200));  // 使用半透明的红色
            item->setIcon(QIcon(":/icon/error.png").pixmap(16, 16)); // 调整图标大小
        } else if (DTool::getErrorLevel(error.errorType) == WARNING) {
            // item->setBackground(QColor(199,141,99, 200));  // 使用半透明的黄色
            item->setIcon(QIcon(":/icon/warning.png").pixmap(16, 16)); // 调整图标大小
        }
        item->setData(Qt::UserRole, QVariant::fromValue(static_cast<void*>(error.item)));
        item->setData(Qt::UserRole + 1, QVariant(error.errorType));
        errorListWidget->addItem(item);

        QListWidgetItem *separator = new QListWidgetItem();
        separator->setSizeHint(QSize(0, 1));
        separator->setBackground(QColor(200, 200, 200));  // 使用较浅的灰色
        errorListWidget->addItem(separator);
    }
}


void Inspector::onItemClicked(QListWidgetItem *item){
    QVariant itemData = item->data(Qt::UserRole);
    DAbstractBase *abstractItem = static_cast<DAbstractBase*>(itemData.value<void*>());
    if(abstractItem == nullptr) return;
    view->centerOn(abstractItem);
    scene->setItemSelected(abstractItem);
}

void Inspector::showAllType(){
    for(int i = 0; i < errorListWidget->count(); ++i)
        errorListWidget->item(i)->setHidden(false);
}

void Inspector::clearAllErrors() {
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

void Inspector::onShowErrorActionClicked(){
    if(ifShowErrorOnly) ifShowErrorOnly = false;
    else ifShowErrorOnly = true;
    if(ifShowErrorOnly) showErrorsOnly();
    else showAllType();
}

void Inspector::showFlowChartErrorsOnly(){
    for(int i = 0; i < errorListWidget->count(); ++i){
        QListWidgetItem *item = errorListWidget->item(i);
        QVariant itemData = item->data(Qt::UserRole);
        DAbstractBase *abstractItem = static_cast<DAbstractBase*>(itemData.value<void*>());
        if(abstractItem == nullptr) continue;
        if(DTool::isFlowChartShape(abstractItem->type())) item->setHidden(false);
        else item->setHidden(true);
    }
}

void Inspector::onShowFlowChartErrorsClicked(){
    if(ifShowFlowChartErrorsOnly) ifShowErrorOnly = false;
    else ifShowFlowChartErrorsOnly = true;
    if(ifShowFlowChartErrorsOnly) showFlowChartErrorsOnly();
    else showAllType();
}

void Inspector::onCloseActionClicked(){
    this->hide();
    restoreView();
}
