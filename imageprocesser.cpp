#include "imageprocesser.h"
#include <QtWidgets>

//构造函数
ImageProcesser::ImageProcesser():
    imageLabel(new QLabel), scrollArea(new QScrollArea){
    imageLabel->setBackgroundRole(QPalette::Base);//设置背景颜色
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);//设置大小可调整
    imageLabel->setScaledContents(true);//设置窗口自适应

    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);//将scrollArea中的内容设置为imageLabel
    scrollArea->setVisible(false);//初始不可见
    setCentralWidget(scrollArea);//将窗口中心部件设置为scrollArea

    createActions();

    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);//将窗口设置为默认的3/5大小
}

//创建行为（功能）函数
void ImageProcesser::createActions(){
    //菜单栏-文件
    QMenu *fileMenu = menuBar()->addMenu(tr("&文件"));
    //打开按钮
    QAction *openAct = fileMenu->addAction(tr("&打开..."), this, &ImageProcesser::open);
    openAct->setShortcut(QKeySequence::Open);//快捷键设置为"open"的快捷键
    //另存为按钮
    saveAsAct = fileMenu->addAction(tr("&另存为..."), this, &ImageProcesser::saveAs);
    saveAsAct->setShortcut(QKeySequence::SaveAs);
    saveAsAct->setEnabled(false);//初始不可用(未打开时)

    fileMenu->addSeparator();//分隔符
    //关闭按钮
    QAction *exitAct = fileMenu->addAction(tr("&关闭"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    //菜单栏-处理
    QMenu *processMenu = menuBar()->addMenu(tr("&处理"));
    //转为灰度图
    changeToGrayAct = processMenu->addAction(tr("&查看灰度图"), this, &ImageProcesser::changeToGray);
    changeToGrayAct->setEnabled(false);
    changeToGrayAct->setCheckable(true);

    //菜单栏-关于
    QMenu *aboutMenu = menuBar()->addMenu(tr("&关于"));
    QAction *about1Act = aboutMenu->addAction(tr("&关于作者..."), this, &ImageProcesser::about1);
    QAction *about2Act = aboutMenu->addAction(tr("&项目介绍..."), this, &ImageProcesser::about2);
}

//初始化文件选择对话框
static void initializeImageFileDialog(QFileDialog &dialog){
    static bool firstDialog = true;

    if (firstDialog) {//如果是第一次open
        firstDialog = false;
        //打开系统的默认图片路径(如果没有就打开当前文件路径)
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }
    //文件过滤器
    QStringList filters;
    filters.append("ALL Images (*.bmp *.gif *.jpg *png)");
    filters.append("jpg Images (*.jpg)");
    filters.append("png Images (*.png)");
    filters.append(" gifImages (*.gif)");
    dialog.setNameFilters(filters);
}
//打开图片
void ImageProcesser::open(){
    QFileDialog dialog(this, tr("打开图片"));
    initializeImageFileDialog(dialog);
    while(dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())){}
}
//加载图片
bool ImageProcesser::loadFile(const QString &fileName){
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    image = reader.read();
    if(image.isNull()){//如果图片不能打开， 报错
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }

    setImage(image);

    setWindowFilePath(fileName);

    const QString message = tr("已打开 \"%1\", %2x%3, Depth: %4")
        .arg(QDir::toNativeSeparators(fileName)).arg(image.width()).arg(image.height()).arg(image.depth());
    statusBar()->showMessage(message);
    return true;
}
//将图像显示在imageLabel上
void ImageProcesser::setImage(const QImage &newImage){
    //将图像显示在imageLabel上

    imageLabel->setPixmap(QPixmap::fromImage(newImage));
    //以下控件可见或启用
    scrollArea->setVisible(true);
    saveAsAct->setEnabled(true);
    changeToGrayAct->setEnabled(true);

    imageLabel->adjustSize();//imageLabel的大小可调整
}
//保存图片
bool ImageProcesser::saveFile(const QString &fileName)
{
    QImageWriter writer(fileName);

    if (!writer.write(image)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("无法写入 %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName)), writer.errorString());
        return false;
    }
    const QString message = tr("已保存 \"%1\"").arg(QDir::toNativeSeparators(fileName));
    statusBar()->showMessage(message);
    return true;
}
//图片另存为(弹出窗口)
void ImageProcesser::saveAs()
{
    QFileDialog dialog(this, tr("图片另存为"));
    initializeImageFileDialog(dialog);

    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().first())) {}
}
//关于
void ImageProcesser::about1(){
    QMessageBox:: about(this, tr("作者"),tr("<p>作者：郝正亮</p><p>学号：1120172919</p>"));
}
void ImageProcesser::about2(){
    QMessageBox:: about(this, tr("项目介绍"),tr("<p>《数字图像处理》课程作业</p> 。。。"));
}
//转化为灰度图像
void ImageProcesser::toGray(const QImage image){
    if(!grayimage.isNull())
        setImage(grayimage);
    int width = image.width();
    int height = image.height();
    QImage tmp(width, height, QImage::Format_Indexed8);//先建立一个同等尺寸的8位灰度图
    //如果原图本来就是灰度图,直接拷贝
    if(image.format() == QImage::Format_Indexed8){
        tmp = image;
    }
    //设置灰度表
    tmp.setColorCount(256);
    for(int i=0;i<256;i++){
        tmp.setColor(i,qRgb(i,i,i));
    }
    for(int i =0;i<width;i++) {
         for(int j=0;j<height;j++)  {
            QRgb pixel=image.pixel(i,j);
            int r=qRed(pixel);
            int g=qGreen(pixel);
            int b=qBlue(pixel);
            tmp.setPixel(i,j,qGray(r,g,b));
         }
    }
    grayimage = tmp;
    setImage(grayimage);
}
void ImageProcesser::changeToGray(){
    bool isChecked = changeToGrayAct->isChecked();
    if(!isChecked)
        setImage(image);
    else
        toGray(image);
}
