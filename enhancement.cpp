/*图像增强
参考于  https://blog.csdn.net/zaishuiyifangxym/article/details/89788020 */
#include "imageprocesser.h"

//平滑
void ImageProcesser::meanFilter(){
    hideSpinBoxes();

    int h = image.height();
    int w = image.width();

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

    int h = image.height();
    int w = image.width();

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

    int h = image.height();
    int w = image.width();

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
