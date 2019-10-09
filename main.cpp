#include "imageprocesser.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageProcesser w;
    w.show();

    return a.exec();
}
