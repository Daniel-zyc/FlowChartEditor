#ifndef FORMWORKWIDGET_H
#define FORMWORKWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>
#include <QToolTip>
#include <QMouseEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QList>
#include <QDialog>
#include <QStringList>
#include <QLineEdit>
#include <QTextEdit>

#include "dscene.h"
#include "dview.h"

struct FormworkData
{
    QString imgPath;
    QString FilePath;
    QString Title;
    QString Detail;
};


class FormworkWidget : public QWidget {
    Q_OBJECT

public:
    FormworkWidget(QWidget *parent = nullptr,DScene *scene = nullptr, DView* view = nullptr);
    void addItem(const FormworkData &data);
protected:
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void handleItemClick();

    void handleNewFormwork();

    void handleDeleteFormwork();

private:
    void loadFormwork();

    void updateForm();

    DScene * scene;

    DView *view;

    bool deleting = false;

    QList<FormworkData> fromworkData;

    QGridLayout *gridLayout;
    QList<QPushButton*> items;
    QPoint tooltipPos;
};

//=============================================

class NewFormworkDialog : public QDialog {
    Q_OBJECT
public:
    NewFormworkDialog(QWidget *parent = nullptr);

    QString getTitle() const;
    QString getDetail() const;

private slots:
    void accept() override;
    void reject() override;

private:
    QLineEdit *titleEdit;
    QTextEdit *detailEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;

    QString title;
    QString detail;
};



#endif // FORMWORKWIDGET_H
