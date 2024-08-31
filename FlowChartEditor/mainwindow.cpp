#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSvgGenerator>
#include <QPainter>
#include <QFileDialog>
#include "dlineitem.h"
#include "dshapebase.h"

#include "saveandloadmanager.h"

#include <QSize>
#include <QColor>
#include <QGraphicsRectItem>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);
    setWindowTitle("Flowchart Editor");

    // 绑定序列化管理
	scene = new DScene(this);

    SaveAndLoadManager::instance().bindScene(scene);

	QMenu *m = new QMenu();
	m->addAction(ui->actDelSelectedItem);
	m->addAction(ui->actCombine);
	m->addAction(ui->actSeperate);
	m->addAction(ui->actAddRect);
	m->addAction(ui->actAddEll);
	m->addAction(ui->actAddLine);
    m->addAction(ui->actAddPargram);
    m->addAction(ui->actAddDoc);
    m->addAction(ui->actSelectFrameCol);
    m->addAction(ui->actSelectFillCol);
    m->addAction(ui->actSelectTextCol);
    m->addAction(ui->actSelectTextFont);

    findDia = new DFindDialog();

    colorDia = new QColorDialog(Qt::blue, this);
    colorDia->setOption(QColorDialog::ShowAlphaChannel);
    colorDia->setOption(QColorDialog::DontUseNativeDialog);

    fontDia = new QFontDialog(this);
    // fontDia->setOption(QFontDialog::DontUseNativeDialog);
    fontDia->setOption(QFontDialog::ScalableFonts);
    fontDia->setOption(QFontDialog::ProportionalFonts);

	scene->setMenu(m);
	scene->addLine(-1000, 0, 1000, 0);
	scene->addLine(0, -1000, 0, 1000);

	view = new DView(scene);

	view->setDragMode(QGraphicsView::RubberBandDrag);

    initUi();

    createMenu();
    // createToolBar();

    bindAction();

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::initUi()
{
    mainsplitter = new QSplitter(Qt::Horizontal, this);
    leftw = new QWidget();
    leftUpV = new QVBoxLayout(leftw);
    leftGrid = new QGridLayout();

    rectBtn = new QPushButton();
    roundRectBtn = new QPushButton();
    ellipseBtn = new QPushButton();
    lineBtn = new QPushButton();
    parellgramBtn = new QPushButton();
    trapBtn = new QPushButton();
    rhomBtn = new QPushButton();
    fileBtn = new QPushButton();
    textBtn = new QPushButton();
    triBtn = new QPushButton();

    rectBtn->setIcon(QPixmap(":/icon/rect.png"));
    roundRectBtn->setIcon(QPixmap(":/icon/roundrect.png"));
    ellipseBtn->setIcon(QPixmap(":/icon/ellipse.png"));
    lineBtn->setIcon(QPixmap(":/icon/line.png"));
    parellgramBtn->setIcon(QPixmap(":/icon/parellgram.png"));
    trapBtn->setIcon(QPixmap(":/icon/trapezoid.png"));
    rhomBtn->setIcon(QPixmap(":/icon/rhombic.png"));
    fileBtn->setIcon(QPixmap(":/icon/file.png"));
    textBtn->setIcon(QPixmap(":/icon/text.png"));
    triBtn->setIcon(QPixmap(":/icon/triangle.png"));

    leftGrid->addWidget(rectBtn, 0, 0);
    leftGrid->addWidget(roundRectBtn, 0, 1);
    leftGrid->addWidget(ellipseBtn, 1, 0);
    leftGrid->addWidget(lineBtn, 1, 1);
    leftGrid->addWidget(parellgramBtn, 2, 0);
    leftGrid->addWidget(trapBtn, 2, 1);
    leftGrid->addWidget(rhomBtn, 3, 0);
    leftGrid->addWidget(fileBtn, 3, 1);
    leftGrid->addWidget(triBtn, 4, 0);
    leftGrid->addWidget(textBtn, 4, 1);

    leftUpV->addLayout(leftGrid);
    leftUpV->addStretch();
    leftw->setLayout(leftUpV);

    mainsplitter->addWidget(leftw);
    mainsplitter->addWidget(view);
    mainsplitter->setStretchFactor(1, 1);

    setCentralWidget(mainsplitter);
}

void MainWindow::createMenu()
{
	ui->fileMenu->addAction(ui->actNewFile);
	ui->fileMenu->addAction(ui->actOpenFile);
	ui->fileMenu->addAction(ui->actSaveFile);
    ui->fileMenu->addAction(ui->actSvgFile);
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
    ui->headToolBar->addAction(ui->actAddRhom);
    ui->headToolBar->addAction(ui->actAddTrap);
    ui->headToolBar->addAction(ui->actAddPargram);
    ui->headToolBar->addAction(ui->actAddDoc);
}

void MainWindow::bindAction()
{
    connect(ui->actSaveFile,SIGNAL(triggered(bool)), this, SLOT(saveFile()));
    connect(ui->actOpenFile,SIGNAL(triggered(bool)), this, SLOT(loadFile()));

    connect(ui->actSvgFile, SIGNAL(triggered(bool)), this, SLOT(saveAsSvg()));

	connect(ui->actAddLine, SIGNAL(triggered(bool)), this, SLOT(addLine()));
	connect(ui->actAddRect, SIGNAL(triggered(bool)), this, SLOT(addRect()));
	connect(ui->actAddRoundRect, SIGNAL(triggered(bool)), this, SLOT(addRoundRect()));
	connect(ui->actAddEll, SIGNAL(triggered(bool)), this, SLOT(addEll()));
	connect(ui->actAddText, SIGNAL(triggered(bool)), this, SLOT(addText()));
    connect(ui->actAddTri, SIGNAL(triggered(bool)), this, SLOT(addTri()));
    connect(ui->actAddRhom, SIGNAL(triggered(bool)), this, SLOT(addDia()));
    connect(ui->actAddTrap, SIGNAL(triggered(bool)), this, SLOT(addTrap()));

    connect(ui->actAddPargram, SIGNAL(triggered(bool)), this, SLOT(addParallegram()));
    connect(ui->actAddDoc, SIGNAL(triggered(bool)), this, SLOT(addDocShape()));

    connect(ui->actSelectFillCol, SIGNAL(triggered(bool)), this, SLOT(selectFillCol()));
    connect(ui->actSelectFrameCol, SIGNAL(triggered(bool)), this, SLOT(selectFrameCol()));
    connect(ui->actSelectTextCol, SIGNAL(triggered(bool)), this, SLOT(selectTextCol()));
    connect(ui->actSelectTextFont, SIGNAL(triggered(bool)), this, SLOT(selectTextFont()));

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

    connect(rectBtn, &QPushButton::clicked, this, &MainWindow::addRect);
    connect(lineBtn, &QPushButton::clicked, this, &MainWindow::addLine);
    connect(roundRectBtn, &QPushButton::clicked, this, &MainWindow::addRoundRect);
    connect(ellipseBtn, &QPushButton::clicked, this, &MainWindow::addEll);
    connect(textBtn, &QPushButton::clicked, this, &MainWindow::addText);
    connect(triBtn, &QPushButton::clicked, this, &MainWindow::addTri);
}

void MainWindow::saveAsSvg()
{
    QString filePath = QFileDialog::getSaveFileName(this, "save as svg file");
    if(filePath == "") return;
    QSvgGenerator generator;
    generator.setFileName(filePath);
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
}

void MainWindow::addLine()
{
	scene->addLineItem();
}

void MainWindow::addRect()
{
	scene->addRectItem();
}

void MainWindow::addRoundRect()
{
	scene->addRoundRectItem();
}

void MainWindow::addEll()
{
	scene->addEllItem();
}

void MainWindow::addText()
{
	scene->addTextItem();
}

void MainWindow::addTri()
{
    scene->addTriItem();
}

void MainWindow::addDia()
{
    scene->addDiaItem();
}
void MainWindow::addTrap()
{
    scene->addTrapItem();
}

void MainWindow::addParallegram()
{
    scene->addParallegramItem();
}

void MainWindow::addDocShape()
{
    scene->addDocItem();
}

QSet<DTextBase *> MainWindow::getTextBases()
{
    QSet<DTextBase *> texts;
    QList<QGraphicsItem *> items = scene->selectedItems();
    for(QGraphicsItem *item : items) {
        DTextItem *textitem = dynamic_cast<DTextItem *>(item);
        if(textitem != nullptr) {
            texts.insert(&(textitem->textBase));
            continue;
        }
        DShapeBase *shape = dynamic_cast<DShapeBase *>(item);
        if(shape != nullptr) {
            texts.insert(&(shape->textItem->textBase));
            continue;
        }
        DTextBase *textbase = dynamic_cast<DTextBase *>(item);
        if(textbase != nullptr) {
            // qDebug() << "text not null";
            texts.insert(textbase);
        }
    }
    return texts;
}

void MainWindow::selectFrameCol()
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    QColor color = colorDia->getColor(Qt::white, this);
    for(QGraphicsItem *item : items) {
        DLineItem *line = dynamic_cast<DLineItem *>(item);
        if(line != nullptr) {
            QPen npen = line->pen();
            npen.setColor(color);
            line->setPen(npen);
            continue;
        }
        DShapeBase *shape = dynamic_cast<DShapeBase *>(item);
        if(shape != nullptr) {
            DTextItem *text = dynamic_cast<DTextItem *>(shape);
            if(text != nullptr) continue;
            QPen npen = shape->pen();
            npen.setColor(color);
            shape->setPen(npen);
        }
    }
}

void MainWindow::selectFillCol()
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    QColor color = colorDia->getColor(Qt::white, this);
    // qDebug() << items;
    // qDebug() << color;
    for(QGraphicsItem *item : items) {
        DShapeBase *shape = dynamic_cast<DShapeBase *>(item);
        if(shape != nullptr) {
            DTextItem *text = dynamic_cast<DTextItem *>(shape);
            if(text != nullptr) continue;
            QBrush nbrush = shape->brush();
            nbrush.setColor(color);
            shape->setBrush(nbrush);
        }
    }
}

void MainWindow::selectTextCol()
{
    QSet<DTextBase *> texts = getTextBases();
    QColor color = colorDia->getColor(Qt::white, this);
    // qDebug() << items;
    // qDebug() << color;

    QTextCharFormat charformat;
    charformat.setForeground(color);
    // charformat.setForeground(color);
    for(DTextBase *tbase : texts) {
        QTextCursor cursor(tbase->document());
        // if(cursor.hasSelection()) {
        //     // qDebug() << "has selection";
        //     cursor.mergeCharFormat(charformat);
        // }
        // cursor.beginEditBlock();
        cursor.movePosition(QTextCursor::Start);
        while(!cursor.isNull() && !cursor.atEnd()) {
            cursor.select(QTextCursor::BlockUnderCursor);
            cursor.mergeCharFormat(charformat);
            cursor.movePosition(QTextCursor::NextBlock);
		}
        // cursor.endEditBlock();
    }
}

void MainWindow::selectTextFont()
{
    QSet<DTextBase *> texts = getTextBases();
    bool flag = true;
    QFont font = fontDia->getFont(&flag, this);
    if(flag) {
        for(DTextBase *tbase : texts) {
            tbase->document()->setDefaultFont(font);
        }
    }
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
    findDia->docs.clear();
    QList<QGraphicsItem *> items = scene->items();
    for(QGraphicsItem *item : items){
        DTextBase *text = dynamic_cast<DTextBase *>(item);
        if(text != nullptr){
            QTextDocument *doc0= text->document();
            findDia->docs.push_back(doc0);
        }
    }
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

void MainWindow::saveFile(){
    QString filePath = QFileDialog::getSaveFileName(this, "save");
    if(filePath == "") return;

    QList<QGraphicsItem *> items = scene->selectedItems();
    for(QGraphicsItem *item : items) {
        item->setSelected(false);
    }

    SaveAndLoadManager::instance().saveToFile(filePath);
}

void MainWindow::loadFile(){
    QString filePath = QFileDialog::getOpenFileName(this, "load");
    if(filePath == "") return;

    SaveAndLoadManager::instance().loadFromFile(filePath);
}
