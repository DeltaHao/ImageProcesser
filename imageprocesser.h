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

private slots://槽函数
//内联函数
    void showOriginal();//显示原图
    void showGrayImage();//显示灰度图
    void showImage(QImage);//更新画面
    void hideSpinBoxes();//隐藏所有调节框
    void confirmChange();//确认改变
    void revokeChange();//撤销改变
//---menuActions.cpp---
    void open();//打开图片
    void saveAs();//另存为    
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
    void about1();//显示“关于作者”信息
    void about2();//显示“关于软件”信息
//---pointOperation.cpp---
    void showIndex8Image();//显示256色彩图
    void showBalanceImage();//显示均衡处理后的图片
    void showNewBalanceImage();//显示优化均衡处理后的图片
    void showSpinBox1();//显示阈值灰度调节框
    void showBinaryImage(int threshold);//根据阈值灰度生成二值图
    void showSpinBox2();//显示线性变换调节框
    void showlinearImage1(double a);//对比度变化
    void showlinearImage2(int b);//亮度变化
    void showSpinBox3();//显示非线性变换调节框
    void showUnlinearImage1(double a);//中值对比度
    void showUnlinearImage2(double b);//暗部增强
//---geomestricOperation.cpp---
    void showSpinBox6();//显示平移调节框
    void showTranslation1(int);//x轴平移
    void showTranslation2(int);//y轴平移
    void showSpinBox7();//显示旋转调节框
    void showRotation(double);//旋转
    void showSpinBox8();//显示缩放调节框
    void nearstInterpolation(double);//最近邻插值法缩放
    void bilinearInterpolation(double);//双线性插值法缩放
//---enhancement.cpp---
    void meanFilter();//均值滤波 - 平滑
    void medianFilter();//中值滤波 - 平滑
    void gaussFilter();//高斯滤波 - 平滑
    void RobertsSharpen();//Roberts锐化
    void SobelSharpen();//Sobel锐化
    void LaplacianSharpen();//Laplacian锐化
    void showTemplateEdit();//显示模板编辑框和确认按钮
    void toConvolution();//根据模板求卷积
//---segmentation.cpp---
    void SobelEdge();
    void PrewittEdge();
    void LaplacianEdge();
    void HoughTransform();
private://私有函数
//---imageprocesser.cpp---
    void createMenuActions();//生成菜单栏
    QLabel *getSeparator();//生成分割线
//---menuAction.cpp---
    bool loadFile(const QString &);
    bool saveFile(const QString &filename);
    void toGray();
    void toBitplane();
    void setImage();
    void scaleImage(double factor);//缩放显示图像
//---histogram.cpp---
    void calculateGrayInfo(QImage, double*);//计算灰度的信息
    void showHistogram(QImage);//显示灰度直方图            

//图像及其相关信息
    double scaleFactor;//图像的显示比例
    QImage image;//原图像

    QImage grayimage;//灰度图像
    double grayInfo[4];//灰度图像的四个相关信息：平均灰度、中值灰度、标准差和像素总数

    QImage bitplaneimage[8];//8幅位平面二值图
    QImage tempImage;//暂存图像

    QImage showingImage;//正在展示的图像
    double showinggrayInfo[4];//正在展示的灰度图像的四个相关信息：平均灰度、中值灰度、标准差和像素总数
//控件
    QWidget *widget;    
    QLabel *imageLabel;    
    QScrollArea *scrollArea;    
    QChartView *chartview;    
    QLabel *GrayInfo;

    //根据阈值灰度生成二值图
    QSpinBox *spinbox1;
    //线性变换
    QDoubleSpinBox *spinbox2_1;
    QSpinBox *spinbox2_2;
    //非线性变换
    QDoubleSpinBox *spinbox3_1;
    QDoubleSpinBox *spinbox3_2;
    //平移
    QSpinBox *spinbox6_1;
    QSpinBox *spinbox6_2;
    //旋转
    QDoubleSpinBox *spinbox7;
    QRadioButton *radioButton7;
    //缩放
    QDoubleSpinBox *spinbox8_1;
    QDoubleSpinBox *spinbox8_2;
    //给定模板求卷积
    QPushButton *confrimEdit;
    QTextEdit *templateEdit;

    //菜单命令
    QAction *saveAsAct;
    QAction *changeToGrayAct;
    QAction *changeToBitplaneAct[8];
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
};

#endif // IMAGEPROCESSER_H
//显示灰度图
inline void ImageProcesser::showGrayImage(){
    hideSpinBoxes();
    showImage(grayimage);
}
//显示原图
inline void ImageProcesser::showOriginal(){
    hideSpinBoxes();

    showingImage = image;
    imageLabel->setPixmap(QPixmap::fromImage(showingImage));//显示原图
    imageLabel->adjustSize();

    showHistogram(grayimage);
}
//隐藏其它输入框
inline void ImageProcesser::hideSpinBoxes(){
    spinbox1->setVisible(false);
    spinbox2_1->setVisible(false);
    spinbox2_2->setVisible(false);
    spinbox3_1->setVisible(false);
    spinbox3_2->setVisible(false);
    spinbox6_1->setVisible(false);
    spinbox6_2->setVisible(false);
    spinbox7->setVisible(false);
    spinbox8_1->setVisible(false);
    spinbox8_2->setVisible(false);
    templateEdit->setEnabled(false);
    confrimEdit->setEnabled(false);
}
//更新画面
inline void ImageProcesser::showImage(QImage img){
    showingImage = img;
    imageLabel->setPixmap(QPixmap::fromImage(img));
    imageLabel->adjustSize();//imageLabel的大小可调整
    showHistogram(img);//显示直方图
}

inline void ImageProcesser::confirmChange(){
    if(showingImage.format() == QImage::Format_Indexed8)
        grayimage = showingImage;
    spinbox7->setValue(0);
    statusBar()->showMessage(" 已暂存中间结果");
}
inline void ImageProcesser::revokeChange(){
    grayimage = tempImage;
    spinbox7->setValue(0);
    showGrayImage();
    statusBar()->showMessage("");
}
