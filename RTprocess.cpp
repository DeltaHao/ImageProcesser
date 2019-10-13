/*实时处理相关*/

#include"imageprocesser.h"
#include<QtWidgets>
//根据阈值灰度生成二值图
void ImageProcesser::showSpinBox1(){
    spinbox2_1->setVisible(false);
    spinbox2_2->setVisible(false);

    spinbox1->setRange(0, 255);
    spinbox1->setSingleStep(16);
    spinbox1->setVisible(true);

    showBinaryImage(spinbox1->value());

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

//线性变换
void ImageProcesser::showSpinBox2(){
    spinbox1->setVisible(false);

    spinbox2_1->setPrefix("对比度：");
    spinbox2_1->setSuffix("倍");
    spinbox2_1->setRange(-255, 255);
    spinbox2_1->setSingleStep(0.1);
    spinbox2_1->setVisible(true);
    showlinearImage1(spinbox2_1->value());

    spinbox2_2->setPrefix("亮度：");

    spinbox2_2->setRange(-255, 255);
    spinbox2_2->setSingleStep(8);
    spinbox2_2->setVisible(true);
    showlinearImage1(spinbox2_2->value());
}

void ImageProcesser::showlinearImage1(double a){
    int width = image.width();
    int height = image.height();
    QImage tmp(width, height, QImage::Format_Indexed8);
    tmp.setColorCount(256);
    for(int i=0;i<256;i++){
        tmp.setColor(i,qRgb(i,i,i));
    }
    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
           int newPix = grayimage.pixelIndex(i, j) * a;
           if(newPix > 255) newPix = 255;
           tmp.setPixel(i, j, newPix);
        }
    }
    showingImage = tmp;
    imageLabel->setPixmap(QPixmap::fromImage(showingImage));

    showHistogram(showingImage);
    showGrayInfo(showingImage);
}

void ImageProcesser::showlinearImage2(int b){
    int h = image.height();
    int w = image.width();
    QImage tmp(w, h, QImage::Format_Indexed8);
    tmp.setColorCount(256);
    for(int i=0;i<256;i++){
        tmp.setColor(i,qRgb(i,i,i));
    }
    for(int i=0; i<w; i++){
        for(int j=0; j<h; j++){
           int newPix = grayimage.pixelIndex(i, j) + b;
           if(newPix > 255) newPix = 255;
           if(newPix < 0) newPix = 0;
           tmp.setPixel(i, j ,newPix);
        }
    }
    showingImage = tmp;
    imageLabel->setPixmap(QPixmap::fromImage(showingImage));

    showHistogram(showingImage);
    showGrayInfo(showingImage);
}
