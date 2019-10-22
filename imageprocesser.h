#ifndef IMAGEPROCESSER_H
#define IMAGEPROCESSER_H

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
    void zoomIn();//放大显示
    void zoomOut();//缩小显示
    void normalSize();//原始大小

    void showCommenImage();//显示原图
    void showBalanceImage();//显示均衡处理后的图片
    void showNewBalanceImage();//显示优化后的均衡处理
    void showSpinBox1();//显示阈值灰度调节框
    void showBinaryImage(int threshold);//根据阈值灰度生成二值图
    void showSpinBox2();//显示线性变换调节框
    void showlinearImage1(double a);//对比度变化
    void showlinearImage2(int b);//亮度变化
    void showSpinBox3();//显示非线性变换调节框
    void showUnlinearImage1(double a);//中值对比度
    void showUnlinearImage2(double b);//暗部增强
    void showSpinBox6();//显示平移调节框
    void showTranslation1(int);//x轴平移
    void showTranslation2(int);//y轴平移
    void showSpinBox7();//显示旋转调节框
    void showRotation(double);//旋转
    void showSpinBox8();//缩放
    void nearstInterpolation(double);//最近邻插值法缩放
    void bilinearInterpolation(double);//双线性插值法缩放
private:
    //私有函数
    void createActions();//创造命令
    bool loadFile(const QString &);
    bool saveFile(const QString &filename);
    void setImage();
    void toGray();
    void toBitplane();
    void showHistogram(QImage);//显示灰度直方图
    void showGrayInfo(QImage, double*);//展示灰度的信息
    void scaleImage(double factor);//缩放显示图像
    void hideSpinBoxes();
    //实体成员
    QImage image;//原图像
    double scaleFactor;//图像的显示比例
    QImage grayimage;//灰度图像
    double grayInfo[4];//灰度图像的四个相关信息：平均灰度、中值灰度、标准差和像素总数
    QImage bitplaneimage[8];//8幅位平面二值图
    QImage showingImage;//正在展示的图像
    double showingGrayInfo[4];//正在展示的图像的四个相关信息：平均灰度、中值灰度、标准差和像素总数
    //控件
    QWidget *widget;    
    QLabel *imageLabel;    
    QScrollArea *scrollArea;    
    QChartView *chartview;    
    QLabel *GrayInfo;

    QRadioButton *radioButton0;
    QSpinBox *spinbox1;
    QRadioButton *radioButton1;    
    QDoubleSpinBox *spinbox2_1;
    QSpinBox *spinbox2_2;
    QRadioButton *radioButton2;
    QDoubleSpinBox *spinbox3_1;
    QDoubleSpinBox *spinbox3_2;
    QRadioButton *radioButton3;    
    QRadioButton *radioButton4;    
    QRadioButton *radioButton5;
    QSpinBox *spinbox6_1;
    QSpinBox *spinbox6_2;
    QRadioButton *radioButton6;
    QDoubleSpinBox *spinbox7;
    QRadioButton *radioButton7;
    QDoubleSpinBox *spinbox8_1;
    QDoubleSpinBox *spinbox8_2;
    QRadioButton *radioButton8;
    //菜单命令
    QAction *saveAsAct;
    QAction *changeToGrayAct;
    QAction *changeToBitplaneAct[8];
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
};

#endif // IMAGEPROCESSER_H
