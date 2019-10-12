#include"imageprocesser.h"
#include<QtWidgets>

void ImageProcesser::showSpinBox(){
    if(radioButton->isChecked()){
        spinbox->setRange(0, 255);
        spinbox->setSingleStep(16);
        spinbox->setVisible(true);
        showBinaryImage(spinbox->value());
    }
    else{
        spinbox->setVisible(false);
        showingImage = image;
        imageLabel->setPixmap(QPixmap::fromImage(showingImage));
    }
}

void ImageProcesser::showBinaryImage(int threshold){
    int h = image.height();
    int w = image.width();
    QImage tmp(w, h, QImage::Format_Mono);
    for(int i=0; i<w; i++){
        for(int j=0; j<h; j++){
            if(threshold >= grayimage.pixelIndex(i, j))
                tmp.setPixel(i, j, 0);
            else
                tmp.setPixel(i, j ,1);
        }
    }
    showingImage = tmp;
    imageLabel->setPixmap(QPixmap::fromImage(showingImage));
}


