#ifndef IMAGEPROCESSER_H
#define IMAGEPROCESSER_H

#include <QMainWindow>

namespace Ui {
class ImageProcesser;
}

class ImageProcesser : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageProcesser(QWidget *parent = nullptr);
    ~ImageProcesser();

private:
    Ui::ImageProcesser *ui;
};

#endif // IMAGEPROCESSER_H
