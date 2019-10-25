/*图像增强
参考于  https://blog.csdn.net/zaishuiyifangxym/article/details/89788020 */
#include "imageprocesser.h"

//平滑
void ImageProcesser::meanFilter(){
    hideSpinBoxes();

    int h = grayimage.height();
    int w = grayimage.width();

    QImage tmp = grayimage;
    tmp.setColorCount(256);
    for(int i=0;i<256;i++){
        tmp.setColor(i,qRgb(i,i,i));
    }

    for(int i=0; i<w; i++){
        for(int j=0; j<h; j++){
            int sum = 0;
            int count = 0;
            for(int I=i-2; I<=i+2; I++){
                for(int J=j-2; J<=j+2; J++){
                    if(I<0 || J<0 || I>=w || J>=h) continue;
                    sum += grayimage.pixelIndex(I, J);
                    ++count;
                }
            }
            tmp.setPixel(i ,j, sum/count);
        }
    }

    showImage(tmp);
}
void ImageProcesser::medianFilter(){
    hideSpinBoxes();

    int h = grayimage.height();
    int w = grayimage.width();

    QImage tmp = grayimage;
    tmp.setColorCount(256);
    for(int i=0;i<256;i++){
        tmp.setColor(i,qRgb(i,i,i));
    }

    for(int i=0; i<w; i++){
        for(int j=0; j<h; j++){
            int sum = 0;
            int count = 0;
            int pixels[25];
            for(int I=i-2; I<=i+2; I++){
                for(int J=j-2; J<=j+2; J++){
                    if(I<0 || J<0 || I>=w || J>=h) continue;
                    pixels[count++] = grayimage.pixelIndex(I, J);
                }
            }

            std::sort(pixels, pixels+count+1);

            tmp.setPixel(i ,j, pixels[count/2]);
        }
    }

    showImage(tmp);

}
void ImageProcesser::gaussFilter(){
    hideSpinBoxes();

    int h = grayimage.height();
    int w = grayimage.width();

    QImage tmp = grayimage;
    tmp.setColorCount(256);
    for(int i=0;i<256;i++){
        tmp.setColor(i,qRgb(i,i,i));
    }
    int wights[5][5] = {{1,1,2,1,1},
                        {1,3,4,3,1},
                        {2,4,8,4,2},
                        {1,3,4,3,1},
                        {1,1,2,1,1}};
    for(int i=0; i<w; i++){
        for(int j=0; j<h; j++){
            double arv = 0;
            for(int I=i-2; I<=i+2; I++){
                for(int J=j-2; J<=j+2; J++){
                    if(I<0 || J<0 || I>=w || J>=h) continue;
                    arv += grayimage.pixelIndex(I, J) * wights[I-(i-2)][J-(j-2)] / 56.0;
                }
            }
            tmp.setPixel(i ,j, (uint)(arv+0.5));
        }
    }

    showImage(tmp);
}

//锐化
void ImageProcesser::RobertsSharpen(){
    hideSpinBoxes();
    int h = grayimage.height();
    int w = grayimage.width();

    QImage tmp = grayimage;
    tmp.setColorCount(256);
    for(int i=0;i<256;i++){
        tmp.setColor(i,qRgb(i,i,i));
    }

    for(int i=1; i<w; i++){
        for(int j=1; j<h; j++){
            int Gx = grayimage.pixelIndex(i-1, j-1)-grayimage.pixelIndex(i, j);
            int Gy = grayimage.pixelIndex(i-1, j)-grayimage.pixelIndex(i, j-1);
            double sum = sqrt(Gx*Gx + Gy*Gy);

            int index = (int)(sum+0.5);
            if(index<0) index = 0;
            if(index>255) index = 255;
            tmp.setPixel(i ,j, index);
        }
    }
    showImage(tmp);
}
void ImageProcesser::SobelSharpen(){
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
void ImageProcesser::LaplacianSharpen(){
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

//给定模版求卷积
