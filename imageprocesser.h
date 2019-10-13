#ifndef IMAGEPROCESSER_H
#define IMAGEPROCESSER_H


#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QtCharts>

//一些要用到的类
QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
QT_END_NAMESPACE

class ImageProcesser : public QMainWindow
{
    Q_OBJECT

public:
    ImageProcesser();//构造函数

private slots://私有槽函数
    void open();//打开图片
    void saveAs();//另存为
    void about1();//显示“关于作者”信息
    void about2();//显示“关于软件”信息
    void changeToGray();//转换为灰度图片
    //将8位灰度图像转换为8幅位平面二值图
    void changeToBitplane1();
    void changeToBitplane2();
    void changeToBitplane3();
    void changeToBitplane4();
    void changeToBitplane5();
    void changeToBitplane6();
    void changeToBitplane7();
    void changeToBitplane8();
    void showSpinBox1();//显示阈值灰度调节框
    void showBinaryImage(int threshold);//根据阈值灰度生成二值图
    void showSpinBox2();//显示线性变换调节框
    void showlinearImage1(double a);//对比度变化
    void showlinearImage2(int b);//亮度变化
    void showSpinBox3();//显示非线性变换调节框
    void showUnlinearImage1(double a);//中值对比度
    void showUnlinearImage2(double b);//暗部增强
private:
    //私有函数
    void createActions();//创造命令
    bool loadFile(const QString &);
    bool saveFile(const QString &filename);
    void setImage();
    void toGray();
    void toBitplane();
    void showHistogram(QImage);
    void showGrayInfo(QImage);//展示灰度的信息

    //实体成员
    QImage image;//原图像
    QImage grayimage;//灰度图像
    double grayInfo[4];//灰度图像的四个相关信息：平均灰度、中值灰度、标准差和像素总数
    QImage bitplaneimage[8];//8幅位平面二值图
    QImage showingImage;
    //控件
    QWidget *widget;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    QChartView *chartview;
    QLabel *GrayInfo;
    QSpinBox *spinbox1;
    QRadioButton *radioButton1;
    QDoubleSpinBox *spinbox2_1;
    QSpinBox *spinbox2_2;
    QRadioButton *radioButton2;
    QRadioButton *radioButton0;
    QDoubleSpinBox *spinbox3_1;
    QDoubleSpinBox *spinbox3_2;
    QRadioButton *radioButton3;

    //菜单命令
    QAction *saveAsAct;
    QAction *changeToGrayAct;
    QAction *changeToBitplaneAct[8];
};

#endif // IMAGEPROCESSER_H
