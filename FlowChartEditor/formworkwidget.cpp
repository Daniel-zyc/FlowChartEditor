#include <QSvgRenderer>
#include <QPainter>
#include <QDebug>

#include "formworkwidget.h"
#include "saveandloadmanager.h"
#include "global.h"

FormworkWidget::FormworkWidget(QWidget *parent)
    : QWidget(parent), gridLayout(new QGridLayout(this)) {
    setLayout(gridLayout);

    // 假设 formworkDataList 是你定义的数据列表
    for(FormworkData data : FormworkDataList){
        addItem(data);
    }
}

void FormworkWidget::addItem(const FormworkData &data) {
    int row = items.size() / 2;
    int col = items.size() % 2;

    QPushButton *itemButton = new QPushButton(this);
    itemButton->setFixedSize(100, 180);
    itemButton->setStyleSheet("QPushButton { border: 1px solid lightgray; border-radius: 5px; background-color: white; }");

    QWidget *itemWidget = new QWidget(itemButton);
    QVBoxLayout *vLayout = new QVBoxLayout(itemWidget);
    vLayout->setAlignment(Qt::AlignCenter);

    QLabel *imageLabel = new QLabel(itemWidget);
    if (data.imgPath.endsWith(".svg", Qt::CaseInsensitive)) {
        QSvgRenderer *renderer = new QSvgRenderer(data.imgPath, this);
        QPixmap pixmap(100, 100);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        renderer->render(&painter);
        imageLabel->setPixmap(pixmap);
    } else {
        QPixmap pixmap(data.imgPath);
        imageLabel->setPixmap(pixmap);
    }
    imageLabel->setAlignment(Qt::AlignCenter);

    QLabel *titleLabel = new QLabel(data.Title, itemWidget);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("border: none;");

    vLayout->addWidget(imageLabel);
    vLayout->addWidget(titleLabel);
    vLayout->setStretch(0, 3);  // 图片占比 3
    vLayout->setStretch(1, 1);  // 标题占比 1

    itemWidget->setLayout(vLayout);
    itemButton->setLayout(new QVBoxLayout());
    itemButton->layout()->addWidget(itemWidget);

    gridLayout->addWidget(itemButton, row, col);
    items.append(itemButton);

    connect(itemButton, &QPushButton::clicked, this, &FormworkWidget::handleItemClick);

    itemButton->setToolTip(data.Detail);
}

void FormworkWidget::handleItemClick() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        int index = items.indexOf(button);
        if (index != -1 && index < FormworkDataListSize) {
            QString filePath = FormworkDataList[index].FilePath;
            SaveAndLoadManager::instance().loadFromFile(filePath);
        }
    }
}

void FormworkWidget::mouseMoveEvent(QMouseEvent *event) {
    QWidget::mouseMoveEvent(event);
    tooltipPos = event->pos();
}
