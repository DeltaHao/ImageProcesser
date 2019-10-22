/*几何运算*/
#include"imageprocesser.h"

//平移
void ImageProcesser::showSpinBox6(){
    hideSpinBoxes();

    spinbox6_1->setPrefix("x轴：");
    spinbox6_1->setSingleStep(10);
    spinbox6_1->setRange(-1000, +1000);
    spinbox6_1->setValue(0);
    spinbox6_1->setVisible(true);

    spinbox6_2->setPrefix("y轴：");
    spinbox6_2->setSingleStep(10);
    spinbox6_2->setRange(-1000, +1000);
    spinbox6_2->setValue(0);
    spinbox6_2->setVisible(true);
}
void ImageProcesser::showTranslation1(int len){
    spinbox6_2->setValue(0);
    int h = image.height();
    int w = image.width();
    QImage tmp(w+abs(len), h, QImage::Format_Indexed8);
    tmp.setColorCount(256);
    for(int i=0;i<256;i++){
        tmp.setColor(i,qRgb(i,i,i));
    }
    if(len > 0){//向右平移
        for(int i=0; i<len; i++){//将前len列置白
            for(int j=0; j<h; j++){
                tmp.setPixel(i, j, 255);
            }
        }
        for(int i=len; i<w + len; i++){//
            for(int j=0; j<h; j++){
                int index = grayimage.pixelIndex(i - len, j);
                tmp.setPixel(i, j, index);
            }
        }
    }
    else{//向左平移
        for(int i=w; i<w +abs(len); i++){//将后len列置白
            for(int j=0; j<h; j++){
                tmp.setPixel(i, j, 255);
            }
        }
        for(int i=0; i<w; i++){
            for(int j=0; j<h; j++){
                int index = grayimage.pixelIndex(i, j);
                tmp.setPixel(i, j, index);
            }
        }

    }

    showingImage = tmp;
    imageLabel->setPixmap(QPixmap::fromImage(showingImage));
    imageLabel->adjustSize();//imageLabel的大小可调整

    showHistogram(showingImage);
    showGrayInfo(showingImage, showingGrayInfo);
}
void ImageProcesser::showTranslation2(int len){
    spinbox6_1->setValue(0);

    int h = image.height();
    int w = image.width();
    QImage tmp(w, h+abs(len), QImage::Format_Indexed8);
    tmp.setColorCount(256);
    for(int i=0;i<256;i++){
        tmp.setColor(i,qRgb(i,i,i));
    }
    if(len > 0){//向右平移
        for(int i=0; i<w; i++){//将前len行置白
            for(int j=0; j<len; j++){
                tmp.setPixel(i, j, 255);
            }
        }
        for(int i=0; i<w; i++){//
            for(int j=len; j<h+len; j++){
                int index = grayimage.pixelIndex(i, j-len);
                tmp.setPixel(i, j, index);
            }
        }
    }
    else{//向左平移
        for(int i=0; i<w; i++){//将后len行置白
            for(int j=h; j<h + abs(len); j++){
                tmp.setPixel(i, j, 255);
            }
        }
        for(int i=0; i<w; i++){
            for(int j=0; j<h; j++){
                int index = grayimage.pixelIndex(i, j);
                tmp.setPixel(i, j, index);
            }
        }

    }

    showingImage = tmp;
    imageLabel->setPixmap(QPixmap::fromImage(showingImage));
    imageLabel->adjustSize();//imageLabel的大小可调整

    showHistogram(showingImage);
    showGrayInfo(showingImage, showingGrayInfo);
}

//旋转
void ImageProcesser::showSpinBox7(){
    hideSpinBoxes();

    spinbox7->setPrefix("旋转");
    spinbox7->setSuffix(" 度");
    spinbox7->setSingleStep(10);
    spinbox7->setRange(-180, +180);
    spinbox7->setVisible(true);
}
void ImageProcesser::showRotation(double angle){
    //todo
}

//缩放
void ImageProcesser::showSpinBox8(){
    hideSpinBoxes();

    spinbox8_1->setPrefix("最近邻插值法：");
    spinbox8_1->setSuffix("倍");
    spinbox8_1->setSingleStep(10);
    spinbox8_1->setRange(-1000, +1000);
    spinbox8_1->setValue(1);
    spinbox8_1->setVisible(true);

    spinbox8_2->setPrefix("双线性插值法：");
    spinbox8_2->setSuffix("倍");
    spinbox8_2->setSingleStep(10);
    spinbox8_2->setRange(-1000, +1000);
    spinbox8_2->setValue(1);
    spinbox8_2->setVisible(true);
}
void ImageProcesser::nearstInterpolation(double factor){
    //todo
}
void ImageProcesser::bilinearInterpolation(double factor){
    //todo
}
