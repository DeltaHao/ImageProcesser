/*图像分割*/

#include"imageprocesser.h"
//边缘检测
void ImageProcesser::SobelEdge(){
    hideSpinBoxes();

    int h = grayimage.height();
    int w = grayimage.width();

    QImage tmp = grayimage;
    tmp.setColorCount(256);
    for(int i=0;i<256;i++){
        tmp.setColor(i,qRgb(i,i,i));
    }
    int wights_x[3][3] = { -1, 0, +1, -2, 0, +2, -1, 0, +1 };
    int wights_y[3][3] = { +1, +2, +1, 0, 0, 0, -1, -2, -1 };

    for(int i=0; i<w; i++){
        for(int j=0; j<h; j++){
            int Gx=0, Gy=0;
            for(int I=i-1; I<=i+1; I++){
                for(int J=j-1; J<=j+1; J++){
                    if(I<0 || J<0 || I>=w || J>=h) continue;
                    Gx += (grayimage.pixelIndex(I, J) * wights_x[I-(i-1)][J-(j-1)]);
                    Gy += (grayimage.pixelIndex(I, J) * wights_y[I-(i-1)][J-(j-1)]);
                }
            }
            double sum = sqrt(Gx*Gx + Gy*Gy);

            int index = (int)(sum+0.5);
            if(index<0) index = 0;
            if(index>255) index = 255;
            tmp.setPixel(i ,j, index);
        }
    }
    showImage(tmp);

}
void ImageProcesser::PrewittEdge(){
    hideSpinBoxes();

    int h = grayimage.height();
    int w = grayimage.width();

    QImage tmp = grayimage;
    tmp.setColorCount(256);
    for(int i=0;i<256;i++){
        tmp.setColor(i,qRgb(i,i,i));
    }
    int wights_x[3][3] = { -1, 0, +1, -1, 0, +1, -1, 0, +1 };
    int wights_y[3][3] = { +1, +1, +1, 0, 0, 0, -1, -1, -1 };

    for(int i=0; i<w; i++){
        for(int j=0; j<h; j++){
            int Gx=0, Gy=0;
            for(int I=i-1; I<=i+1; I++){
                for(int J=j-1; J<=j+1; J++){
                    if(I<0 || J<0 || I>=w || J>=h) continue;
                    Gx += (grayimage.pixelIndex(I, J) * wights_x[I-(i-1)][J-(j-1)]);
                    Gy += (grayimage.pixelIndex(I, J) * wights_y[I-(i-1)][J-(j-1)]);
                }
            }
            double sum = sqrt(Gx*Gx + Gy*Gy);

            int index = (int)(sum+0.5);
            if(index<0) index = 0;
            if(index>255) index = 255;
            tmp.setPixel(i ,j, index);
        }
    }
    showImage(tmp);
}
void ImageProcesser::LaplacianEdge(){
    hideSpinBoxes();

    int h = grayimage.height();
    int w = grayimage.width();

    QImage tmp = grayimage;
    tmp.setColorCount(256);
    for(int i=0;i<256;i++){
        tmp.setColor(i,qRgb(i,i,i));
    }
    int wights[3][3] = { -1, -1, -1, -1, 8, -1, -1, -1, -1 };

    for(int i=0; i<w; i++){
        for(int j=0; j<h; j++){
            int sum = 0;
            for(int I=i-1; I<=i+1; I++){
                for(int J=j-1; J<=j+1; J++){
                    if(I<0 || J<0 || I>=w || J>=h) continue;
                    sum += grayimage.pixelIndex(I, J) * wights[I-(i-1)][J-(j-1)] ;
                }
            }
            if(sum<0) sum = 0;
            if(sum>255) sum = 255;
            tmp.setPixel(i ,j, sum);
        }
    }
    showImage(tmp);
}

//霍夫变换检测直线
void ImageProcesser::HoughTransform(){
    // https://blog.csdn.net/yuyuntan/article/details/80141392
    // 看不懂
    // 不会做
    // 累了
    hideSpinBoxes();
}
