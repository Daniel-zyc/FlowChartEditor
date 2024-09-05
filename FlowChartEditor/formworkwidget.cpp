#include <QSvgRenderer>
#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QToolBar>
#include <QSvgGenerator>

#include "formworkwidget.h"
#include "saveandloadmanager.h"
#include "global.h"

FormworkWidget::FormworkWidget(QWidget *parent,DScene *scene,DView *view)
    : QWidget(parent), scene(scene),view(view),gridLayout(new QGridLayout(this)) {

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QToolBar *toolBar = new QToolBar(this);
    QAction *newFormwork = new QAction("保存当前为模板", this);
    QAction *deleteFormwork = new QAction("删除模板",this);

    toolBar->addAction(newFormwork);
    toolBar->addAction(deleteFormwork);
    mainLayout->addWidget(toolBar, 0);
    mainLayout->addLayout(gridLayout, 1);

    connect(newFormwork,&QAction::triggered,this,&FormworkWidget::handleNewFormwork);
    connect(deleteFormwork,&QAction::triggered,this,&FormworkWidget::handleDeleteFormwork);

    setLayout(gridLayout);
    loadFormwork();
    updateForm();
}

void FormworkWidget::addItem(const FormworkData &data) {
    int row = items.size() / 2;
    int col = items.size() % 2;

    QPushButton *itemButton = new QPushButton(this);
    itemButton->setFixedSize(100, 180);
    itemButton->setStyleSheet("QPushButton { "
                              "border: 1px solid lightgray; "
                              "border-radius: 5px; "
                              "background-color: white; }");
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
    vLayout->setStretch(0, 3);
    vLayout->setStretch(1, 1);

    itemWidget->setLayout(vLayout);
    itemButton->setLayout(new QVBoxLayout());
    itemButton->layout()->addWidget(itemWidget);

    gridLayout->addWidget(itemButton, row, col);
    items.append(itemButton);

    connect(itemButton, &QPushButton::clicked, this, &FormworkWidget::handleItemClick);

    itemButton->setToolTip(data.Detail);
}

void FormworkWidget::handleNewFormwork(){
    NewFormworkDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        QString title = dialog.getTitle();
        QString detail = dialog.getDetail();

        if (title.isEmpty()) {
            QMessageBox::warning(this, "oi!", "标题不能为空");
            return;
        }

        QDir rootDir("../../formwork");
        if (!rootDir.exists()) {
            rootDir.mkpath("..");
            rootDir.mkpath("formwork");
        }

        QDir newDir(rootDir.filePath(title));
        if (newDir.exists()) {
            QMessageBox::warning(this, "oi!", "该名称的模板已存在");
            return;
        }
        QString bitFilePath = newDir.filePath(title + ".bit");
        QString svgFilePath = newDir.filePath(title+".svg");
        QString detailFilePath = newDir.filePath(title+".txt");

        if (!newDir.mkpath(".")) {
            QMessageBox::critical(this, "oi!", "无法创建文件夹");
            return;
        }

        // 保存bit文件
        SaveAndLoadManager::instance().saveToFile(newDir.filePath(title + ".bit"));

        // 保存封面
        if(view == nullptr || scene == nullptr) return;
        QSvgGenerator generator;
        generator.setFileName(svgFilePath);
        generator.setSize(QSize(this->width(), this->height()));
        generator.setViewBox(QRect(0, 0, this->width(), this->height()));

        QList<QGraphicsItem *> items = scene->selectedItems();
        for(QGraphicsItem *item : items) {
            item->setSelected(false);
        }

        QPainter painter;
        painter.begin(&generator);
        view->render(&painter);
        painter.end();

        // 保存介绍
        QFile txtFile(detailFilePath);
        if (!txtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "错误", "无法创建 .txt 文件！");
            return;
        }
        QTextStream out(&txtFile);
        out << detail;
        txtFile.close();

        // 添加模板
        FormworkData newData;
        newData.FilePath = bitFilePath;
        newData.Title = title;
        newData.imgPath = svgFilePath;
        newData.Detail = detail;

        fromworkData.append(newData);
        addItem(newData);
    }
}

void FormworkWidget::handleDeleteFormwork(){
    deleting = !deleting;
}

void FormworkWidget::updateForm(){
    for(FormworkData &data : fromworkData) addItem(data);
}

void FormworkWidget::handleItemClick() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if(button == nullptr) return;
    int index = items.indexOf(button);
    if (index == -1 || index >= fromworkData.size()) return;
    if(!deleting){
        QString filePath = fromworkData[index].FilePath;
        if(FILE_PATH == ""){
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this,tr("保存当前文件"),tr("当前文件未保存，是否保存"),
                                          QMessageBox::Yes | QMessageBox::No);
            if(reply == QMessageBox::Yes){
                if(FILE_PATH == nullptr || FILE_PATH == "")
                    FILE_PATH = QFileDialog::getSaveFileName(this, tr("保存.bit文件"),"./",tr("(*.bit)"));
                if(FILE_PATH == "") return;
                    SaveAndLoadManager::instance().saveToFile(FILE_PATH);
            }
        }else{
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this,tr("保存当前文件"),tr("当前文件未保存，是否保存到%1").arg(FILE_PATH),
                                          QMessageBox::Yes | QMessageBox::No);
            if(reply == QMessageBox::Yes){
                   SaveAndLoadManager::instance().saveToFile(FILE_PATH);
            }
        }
        SaveAndLoadManager::instance().loadFromFile(filePath);
        FILE_PATH = "";
    }else{
        QString title = fromworkData[index].Title;
        QString folderPath = QString("../../formwork/%1").arg(title);
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "确认删除", QString("确认删除模板 \"%1\" 吗？").arg(title),
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QDir dir(folderPath);
            if (dir.exists()) {
                if (!dir.removeRecursively()) {
                    QMessageBox::critical(this, "错误", "无法删除文件夹，请检查文件夹权限！");
                } else {
                    fromworkData.removeAt(index);
                    delete button;
                    items.removeAt(index);
                    qDebug() << "文件夹已删除:" << folderPath;
                }
            } else {
                QMessageBox::warning(this, "警告", "指定的文件夹不存在！");
            }
        }
        deleting = false;
    }
}

void FormworkWidget::mouseMoveEvent(QMouseEvent *event) {
    QWidget::mouseMoveEvent(event);
    tooltipPos = event->pos();
}

void FormworkWidget::loadFormwork() {
    fromworkData.clear();
    QDir rootDir("../../formwork");
    QStringList subDirs = rootDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QString &subDirName : subDirs) {
        FormworkData data;
        QDir subDir(rootDir.filePath(subDirName));
        data.Title = subDirName;
        QStringList bitFiles = subDir.entryList(QStringList() << "*.bit", QDir::Files);
        if (!bitFiles.isEmpty()) {
            QString bitFileName = bitFiles.first();
            data.FilePath = subDir.filePath(bitFileName);
        } else {
            continue;
        }
        QStringList svgFiles = subDir.entryList(QStringList() << "*.svg", QDir::Files);
        if (!svgFiles.isEmpty()) {
            data.imgPath = subDir.filePath(svgFiles.first());
        }
        QStringList txtFiles = subDir.entryList(QStringList() << "*.txt", QDir::Files);
        if (!txtFiles.isEmpty()) {
            QString txtFilePath = subDir.filePath(txtFiles.first());
            QFile textFile(txtFilePath);
            if (textFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&textFile);
                data.Detail = in.readAll();
                textFile.close();
            } else {
                continue;
            }
        } else {
            continue;
        }
        fromworkData.append(data);
    }
}

//============================================


NewFormworkDialog::NewFormworkDialog(QWidget *parent)
    : QDialog(parent), titleEdit(new QLineEdit(this)), detailEdit(new QTextEdit(this)) {

    titleEdit->setPlaceholderText("请输入标题");
    detailEdit->setPlaceholderText("添加标注");

    okButton = new QPushButton("确定", this);
    cancelButton = new QPushButton("取消", this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(titleEdit);
    mainLayout->addWidget(detailEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
    setWindowTitle("新建模板");

    connect(okButton, &QPushButton::clicked, this, &NewFormworkDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &NewFormworkDialog::reject);
}

QString NewFormworkDialog::getTitle() const {
    return title;
}

QString NewFormworkDialog::getDetail() const {
    return detail;
}

void NewFormworkDialog::accept() {
    title = titleEdit->text();
    detail = detailEdit->toPlainText();
    QDialog::accept();
}

void NewFormworkDialog::reject() {
    QDialog::reject();
}
