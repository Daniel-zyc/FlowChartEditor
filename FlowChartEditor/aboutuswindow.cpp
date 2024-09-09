#include "aboutuswindow.h"

#include <QLabel>
#include <QVBoxLayout>

AboutUsWindow::AboutUsWindow(QWidget *parent) : QDialog(parent) {
    setWindowTitle("About Us");
    resize(400, 300); // 调整窗口大小以适应文本

    // 使用 HTML 格式来显示带有链接的文本
    QString markdownText = R"(
        <h1>关于我们</h1>
        <p>欢迎访问关于我们. 北京理工大学2022级软件工程大三小学期实践项目</p>
        <p>本项目采用Qt6.4.2，使用MinGW构建，
            通过<a href='https://gitee.com/daniel-zyc/flow-chart-editor.git'>访问</a>
            我们的gitee代码仓库获取源代码</p>
        <ul>组员:
            <li>赵一辰</li>
            <li>崔琛浩</li>
            <li>杨林帆</li>
            <li>郑佳娴</li>
            <li>张润宁</li>
            <li>宋卓一</li>
        </ul>
        <p>2024.8.30 - 2024.9.9</p>
    )";

    // 使用 QLabel 来显示文本，并设置为富文本格式
    QLabel *label = new QLabel(markdownText, this);
    label->setWordWrap(true); // 允许自动换行
    label->setTextFormat(Qt::RichText); // 设置文本格式为富文本
    label->setTextInteractionFlags(Qt::TextBrowserInteraction); // 允许链接点击
    label->setOpenExternalLinks(true); // 允许打开外部链接

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
}
