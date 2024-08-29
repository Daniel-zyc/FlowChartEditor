#include "dfinddialog.h"
#include "dshapebase.h"
#include "dtextitem.h"
#include <QList>
#include <QMessageBox>
#include <QTextCharFormat>
#include <QTextCursor>

DFindDialog::DFindDialog(QWidget *parent)
    :QDialog(parent)
{
    // setWindowFlags(Qt::CustomizeWindowHint);

    gridLayout->addWidget(searchLabel, 0, 0);
    gridLayout->addWidget(searchEdit, 0, 1);
    gridLayout->addWidget(findNextBtn, 0, 2);
    gridLayout->addWidget(replaceLabel, 1, 0);
    gridLayout->addWidget(replaceEdit, 1, 1);

    backwardBtn->setChecked(true);
    hLayout->addWidget(forwardBtn);
    hLayout->addWidget(backwardBtn);
    dirGroup->setTitle("方向");
    dirGroup->setLayout(hLayout);

    // QLabel *label = new QLabel("");
    gridLayout->addWidget(replaceAllBtn, 2, 0);
    gridLayout->addWidget(dirGroup, 2, 1);

    // vLayout->addWidget(replaceBtn);
    // vLayout->addWidget(replaceAllBtn);
    // rplGroup->setLayout(vLayout);
    gridLayout->addWidget(replaceBtn, 1, 2);
    gridLayout->addWidget(cancelHighlightBtn, 2, 2);

    setLayout(gridLayout);
    setWindowTitle("Find and Replace");

    connect(cancelHighlightBtn, &QPushButton::clicked, this, &DFindDialog::cancelHihglight);
    connect(findNextBtn, &QPushButton::clicked, this, &DFindDialog::findNextBtn_Click);
    connect(replaceBtn, &QPushButton::clicked, this, &DFindDialog::replace);
    connect(replaceAllBtn, &QPushButton::clicked, this, &DFindDialog::repalceAll);
}


void DFindDialog::cancelHihglight()
{
    for(QTextCursor cursor : curs) {
        cursor.setCharFormat(QTextCharFormat());
    }
    index = 0;
    num = 0;
    searchstr = "";
    curs.clear();
}

void DFindDialog::findFirst()
{
    QMessageBox msgBox;

    for(QTextCursor cursor : curs) cursor.setCharFormat(QTextCharFormat());
    searchstr = searchEdit->text();
    index = 0;
    num = 0;
    curs.clear();
    searchstr = searchEdit->text();

    QTextCharFormat highlight;
    highlight.setBackground(Qt::yellow);
    for(QTextDocument *doc : docs) {
        QTextCursor cursor = doc->find(searchstr);
        while(!cursor.isNull()) {
            cursor.setCharFormat(highlight);
            curs.push_back(cursor);
            cursor = doc->find(searchstr, cursor);
            num++;
        }
    }
    if(curs.empty()) {
        msgBox.setText("无可查找对象");
        msgBox.exec();
        return;
    }
    qDebug() << "num:" << num;
    highlight.setBackground(Qt::cyan);
    curs[0].setCharFormat(highlight);
    // index++;
}

void DFindDialog::findUp()
{
    QMessageBox msgBox;
    if(index <= 0) {
        msgBox.setText("已经到达第一个，无法再查找上一个");
        msgBox.exec();
        return;
    }
    QTextCharFormat highlight;
    highlight.setBackground(Qt::yellow);
    curs[index].setCharFormat(highlight);

    QTextCursor cursor = curs[--index];
    highlight.setBackground(Qt::cyan);
    curs[index].setCharFormat(highlight);
}

void DFindDialog::findDown()
{
    QMessageBox msgBox;
    if(index >= num - 1) {
        msgBox.setText("已经到达最后一个，无法再查找下一个");
        msgBox.exec();
        return;
    }
    QTextCharFormat highlight;
    highlight.setBackground(Qt::yellow);
    curs[index].setCharFormat(highlight);

    QTextCursor cursor = curs[++index];
    highlight.setBackground(Qt::cyan);
    curs[index].setCharFormat(highlight);
}

void DFindDialog::replace()
{
    if(searchstr != searchEdit->text()) DFindDialog::findFirst();
    curs[index].setCharFormat(QTextCharFormat());
    curs[index].insertText(replaceEdit->text());
    curs.removeAt(index);
    num--;
    index--;
    qDebug() << "num:" << num;
    qDebug() << "index:" << index;
    if(num == 0) {
        QMessageBox msgBox;
        msgBox.setText("无可查找对象");
        msgBox.exec();
        return;
    }
    if(index >= num - 1 || index < 0) {
        index = 0;
        QTextCharFormat highlight;
        highlight.setBackground(Qt::cyan);
        curs[0].setCharFormat(highlight);
    }
    else DFindDialog::findDown();
}

void DFindDialog::repalceAll()
{
    if(replaceEdit->text() == "") {
        QMessageBox msgBox;
        msgBox.setText("提示");
        msgBox.setInformativeText("确认将其替换为空句子？");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        if(ret == QMessageBox::Cancel) return;
    }
    DFindDialog::findFirst();
    QString replaceStr = replaceEdit->text();
    for(QTextCursor cursor : curs) {
        cursor.setCharFormat(QTextCharFormat());
        cursor.insertText(replaceStr);
    }
    index = 0;
    num = 0;
    searchstr = "";
    curs.clear();
}

void DFindDialog::findNextBtn_Click()
{
    if(searchstr != searchEdit->text()) {
        DFindDialog::findFirst();
        return;
    }
    if(curs.empty()) {
        QMessageBox msgBox;
        msgBox.setText("无可查找对象");
        msgBox.exec();
        return;
    }
    if(forwardBtn->isChecked()) {
        DFindDialog::findUp();
        return;
    }
    if(backwardBtn->isChecked()) {
        DFindDialog::findDown();
        return;
    }
}

void DFindDialog::closeEvent(QCloseEvent *event)
{
    if(searchEdit->text() != "") searchEdit->clear();
    if(replaceEdit->text() != "") replaceEdit->clear();
    for(QTextCursor cursor : curs)
    {
        cursor.setCharFormat(QTextCharFormat());
    }
    index = 0;
    num = 0;
    searchstr = "";
    curs.clear();
    docs.clear();
    backwardBtn->setChecked(true);
    this->hide();
}

