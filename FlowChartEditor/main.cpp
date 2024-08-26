#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	a.setStyle(QStyleFactory::create("Fusion"));

	// 创建一个浅色调的 QPalette
	QPalette lightPalette;
	lightPalette.setColor(QPalette::Window, QColor(240, 240, 240));
	lightPalette.setColor(QPalette::WindowText, Qt::black);
	lightPalette.setColor(QPalette::Base, QColor(255, 255, 255));
	lightPalette.setColor(QPalette::AlternateBase, QColor(225, 225, 225));
	lightPalette.setColor(QPalette::ToolTipBase, Qt::black);
	lightPalette.setColor(QPalette::ToolTipText, Qt::white);
	lightPalette.setColor(QPalette::Text, Qt::black);
	lightPalette.setColor(QPalette::Button, QColor(240, 240, 240));
	lightPalette.setColor(QPalette::ButtonText, Qt::black);
	lightPalette.setColor(QPalette::BrightText, Qt::red);
	lightPalette.setColor(QPalette::Link, QColor(42, 130, 218));
	lightPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
	lightPalette.setColor(QPalette::HighlightedText, Qt::black);

	// 将浅色调的 QPalette 设置为应用程序的默认调色板
	a.setPalette(lightPalette);

	MainWindow w;
	w.show();
	return a.exec();
}
