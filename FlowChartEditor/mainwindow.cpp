#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSvgGenerator>
// #include <QPainter>
#include "dtextitem.h"
#include "dshapebase.h"

#include <QGraphicsRectItem>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	createMenu();
	createToolBar();

	bindAction();

	scene = new DScene(this);

	QMenu *m = new QMenu();
	m->addAction(ui->actDelSelectedItem);
	m->addAction(ui->actCombine);
	m->addAction(ui->actSeperate);
	m->addAction(ui->actAddRect);
	m->addAction(ui->actAddEll);
	m->addAction(ui->actAddLine);

    findDia = new DFindDialog();

	scene->setMenu(m);
	scene->addLine(-1000, 0, 1000, 0);
	scene->addLine(0, -1000, 0, 1000);

	view = new DView(scene);

	view->setDragMode(QGraphicsView::RubberBandDrag);

	setCentralWidget(view);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::createMenu()
{
	ui->fileMenu->addAction(ui->actNewFile);
	ui->fileMenu->addAction(ui->actOpenFile);
	ui->fileMenu->addAction(ui->actSaveFile);
	ui->fileMenu->addAction(ui->actExportFile);
	ui->fileMenu->addAction(ui->actExit);

	ui->editMenu->addAction(ui->actUndo);
	ui->editMenu->addAction(ui->actRedo);
	ui->editMenu->addAction(ui->actSelectAll);
	ui->editMenu->addAction(ui->actCopy);
	ui->editMenu->addAction(ui->actCut);
	ui->editMenu->addAction(ui->actPaste);
    ui->editMenu->addAction(ui->actFindandReplace);
    // ui->editMenu->addAction(ui->actReplace);

	// ui->editMenu->addAction(ui->actEditEnlarge);
	// ui->editMenu->addAction(ui->actEditShrink);
	// ui->editMenu->addAction(ui->actEditRotateCW);
	// ui->editMenu->addAction(ui->actEditRotateCCW);
	// ui->editMenu->addAction(ui->actEditMoveLeft);
	// ui->editMenu->addAction(ui->actEditMoveRight);
	// ui->editMenu->addAction(ui->actEditMoveUp);
	// ui->editMenu->addAction(ui->actEditMoveDown);

	ui->editMenu->addAction(ui->actEnlarge);
	ui->editMenu->addAction(ui->actShrink);
	ui->editMenu->addAction(ui->actRotateCW);
	ui->editMenu->addAction(ui->actRotateCCW);
	ui->editMenu->addAction(ui->actMoveLeft);
	ui->editMenu->addAction(ui->actMoveRight);
	ui->editMenu->addAction(ui->actMoveUp);
	ui->editMenu->addAction(ui->actMoveDown);

	ui->addMenu->addAction(ui->actAddLine);
	ui->addMenu->addAction(ui->actAddArrowLine);
	ui->addMenu->addAction(ui->actAddDoubleArrowLine);
	ui->addMenu->addAction(ui->actAddEll);
	ui->addMenu->addAction(ui->actAddRect);
	ui->addMenu->addAction(ui->actAddRoundRect);
	ui->addMenu->addAction(ui->actAddPargram);
	ui->addMenu->addAction(ui->actAddRhom);
	ui->addMenu->addAction(ui->actAddTrap);
	ui->addMenu->addAction(ui->actAddTri);
	ui->addMenu->addAction(ui->actAddText);

	ui->addMenu->addAction(ui->actSetTextFont);
	ui->addMenu->addAction(ui->actSetTextColor);
	ui->addMenu->addAction(ui->actSetBorderColor);
	ui->addMenu->addAction(ui->actSetFillColor);

	ui->viewMenu->addAction(ui->actViewEnlarge);
	ui->viewMenu->addAction(ui->actViewShrink);
	ui->viewMenu->addAction(ui->actViewRotateCW);
	ui->viewMenu->addAction(ui->actViewRotateCCW);
	ui->viewMenu->addAction(ui->actViewMoveLeft);
	ui->viewMenu->addAction(ui->actViewMoveRight);
	ui->viewMenu->addAction(ui->actViewMoveDown);
	ui->viewMenu->addAction(ui->actViewMoveUp);
}

void MainWindow::createToolBar()
{
	ui->headToolBar->addAction(ui->actAddLine);
	ui->headToolBar->addAction(ui->actAddRect);
	ui->headToolBar->addAction(ui->actAddEll);
	ui->headToolBar->addAction(ui->actAddText);
	ui->headToolBar->addAction(ui->actAddRoundRect);
    ui->headToolBar->addAction(ui->actAddTri);
}

void MainWindow::bindAction()
{
	connect(ui->actAddLine, SIGNAL(triggered(bool)), this, SLOT(addLine()));
	connect(ui->actAddRect, SIGNAL(triggered(bool)), this, SLOT(addRect()));
	connect(ui->actAddRoundRect, SIGNAL(triggered(bool)), this, SLOT(addRoundRect()));
	connect(ui->actAddEll, SIGNAL(triggered(bool)), this, SLOT(addEll()));
	connect(ui->actAddText, SIGNAL(triggered(bool)), this, SLOT(addText()));
    connect(ui->actAddTri, SIGNAL(triggered(bool)), this, SLOT(addTri()));

	connect(ui->actViewRotateCW, SIGNAL(triggered(bool)), this, SLOT(viewRotateCW()));
	connect(ui->actViewRotateCCW, SIGNAL(triggered(bool)), this, SLOT(viewRotateCCW()));
	connect(ui->actViewEnlarge, SIGNAL(triggered(bool)), this, SLOT(viewEnlarge()));
	connect(ui->actViewShrink, SIGNAL(triggered(bool)), this, SLOT(viewShrink()));
	connect(ui->actViewMoveLeft, SIGNAL(triggered(bool)), this, SLOT(viewMoveLeft()));
	connect(ui->actViewMoveRight, SIGNAL(triggered(bool)), this, SLOT(viewMoveRight()));
	connect(ui->actViewMoveUp, SIGNAL(triggered(bool)), this, SLOT(viewMoveUp()));
	connect(ui->actViewMoveDown, SIGNAL(triggered(bool)), this, SLOT(viewMoveDown()));

	connect(ui->actEditRotateCW, SIGNAL(triggered(bool)), this, SLOT(editRotateCW()));
	connect(ui->actEditRotateCCW, SIGNAL(triggered(bool)), this, SLOT(editRotateCCW()));
	connect(ui->actEditEnlarge, SIGNAL(triggered(bool)), this, SLOT(editEnlarge()));
	connect(ui->actEditShrink, SIGNAL(triggered(bool)), this, SLOT(editShrink()));
	connect(ui->actEditMoveLeft, SIGNAL(triggered(bool)), this, SLOT(editMoveLeft()));
	connect(ui->actEditMoveRight, SIGNAL(triggered(bool)), this, SLOT(editMoveRight()));
	connect(ui->actEditMoveUp, SIGNAL(triggered(bool)), this, SLOT(editMoveUp()));
	connect(ui->actEditMoveDown, SIGNAL(triggered(bool)), this, SLOT(editMoveDown()));
    connect(ui->actFindandReplace, SIGNAL(triggered(bool)), this, SLOT(findandReplace()));

	connect(ui->actRotateCW, SIGNAL(triggered(bool)), this, SLOT(rotateCW()));
	connect(ui->actRotateCCW, SIGNAL(triggered(bool)), this, SLOT(rotateCCW()));
	connect(ui->actEnlarge, SIGNAL(triggered(bool)), this, SLOT(enlarge()));
	connect(ui->actShrink, SIGNAL(triggered(bool)), this, SLOT(shrink()));
	connect(ui->actMoveLeft, SIGNAL(triggered(bool)), this, SLOT(moveLeft()));
	connect(ui->actMoveRight, SIGNAL(triggered(bool)), this, SLOT(moveRight()));
	connect(ui->actMoveUp, SIGNAL(triggered(bool)), this, SLOT(moveUp()));
	connect(ui->actMoveDown, SIGNAL(triggered(bool)), this, SLOT(moveDown()));

	connect(ui->actDelSelectedItem, SIGNAL(triggered(bool)), this, SLOT(delSelectedItem()));
	connect(ui->actCombine, SIGNAL(triggered(bool)), this, SLOT(combineSelected()));
	connect(ui->actSeperate, SIGNAL(triggered(bool)), this, SLOT(seperateSelected()));
}

void MainWindow::addLine()
{
	scene->addLineItem();
	// DText *text = new DText("Hello World");
	// text->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	// scene->addItem(text);
}

void MainWindow::addRect()
{
	scene->addRectItem();
	// DRect *rect = new DRect(-100, -100, 200, 200);
	// rect->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	// scene->addItem(rect);
}

void MainWindow::addRoundRect()
{
	scene->addRoundRectItem();
}

void MainWindow::addEll()
{
	scene->addEllItem();
	// DShape *ell = new DShape();
	// ell->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	// scene->addItem(ell);
}

void MainWindow::addText()
{
	scene->addTextItem();
	// DText *text = new DText("Hello World");
	// text->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	// scene->addItem(text);
}

void MainWindow::addTri()
{
    scene->addTriItem();
}

void MainWindow::rotateCW()
{
	if(scene->selectedItems().isEmpty())
		view->rotateCW();
	else
		scene->rotateCW();
}

void MainWindow::rotateCCW()
{
	if(scene->selectedItems().isEmpty())
		view->rotateCCW();
	else
		scene->rotateCCW();
}

void MainWindow::enlarge()
{
	if(scene->selectedItems().isEmpty())
		view->enlarge();
	else
		scene->enlarge();
}

void MainWindow::shrink()
{
	if(scene->selectedItems().isEmpty())
		view->shrink();
	else
		scene->shrink();
}

void MainWindow::moveLeft()
{
	if(scene->selectedItems().isEmpty())
		view->moveLeft();
	else
		scene->moveLeft();
}

void MainWindow::moveRight()
{
	if(scene->selectedItems().isEmpty())
		view->moveRight();
	else
		scene->moveRight();
}

void MainWindow::moveUp()
{
	if(scene->selectedItems().isEmpty())
		view->moveUp();
	else
		scene->moveUp();
}

void MainWindow::moveDown()
{
	if(scene->selectedItems().isEmpty())
		view->moveDown();
	else
		scene->moveDown();
}

void MainWindow::findandReplace()
{
	// qDebug() << "f1";
    findDia->docs.clear();
	// qDebug() << "f2";
    QList<QGraphicsItem *> items = scene->items();
    for(QGraphicsItem *item : items){
		// qDebug() << "f3";
        DTextBase *text = dynamic_cast<DTextBase *>(item);
        if(text != nullptr){
            QTextDocument *doc0= text->document();
            findDia->docs.push_back(doc0);
			// qDebug() << "text";
        }
    }
	// qDebug() << "f4";
    findDia->show();
}

void MainWindow::delSelectedItem()
{
	scene->delSelectedItem();
}

void MainWindow::combineSelected()
{
	scene->combineSelected();
}

void MainWindow::seperateSelected()
{
	scene->seperateSelected();
}
