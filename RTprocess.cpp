/*实时处理相关*/

#include"imageprocesser.h"
#include<QtWidgets>

#define PI 3.1415926
//根据阈值灰度生成二值图
void ImageProcesser::showSpinBox1(){
    spinbox2_1->setVisible(false);
    spinbox2_2->setVisible(false);
    spinbox3_1->setVisible(false);
    spinbox3_2->setVisible(false);

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
    spinbox3_1->setVisible(false);
    spinbox3_2->setVisible(false);

    spinbox2_1->setPrefix("对比度：");
    spinbox2_1->setSuffix("倍");
    spinbox2_1->setRange(0, 255);
    spinbox2_1->setSingleStep(0.1);
    //spinbox2_1->setValue(0);
    spinbox2_1->setVisible(true);
    showlinearImage1(spinbox2_1->value());

    spinbox2_2->setPrefix("亮度：");
    spinbox2_2->setRange(-255, 255);
    spinbox2_2->setSingleStep(16);
    spinbox2_2->setVisible(true);
    showlinearImage2(spinbox2_2->value());
}

void ImageProcesser::showlinearImage1(double a){
    spinbox2_2->setValue(0);

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
    spinbox2_1->setValue(1);

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

//非线性变换
void ImageProcesser::showSpinBox3(){
    spinbox1->setVisible(false);
    spinbox2_1->setVisible(false);
    spinbox2_2->setVisible(false);

    spinbox3_1->setPrefix("中值对比度：");
    spinbox3_1->setRange(0.01, 1);
    spinbox3_1->setSingleStep(0.1);
    spinbox3_1->setValue(0.5);
    spinbox3_1->setVisible(true);
    showUnlinearImage1(spinbox3_1->value());

    spinbox3_2->setPrefix("暗部增强：");
    spinbox3_2->setRange(1, 1000);
    spinbox3_2->setSingleStep(0.5);
    spinbox3_2->setVisible(true);
    showUnlinearImage2(spinbox3_2->value());
}

void ImageProcesser::showUnlinearImage1(double a){
    spinbox3_2->setValue(1);

    int width = image.width();
    int height = image.height();
    QImage tmp(width, height, QImage::Format_Indexed8);
    tmp.setColorCount(256);
    for(int i=0;i<256;i++){
        tmp.setColor(i,qRgb(i,i,i));
    }
    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            double temp = grayimage.pixelIndex(i, j) / 255.0;
            int newPix =  int(255 * (255 / 2) * (1 + (1 / sin(a * PI / 2)) * sin(a * PI * (temp / 255 - 0.5))));
            if(newPix > 255) newPix = 255;
            if(newPix < 0) newPix = 0;
            tmp.setPixel(i, j, newPix);
        }
    }
    showingImage = tmp;
    imageLabel->setPixmap(QPixmap::fromImage(showingImage));

    showHistogram(showingImage);
    showGrayInfo(showingImage);
}

void ImageProcesser::showUnlinearImage2(double b){
    spinbox3_1->setValue(0.5);

    int h = image.height();
    int w = image.width();
    QImage tmp(w, h, QImage::Format_Indexed8);
    tmp.setColorCount(256);
    for(int i=0;i<256;i++){
        tmp.setColor(i,qRgb(i,i,i));
    }
    for(int i=0; i<w; i++){
        for(int j=0; j<h; j++){
            double temp = grayimage.pixelIndex(i, j) / 255.0;
            int newPix = int(255 * (log(1 + b * temp) / log(b)));;
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