#include "aboutuswindow.h"

#include <QLabel>
#include <QVBoxLayout>

AboutUsWindow::AboutUsWindow(QWidget *parent) : QDialog(parent) {
    setWindowTitle("About Us");
    resize(400, 300); // 调整窗口大小以适应文本

    // 使用 HTML 格式来显示带有链接的文本
    QString markdownText = R"(
        <h1>关于我们</h1>
        <p><b>欢迎</b>访问关于我们. 这是北京理工大学2022级软件工程大三小学期实践项目</p>
        <p>不啦不啦不啦不啦不啦不啦不啦不啦不啦不啦不啦不啦，通过<a href='https://gitee.com/daniel-zyc/flow-chart-editor.git'>访问</a>我们的gitee代码仓库获取源代码</p>
        <ul>
            <li>不啦不啦不啦</li>
            <li>不啦不啦不啦</li>
            <li>不啦不啦不啦</li>
        </ul>
        <p>感谢</p>
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
