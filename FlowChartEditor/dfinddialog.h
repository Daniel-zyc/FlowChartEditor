#ifndef DFINDDIALOG_H
#define DFINDDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QGroupBox>
#include <QTextDocument>
#include <QList>
#include <QVector>
#include <QTextCursor>
#include <QCloseEvent>

class DFindDialog : public QDialog
{
    Q_OBJECT
public:
    DFindDialog(QWidget *parent = nullptr);
    QVector<QTextDocument *> docs;
    QList<QTextCursor> curs;

protected:
    QGridLayout *gridLayout = new QGridLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();
    // QVBoxLayout *vLayout = new QVBoxLayout();
    QLabel *searchLabel = new QLabel("查找内容");
    QLabel *replaceLabel = new QLabel("替换内容");
    QLineEdit *searchEdit = new QLineEdit();
    QLineEdit *replaceEdit = new QLineEdit();
    QPushButton *findNextBtn = new QPushButton("查找");
    QRadioButton *forwardBtn = new QRadioButton("向前");
    QRadioButton *backwardBtn = new QRadioButton("向后");
    QGroupBox *dirGroup = new QGroupBox();
    // QGroupBox *rplGroup = new QGroupBox();
    QPushButton *findDownBtn = new QPushButton("查找下一个");
    QPushButton *replaceBtn = new QPushButton("替换");
    QPushButton *replaceAllBtn = new QPushButton("替换所有");
    QPushButton *cancelHighlightBtn = new QPushButton("取消高亮");


private slots:
    void cancelHihglight();
    void findFirst();
    void findUp();
    void findDown();
    void replace();
    void repalceAll();
    void findNextBtn_Click();

    void closeEvent(QCloseEvent *event) override;

private:
    int index = 0;
    int num = 0;
    QString searchstr = "";

};

#endif // DFINDDIALOG_H
