#ifndef INSPECTOR_H
#define INSPECTOR_H
#include "dscene.h"
#include "dview.h"
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QListWidget>

struct errorMessage{
    int errorType;
    QString message;
    DAbstractBase * item;
};

class Inspector : public QWidget
{
    Q_OBJECT
public:
    explicit Inspector(QWidget *parent = nullptr, DScene *scene = nullptr, DView *view = nullptr);
    ~Inspector();

    void checkAll();
    void checkItem(QGraphicsItem * item);
    void checkItems(QList<QGraphicsItem*> item);

    void checkChartFlowItem(QGraphicsItem * item);
    void checkLineItem(QGraphicsItem * item);
    void checkOtherItem(QGraphicsItem * item);

    void restoreView();
    void updateErrorList();

private:
    DScene *scene;
    DView *view;

    QList<errorMessage> errorMessage;

    QPointF originalCentrer;
    QTransform originalTransform;

    QListWidget *errorListWidget;

    void showAllType();

    void showErrorsOnly();

    void showFlowChartErrorsOnly();

    bool ifShowErrorOnly = false;

    bool ifShowFlowChartErrorsOnly = false;

private slots:
    void onItemClicked(QListWidgetItem *item);

    // 清空所有错误
    void clearAllErrors();

    // 仅显示错误不显示警告
    void onShowErrorActionClicked();

    // 仅显示流程图错误
    void onShowFlowChartErrorsClicked();

    void onCloseActionClicked();
};

#endif // INSPECTOR_H
