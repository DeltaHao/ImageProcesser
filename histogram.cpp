#include<QtCharts>
#include "imageprocesser.h"

void ImageProcesser::Imhist(){
    //  对直方图进行数据处理,求出各个灰度值的像素数
    int data[256]{0};
    int yRange = 0;
    int width = grayimage.width();
    int height = grayimage.height();
    for(int i = 0; i <width; i++){
        for(int j = 0; j < height; j++){
            int index = grayimage.pixelIndex(i, j);
            ++data[index];
        }
    }

    //设置每个“柱”的颜色，值，宽度等
    QBarSet *set = new QBarSet("灰度值");
    for (int i=0;i<256;i++) {
       set->append(data[i]);
       if(yRange<data[i]) yRange = data[i];
       set->setColor(QColor::Rgb);
    }
    QBarSeries *series = new QBarSeries();
    series->append(set);
    series->setBarWidth(1.2);

    //设置横坐标
    QStringList categories;
    for(int i=0; i<256; i++)
        categories.push_back("");
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);

    //设置纵坐标
    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%d");
    axisY->setRange(0,yRange);

    //建表
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setAxisX(axisX);
    chart->setAxisY(axisY);
    chart->setTitle("灰度直方图");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(false);

    //表的容器
    chartview->setChart(chart);
    chartview->setRenderHint(QPainter::Antialiasing);
    chartview->setVisible(true);
}

void ImageProcesser::calGrayInfo(){
    int h = grayimage.height();
    int w = grayimage.width();
    //像素总数
    grayInfo[3] = h*w;
    //平均灰度
    int graySum = 0;
    for(int i=0; i<w; i++){
        for(int j=0; j<h; j++){
            int index = grayimage.pixelIndex(i, j);
            graySum += index;
        }
    }
    grayInfo[0] = graySum / grayInfo[3];
    //todo: 中值灰度？？？
    //不知道是啥东西，生成个随机数充数
    grayInfo[1] = rand()%256;
    //标准差
    int Sum = 0;
    for(int i=0; i<w; i++){
        for(int j=0; j<h; j++){
            int index = grayimage.pixelIndex(i, j);
            Sum += (index - grayInfo[0]) * (index - grayInfo[0]);
        }
    }
    grayInfo[2] = sqrt(Sum/grayInfo[3]);
}

void ImageProcesser::showGrayInfo(){
    calGrayInfo();
    GrayInfo->setMargin(20);
    GrayInfo->setText(tr("平均灰度：%1  中值灰度：%2  标准差：%3  像素总数：%4")
                       .arg(grayInfo[0]).arg(grayInfo[1]).arg(grayInfo[2]).arg(grayInfo[3]));
    GrayInfo->setVisible(true);
}
