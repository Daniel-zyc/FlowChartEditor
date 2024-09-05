#pragma once

#include "dscene.h"
#include "dview.h"
#include "dfinddialog.h"
#include "dclass/text/dtextitem.h"
#include "inspector.h"

#include <QSet>
#include <QColorDialog>
#include <QFontDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QToolButton>
#include <QFormLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QTabWidget>
#include <QHeaderView>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QMainWindow>
#include <formworkwidget.h>

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
	void createStatusBar();
	void bindAction();
    void initUi();
    void initleftUi();
    void initmiddleUi();
    void initrightUi();
    void connectLeft();
    void connectRight();
	QMenu* createSceneMenu();

private slots:
    void check();
    void saveAsSvg();
    void selectAll();
    void addLine() { scene->addLineItem(); }
	void addPolyLine() { scene->addPolyLineItem(); }
	void addCurveLine() { scene->addCurveLineItem(); }

	void addText() { scene->addTextItem(); }

	void addRect() { scene->addRectItem(); }
	void addRoundRect() { scene->addRoundRectItem(); }
	void addEll() { scene->addEllItem(); }
	void addTri() { scene->addTriItem(); }
	void addDia() { scene->addDiaItem(); }
	void addTrap() { scene->addTrapItem(); }
	void addParagram() { scene->addParagramItem(); }
    void addpentagon() { scene->addpentagonItem(); }
    void addhexagon() { scene->addhexagonItem(); }

	void addDFDocItem() { scene->addDFDocItem(); }
	void addDFManualOperateItem() { scene->addDFManualOperateItem(); }
    void addDFInformationItem() { scene->addDFInformationItem(); }
	void addDFInternalStoreItem() { scene->addDFInternalStoreItem(); }
	void addDFPrepareItem() { scene->addDFPrepareItem(); }
	void addDFProcessItem() { scene->addDFProcessItem(); }
	void addDFOptionalProcessItem() { scene->addDFOptionalProcessItem(); }
	void addDFConditionItem() { scene->addDFConditionItem(); }
	void addDFDataItem() { scene->addDFDataItem(); }
	void addDFNodeItem() { scene->addDFNodeItem(); }
    void addDFStartEndItem() { scene->addDFStartEndItem(); }
    void addDFPredefineItem() { scene->addDFPredefineItem(); }
    void addDFSummaryconnItem() { scene->addDFSummaryconnItem(); }
    void addDFOrItem() { scene->addDFOrItem();}
    void addDFCardItem() { scene->addDFCardItem();}
    void addDFManualinputItem() { scene->addManualinItem(); }
    void addDFDelayItem() { scene->addDFDelayItem(); }
    void addDFManualInputItem() { scene->addManualInputItem(); }
    void addDFCompareItem(){scene->addDFCompareItem();}
    void addDFDirecrAccessItem() {scene->addDFDirecrAccessItem();}
    void addDFDiskItem(){scene->addDFDiskItem();}
    void addDFDisplayItem(){scene->addDFDisplayItem();}
    void addDFMergeItem(){scene->addDFMergeItem();}
    void addDFMultiDocItem(){scene->addDFMultiDocItem();}
    void addDFOffPageItem(){scene->addDFOffPageItem();}
    void addDFSequentialAccessItem(){scene->addDFSequentialAccessItem();}
    void addDFStoreDataItem(){scene->addDFStoreDataItem();}
    void addDFSortItem(){scene->addDFSortItem();}

    void changeLineType(Qt::PenStyle linestyle);
    void changeBeginArrow();
    void changeEndArrow();
    void changeLineColor();
    void setSceneBg(QString path);
    void setColorIcon(QPushButton *button) {
        button->setStyleSheet("QPushButton {"
                                  "    background-image: url(:/icon/palette.png) 0 0 0 0 stretch stretch;"
                                  "}");
        button->setFixedWidth(100);
    };
    void changeItemRot();
    void changeItemScale();

    void changeBorderType();
    void changeBorderWidth();
    void changeBorderColor();

    void changeFillType();
    void changeFillColor();

    QSet<DTextBase *> getTextBases();
    void changeTextCol();
    void changeTextFont();

	void rotateCW();
	void rotateCCW();
	void enlarge();
	void shrink();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

    void moveSelectedZUp();
    void moveSelectedZDown();
    void moveSelectedMaxZUp();
    void moveSelectedMaxZDown();
    void changeLayer();

	void viewRotateCW() { view->rotateCW(); }
	void viewRotateCCW() { view->rotateCCW(); }
	void viewEnlarge() { view->enlarge(); }
	void viewShrink() { view->shrink(); }
	void viewMoveLeft() { view->moveLeft(); }
	void viewMoveRight() { view->moveRight(); }
	void viewMoveUp() { view->moveUp(); }
	void viewMoveDown() { view->moveDown(); }

    void editRotateCW() { scene->rotateCW(); shot();}
    void editRotateCCW() { scene->rotateCCW(); shot();}
    void editEnlarge() { scene->enlarge(); shot();}
    void editShrink() { scene->shrink(); shot();}
    void editMoveLeft() { scene->moveLeft(); shot();}
    void editMoveRight() { scene->moveRight(); shot();}
    void editMoveUp() { scene->moveUp(); shot();}
    void editMoveDown() { scene->moveDown(); shot();}

	void findandReplace();

	void delSelectedItem();

    void saveFile();
    void loadFile();
    void newFile();
    void saveNewFile();

	void copy();
    void shear();
	void paste();

	void redo();
	void undo();

	void showAboutUsWindow();

    void itemTopAlign() {scene->itemTopAlign(); shot();}
    void itemBottomAlign() {scene->itemBottomAlign(); shot();}
    void itemLeftAlign() {scene->itemLeftAlign(); shot();}
    void itemRightAlign() {scene->itemRightAlign(); shot();}
    void itemHorizAlign() {scene->itemHorizAlign(); shot();}
    void itemVertiAlign() {scene->itemVertiAlign(); shot();}

    void itemHorizEven() {scene->itemHorizEven(); shot();}
    void itemVertiEven() {scene->itemVertiEven(); shot();}
	void changeAlign();

	void setAutoAlign();

	void myDebug();
    void shot();
private:
	Ui::MainWindow *ui;

	Inspector *inspector;
	DScene *scene;
	DView *view;
	DFindDialog *findDia;
	QColorDialog *colorDia;
	QFontDialog *fontDia;

	QSplitter *middlesplitter;
	QSplitter *mainsplitter;

	QCheckBox *isInspect;
	QCheckBox *isOpenReference;

	QWidget *leftw;
	QGridLayout *flowGrid;
	QGridLayout *primaryGrid;
	QGridLayout *lineGrid;
	QGridLayout *textGrid;
	QVBoxLayout *leftUpV;

	QGroupBox *primaryGroup;
	QGroupBox *flowcGroup;
	QGroupBox *lineGroup;
	QGroupBox *textGroup;
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
	QPushButton *endBtn;
	QPushButton *preBtn;
	QPushButton *prepareBtn;
	QPushButton *storeBtn;
	QPushButton *polyLineBtn;
	QPushButton *aggreconnectBtn;
	QPushButton *cardBtn;
	QPushButton *compareBtn;
	QPushButton *dataBtn;
	QPushButton *directaccessBtn;
	QPushButton *diskBtn;
	QPushButton *displayBtn;
	QPushButton *manulinputBtn;
	QPushButton *mergeBtn;
	QPushButton *multidocBtn;
	QPushButton *offpageBtn;
	QPushButton *orBtn;
	QPushButton *postponeBtn;
	QPushButton *sequentialaccessBtn;
	QPushButton *storedataBtn;
	QPushButton *curveBtn;
	QPushButton *sortBtn;

	QPushButton *addRectBtn;
	QPushButton *addEllBtn;
	QPushButton *addParagramBtn;
	QPushButton *addDiaBtn;
	QPushButton *addTrapBtn;
	QPushButton *addTriBtn;
	QPushButton *addPenBtn; //五边形
	QPushButton *addHexBtn; //六边形

	FormworkWidget *formworkWidget;
	QTabWidget *rightTab;
	QComboBox *lineType;
	QComboBox *endarrowType;
	QComboBox *beginarrowType;
	QDoubleSpinBox *linebound;
	QPushButton *linecolor;

	QToolButton *saveSvgTln;

	QGroupBox *bgGroup;
	QRadioButton *colorBg;
	QRadioButton *blankBg;
	QRadioButton *gridBg;
	QRadioButton *dotBg;
	QRadioButton *customizeBg;

	QWidget *rightBgw;
	QFormLayout *rightBgf;
	QPushButton *reColorBtn;
	QPushButton *reFileBtn;

	QWidget *rightStylew;
	QFormLayout *rightStylef;
	QComboBox *alignBox;
	QComboBox *layerBox;

	QTreeWidget *rightShapew;
	QTreeWidgetItem *borderTop;
	QTreeWidgetItem *borderChildColor;
	QTreeWidgetItem *borderChildType;
	QTreeWidgetItem *borderChildWidth;
	QTreeWidgetItem *fillTop;
	QTreeWidgetItem *fillChildColor;
	QTreeWidgetItem *fillChildType;
	QTreeWidgetItem *fillChildPic;
	QTreeWidgetItem *rotTop;
	QTreeWidgetItem *scaleTop;
	QTreeWidgetItem *lineTop;
	QTreeWidgetItem *lineChildColor;
	QTreeWidgetItem *lineChildType;
	QTreeWidgetItem *lineChildEndArrow;
	QTreeWidgetItem *lineChildBeginArrow;
	QTreeWidgetItem *lineChildWidth;
	QPushButton *borderColor;
	QComboBox *borderStyle;
	QDoubleSpinBox *borderWidth;
	QPushButton *fillColor;
	QComboBox *fillType;
	QSpinBox *rotationBox;
	QSpinBox *scaleBox;
	QCheckBox *customizePic;
	QPushButton *picfile;

	QWidget *rightFontw;
	QFormLayout *rightFontf;
	QPushButton *textColor;
	QPushButton *textFont;

	QLabel *labelViewCord;
	QLabel *labelSceneCord;
};
