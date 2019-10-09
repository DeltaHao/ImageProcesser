#include "imageprocesser.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGuiApplication::setApplicationDisplayName(ImageProcesser::tr("数字图像处理"));//设置应用程序的标题
    ImageProcesser w;
    w.show();
    return a.exec();
}
