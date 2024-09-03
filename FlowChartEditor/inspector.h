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
    QGraphicsItem * item;
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

    void checkDItem(QGraphicsItem * item);
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

    bool ifShowErrorOnly = false;

private slots:
    void onItemClicked(QListWidgetItem *item);

    void clearAllItems();

    void onshowErrorActionClicked();

    void showDErrorsOnly();
};


#endif // INSPECTOR_H
