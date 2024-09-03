#ifndef INSPECTOR_H
#define INSPECTOR_H
#include "dscene.h"
#include "dview.h"
#include "qlabel.h"
#include <QDialog>
#include <QPushButton>
#include <QGridLayout>

struct errorItem{
    QString message;
    QGraphicsItem * item;
};

class Inspector : public QDialog
{
public:
    Inspector(QWidget *parent = nullptr,DScene *scene = nullptr,DView *view = nullptr);

    void check();

private:
    DScene *scene;
    DView * view;

    QList<errorItem> errorMessage;

    QGridLayout *gridLayout = new QGridLayout();
    QPushButton *forwardBtn = new QPushButton("向前");
    QPushButton *backwardBtn = new QPushButton("向后");
    QLabel *message = new QLabel;

    QPointF originalCentrer;
    QTransform originalTransform;

    void restoreView();
protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    void forward();
    void backward();

    int index;
};

#endif // INSPECTOR_H
