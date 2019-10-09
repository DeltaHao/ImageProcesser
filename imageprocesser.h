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
    //void about();//显示“关于”信息
private:
    //私有函数
    void createActions();//创造命令
    void createMenus();//创造菜单界面
    bool saveFile(const QString &filename);
    void setImage(const QImage &newImage);

    //实体成员
    QImage image;
    QLabel *imageLabel;
    QScrollArea *scrollArea;

    //一些要用到的命令
    QAction *saveAsAct;
};

#endif // IMAGEPROCESSER_H
