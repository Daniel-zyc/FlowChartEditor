#ifndef FORMWORKWIDGET_H
#define FORMWORKWIDGET_H

#include "global.h"
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>
#include <QToolTip>
#include <QMouseEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QList>
#include <QStringList>

class FormworkWidget : public QWidget {
    Q_OBJECT

public:
    FormworkWidget(QWidget *parent = nullptr);
    void addItem(const FormworkData &data);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void handleItemClick();

private:
    QGridLayout *gridLayout;
    QList<QPushButton*> items;
    QList<FormworkData> itemsData;  // 保存每个 item 对应的文件路径
    QPoint tooltipPos;
};

#endif // FORMWORKWIDGET_H
