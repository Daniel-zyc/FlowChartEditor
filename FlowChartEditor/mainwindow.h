#pragma once

#include "dscene.h"
#include "dview.h"
#include "dfinddialog.h"
#include "dtextitem.h"
#include "inspector.h"

#include <QSet>
#include <QColorDialog>
#include <QFontDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QWidget>
#include <QPushButton>
#include <QToolButton>
#include <QFormLayout>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QTabWidget>
#include <QHeaderView>
#include <QTreeWidget>
#include <QTreeWidgetItem>
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
    void initleftUi();
    void initrightUi();
    void connectLeft();
    void connectRight();

private slots:
    void check();
    void saveAsSvg();

	void addLine();
	void addRect();
	void addRoundRect();
	void addEll();
	void addText();
    void addTri();
    void addDia();
    void addTrap();
    void addEnd();
    void addPre();
    void addParallegram();
    void addDocShape();
	void addPolyLine();

	void addDFDocItem() { scene->addDFDocItem(); }
	void addDFEndItem() { scene->addDFEndItem(); }
	void addDFManualOperateItem() { scene->addDFManualOperateItem(); }
	void addDFInternalStoreItem() { scene->addDFInternalStoreItem(); }
	void addDFPrepareItem() { scene->addDFPrepareItem(); }
	void addDFProcessItem() { scene->addDFProcessItem(); }
	void addDFOptionalProcessItem() { scene->addDFOptionalProcessItem(); }
	void addDFConditionItem() { scene->addDFConditionItem(); }
	void addDFDataItem() { scene->addDFDataItem(); }
	void addDFNodeItem() { scene->addDFNodeItem(); }

    void changeLineType(Qt::PenStyle linestyle);
    void changeEndArrow(int endArrowType);
    void changeLineColor(QColor color);
    void setSceneBg(QString path);

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

    void moveSelectedZUp();
    void moveSelectedZDown();

    void moveSelectedMaxZUp();
    void moveSelectedMaxZDown();

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

    void showAboutUsWindow();
private:
	Ui::MainWindow *ui;

    Inspector *inspector;
	DScene *scene;
	DView *view;
    DFindDialog *findDia;
    QColorDialog *colorDia;
    QFontDialog *fontDia;

    QSplitter *mainsplitter;
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

    QPushButton *prectBtn;
    QPushButton *pellipseBtn;
    QPushButton *pparellgramBtn;
    QPushButton *prhomBtn;
    QPushButton *ptrapBtn;
    QPushButton *ptriBtn;

    QTabWidget *rightTab;
    QWidget *rightLinew;
    QPushButton *confirm;
    QPushButton *cancle;
    QFormLayout *formright;
    QHBoxLayout *rbtnLayout;
    QComboBox *lineType;
    QComboBox *arrowType;
    QDoubleSpinBox *linebound;
    QPushButton *lineConfirm;
    QPushButton *arrowConfirm;
    QPushButton *lineboundConfirm;
    QPushButton *linecolor;

    QToolButton *createTln;
    QToolButton *openTln;
    QToolButton *saveTln;
    QToolButton *saveSvgTln;

    QGroupBox *bgGroup;
    QRadioButton *blankBg;
    QRadioButton *gridBg;
    QRadioButton *dotBg;

    QTreeWidget *rightBgw;
    QTreeWidgetItem *colorTop;
    QTreeWidgetItem *patternTop;
    QPushButton *selectedColor;
    QTreeWidgetItem *colorChild0;
    QTreeWidgetItem *patternChild0;
    QTreeWidgetItem *patternChild1;
    QTreeWidgetItem *patternChild2;
    QTreeWidgetItem *patternChild3;
    QRadioButton *customizeBg;
    QPushButton *repickBtn;

    QString filePath = nullptr;
};
