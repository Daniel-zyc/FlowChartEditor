#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "dscene.h"
#include "dview.h"
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QListWidget>

struct errorMessage {
    int errorType;
    QString message;
    DAbstractBase *item;
};

class Inspector : public QWidget {
    Q_OBJECT
public:
    static Inspector* instance(QWidget *parent = nullptr, DScene *scene = nullptr, DView *view = nullptr);

    ~Inspector();

    void checkAll();
    void checkItem(QGraphicsItem *item);
    void checkItems(QList<QGraphicsItem*> item);

    void checkTextItem(QGraphicsItem *item);
    void checkChartFlowItem(QGraphicsItem *item);
    void checkLineItem(QGraphicsItem *item);
    void checkOtherItem(QGraphicsItem *item);

    void restoreView();

    void setAutoCheck(bool activate);

private:
    explicit Inspector(QWidget *parent = nullptr, DScene *scene = nullptr, DView *view = nullptr);

    static Inspector* m_instance;

    void updateErrorList();

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

    QAction* showErrorAction;

    QAction* showFlowChartErrorsAction;

    bool autoCheck;

private slots:
    void onItemClicked(QListWidgetItem *item);
    void clearAllErrors();
    void onShowErrorActionClicked();
    void onShowFlowChartErrorsClicked();
};

#endif // INSPECTOR_H
