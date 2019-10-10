#ifndef IMAGEPROCESSER_H
#define IMAGEPROCESSER_H


#include <QMainWindow>
#include <QImage>
#include <QPainter>

//一些要用到的类
QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
QT_END_NAMESPACE

class ImageProcesser : public QMainWindow
{
    Q_OBJECT

public:
    ImageProcesser();//构造函数
    bool loadFile(const QString &);
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
private:
    //私有函数
    void createActions();//创造命令
    bool saveFile(const QString &filename);
    void setImage(const QImage &newImage);
    void toGray();
    void toBitplane();

    //实体成员
    QImage image;//原图像
    QImage grayimage;//灰度图像
    QImage bitplaneimage[8];//8幅位平面二值图
    QLabel *imageLabel;
    QScrollArea *scrollArea;

    //一些要用到的命令
    QAction *saveAsAct;
    QAction *changeToGrayAct;
    QAction *changeToBitplaneAct[8];
};

#endif // IMAGEPROCESSER_H
