#pragma once

#include "dscene.h"
#include "dview.h"

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

private slots:
	void addLine();
	void addRect();
	void addRoundRect();
	void addEll();
	void addText();
    void addTri();

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

	void delSelectedItem();

private:
	Ui::MainWindow *ui;

	DScene *scene;
	DView *view;
};
