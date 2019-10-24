/*几何运算*/
#include"imageprocesser.h"

#define PI 3.1415926
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
    showTranslation2(0);
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

    showImage(tmp);
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

    showImage(tmp);
}

//旋转
void ImageProcesser::showSpinBox7(){
    hideSpinBoxes();

    spinbox7->setPrefix("旋转");
    spinbox7->setSuffix(" 度");
    spinbox7->setSingleStep(10);
    spinbox7->setRange(-90, +90);
    spinbox7->setValue(0);
    spinbox7->setVisible(true);

    showRotation(0);
}
void ImageProcesser::showRotation(double angle){
    double R = PI*angle/180.0;//弧度
    int w = image.width();
    int h = image.height();
    //确定画布大小
    int newW = (int)(std::abs(w*cos(R)) + std::abs(h*sin(R)) + 0.5);
    int newH = (int)(std::abs(w*sin(R)) + std::abs(h*cos(R)) + 0.5);
    //建立新图片
    QImage tmp(newW, newH, QImage::Format_Indexed8);
    tmp.setColorCount(256);
    for(int i=0;i<256;i++)
        tmp.setColor(i,qRgb(i,i,i));

    if(angle>=0){
        int Offset = (int) (h*sin(R));
        for(int i=0; i<newW; i++){
            for(int j=0; j<newH; j++){
                int old_i = (int)((i-Offset)*cos(R) + (j)*sin(R) + 0.5);
                int old_j = (int)(-(i-Offset)*sin(R) + (j)*cos(R) + 0.5);
                if(old_i<0 || old_i >= w || old_j<0 || old_j >= h)
                    tmp.setPixel(i, j, 255);
                else{
                    int index = grayimage.pixelIndex(old_i, old_j);
                    tmp.setPixel(i, j, index);
                }
            }
        }
    }
    else{
        int Offset = (int) (w*sin(R));
        for(int i=0; i<newW; i++){
            for(int j=0; j<newH; j++){
                int old_i = (int) ((i)*cos(R) + (j+Offset)*sin(R) + 0.5);
                int old_j = (int) (-(i)*sin(R) + (j+Offset)*cos(R) + 0.5);
                if(old_i<0 || old_i >= w || old_j<0 || old_j >= h)
                    tmp.setPixel(i, j, 255);
                else{
                    int index = grayimage.pixelIndex(old_i, old_j);
                    tmp.setPixel(i, j, index);
                }
            }
        }
    }
    showImage(tmp);
}

//缩放
void ImageProcesser::showSpinBox8(){
    hideSpinBoxes();

    spinbox8_1->setPrefix("最近邻插值法：");
    spinbox8_1->setSuffix("倍");
    spinbox8_1->setSingleStep(0.1);
    spinbox8_1->setRange(-1000, +1000);
    spinbox8_1->setValue(1);
    spinbox8_1->setVisible(true);

    spinbox8_2->setPrefix("双线性插值法：");
    spinbox8_2->setSuffix("倍");
    spinbox8_2->setSingleStep(0.1);
    spinbox8_2->setRange(-1000, +1000);
    spinbox8_2->setValue(1);
    spinbox8_2->setVisible(true);

    nearstInterpolation(1);
}
void ImageProcesser::nearstInterpolation(double factor){
    spinbox8_2->setValue(1);

    int w = (int)(image.width() * factor + 0.5);
    int h = (int)(image.height() * factor + 0.5);
    QImage tmp(w, h, QImage::Format_Indexed8);
    tmp.setColorCount(256);
    for(int i=0;i<256;i++)
        tmp.setColor(i,qRgb(i,i,i));
    for(int i=0; i<w; i++){
        for(int j=0; j<h; j++){
            int old_i = (int)(i / factor + 0.5);
            int old_j = (int)(j / factor + 0.5);
            if(old_i<0 || old_i >= image.width() || old_j<0 || old_j >= image.height())
                tmp.setPixel(i, j, 255);
            else{
                int index = grayimage.pixelIndex(old_i, old_j);
                tmp.setPixel(i, j, index);
            }
        }
    }

    showImage(tmp);
}
void ImageProcesser::bilinearInterpolation(double factor){
    spinbox8_1->setValue(1);

    int w = (int)(image.width() * factor + 0.5);
    int h = (int)(image.height() * factor + 0.5);
    QImage tmp(w, h, QImage::Format_Indexed8);
    tmp.setColorCount(256);
    for(int i=0;i<256;i++)
        tmp.setColor(i,qRgb(i,i,i));
    if(factor <= 1.0){//缩小
        for(int i=0; i<w; i++){
            for(int j=0; j<h; j++){
                int old_i = (int)(i / factor + 0.5);
                int old_j = (int)(j / factor + 0.5);
                if(old_i<0 || old_i >= image.width() || old_j<0 || old_j >= image.height())
                    tmp.setPixel(i, j, 255);
                else{
                    int index = grayimage.pixelIndex(old_i, old_j);
                    tmp.setPixel(i, j, index);
                }
            }
        }
    }
    else{//放大
        //todo 双线性插值法放大图像    https://blog.csdn.net/u011776903/article/details/78782499

        int x1, x2, y1, y2;
        double temp1, temp2;
        for(int i=0; i<w; i++){
            for(int j=0; j<h; j++){
                x1 = (int) (i/factor);
                x2 = x1 + 1;
                y1 = (int) (j/factor);
                y2 = y1 + 1;

                //判断边界
                if(y2 >= image.height() || x2 >= image.width()){
                    tmp.setPixel(i, j, (grayimage.pixelIndex(x1, y1)));
                    continue;
                }

                temp1 = (x2 - i / factor) * grayimage.pixelIndex(x1, y1) + (i / factor - x1) * grayimage.pixelIndex(x2, y1);
                temp2 = (x2 - i / factor) * grayimage.pixelIndex(x1, y2) + (i / factor - x1) * grayimage.pixelIndex(x2, y2);
                tmp.setPixel(i, j, (uchar)((y2 - j / factor) * temp1 + (j / factor - y1) * temp2));
            }
        }
    }

    showImage(tmp);
}
