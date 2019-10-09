#include "imageprocesser.h"
#include "ui_imageprocesser.h"

ImageProcesser::ImageProcesser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageProcesser)
{
    ui->setupUi(this);
}

ImageProcesser::~ImageProcesser()
{
    delete ui;
}
