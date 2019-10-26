/*点运算*/

#include"imageprocesser.h"

#define PI 3.1415926

//显示灰度图原图
void ImageProcesser::showGrayImage(){
    hideSpinBoxes();
    showImage(grayimage);
}

void ImageProcesser::showIndex8Image(){
    hideSpinBoxes();

}

//显示均衡处理后的图片
void ImageProcesser::showBalanceImage(){
    hideSpinBoxes();

    int width = grayimage.width();
    int height = grayimage.height();
    QImage tmp(width, height, QImage::Format_Indexed8);
    //设置灰度表
    tmp.setColorCount(256);
    for(int i=0;i<256;i++){
        tmp.setColor(i,qRgb(i,i,i));
    }
    //求灰度分布概率
    int grays[256]{0};
    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            int index = grayimage.pixelIndex(i, j);
            ++grays[index];
        }
    }
    double P[256];
    for(int i=0; i<256; i++){
        if(!i)
            P[i] = double(grays[i]) / grayInfo[3];
        else{
            P[i] = P[i-1] + double(grays[i]) / grayInfo[3];
        }
    }

    //生成新图像
    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
           int newPix = grayimage.pixelIndex(i, j) * P[grayimage.pixelIndex(i, j)];
           if(newPix > 255) newPix = 255;
           tmp.setPixel(i, j, newPix);
        }
    }

    showImage(tmp);
}

//显示优化后的均衡处理图片（BBHE方法）
void ImageProcesser::showNewBalanceImage(){
    hideSpinBoxes();

    //将图像分成灰度大于平均值与小于平均值的两部分， 分别进行均衡化处理
    int width = grayimage.width();
    int height = grayimage.height();
    QImage tmp(width, height, QImage::Format_Indexed8);
    tmp.setColorCount(256);
    for(int i=0;i<256;i++){
        tmp.setColor(i,qRgb(i,i,i));
    }

    int grays[256]{0};
    int f1count = 0 , f2count = 0;//分别记录f1与f2的像素数量
    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            int index = grayimage.pixelIndex(i, j);
            ++grays[index];
            if(index < int(grayInfo[0]))
                f1count++;
            else f2count++;
        }
    }

    double P[256]{0};
    //均衡化f1
    for(int i=0; i<grayInfo[0]; i++){
        if(!i)
            P[i] = double(grays[i]) / f1count;
        else{
            P[i] = P[i-1] + double(grays[i]) / f1count;
        }
    }
    //均衡化f2
    for( int i=int(grayInfo[0])+1; i<256; i++){
        if(i == int(grayInfo[0])+1)
            P[i] = double(grays[i])  / f2count;
        else{
            P[i] = P[i-1] + double(grays[i]) / f2count;
        }
    }
    //生成新图像
    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            int newPix;
            if(grayimage.pixelIndex(i, j) > int(grayInfo[0]))
                newPix = grayimage.pixelIndex(i, j) * P[grayimage.pixelIndex(i, j)] + int(grayInfo[0])*P[int(grayInfo[0])];
            else
            newPix = grayimage.pixelIndex(i, j) * P[grayimage.pixelIndex(i, j)];
            if(newPix > 255) newPix = 255;
            tmp.setPixel(i, j, newPix);
        }
    }

    showImage(tmp);
}

//根据阈值灰度生成二值图
void ImageProcesser::showSpinBox1(){
    hideSpinBoxes();

    spinbox1->setRange(0, 255);
    spinbox1->setSingleStep(16);
    spinbox1->setValue(128);
    spinbox1->setVisible(true);

    showBinaryImage(spinbox1->value());

}
void ImageProcesser::showBinaryImage(int threshold){
    int w = grayimage.width();
    int h = grayimage.height();
    QImage tmp(w, h, QImage::Format_Mono);
    for(int i=0; i<w; i++){
        for(int j=0; j<h; j++){
            if(threshold >= grayimage.pixelIndex(i, j))
                tmp.setPixel(i, j, 0);
            else
                tmp.setPixel(i, j ,1);
        }
    }
    showImage(tmp);
    GrayInfo->setVisible(false);//隐藏灰度信息（因为二值图计算出来是错的）
}

//线性变换
void ImageProcesser::showSpinBox2(){
    hideSpinBoxes();

    spinbox2_1->setPrefix("对比度：");
    spinbox2_1->setSuffix("倍");
    spinbox2_1->setRange(0, 255);
    spinbox2_1->setSingleStep(0.1);
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

    int width = grayimage.width();
    int height = grayimage.height();
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
    showImage(tmp);
}
void ImageProcesser::showlinearImage2(int b){
    spinbox2_1->setValue(1);

    int w = grayimage.width();
    int h = grayimage.height();
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
    showImage(tmp);
}

//非线性变换
void ImageProcesser::showSpinBox3(){
    hideSpinBoxes();

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

    int width = grayimage.width();
    int height = grayimage.height();
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
    showImage(tmp);
}
void ImageProcesser::showUnlinearImage2(double b){
    spinbox3_1->setValue(0.5);

    int h = grayimage.height();
    int w = grayimage.width();
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
    showImage(tmp);
}
