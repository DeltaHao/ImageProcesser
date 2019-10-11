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
