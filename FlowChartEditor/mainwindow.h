#pragma once

#include "dscene.h"
#include "dview.h"
#include "dfinddialog.h"
#include "dtextitem.h"

#include <QSet>
#include <QColorDialog>
#include <QFontDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QWidget>
#include <QPushButton>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	void createMenu();
	void createToolBar();
	void bindAction();
    void initUi();

private slots:
    void saveAsSvg();

	void addLine();
	void addRect();
	void addRoundRect();
	void addEll();
	void addText();
    void addTri();
    void addDia();
    void addTrap();
    void addParallegram();
    void addDocShape();

    QSet<DTextBase *> getTextBases();
    void selectFrameCol();
    void selectFillCol();
    void selectTextCol();
    void selectTextFont();

	void rotateCW();
	void rotateCCW();
	void enlarge();
	void shrink();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	void viewRotateCW() { view->rotateCW(); }
	void viewRotateCCW() { view->rotateCCW(); }
	void viewEnlarge() { view->enlarge(); }
	void viewShrink() { view->shrink(); }
	void viewMoveLeft() { view->moveLeft(); }
	void viewMoveRight() { view->moveRight(); }
	void viewMoveUp() { view->moveUp(); }
	void viewMoveDown() { view->moveDown(); }

	void editRotateCW() { scene->rotateCW(); }
	void editRotateCCW() { scene->rotateCCW(); }
	void editEnlarge() { scene->enlarge(); }
	void editShrink() { scene->shrink(); }
	void editMoveLeft() { scene->moveLeft(); }
	void editMoveRight() { scene->moveRight(); }
	void editMoveUp() { scene->moveUp(); }
	void editMoveDown() { scene->moveDown(); }
    void findandReplace();

	void delSelectedItem();
	// void combineSelected();
	// void seperateSelected();

    void saveFile();
    void loadFile();

    void copy();
    void paste();

    void redo();
    void undo();
private:
	Ui::MainWindow *ui;

	DScene *scene;
	DView *view;
    DFindDialog *findDia;
    QColorDialog *colorDia;
    QFontDialog *fontDia;

    QSplitter *mainsplitter;
    QWidget *leftw;
    QGridLayout *leftGrid;
    QVBoxLayout *leftUpV;

    QPushButton *rectBtn;
    QPushButton *roundRectBtn;
    QPushButton *ellipseBtn;
    QPushButton *lineBtn;
    QPushButton *parellgramBtn;
    QPushButton *trapBtn;
    QPushButton *rhomBtn;
    QPushButton *fileBtn;
    QPushButton *triBtn;
    QPushButton *textBtn;
};
