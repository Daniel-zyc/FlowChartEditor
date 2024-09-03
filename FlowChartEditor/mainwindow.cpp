#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSvgGenerator>
#include <QPainter>
#include <QFileDialog>
#include "dlineitem.h"
#include "dshapebase.h"

#include "saveandloadmanager.h"
#include "undomanager.h"
#include "aboutuswindow.h"

#include <QMessageBox>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QActionGroup>
#include <QShortcut>
#include <QKeySequence>
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

	// scene->removeItem(rect);
	// delete rect; rect = nullptr;

    UndoManager::instance().bindScene(scene);
    SaveAndLoadManager::instance().bindScene(scene);

	QMenu *m = new QMenu();
	m->addAction(ui->actDelSelectedItem);
	// m->addAction(ui->actCombine);
	// m->addAction(ui->actSeperate);
	m->addAction(ui->actAddRect);
	m->addAction(ui->actAddEll);
    m->addAction(ui->actAddRhom);
	m->addAction(ui->actAddLine);
    m->addAction(ui->actAddPargram);
    m->addAction(ui->actAddDoc);
    m->addAction(ui->actAddTrap);
    m->addAction(ui->actAddPrede);
    m->addAction(ui->actAddEnd);
    m->addAction(ui->actAddManualinput);
    m->addAction(ui->actSelectFrameCol);
    m->addAction(ui->actSelectFillCol);
    m->addAction(ui->actSelectTextCol);
    m->addAction(ui->actSelectTextFont);
    m->addAction(ui->actStyleSheet);
    // m->addAction(ui->actMoveSelectedZUp);
    // m->addAction(ui->actMoveSelectedZDown);
    m->addAction(ui->actMoveSelectedMaxZUp);
    m->addAction(ui->actMoveSelectedMaxZDown);


    findDia = new DFindDialog();

    colorDia = new QColorDialog(Qt::blue, this);
    // colorDia->setOption(QColorDialog::ShowAlphaChannel);
    // colorDia->setOption(QColorDialog::DontUseNativeDialog);

    fontDia = new QFontDialog(this);
    // fontDia->setOption(QFontDialog::DontUseNativeDialog);
    fontDia->setOption(QFontDialog::ScalableFonts);
    fontDia->setOption(QFontDialog::ProportionalFonts);

	scene->setMenu(m);
    scene->clear();

	view = new DView(scene);
	scene->setView(view);

	inspector = new Inspector(this,scene,view);

    initUi();

    createMenu();
    createToolBar();

    bindAction();
    connectLeft();
    connectRight();
	// 调试用
	// scene->addDFNodeItem();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::initUi()
{
    initleftUi();
    initrightUi();

    mainsplitter->addWidget(leftw);
    mainsplitter->addWidget(view);
    mainsplitter->setStretchFactor(1, 1);
    mainsplitter->addWidget(rightTab);


    setCentralWidget(mainsplitter);
}

void MainWindow::initleftUi()
{
    //图形页面
    mainsplitter = new QSplitter(Qt::Horizontal, this);
    leftw = new QWidget();
    leftUpV = new QVBoxLayout(leftw);
    flowGrid = new QGridLayout();
    primaryGrid = new QGridLayout();
    primaryGroup = new QGroupBox("基本图形");
    flowcGroup = new QGroupBox("流程图图形");
    lineGroup = new QGroupBox("线条");
    textGroup = new QGroupBox("文本");

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
    preBtn = new QPushButton();
    endBtn = new QPushButton();
    prepareBtn = new QPushButton();
    storeBtn = new QPushButton();
    polyLineBtn = new QPushButton();

	addRectBtn = new QPushButton();
	addEllBtn = new QPushButton();
	addParagramBtn = new QPushButton();
	addTrapBtn = new QPushButton();
	addDiaBtn = new QPushButton();
	addTriBtn = new QPushButton();

    rectBtn->setIcon(QPixmap(":/icon/flowchart/rect.png"));
    roundRectBtn->setIcon(QPixmap(":/icon/flowchart/roundrect.png"));
    ellipseBtn->setIcon(QPixmap(":/icon/flowchart/node.png"));
    lineBtn->setIcon(QPixmap(":/icon/line.png"));
    parellgramBtn->setIcon(QPixmap(":/icon/flowchart/parallgram.png"));
    trapBtn->setIcon(QPixmap(":/icon/flowchart/manul.png"));
    rhomBtn->setIcon(QPixmap(":/icon/flowchart/rhomb.png"));
    fileBtn->setIcon(QPixmap(":/icon/flowchart/doc.png"));
    textBtn->setIcon(QPixmap(":/icon/text.png"));
    triBtn->setIcon(QPixmap(":/icon/flowchart/triangle.png"));
    preBtn->setIcon(QPixmap(":/icon/flowchart/pre.png"));
    endBtn->setIcon(QPixmap(":/icon/flowchart/teminate.png"));
    prepareBtn->setIcon(QPixmap(":/icon/flowchart/prepare.png"));
    storeBtn->setIcon(QPixmap(":/icon/flowchart/store.png"));
    polyLineBtn->setIcon(QPixmap(":icon/polyLine.png"));

    rectBtn->setToolTip("过程");
    roundRectBtn->setToolTip("可选过程");
    ellipseBtn->setToolTip("接点");
    lineBtn->setToolTip("直线");
    parellgramBtn->setToolTip("数据");
    trapBtn->setToolTip("手动操作");
    rhomBtn->setToolTip("决策");
    fileBtn->setToolTip("文档");
    textBtn->setToolTip("文本框");
    triBtn->setToolTip("摘录");
    preBtn->setToolTip("预定义过程");
    endBtn->setToolTip("终止");
    prepareBtn->setToolTip("准备");
    storeBtn->setToolTip("内部贮存");
    polyLineBtn->setToolTip("折线");

    addRectBtn->setToolTip("矩形");
    addEllBtn->setToolTip("椭圆");
    addParagramBtn->setToolTip("平行四边形");
    addTrapBtn->setToolTip("梯形");
    addDiaBtn->setToolTip("菱形");
    addTriBtn->setToolTip("三角形");

    addRectBtn->setIcon(QPixmap(":/icon/primary/rect.png"));
    addEllBtn->setIcon(QPixmap(":/icon/primary/ellipse.png"));
    addParagramBtn->setIcon(QPixmap(":/icon/primary/parallgram.png"));
    addTrapBtn->setIcon(QPixmap(":/icon/primary/trapezoid.png"));
    addDiaBtn->setIcon(QPixmap(":/icon/primary/rhomb.png"));
    addTriBtn->setIcon(QPixmap(":/icon/primary/triangle.png"));

    flowGrid->addWidget(rectBtn, 0, 0);
    flowGrid->addWidget(roundRectBtn, 0, 1);
    flowGrid->addWidget(ellipseBtn, 0, 2);
    flowGrid->addWidget(parellgramBtn, 1, 0);
    flowGrid->addWidget(trapBtn, 1, 1);
    flowGrid->addWidget(rhomBtn, 1, 2);
    flowGrid->addWidget(fileBtn, 2, 0);
    flowGrid->addWidget(triBtn, 2, 1);
    flowGrid->addWidget(preBtn, 2, 2);
    flowGrid->addWidget(endBtn, 3, 0);
    flowGrid->addWidget(prepareBtn, 3, 1);
    flowGrid->addWidget(storeBtn, 3, 2);

	primaryGrid->addWidget(addRectBtn, 0, 0);
	primaryGrid->addWidget(addEllBtn, 0, 1);
	primaryGrid->addWidget(addParagramBtn, 0, 2);
	primaryGrid->addWidget(addTrapBtn, 1, 0);
	primaryGrid->addWidget(addDiaBtn, 1, 1);
	primaryGrid->addWidget(addTriBtn, 1, 2);

    primaryGroup->setLayout(primaryGrid);
    flowcGroup->setLayout(flowGrid);
    leftUpV->addWidget(primaryGroup);
    leftUpV->addWidget(flowcGroup);
    leftw->setLayout(leftUpV);

    lineGrid = new QGridLayout();
    lineGrid->addWidget(lineBtn, 0, 0);
    lineGrid->addWidget(polyLineBtn, 0, 1);
    QLabel *noneLabel = new QLabel();
    lineGrid->addWidget(noneLabel, 0, 2);
    lineGroup->setLayout(lineGrid);

    textGrid = new QGridLayout();
    textGrid->addWidget(textBtn, 0, 0);
    QLabel *noneLabel1 = new QLabel();
    textGrid->addWidget(noneLabel1, 0, 1);
    QLabel *noneLabel2 = new QLabel();
    textGrid->addWidget(noneLabel2, 0, 2);
    textGroup->setLayout(textGrid);

    leftUpV->addWidget(lineGroup);
    leftUpV->addWidget(textGroup);
    leftUpV->addStretch();
}

void MainWindow::initrightUi()
{
    //样式表
    rightTab = new QTabWidget();
    rightTab->setMovable(true);
    rightTab->setMinimumWidth(230);
    rightTab->setMaximumWidth(320);

    //背景样式表
    blankBg = new QRadioButton("无背景");
    gridBg = new QRadioButton("网格");
    dotBg = new QRadioButton("点状");
    blankBg->setChecked(true);

    rightBgw = new QTreeWidget();
    rightBgw->setColumnCount(2);
    // rightBgw->setHeaderLabels({"背景选择", "", ""});
    rightBgw->setHeaderHidden(true);
    rightBgw->header()->setStretchLastSection(true);
    rightBgw->setColumnWidth(0, 150);
    colorTop = new QTreeWidgetItem(rightBgw);
    patternTop = new QTreeWidgetItem(rightBgw);

    colorTop->setText(0, "颜色");
    patternTop->setText(0, "图案");

    colorChild0 = new QTreeWidgetItem(colorTop);
    colorChild0->setText(0, "纯色填充");
    selectedColor = new QPushButton();
    setColorIcon(selectedColor);
    // selectedColor->setIcon(QPixmap(":/icon/palette.png"));
    rightBgw->setItemWidget(colorChild0, 1, selectedColor);

    patternChild0 = new QTreeWidgetItem(patternTop);
    patternChild1 = new QTreeWidgetItem(patternTop);
    patternChild2 = new QTreeWidgetItem(patternTop);
    patternChild3 = new QTreeWidgetItem(patternTop);

    customizeBg = new QRadioButton("自定义图片填充");
    repickBtn = new QPushButton("重选");

    rightBgw->setItemWidget(patternChild0, 0, blankBg);
    rightBgw->setItemWidget(patternChild1, 0, gridBg);
    rightBgw->setItemWidget(patternChild2, 0, dotBg);
    rightBgw->setItemWidget(patternChild3, 0, customizeBg);
    rightBgw->setItemWidget(patternChild3, 1, repickBtn);

    rightBgw->addTopLevelItem(colorTop);
    rightBgw->addTopLevelItem(patternTop);

    rightTab->addTab(rightBgw, "背景");

    //形状样式表
    rightShapew = new QWidget();
    rightShapef = new QFormLayout();
    QHBoxLayout *frameH = new QHBoxLayout();
    QHBoxLayout *fillH = new QHBoxLayout();
    frameColor = new QPushButton();
    fillColor = new QPushButton();

    setColorIcon(frameColor);
    setColorIcon(fillColor);

    frameH->addStretch();
    frameH->addWidget(frameColor);
    fillH->addStretch();
    fillH->addWidget(fillColor);

    rightShapef->addRow("边框：", frameH);
    rightShapef->addRow("填充：", fillH);
    rightShapew->setLayout(rightShapef);

    rightTab->addTab(rightShapew, "形状");

    //文本样式表
    rightFontw = new QWidget();
    rightFontf = new QFormLayout();
    QHBoxLayout *colorH = new QHBoxLayout();
    QHBoxLayout *fontH = new QHBoxLayout();
    textColor = new QPushButton();
    setColorIcon(textColor);
    textFont = new QPushButton("字体");
    textFont->setFixedWidth(100);

    colorH->addStretch();
    colorH->addWidget(textColor);
    fontH->addStretch();
    fontH->addWidget(textFont);

    rightFontf->addRow("文本颜色：", colorH);
    rightFontf->addRow("文本字体：", fontH);

    rightFontw->setLayout(rightFontf);
    rightTab->addTab(rightFontw, "文本");

    //线条样式表
    rightLinew = new QWidget();
    QHBoxLayout *lineH = new QHBoxLayout();
    QHBoxLayout *arrowH = new QHBoxLayout();
    QHBoxLayout *lineboundH = new QHBoxLayout();
    lineConfirm = new QPushButton("确认");
    arrowConfirm = new QPushButton("确认");
    lineboundConfirm  = new QPushButton("确认");
    linecolor  = new QPushButton();

    lineConfirm->setFixedWidth(50);
    arrowConfirm->setFixedWidth(50);
    lineboundConfirm->setFixedWidth(50);
    setColorIcon(linecolor);
    // linecolor->setIcon(QPixmap(":/icon/palette.png"));
    // linecolor->setIconSize(linecolor->size());
    formright = new QFormLayout();
    formright->setRowWrapPolicy(QFormLayout::DontWrapRows);
    // formright->setLabelAlignment(Qt::AlignLeft);

    lineType = new QComboBox();
    lineType->addItem(QIcon(":/icon/solidLine.png"), "实线");
    lineType->addItem(QIcon(":/icon/dashLine.png"), "短划线");
    lineType->addItem(QIcon(":/icon/dotLine.png"), "点线");
    lineType->addItem(QIcon(":/icon/dashDotLine.png"), "点划线");
    lineType->addItem(QIcon(":/icon/dashDDLine.png"), "双点划线");
    lineH->addWidget(lineType);
    lineH->addWidget(lineConfirm);

    arrowType = new QComboBox();
    arrowType->addItem(QIcon(":/icon/noArrow.png"), "无箭头");
    arrowType->addItem(QIcon(":/icon/arrow.png"), "箭头");
    arrowType->addItem(QIcon(":/icon/openArrow.png"), "开放型箭头");
    arrowType->addItem(QIcon(":/icon/dovetailArrow.png"), "燕尾箭头");
    arrowType->addItem(QIcon(":/icon/diaArrow.png"), "菱形箭头");
    arrowType->addItem(QIcon(":/icon/roundArrow.png"), "圆型箭头");
    arrowH->addWidget(arrowType);
    arrowH->addWidget(arrowConfirm);

    linebound = new QDoubleSpinBox();
    linebound->setRange(0, 1000);
    linebound->setSingleStep(0.25);
    linebound->setValue(1);
    linebound->setSuffix("磅");
    linebound->setWrapping(true);
    lineboundH->addWidget(linebound);
    lineboundH->addWidget(lineboundConfirm);

    formright->addRow("线条颜色", linecolor);
    formright->addRow("线条类型", lineH);
    formright->addRow("箭头类型", arrowH);
    formright->addRow("线条磅数", lineboundH);

    rightLinew->setLayout(formright);
    // rightw->setVisible(false);
    rightTab->addTab(rightLinew, "线条");

    // rightTab->setVisible(false);
}

void MainWindow::connectLeft()
{
	connect(addRectBtn, &QPushButton::clicked, this, &MainWindow::addRect);
	connect(addEllBtn, &QPushButton::clicked, this, &MainWindow::addEll);
	connect(addTriBtn, &QPushButton::clicked, this, &MainWindow::addTri);
	connect(addDiaBtn, &QPushButton::clicked, this, &MainWindow::addDia);
	connect(addTrapBtn, &QPushButton::clicked, this, &MainWindow::addTrap);
	connect(addParagramBtn, &QPushButton::clicked, this, &MainWindow::addParagram);

	connect(rectBtn, &QPushButton::clicked, this, &MainWindow::addDFProcessItem);
	connect(lineBtn, &QPushButton::clicked, this, &MainWindow::addLine);
	connect(roundRectBtn, &QPushButton::clicked, this, &MainWindow::addDFOptionalProcessItem);
	connect(ellipseBtn, &QPushButton::clicked, this, &MainWindow::addDFNodeItem);
	connect(textBtn, &QPushButton::clicked, this, &MainWindow::addText);
	connect(triBtn, &QPushButton::clicked, this, &MainWindow::addTri);
	connect(rhomBtn, &QPushButton::clicked, this, &MainWindow::addDFConditionItem);
	connect(fileBtn, &QPushButton::clicked, this, &MainWindow::addDFDocItem);
	connect(trapBtn, &QPushButton::clicked, this, &MainWindow::addDFManualOperateItem);
	connect(endBtn, &QPushButton::clicked, this, &MainWindow::addDFStartEndItem);
	connect(preBtn, &QPushButton::clicked, this, &MainWindow::addDFPredefineItem);
	connect(parellgramBtn, &QPushButton::clicked, this, &MainWindow::addDFDataItem);
	connect(storeBtn,  &QPushButton::clicked, this, &MainWindow::addDFInternalStoreItem);
	connect(prepareBtn,  &QPushButton::clicked, this, &MainWindow::addDFPrepareItem);
	connect(polyLineBtn,  &QPushButton::clicked, this, &MainWindow::addPolyLine);
}

void MainWindow::connectRight()
{
    connect(ui->actSolidLine, &QAction::triggered, this, [this]() {
        changeLineType(Qt::SolidLine);
    });
    connect(ui->actDashLine, &QAction::triggered, this, [this]() {
        changeLineType(Qt::DashLine);
    });
    connect(ui->actDotLine, &QAction::triggered, this, [this]() {
        changeLineType(Qt::DotLine);
    });
    connect(ui->actDashDotLine, &QAction::triggered, this, [this]() {
        changeLineType(Qt::DashDotLine);
    });
    connect(ui->actDashDDLine, &QAction::triggered, this, [this]() {
        changeLineType(Qt::DashDotDotLine);
    });

    connect(ui->actNoArrow, &QAction::triggered, this, [this]() {
        changeEndArrow(0);
    });
    connect(ui->actArrow, &QAction::triggered, this, [this]() {
        changeEndArrow(1);
    });
    connect(ui->actOpenArrow, &QAction::triggered, this, [this]() {
        changeEndArrow(2);
    });
    connect(ui->actDovetailArrow, &QAction::triggered, this, [this]() {
        changeEndArrow(3);
    });
    connect(ui->actDiaArrow, &QAction::triggered, this, [this]() {
        changeEndArrow(4);
    });
    connect(ui->actRoundArrow, &QAction::triggered, this, [this]() {
        changeEndArrow(5);
    });

    connect(blankBg, &QRadioButton::toggled, this, [this](bool checked) {
        if(checked) setSceneBg(":/icon/blankBg.png");
    });
    connect(gridBg, &QRadioButton::toggled, this, [this](bool checked) {
        if(checked) setSceneBg(":/icon/gridBg.png");
    });
    connect(dotBg, &QRadioButton::toggled, this, [this](bool checked) {
        if(checked) setSceneBg(":/icon/dotBg.png");
    });
    connect(customizeBg, &QRadioButton::toggled, this, [this](bool checked) {
        if(checked) {
            QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "", ("Images(*.jpg *.png *.svg *.bmp"));
            if(!fileName.isEmpty()) {
                setSceneBg(fileName);
            }else {
                blankBg->setChecked(true);
            }
        }
    });
    connect(repickBtn, &QPushButton::clicked, this, [this](){
        if(customizeBg->isChecked()) {
            QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "", ("Images(*.jpg *.png *.svg"));
            if(!fileName.isEmpty()) setSceneBg(fileName);
        }
    });
    connect(selectedColor, &QPushButton::clicked, this, [this](){
        QColor color = QColorDialog::getColor(Qt::white, this);
        if(color.isValid()) scene->setBackgroundBrush(QBrush(color));
        blankBg->setChecked(true);
    });
    connect(lineConfirm, &QPushButton::clicked, this, [this](){
        int penstyle = lineType->currentIndex();
        qDebug() << "penstyle;" << penstyle;
        switch(penstyle) {
        case 0: changeLineType(Qt::SolidLine); break;
        case 1: changeLineType(Qt::DashLine); break;
        case 2: changeLineType(Qt::DotLine); break;
        case 3: changeLineType(Qt::DashDotLine); break;
        case 4: changeLineType(Qt::DashDotDotLine); break;
        }
    });
    connect(arrowConfirm, &QPushButton::clicked, this, [this](){
        int arrowstyle = arrowType->currentIndex();
        // qDebug() << "arrowstyle" << arrowstyle;
        switch(arrowstyle) {
        case 0: changeEndArrow(0); break;
        case 1: changeEndArrow(1); break;
        case 2: changeEndArrow(2); break;
        case 3: changeEndArrow(3); break;
        case 4: changeEndArrow(4); break;
        case 5: changeEndArrow(5); break;
        }
    });
    connect(lineboundConfirm, &QPushButton::clicked, this, [this](){
        scene->changeLineWidth(linebound->value());
    });
    connect(linecolor, &QPushButton::clicked, this, [this](){
        QColor color = QColorDialog::getColor(Qt::white, this);
        scene->changeLineColor(color);
    });
    connect(frameColor, &QPushButton::clicked, this, &MainWindow::selectFrameCol);
    connect(fillColor, &QPushButton::clicked, this, &MainWindow::selectFillCol);
    connect(textColor, &QPushButton::clicked, this, &MainWindow::selectTextCol);
    connect(textFont, &QPushButton::clicked, this, &MainWindow::selectTextFont);
}

void MainWindow::createMenu()
{
    QMenu *lineType = new QMenu("线条");

    QMenu *arrowTypeM = new QMenu("箭头类型");
    arrowTypeM->addAction(ui->actNoArrow);
    arrowTypeM->addAction(ui->actArrow);
    arrowTypeM->addAction(ui->actOpenArrow);
    arrowTypeM->addAction(ui->actDovetailArrow);
    arrowTypeM->addAction(ui->actDiaArrow);
    arrowTypeM->addAction(ui->actRoundArrow);
    QActionGroup *arrowGroup = new QActionGroup(lineType);
    arrowGroup->setExclusive(true);
    arrowGroup->addAction(ui->actNoArrow);
    arrowGroup->addAction(ui->actArrow);
    arrowGroup->addAction(ui->actOpenArrow);
    arrowGroup->addAction(ui->actDovetailArrow);
    arrowGroup->addAction(ui->actDiaArrow);
    arrowGroup->addAction(ui->actRoundArrow);

    QMenu *lineTypeM = new QMenu("线条类型");
    lineTypeM->addAction(ui->actSolidLine);
    lineTypeM->addAction(ui->actDashLine);
    lineTypeM->addAction(ui->actDotLine);
    lineTypeM->addAction(ui->actDashDotLine);
    lineTypeM->addAction(ui->actDashDDLine);
    QActionGroup *lineGroup = new QActionGroup(lineType);
    lineGroup->setExclusive(true);
    lineGroup->addAction(ui->actSolidLine);
    lineGroup->addAction(ui->actDashLine);
    lineGroup->addAction(ui->actDotLine);
    lineGroup->addAction(ui->actDashDotLine);
    lineGroup->addAction(ui->actDashDDLine);

    lineType->addMenu(arrowTypeM);
    lineType->addMenu(lineTypeM);
    // ui->styleMenu->addMenu(lineType);
    scene->menu->addMenu(lineType);

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
    ui->addMenu->addAction(ui->actAddDoc);
	ui->addMenu->addAction(ui->actAddRhom);
	ui->addMenu->addAction(ui->actAddTrap);
    ui->addMenu->addAction(ui->actAddTri);
    ui->addMenu->addAction(ui->actAddEnd);
    ui->addMenu->addAction(ui->actAddPrede);
	ui->addMenu->addAction(ui->actAddText);
    ui->addMenu->addAction(ui->actAddPrede);
    ui->addMenu->addAction(ui->actAddEnd);
    ui->addMenu->addAction(ui->actAddPolyLine);

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

    ui->helpMenu->addAction(ui->actAboutUs);
    ui->helpMenu->addAction(ui->actCheck);
}

void MainWindow::createToolBar()
{
    QFile qssfile(":/stylesheet.qss");
    if(qssfile.open(QFile::ReadOnly)) {
        QTextStream textstream(&qssfile);
        QString stylesheet = textstream.readAll();
        setStyleSheet(stylesheet);
        qssfile.close();
    }

    createTln = new QToolButton();
    openTln = new QToolButton();
    saveTln = new QToolButton();
    saveSvgTln = new QToolButton();

    createTln->addAction(ui->actNewFile);
    openTln->addAction(ui->actOpenFile);
    saveTln->addAction(ui->actSaveFile);
    saveSvgTln->addAction(ui->actSvgFile);

    createTln->setIcon(QIcon(":/icon/createfile.png"));
    openTln->setIcon(QIcon(":/icon/openfile.png"));
    saveTln->setIcon(QIcon(":/icon/savefile.png"));
    saveSvgTln->setIcon(QIcon(":/icon/savesvg.png"));

    createTln->setToolTip("新建文件");
    openTln->setToolTip("打开文件");
    saveTln->setToolTip("保存文件");
    saveSvgTln->setToolTip("导出Svg格式");

    ui->headToolBar->addWidget(createTln);
    ui->headToolBar->addWidget(openTln);
    ui->headToolBar->addWidget(saveTln);
    ui->headToolBar->addWidget(saveSvgTln);

    // ui->headToolBar->addAction(ui->actAddLine);
    // ui->headToolBar->addAction(ui->actAddRect);
    // ui->headToolBar->addAction(ui->actAddEll);
    // ui->headToolBar->addAction(ui->actAddText);
    // ui->headToolBar->addAction(ui->actAddRoundRect);
 //    ui->headToolBar->addAction(ui->actAddTri);
 //    ui->headToolBar->addAction(ui->actAddRhom);
 //    ui->headToolBar->addAction(ui->actAddTrap);
 //    ui->headToolBar->addAction(ui->actAddPargram);
 //    ui->headToolBar->addAction(ui->actAddDoc);
 //    ui->headToolBar->addAction(ui->actAddPolyLine);
}

void MainWindow::bindAction()
{
	connect(ui->actCheck,SIGNAL(triggered(bool)),this,SLOT(check()));
	connect(ui->actAboutUs,SIGNAL(triggered(bool)),this,SLOT(showAboutUsWindow()));
	connect(ui->actRedo,SIGNAL(triggered(bool)), this, SLOT(redo()));
	connect(ui->actUndo,SIGNAL(triggered(bool)),this, SLOT(undo()));

	connect(ui->actSaveFile,SIGNAL(triggered(bool)), this, SLOT(saveFile()));
	connect(ui->actOpenFile,SIGNAL(triggered(bool)), this, SLOT(loadFile()));
	connect(ui->actSvgFile, SIGNAL(triggered(bool)), this, SLOT(saveAsSvg()));

	connect(ui->actCopy,SIGNAL(triggered(bool)), this, SLOT(copy()));
	connect(ui->actPaste,SIGNAL(triggered(bool)), this, SLOT(paste()));

	connect(ui->actAddLine, SIGNAL(triggered(bool)), this, SLOT(addLine()));
	connect(ui->actAddRect, SIGNAL(triggered(bool)), this, SLOT(addRect()));
	connect(ui->actAddRoundRect, SIGNAL(triggered(bool)), this, SLOT(addRoundRect()));
	connect(ui->actAddEll, SIGNAL(triggered(bool)), this, SLOT(addEll()));
	connect(ui->actAddText, SIGNAL(triggered(bool)), this, SLOT(addText()));
    connect(ui->actAddTri, SIGNAL(triggered(bool)), this, SLOT(addTri()));
    connect(ui->actAddRhom, SIGNAL(triggered(bool)), this, SLOT(addDia()));
    connect(ui->actAddTrap, SIGNAL(triggered(bool)), this, SLOT(addTrap()));
	connect(ui->actAddEnd, SIGNAL(triggered(bool)), this, SLOT(addDFStartEndItem()));
	connect(ui->actAddPrede, SIGNAL(triggered(bool)), this, SLOT(addDFPredefineItem()));
	connect(ui->actAddPargram, SIGNAL(triggered(bool)), this, SLOT(addParagram()));
	connect(ui->actAddManualinput, SIGNAL(triggered(bool)), this, SLOT(addDFManualInputItem()));
	connect(ui->actAddDoc, SIGNAL(triggered(bool)), this, SLOT(addDFDocItem()));
    connect(ui->actAddPolyLine, SIGNAL(triggered(bool)), this, SLOT(addPolyLine()));

    connect(ui->actStyleSheet, &QAction::triggered, this, [this]() {
        rightTab->setVisible(true);
    });

    connect(ui->actSelectFillCol, SIGNAL(triggered(bool)), this, SLOT(selectFillCol()));
    connect(ui->actSelectFrameCol, SIGNAL(triggered(bool)), this, SLOT(selectFrameCol()));
    connect(ui->actSelectTextCol, SIGNAL(triggered(bool)), this, SLOT(selectTextCol()));
    connect(ui->actSelectTextFont, SIGNAL(triggered(bool)), this, SLOT(selectTextFont()));

    connect(ui->actMoveSelectedZUp,SIGNAL(triggered(bool)), this, SLOT(moveSelectedZUp()));
    connect(ui->actMoveSelectedZDown,SIGNAL(triggered(bool)),this, SLOT(moveSelectedZDown()));

    connect(ui->actMoveSelectedMaxZUp,SIGNAL(triggered(bool)),this,SLOT(moveSelectedMaxZUp()));
    connect(ui->actMoveSelectedMaxZDown,SIGNAL(triggered(bool)),this,SLOT(moveSelectedMaxZDown()));

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
	// connect(ui->actCombine, SIGNAL(triggered(bool)), this, SLOT(combineSelected()));
 //    connect(ui->actSeperate, SIGNAL(triggered(bool)), this, SLOT(seperateSelected()));

    QShortcut *delshorcut = new QShortcut(QKeySequence("Delete"), this);
    // QShortcut *combinesc = new QShortcut(QKeySequence("ctrl+G"), this);
    // QShortcut *seperatesc = new QShortcut(QKeySequence("ctrl+shift+G"), this);

    connect(delshorcut, SIGNAL(activated()), this, SLOT(delSelectedItem()));
	// connect(combinesc, SIGNAL(activated()), this, SLOT(combineSelected()));
	// connect(seperatesc, SIGNAL(activated()), this, SLOT(seperateSelected()));


    // connect(createTln, &QToolButton::clicked, this, &MainWindow::)
    connect(openTln, &QToolButton::clicked, this, &MainWindow::loadFile);
    connect(saveTln, &QToolButton::clicked, this, &MainWindow::saveFile);
    connect(saveSvgTln, &QToolButton::clicked, this, &MainWindow::saveAsSvg);
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

void MainWindow::changeLineType(Qt::PenStyle linestyle)
{
    scene->changeLineType(linestyle);
}

void MainWindow::changeEndArrow(int endArrowType)
{
    scene->changeEndArrow(endArrowType);
}

void MainWindow::changeLineColor(QColor color)
{
    scene->changeLineColor(color);
}

void MainWindow::setSceneBg(QString path)
{
    scene->setBg(path);
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
    QColor color = colorDia->getColor(Qt::white, this, "颜色选择器", QColorDialog::ShowAlphaChannel);
    for(QGraphicsItem *item : items) {
        DLineItem *line = dynamic_cast<DLineItem *>(item);
        if(line != nullptr) {
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
    QColor color = colorDia->getColor(Qt::white, this, "颜色选择器", QColorDialog::ShowAlphaChannel);
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
    QColor color = colorDia->getColor(Qt::white, this, "颜色选择器", QColorDialog::ShowAlphaChannel);
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
	if(scene->selectedItems().isEmpty()) view->rotateCW();
	else scene->rotateCW();
}

void MainWindow::rotateCCW()
{
	if(scene->selectedItems().isEmpty()) view->rotateCCW();
	else scene->rotateCCW();
}

void MainWindow::enlarge()
{
	if(scene->selectedItems().isEmpty()) view->enlarge();
	else scene->enlarge();
}

void MainWindow::shrink()
{
	if(scene->selectedItems().isEmpty()) view->shrink();
	else scene->shrink();
}

void MainWindow::moveLeft()
{
	if(scene->selectedItems().isEmpty()) view->moveLeft();
	else scene->moveLeft();
}

void MainWindow::moveRight()
{
	if(scene->selectedItems().isEmpty()) view->moveRight();
	else scene->moveRight();
}

void MainWindow::moveUp()
{
	if(scene->selectedItems().isEmpty()) view->moveUp();
	else scene->moveUp();
}

void MainWindow::moveDown()
{
	if(scene->selectedItems().isEmpty()) view->moveDown();
	else scene->moveDown();
}

void MainWindow::moveSelectedZUp(){
    if(scene->selectedItems().isEmpty()) return;
    else scene->moveSelectedZUp(1);
}

void MainWindow::moveSelectedZDown(){
    if(scene->selectedItems().isEmpty()) return;
    else scene->moveSelectedZDown(-1);
}

void MainWindow::moveSelectedMaxZUp(){
    if(scene->selectedItems().isEmpty()) return;
    else scene->moveSelectedZMaxUp();
}

void MainWindow::moveSelectedMaxZDown(){
    if(scene->selectedItems().isEmpty()) return;
    else scene->moveSelectedZMaxDown();
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

// void MainWindow::combineSelected()
// {
// 	scene->combineSelected();
// }

// void MainWindow::seperateSelected()
// {
// 	scene->seperateSelected();
// }

void MainWindow::saveFile(){
    if(filePath == nullptr || filePath == "")
        filePath = QFileDialog::getSaveFileName(this, tr("保存.bit文件"),"./",tr("(*.bit)"));
    if(filePath == "") return;
    QList<QGraphicsItem *> items = scene->selectedItems();
    for(QGraphicsItem *item : items) {
        item->setSelected(false);
    }
    SaveAndLoadManager::instance().saveToFile(filePath);
}

void MainWindow::loadFile(){
    QString filePath = QFileDialog::getOpenFileName(this, tr("打开.bit文件"),"./",tr("(*.bit)"));
    if(filePath == "") return;

    SaveAndLoadManager::instance().loadFromFile(filePath);
}

void MainWindow::copy(){
    scene->copySelectedItems();
}

void MainWindow::paste(){
    scene->pasteItems();
}

void MainWindow::redo(){
    UndoManager::instance().redo();
}
void MainWindow::undo(){
    UndoManager::instance().undo();
}

void MainWindow::showAboutUsWindow(){
    AboutUsWindow* auw = new AboutUsWindow();
    auw->exec();
}

void MainWindow::check(){
    inspector->check();
    inspector->show();
}
