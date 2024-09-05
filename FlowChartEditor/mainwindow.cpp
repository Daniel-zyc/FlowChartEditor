#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSvgGenerator>
#include <QPainter>
#include <QFileDialog>
#include "dclass/line/dlineitem.h"
#include "dclass/base/dshapebase.h"

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

	QFile qssfile(":/stylesheet.qss");
	if(qssfile.open(QFile::ReadOnly)) {
		QTextStream textstream(&qssfile);
		QString stylesheet = textstream.readAll();
		setStyleSheet(stylesheet);
		qssfile.close();
	}

	findDia = new DFindDialog();
	colorDia = new QColorDialog(Qt::blue, this);
	fontDia = new QFontDialog(this);
	fontDia->setOption(QFontDialog::ScalableFonts);
	fontDia->setOption(QFontDialog::ProportionalFonts);

	// 初始化 view 和 scene
	createStatusBar();
	scene = new DScene(this);
	view = new DView(scene, this);

	scene->setMenu(createSceneMenu());
	scene->setView(view);
	view->setViewLabel(labelViewCord);
	view->setSceneLabel(labelSceneCord);

	UndoManager::instance().bindScene(scene);
	SaveAndLoadManager::instance().bindScene(scene);
	inspector = Inspector::instance(this,scene,view);
	inspector->hide();

	initUi();

	createMenu();
	createToolBar();

	bindAction();
	connectLeft();
	connectRight();
}

QMenu* MainWindow::createSceneMenu()
{
	QMenu *m = new QMenu();
	m->addAction(ui->actDelSelectedItem);
	m->addSeparator();
	m->addAction(ui->actSelectFrameCol);
	m->addAction(ui->actSelectFillCol);
	m->addAction(ui->actSelectTextCol);
	m->addAction(ui->actSelectTextFont);
	m->addAction(ui->actStyleSheet);
	m->addSeparator();
	m->addAction(ui->actMoveSelectedZUp);
	m->addAction(ui->actMoveSelectedZDown);
	m->addAction(ui->actMoveSelectedMaxZUp);
	m->addAction(ui->actMoveSelectedMaxZDown);
	return m;
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::initUi()
{
	initleftUi();
	initmiddleUi();
	initrightUi();

	mainsplitter->addWidget(leftw);
	mainsplitter->addWidget(middlesplitter);
	mainsplitter->setStretchFactor(1, 1);
	mainsplitter->addWidget(rightTab);

	setCentralWidget(mainsplitter);
}

void MainWindow::initmiddleUi(){
	middlesplitter = new QSplitter(Qt::Vertical,this);
	middlesplitter->addWidget(view);

	middlesplitter->addWidget(inspector);
	middlesplitter->setStretchFactor(0, 3);
	middlesplitter->setStretchFactor(1, 1);
}

void MainWindow::initrightUi()
{
	//样式表
	rightTab = new QTabWidget();
	rightTab->setMovable(true);
	rightTab->setFixedWidth(260);

	//背景样式表
	rightBgw = new QWidget();
	rightBgf = new QFormLayout();
	blankBg = new QRadioButton("无背景");
	colorBg = new QRadioButton("纯色填充");
	gridBg = new QRadioButton("网格");
	dotBg = new QRadioButton("点状");
	reColorBtn = new QPushButton("选择颜色");
	reFileBtn = new QPushButton("选择文件");

	blankBg->setChecked(true);
	customizeBg = new QRadioButton("自定义图片填充");
	rightBgf->addRow(blankBg);
	rightBgf->addRow(colorBg, reColorBtn);
	rightBgf->addRow(gridBg);
	rightBgf->addRow(dotBg);
	rightBgf->addRow(customizeBg, reFileBtn);

	rightBgw->setLayout(rightBgf);
	rightTab->addTab(rightBgw, "背景");

    //形状样式表
    rightShapew = new QTreeWidget();
    rightShapew->setColumnCount(2);
    rightShapew->setHeaderHidden(true);
    rightShapew->setColumnWidth(0, 140);
    rightShapew->setSelectionMode(QAbstractItemView::NoSelection);
    borderTop = new QTreeWidgetItem();
    fillTop = new QTreeWidgetItem();
    rotTop = new QTreeWidgetItem();
    scaleTop = new QTreeWidgetItem();
    borderChildColor = new QTreeWidgetItem();
    borderChildType = new QTreeWidgetItem();
    borderChildWidth = new QTreeWidgetItem();
    fillChildColor = new QTreeWidgetItem();
    fillChildType = new QTreeWidgetItem();
    fillChildPic = new QTreeWidgetItem();
    lineTop = new QTreeWidgetItem();
    lineChildColor = new QTreeWidgetItem();
    lineChildType = new QTreeWidgetItem();
    lineChildBeginArrow = new QTreeWidgetItem();
    lineChildEndArrow = new QTreeWidgetItem();
    lineChildWidth = new QTreeWidgetItem();

    borderTop->setText(0, "边框");
    fillTop->setText(0, "填充");
    scaleTop->setText(0, "缩放比例：");
    rotTop->setText(0, "旋转角度：");
    lineTop->setText(0, "线条");
    borderChildColor->setText(0, "边框颜色：");
    borderChildType->setText(0, "边框样式：");
    borderChildWidth->setText(0, "边框宽度：");
    fillChildColor->setText(0, "填充颜色");
    fillChildType->setText(0, "填充样式");
    lineChildColor->setText(0, "线条颜色");
    lineChildType->setText(0, "线条类型");
    lineChildBeginArrow->setText(0, "开始箭头");
    lineChildEndArrow->setText(0, "结束箭头");
    lineChildWidth->setText(0, "线条磅数");

	rightShapew->addTopLevelItem(borderTop);
	rightShapew->addTopLevelItem(fillTop);
	rightShapew->addTopLevelItem(rotTop);
	rightShapew->addTopLevelItem(scaleTop);
    rightShapew->addTopLevelItem(lineTop);

	borderTop->addChild(borderChildColor);
	borderTop->addChild(borderChildType);
	borderTop->addChild(borderChildWidth);
	fillTop->addChild(fillChildColor);
	fillTop->addChild(fillChildType);
	fillTop->addChild(fillChildPic);
    lineTop->addChild(lineChildColor);
    lineTop->addChild(lineChildType);
    lineTop->addChild(lineChildBeginArrow);
    lineTop->addChild(lineChildEndArrow);
    lineTop->addChild(lineChildWidth);

	rotationBox = new QSpinBox();
	scaleBox = new QSpinBox();
	borderColor = new QPushButton();
	borderStyle = new QComboBox();
	borderWidth = new QDoubleSpinBox();
	fillColor = new QPushButton();
	fillType = new QComboBox();
	customizePic = new QCheckBox("自定义图片");
	customizePic->setChecked(false);
	picfile = new QPushButton("选择文件");
    linecolor  = new QPushButton();
    lineType = new QComboBox();
    endarrowType = new QComboBox();
    linebound = new QDoubleSpinBox();
    beginarrowType = new QComboBox();

    setColorIcon(linecolor);

    lineType->addItem(QIcon(":/icon/solidLine.png"), "实线");
    lineType->addItem(QIcon(":/icon/dashLine.png"), "短划线");
    lineType->addItem(QIcon(":/icon/dotLine.png"), "点线");
    lineType->addItem(QIcon(":/icon/dashDotLine.png"), "点划线");
    lineType->addItem(QIcon(":/icon/dashDDLine.png"), "双点划线");

    endarrowType->addItem(QIcon(":/icon/noArrow.png"), "无箭头");
    endarrowType->addItem(QIcon(":/icon/arrow.png"), "箭头");
    endarrowType->addItem(QIcon(":/icon/openArrow.png"), "开放型箭头");
    endarrowType->addItem(QIcon(":/icon/dovetailArrow.png"), "燕尾箭头");
    endarrowType->addItem(QIcon(":/icon/diaArrow.png"), "菱形箭头");
    endarrowType->addItem(QIcon(":/icon/roundArrow.png"), "圆型箭头");

    beginarrowType->addItem(QIcon(":/icon/noArrow.png"), "无箭头");
    beginarrowType->addItem(QIcon(":/icon/arrow.png"), "箭头");
    beginarrowType->addItem(QIcon(":/icon/openArrow.png"), "开放型箭头");
    beginarrowType->addItem(QIcon(":/icon/dovetailArrow.png"), "燕尾箭头");
    beginarrowType->addItem(QIcon(":/icon/diaArrow.png"), "菱形箭头");
    beginarrowType->addItem(QIcon(":/icon/roundArrow.png"), "圆型箭头");

    linebound->setRange(0.01, 30);
    linebound->setSingleStep(0.25);
    linebound->setValue(1);
    linebound->setSuffix("磅");
    linebound->setWrapping(true);

	rotationBox->setFixedHeight(25);
	scaleBox->setFixedHeight(25);
	borderColor->setFixedHeight(25);
	borderStyle->setFixedHeight(25);
	borderWidth->setFixedHeight(25);
	fillColor->setFixedHeight(25);
	fillType->setFixedHeight(25);
    picfile->setFixedHeight(25);
    lineType->setFixedHeight(25);
    endarrowType->setFixedHeight(25);
    beginarrowType->setFixedHeight(25);
    linebound->setFixedHeight(25);
    linecolor->setFixedHeight(25);

	borderStyle->addItem(QIcon(":/icon/solidLine.png"), "实线");
	borderStyle->addItem(QIcon(":/icon/dashLine.png"), "短划线");
	borderStyle->addItem(QIcon(":/icon/dotLine.png"), "点线");
	borderStyle->addItem(QIcon(":/icon/dashDotLine.png"), "点划线");
	borderStyle->addItem(QIcon(":/icon/dashDDLine.png"), "双点划线");

	borderWidth->setRange(0, 30);
	borderWidth->setSingleStep(0.25);
	borderWidth->setValue(1);
	borderWidth->setSuffix("磅");
	borderWidth->setWrapping(true);

	fillType->addItem("无图案");
	fillType->addItem("实心图案");
	fillType->addItem("密实图案1");
	fillType->addItem("密实图案2");
	fillType->addItem("密实图案3");
	fillType->addItem("密实图案4");
	fillType->addItem("密实图案5");
	fillType->addItem("密实图案6");
	fillType->addItem("密实图案7");
	fillType->addItem("水平线图案");
	fillType->addItem("垂直线图案");
	fillType->addItem("十字线图案");
	fillType->addItem("左斜线图案");
	fillType->addItem("右倾线图案");
	fillType->addItem("倾斜十字线图案");
    fillType->setCurrentIndex(1);
	// fillType->addItem("线性渐变图案");
	// fillType->addItem("径向渐变图案");
	// fillType->addItem("圆锥渐变图案");
	// fillType->addItem("纹理图案");

	rotationBox->setRange(0, 360);
	rotationBox->setSingleStep(1);
	rotationBox->setSuffix("°");
	rotationBox->setValue(0);
	rotationBox->setWrapping(true);
    scaleBox->setRange(1, 100000);
	scaleBox->setSingleStep(1);
	scaleBox->setValue(100);
	scaleBox->setSuffix("%");
	scaleBox->setWrapping(true);

	setColorIcon(borderColor);
	setColorIcon(fillColor);

	rightShapew->setItemWidget(fillChildPic, 0, customizePic);
	rightShapew->setItemWidget(fillChildPic, 1, picfile);
    rightShapew->setItemWidget(borderChildColor, 1, borderColor);
	rightShapew->setItemWidget(borderChildType, 1, borderStyle);
	rightShapew->setItemWidget(borderChildWidth, 1, borderWidth);
	rightShapew->setItemWidget(fillChildColor, 1, fillColor);
	rightShapew->setItemWidget(fillChildType, 1, fillType);
	rightShapew->setItemWidget(rotTop, 1, rotationBox);
	rightShapew->setItemWidget(scaleTop, 1, scaleBox);
    rightShapew->setItemWidget(lineChildColor, 1, linecolor);
    rightShapew->setItemWidget(lineChildType, 1, lineType);
    rightShapew->setItemWidget(lineChildEndArrow, 1, endarrowType);
    rightShapew->setItemWidget(lineChildBeginArrow, 1, beginarrowType);
    rightShapew->setItemWidget(lineChildWidth, 1, linebound);

	borderTop->setExpanded(true);
    fillTop->setExpanded(true);
    lineTop->setExpanded(true);

	rightTab->addTab(rightShapew, "形状");

    //样式样式表
    rightStylew = new QWidget();
    rightStylef = new QFormLayout();
    QHBoxLayout *alignH = new QHBoxLayout();
    QHBoxLayout *layerH = new QHBoxLayout();
    alignBox = new QComboBox();
    layerBox = new QComboBox();

    alignBox->addItem("左对齐");
    alignBox->addItem("水平居中");
    alignBox->addItem("右对齐");
    alignBox->addItem("顶端对齐");
    alignBox->addItem("垂直居中");
    alignBox->addItem("底端对齐");
    alignBox->addItem("横向中轴均匀分布");
    alignBox->addItem("纵向中轴均匀分布");

    layerBox->addItem("上移一层");
    layerBox->addItem("置于顶层");
    layerBox->addItem("下移一层");
    layerBox->addItem("置于底层");

    alignH->addStretch();
    alignH->addWidget(alignBox);
    layerH->addStretch();
    layerH->addWidget(layerBox);
    rightStylef->addRow("对齐：", alignH);
    rightStylef->addRow("层次：", layerH);
    rightStylew->setLayout(rightStylef);
    rightTab->addTab(rightStylew, "样式");

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

	// rightw->setVisible(false);
    formworkWidget = new FormworkWidget(this,scene,view);
	rightTab->addTab(formworkWidget,"模板");
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

	curveBtn = new QPushButton();
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
	aggreconnectBtn = new QPushButton();
	cardBtn = new QPushButton();
	compareBtn = new QPushButton();
	dataBtn = new QPushButton();
	directaccessBtn = new QPushButton();
	diskBtn = new QPushButton();
	displayBtn = new QPushButton();
	manulinputBtn = new QPushButton();
	mergeBtn = new QPushButton();
	multidocBtn = new QPushButton();
	offpageBtn = new QPushButton();
	orBtn = new QPushButton();
	postponeBtn = new QPushButton();
	sequentialaccessBtn = new QPushButton();
	storedataBtn = new QPushButton();
	sortBtn = new QPushButton();

	addRectBtn = new QPushButton();
	addEllBtn = new QPushButton();
	addParagramBtn = new QPushButton();
	addTrapBtn = new QPushButton();
	addDiaBtn = new QPushButton();
	addTriBtn = new QPushButton();
	addPenBtn = new QPushButton();
	addHexBtn = new QPushButton();

	curveBtn->setIcon(QPixmap(":/icon/curve.png"));
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
	aggreconnectBtn->setIcon(QPixmap(":/icon/flowchart/aggreconnect.png"));
	cardBtn->setIcon(QPixmap(":/icon/flowchart/card.png"));
	compareBtn->setIcon(QPixmap(":/icon/flowchart/compare.png"));
	dataBtn->setIcon(QPixmap(":/icon/flowchart/data.png"));
	directaccessBtn->setIcon(QPixmap(":/icon/flowchart/directaccess.png"));
	diskBtn->setIcon(QPixmap(":/icon/flowchart/disk.png"));
	displayBtn->setIcon(QPixmap(":/icon/flowchart/display.png"));
	manulinputBtn->setIcon(QPixmap(":/icon/flowchart/manulinput.png"));
	mergeBtn->setIcon(QPixmap(":/icon/flowchart/merge.png"));
	multidocBtn->setIcon(QPixmap(":/icon/flowchart/multidoc.png"));
	offpageBtn->setIcon(QPixmap(":/icon/flowchart/offpage.png"));
	orBtn->setIcon(QPixmap(":/icon/flowchart/or.png"));
	postponeBtn->setIcon(QPixmap(":/icon/flowchart/postpone.png"));
	sequentialaccessBtn->setIcon(QPixmap(":/icon/flowchart/sequentialaccess.png"));
	storedataBtn->setIcon(QPixmap(":/icon/flowchart/storedata.png"));
	sortBtn->setIcon(QPixmap(":/icon/flowchart/sort.png"));

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
    aggreconnectBtn->setToolTip("汇总连接");
    cardBtn->setToolTip("卡片");
    compareBtn->setToolTip("对照");
    dataBtn->setToolTip("资料带");
    directaccessBtn->setToolTip("直接访问存储器");
    diskBtn->setToolTip("磁盘");
    displayBtn->setToolTip("显示");
    manulinputBtn->setToolTip("手动输入");
    mergeBtn->setToolTip("合并");
    multidocBtn->setToolTip("多文件");
    offpageBtn->setToolTip("离页连接符");
    orBtn->setToolTip("或者");
    postponeBtn->setToolTip("延期");
    sequentialaccessBtn->setToolTip("顺序访问存储器");
    storedataBtn->setToolTip("存储数据");
    sortBtn->setToolTip("排序");

	addRectBtn->setToolTip("矩形");
	addEllBtn->setToolTip("椭圆");
	addParagramBtn->setToolTip("平行四边形");
	addTrapBtn->setToolTip("梯形");
	addDiaBtn->setToolTip("菱形");
	addTriBtn->setToolTip("三角形");
    addPenBtn->setToolTip("五边形");
    addHexBtn->setToolTip("六边形");

	addRectBtn->setIcon(QPixmap(":/icon/primary/rect.png"));
	addEllBtn->setIcon(QPixmap(":/icon/primary/ellipse.png"));
	addParagramBtn->setIcon(QPixmap(":/icon/primary/parallgram.png"));
	addTrapBtn->setIcon(QPixmap(":/icon/primary/trapezoid.png"));
	addDiaBtn->setIcon(QPixmap(":/icon/primary/rhomb.png"));
	addTriBtn->setIcon(QPixmap(":/icon/primary/triangle.png"));
	addPenBtn->setIcon(QPixmap(":/icon/primary/pen.png"));
	addHexBtn->setIcon(QPixmap(":/icon/primary/hex.png"));

	flowGrid->addWidget(rectBtn, 0, 0);
	flowGrid->addWidget(roundRectBtn, 0, 1);
	flowGrid->addWidget(ellipseBtn, 0, 2);
	flowGrid->addWidget(parellgramBtn, 0, 3);
	flowGrid->addWidget(trapBtn, 1, 0);
	flowGrid->addWidget(rhomBtn, 1, 1);
	flowGrid->addWidget(fileBtn, 1, 2);
	flowGrid->addWidget(triBtn, 1, 3);
	flowGrid->addWidget(preBtn, 2, 0);
	flowGrid->addWidget(endBtn, 2, 1);
	flowGrid->addWidget(prepareBtn, 2, 2);
	flowGrid->addWidget(storeBtn, 2, 3);
	flowGrid->addWidget(aggreconnectBtn, 3, 0);
	flowGrid->addWidget(cardBtn, 3, 1);
	flowGrid->addWidget(compareBtn, 3, 2);
	flowGrid->addWidget(dataBtn, 3, 3);
	flowGrid->addWidget(directaccessBtn, 4, 0);
	flowGrid->addWidget(diskBtn, 4, 1);
	flowGrid->addWidget(displayBtn, 4, 2);
	flowGrid->addWidget(manulinputBtn, 4, 3);
	flowGrid->addWidget(mergeBtn, 5, 0);
	flowGrid->addWidget(multidocBtn, 5, 1);
	flowGrid->addWidget(offpageBtn, 5, 2);
	flowGrid->addWidget(orBtn, 5, 3);
	flowGrid->addWidget(postponeBtn, 6, 0);
	flowGrid->addWidget(sequentialaccessBtn, 6, 1);
	flowGrid->addWidget(storedataBtn, 6, 2);
	flowGrid->addWidget(sortBtn, 6, 3);

	primaryGrid->addWidget(addRectBtn, 0, 0);
	primaryGrid->addWidget(addEllBtn, 0, 1);
	primaryGrid->addWidget(addParagramBtn, 0, 2);
	primaryGrid->addWidget(addTrapBtn, 0, 3);
	primaryGrid->addWidget(addDiaBtn, 1, 0);
	primaryGrid->addWidget(addTriBtn, 1, 1);
	primaryGrid->addWidget(addPenBtn, 1, 2);
	primaryGrid->addWidget(addHexBtn, 1, 3);

	primaryGroup->setLayout(primaryGrid);
	flowcGroup->setLayout(flowGrid);
	leftUpV->addWidget(primaryGroup);
	leftUpV->addWidget(flowcGroup);
	leftw->setLayout(leftUpV);

	lineGrid = new QGridLayout();
	lineGrid->addWidget(lineBtn, 0, 0);
	lineGrid->addWidget(polyLineBtn, 0, 1);
	lineGrid->addWidget(curveBtn, 0, 2);
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

	mainsplitter->addWidget(leftw);

	mainsplitter->setStretchFactor(1, 1);
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
	connect(aggreconnectBtn,  &QPushButton::clicked, this, &MainWindow::addDFSummaryconnItem);
	connect(cardBtn,  &QPushButton::clicked, this, &MainWindow::addDFCardItem);
	connect(compareBtn,  &QPushButton::clicked, this, &MainWindow::addDFCompareItem);
    connect(dataBtn,  &QPushButton::clicked, this, &MainWindow::addDFInformationItem);
	connect(directaccessBtn,  &QPushButton::clicked, this, &MainWindow::addDFDirecrAccessItem);
	connect(diskBtn,  &QPushButton::clicked, this, &MainWindow::addDFDiskItem);
	connect(displayBtn,  &QPushButton::clicked, this, &MainWindow::addDFDisplayItem);
	connect(manulinputBtn,  &QPushButton::clicked, this, &MainWindow::addDFManualinputItem);
	connect(mergeBtn,  &QPushButton::clicked, this, &MainWindow::addDFMergeItem);
	connect(multidocBtn,  &QPushButton::clicked, this, &MainWindow::addDFMultiDocItem);
	connect(offpageBtn,  &QPushButton::clicked, this, &MainWindow::addDFOffPageItem);
	connect(orBtn,  &QPushButton::clicked, this, &MainWindow::addDFOrItem);
	connect(postponeBtn,  &QPushButton::clicked, this, &MainWindow::addDFDelayItem);
	connect(sequentialaccessBtn,  &QPushButton::clicked, this, &MainWindow::addDFSequentialAccessItem);
	connect(storedataBtn, &QPushButton::clicked, this, &MainWindow::addDFStoreDataItem);
	connect(curveBtn, &QPushButton::clicked, this, &MainWindow::addCurveLine);
	connect(addPenBtn, &QPushButton::clicked, this, &MainWindow::addpentagon);
	connect(addHexBtn, &QPushButton::clicked, this, &MainWindow::addhexagon);
	connect(sortBtn, &QPushButton::clicked, this, &MainWindow::addDFSortItem);
}

void MainWindow::connectRight()
{
	connect(blankBg, &QRadioButton::toggled, this, [this](bool checked) {
		if(checked) setSceneBg(":/icon/blankBg.png");
	});
	connect(reColorBtn, &QPushButton::clicked, this, [this]() {
		if(colorBg->isChecked()) {
			QColor color = QColorDialog::getColor(Qt::white, this, "颜色选择器", QColorDialog::ShowAlphaChannel);
			if(color.isValid()) scene->setBackgroundBrush(QBrush(color));
			else blankBg->setChecked(true);
		}
	});
	connect(gridBg, &QRadioButton::toggled, this, [this](bool checked) {
		if(checked) setSceneBg(":/icon/gridBg.png");
	});
	connect(dotBg, &QRadioButton::toggled, this, [this](bool checked) {
		if(checked) setSceneBg(":/icon/dotBg.png");
	});
	connect(reFileBtn, &QPushButton::clicked, this, [this]() {
		if(customizeBg->isChecked()) {
            QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "JPEG Files(*.jpg *.jpeg);;PNG Files(*.png);;SVG Files(*.svg);;BMP Files(*.bmp)");
			if(!fileName.isEmpty()) {
				setSceneBg(fileName);
			}else {
				blankBg->setChecked(true);
			}
		}
	});
    connect(lineType, &QComboBox::activated, this, [this]() {
		int penstyle = lineType->currentIndex();
		// qDebug() << "penstyle;" << penstyle;
		switch(penstyle) {
		case 0: changeLineType(Qt::SolidLine); break;
		case 1: changeLineType(Qt::DashLine); break;
		case 2: changeLineType(Qt::DotLine); break;
		case 3: changeLineType(Qt::DashDotLine); break;
		case 4: changeLineType(Qt::DashDotDotLine); break;
		}
	});
    connect(endarrowType, &QComboBox::activated, this, &MainWindow::changeEndArrow);
    connect(beginarrowType, &QComboBox::activated, this, &MainWindow::changeBeginArrow);
	connect(linebound, &QDoubleSpinBox::valueChanged, this, [this]() {
		scene->changeLineWidth(linebound->value() * globalScale);
	});
	connect(linecolor, &QPushButton::clicked, this, [this](){
		QColor color = QColorDialog::getColor(Qt::white, this, "颜色选择器", QColorDialog::ShowAlphaChannel);
        if(color.isValid()) scene->changeLineColor(color);
	});
	connect(borderColor, &QPushButton::clicked, this, &MainWindow::changeBorderColor);
	connect(fillColor, &QPushButton::clicked, this, &MainWindow::changeFillColor);
	connect(textColor, &QPushButton::clicked, this, &MainWindow::changeTextCol);
	connect(textFont, &QPushButton::clicked, this, &MainWindow::changeTextFont);
	connect(rotationBox, &QSpinBox::valueChanged, this, &MainWindow::changeItemRot);
	connect(scaleBox, &QSpinBox::valueChanged, this, &MainWindow::changeItemScale);
    connect(borderStyle, &QComboBox::activated, this, &MainWindow::changeBorderType);
	connect(borderWidth, &QDoubleSpinBox::valueChanged, this, &MainWindow::changeBorderWidth);
    connect(fillType, &QComboBox::activated, this, &MainWindow::changeFillType);
	connect(picfile, &QPushButton::clicked, this, [this]() {
		if(customizePic->isChecked()) {
            QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "JPEG Files(*.jpg *.jpeg);;PNG Files(*.png);;SVG Files(*.svg);;BMP Files(*.bmp)");
            if(!fileName.isEmpty()) {
                QPixmap pixmap(fileName);
				scene->changeFillPic(pixmap);
			}
		}
	});
	connect(customizePic, &QCheckBox::checkStateChanged, this, [this]() {
		if(!customizePic->isChecked()) {
			scene->changeFillColor(Qt::white);
		}
	});
    connect(alignBox, &QComboBox::activated, this, &MainWindow::changeAlign);
    connect(layerBox, &QComboBox::activated, this, &MainWindow::changeLayer);
}

void MainWindow::createMenu()
{
	QMenu *lineType = new QMenu("线条");

    QMenu *arrowTypeEM = new QMenu("结尾箭头");
    arrowTypeEM->addAction(ui->actNoArrowE);
    arrowTypeEM->addAction(ui->actArrowE);
    arrowTypeEM->addAction(ui->actOpenArrowE);
    arrowTypeEM->addAction(ui->actDovetailArrowE);
    arrowTypeEM->addAction(ui->actDiaArrowE);
    arrowTypeEM->addAction(ui->actRoundArrowE);
    QActionGroup *arrowEGroup = new QActionGroup(lineType);
    arrowEGroup->setExclusive(true);
    arrowEGroup->addAction(ui->actNoArrowE);
    arrowEGroup->addAction(ui->actArrowE);
    arrowEGroup->addAction(ui->actOpenArrowE);
    arrowEGroup->addAction(ui->actDovetailArrowE);
    arrowEGroup->addAction(ui->actDiaArrowE);
    arrowEGroup->addAction(ui->actRoundArrowE);

    QMenu *arrowTypeBM = new QMenu("起始箭头");
    arrowTypeBM->addAction(ui->actNoArrowB);
    arrowTypeBM->addAction(ui->actArrowB);
    arrowTypeBM->addAction(ui->actOpenArrowB);
    arrowTypeBM->addAction(ui->actDovetailArrowB);
    arrowTypeBM->addAction(ui->actDiaArrowB);
    arrowTypeBM->addAction(ui->actRoundArrowB);
    QActionGroup *arrowBGroup = new QActionGroup(lineType);
    arrowBGroup->setExclusive(true);
    arrowBGroup->addAction(ui->actNoArrowB);
    arrowBGroup->addAction(ui->actArrowB);
    arrowBGroup->addAction(ui->actOpenArrowB);
    arrowBGroup->addAction(ui->actDovetailArrowB);
    arrowBGroup->addAction(ui->actDiaArrowB);
    arrowBGroup->addAction(ui->actRoundArrowB);

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

    lineType->addMenu(lineTypeM);
    lineType->addMenu(arrowTypeBM);
    lineType->addMenu(arrowTypeEM);
	// ui->styleMenu->addMenu(lineType);
	scene->getMenu()->addMenu(lineType);

	ui->fileMenu->addAction(ui->actNewFile);
	ui->fileMenu->addAction(ui->actOpenFile);
	ui->fileMenu->addAction(ui->actSaveFile);
    ui->fileMenu->addAction(ui->actSaveNewFile);
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
    // ui->helpMenu->addAction(ui->actCheck);
	ui->helpMenu->addAction(ui->actDebug);
}

void MainWindow::createToolBar()
{
	saveSvgTln = new QToolButton();
	saveSvgTln->addAction(ui->actSvgFile);
	saveSvgTln->setIcon(QIcon(":/icon/savesvg.png"));
	saveSvgTln->setToolTip("导出Svg格式");

	ui->headToolBar->addAction(ui->actNewFile);
	ui->headToolBar->addAction(ui->actOpenFile);
	ui->headToolBar->addAction(ui->actSaveFile);
    // ui->headToolBar->addAction(ui->actSaveNewFile);
	ui->headToolBar->addWidget(saveSvgTln);
	ui->headToolBar->addSeparator();

	ui->headToolBar->addAction(ui->actEnlarge);
	ui->headToolBar->addAction(ui->actShrink);
	ui->headToolBar->addSeparator();

	ui->headToolBar->addAction(ui->actUndo);
	ui->headToolBar->addAction(ui->actRedo);
    ui->headToolBar->addSeparator();

    isInspect = new QCheckBox("开启检查");
    isOpenReference = new QCheckBox("开启参照");
    isOpenReference->setCheckState(Qt::Checked);
    ui->headToolBar->addWidget(isInspect);
    ui->headToolBar->addWidget(isOpenReference);

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

void MainWindow::createStatusBar()
{
	QLabel *lab1 = new QLabel("视口坐标: ");
	lab1->setFixedWidth(50);
	QLabel *lab2 = new QLabel("画布坐标: ");
	lab2->setFixedWidth(50);
	QLabel *lab3 = new QLabel("");
	lab3->setFixedWidth(50);

	labelViewCord = new QLabel();
	labelSceneCord = new QLabel();

	ui->statusbar->addWidget(lab1);
	ui->statusbar->addWidget(labelViewCord);
	ui->statusbar->addWidget(lab3);
	ui->statusbar->addWidget(lab2);
	ui->statusbar->addWidget(labelSceneCord);
}

void MainWindow::bindAction()
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

    connect(ui->actNoArrowB, &QAction::triggered, this, [this]() {
        scene->changeBeginArrow(0);
    });
    connect(ui->actArrowB, &QAction::triggered, this, [this]() {
        scene->changeBeginArrow(1);
    });
    connect(ui->actOpenArrowB, &QAction::triggered, this, [this]() {
        scene->changeBeginArrow(2);
    });
    connect(ui->actDovetailArrowB, &QAction::triggered, this, [this]() {
        scene->changeBeginArrow(3);
    });
    connect(ui->actDiaArrowB, &QAction::triggered, this, [this]() {
        scene->changeBeginArrow(4);
    });
    connect(ui->actRoundArrowB, &QAction::triggered, this, [this]() {
        scene->changeBeginArrow(5);
    });

    connect(ui->actNoArrowE, &QAction::triggered, this, [this]() {
        scene->changeEndArrow(0);
    });
    connect(ui->actArrowE, &QAction::triggered, this, [this]() {
        scene->changeEndArrow(1);
    });
    connect(ui->actOpenArrowE, &QAction::triggered, this, [this]() {
        scene->changeEndArrow(2);
    });
    connect(ui->actDovetailArrowE, &QAction::triggered, this, [this]() {
        scene->changeEndArrow(3);
    });
    connect(ui->actDiaArrowE, &QAction::triggered, this, [this]() {
        scene->changeEndArrow(4);
    });
    connect(ui->actRoundArrowE, &QAction::triggered, this, [this]() {
        scene->changeEndArrow(5);
    });

    connect(ui->actSelectAll, &QAction::triggered, this, &MainWindow::selectAll);

	connect(ui->actDebug, SIGNAL(triggered(bool)), this, SLOT(myDebug()));
    // connect(ui->actCheck,SIGNAL(triggered(bool)),this,SLOT(check()));
	connect(ui->actAboutUs,SIGNAL(triggered(bool)),this,SLOT(showAboutUsWindow()));
	connect(ui->actRedo,SIGNAL(triggered(bool)), this, SLOT(redo()));
	connect(ui->actUndo,SIGNAL(triggered(bool)),this, SLOT(undo()));

    connect(ui->actSaveNewFile,SIGNAL(triggered(bool)),this,SLOT(saveNewFile()));
    connect(ui->actNewFile,SIGNAL(triggered(bool)),this,SLOT(newFile()));
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

	connect(ui->actSelectFillCol, SIGNAL(triggered(bool)), this, SLOT(changeFillColor()));
	connect(ui->actSelectFrameCol, SIGNAL(triggered(bool)), this, SLOT(changeBorderColor()));
	connect(ui->actSelectTextCol, SIGNAL(triggered(bool)), this, SLOT(changeTextCol()));
	connect(ui->actSelectTextFont, SIGNAL(triggered(bool)), this, SLOT(changeTextFont()));

    connect(ui->actMoveSelectedMaxZUp, SIGNAL(triggered(bool)), this, SLOT(moveSelectedZUp()));
    connect(ui->actMoveSelectedMaxZDown, SIGNAL(triggered(bool)), this, SLOT(moveSelectedZDown()));
    connect(ui->actMoveSelectedMaxZUp, SIGNAL(triggered(bool)), this, SLOT(moveSelectedMaxZUp()));
    connect(ui->actMoveSelectedMaxZDown, SIGNAL(triggered(bool)),this, SLOT(moveSelectedMaxZDown()));

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
	QShortcut *delshorcut = new QShortcut(QKeySequence("Delete"), this);
	connect(delshorcut, SIGNAL(activated()), this, SLOT(delSelectedItem()));

	connect(saveSvgTln, &QToolButton::clicked, this, &MainWindow::saveAsSvg);
    connect(isInspect, &QCheckBox::checkStateChanged, this, [this]() {
        if(!isInspect->isChecked()) {
            inspector->hide();
            inspector->setAutoCheck(false);
            inspector->restoreView();
        }else {
            inspector->show();
            inspector->setAutoCheck(true);
            check();
        }
    });
    connect(isOpenReference, &QCheckBox::checkStateChanged, this, &MainWindow::setAutoAlign);
}

void MainWindow::saveAsSvg()
{
    QString filePath = QFileDialog::getSaveFileName(this, "save as svg file", "", ("SVG Files(*.svg"));
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

void MainWindow::selectAll()
{
    scene->selectAllItems();
}

void MainWindow::changeLineType(Qt::PenStyle linestyle)
{
	scene->changeLineType(linestyle);
}

void MainWindow::changeBeginArrow()
{
    int beginArrowType = beginarrowType->currentIndex();
	scene->changeBeginArrow(beginArrowType);
}

void MainWindow::changeEndArrow()
{
    int arrowstyle = endarrowType->currentIndex();
	scene->changeEndArrow(arrowstyle);
}

void MainWindow::changeLineColor(QColor color)
{
	scene->changeLineColor(color);
}

void MainWindow::setSceneBg(QString path)
{
	scene->setBackground(path);
}

void MainWindow::changeItemRot()
{
	scene->changeItemRot(rotationBox->value());
}


void MainWindow::changeItemScale()
{
	scene->changeItemScale(scaleBox->value() / 100.0);
}

void MainWindow::changeBorderType()
{
    int bordertype = borderStyle->currentIndex();
	// qDebug() << "penstyle;" << penstyle;
    switch(bordertype) {
	case 0: scene->changeBorderType(Qt::SolidLine); break;
	case 1: scene->changeBorderType(Qt::DashLine); break;
	case 2: scene->changeBorderType(Qt::DotLine); break;
	case 3: scene->changeBorderType(Qt::DashDotLine); break;
	case 4: scene->changeBorderType(Qt::DashDotDotLine); break;
	}
}

void MainWindow::changeBorderWidth()
{
	qreal width = borderWidth->value() * globalScale;
	scene->changeBorderWidth(width);
}

void MainWindow::changeBorderColor()
{
	QColor color = colorDia->getColor(Qt::white, this, "颜色选择器", QColorDialog::ShowAlphaChannel);
    if(color.isValid()) scene->changeBorderColor(color);
}

void MainWindow::changeFillType()
{
	int brushstyle = fillType->currentIndex();
	switch(brushstyle) {
	case 0: scene->changeFillType(Qt::NoBrush); break;
	case 1: scene->changeFillType(Qt::SolidPattern); break;
	case 2: scene->changeFillType(Qt::Dense1Pattern); break;
	case 3: scene->changeFillType(Qt::Dense2Pattern); break;
	case 4: scene->changeFillType(Qt::Dense3Pattern); break;
	case 5: scene->changeFillType(Qt::Dense4Pattern); break;
	case 6: scene->changeFillType(Qt::Dense5Pattern); break;
	case 7: scene->changeFillType(Qt::Dense6Pattern); break;
	case 8: scene->changeFillType(Qt::Dense7Pattern); break;
	case 9: scene->changeFillType(Qt::HorPattern); break;
	case 10: scene->changeFillType(Qt::VerPattern); break;
	case 11: scene->changeFillType(Qt::CrossPattern); break;
	case 12: scene->changeFillType(Qt::BDiagPattern); break;
	case 13: scene->changeFillType(Qt::FDiagPattern); break;
	case 14: scene->changeFillType(Qt::DiagCrossPattern); break;
	case 15: scene->changeFillType(Qt::LinearGradientPattern); break;
	case 16: scene->changeFillType(Qt::RadialGradientPattern); break;
	case 17: scene->changeFillType(Qt::ConicalGradientPattern); break;
	case 18: scene->changeFillType(Qt::TexturePattern); break;
	}
	customizePic->setCheckState(Qt::Unchecked);
}

void MainWindow::changeFillColor()
{
	QColor color = colorDia->getColor(Qt::white, this, "颜色选择器", QColorDialog::ShowAlphaChannel);
    if(color.isValid()){
        customizePic->setCheckState(Qt::Unchecked);
        scene->changeFillColor(color);
        // changeFillType();
    }
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

void MainWindow::changeTextCol()
{
	QColor color = colorDia->getColor(Qt::white, this, "颜色选择器", QColorDialog::ShowAlphaChannel);
    if(color.isValid()) scene->changeTextColor(color);
}

void MainWindow::changeTextFont()
{
	bool flag = true;
	QFont font = fontDia->getFont(&flag, this);
	if(flag) scene->changeTextFont(font);
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
    else scene->moveSelectedZUp();
}

void MainWindow::moveSelectedZDown(){
	if(scene->selectedItems().isEmpty()) return;
    else scene->moveSelectedZDown();
}

void MainWindow::moveSelectedMaxZUp(){
	if(scene->selectedItems().isEmpty()) return;
	else scene->moveSelectedZMaxUp();
}

void MainWindow::moveSelectedMaxZDown(){
	if(scene->selectedItems().isEmpty()) return;
	else scene->moveSelectedZMaxDown();
}

void MainWindow::changeLayer()
{
    int layer = layerBox->currentIndex();
    switch(layer) {
    case 0 : moveSelectedZUp(); break;
    case 1 : moveSelectedMaxZUp(); break;
    case 2 : moveSelectedZDown(); break;
    case 3 : moveSelectedMaxZDown(); break;
    }
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
    if(FILE_PATH == nullptr || FILE_PATH == "")
        FILE_PATH = QFileDialog::getSaveFileName(this, tr("保存.bit文件"),"./",tr("(*.bit)"));
    if(FILE_PATH == "") return;
    SaveAndLoadManager::instance().saveToFile(FILE_PATH);
}

void MainWindow::loadFile(){
    if(FILE_PATH == ""){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
            this,tr("保存当前文件"),tr("当前文件未保存，是否保存"),
            QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes){
            saveFile();
        }
    }else{
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
            this,tr("保存当前文件"),tr("当前文件未保存，是否保存到%1").arg(FILE_PATH),
            QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes){
            SaveAndLoadManager::instance().saveToFile(FILE_PATH);
        }
    }
	QString filePath = QFileDialog::getOpenFileName(this, tr("打开.bit文件"),"./",tr("(*.bit)"));
	if(filePath == "") return;
    FILE_PATH = filePath;
	SaveAndLoadManager::instance().loadFromFile(filePath);
}

void MainWindow::newFile(){
    qDebug() << " new file";
    if(FILE_PATH == ""){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
            this,tr("保存当前文件"),tr("当前文件未保存，是否保存"),QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes){
            saveFile();
        }
    }else{
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,tr("保存当前文件"),tr("当前文件未保存，是否保存到%1").arg(FILE_PATH),
                                      QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes){
             SaveAndLoadManager::instance().saveToFile(FILE_PATH);
        }
    }
    FILE_PATH = "";
    scene->clear();
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

void MainWindow::changeAlign()
{
    int align = alignBox->currentIndex();
    switch(align) {
    case 0 : itemLeftAlign(); break;
    case 1 : itemHorizAlign(); break;
    case 2 : itemRightAlign(); break;
    case 3 : itemTopAlign(); break;
    case 4 : itemVertiAlign(); break;
    case 5 : itemBottomAlign(); break;
    case 6 : itemHorizEven(); break;
    case 7 : itemVertiEven(); break;
    }
}

void MainWindow::setAutoAlign()
{
    if(isOpenReference->isChecked()) scene->setAutoAlign(true);
    else scene->setAutoAlign(false);
}

void MainWindow::myDebug()
{
    // qDebug() << "debug triggered";
    // scene->itemVertiEven();
    // formworkWidget->loadFormwork();
}

void MainWindow::check(){
    inspector->setAutoCheck(true);
	inspector->checkAll();
	inspector->show();
}

void MainWindow::saveNewFile(){
    FILE_PATH = QFileDialog::getSaveFileName(this, tr("另存为.bit文件"),"./",tr("(*.bit)"));
    if(FILE_PATH == "") return;
    SaveAndLoadManager::instance().saveToFile(FILE_PATH);
}
