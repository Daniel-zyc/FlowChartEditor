#ifndef INSPECTOR_H
#define INSPECTOR_H
#include "dscene.h"
#include "dview.h"

struct errorItem{
    int error_typr;
    QGraphicsItem * item;
};

class Inspector
{
public:
    Inspector(DScene *scene = nullptr,DView *view = nullptr);

    void check() const;

private:
    DScene *scene;
    DView * view;

    QQueue<errorItem> errorMessage;
};

#endif // INSPECTOR_H
