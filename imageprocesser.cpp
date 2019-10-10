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
    changeToGrayAct = processMenu->addAction(tr("&灰度图/RGB图"), this, &ImageProcesser::changeToGray);
    changeToGrayAct->setEnabled(false);
    changeToGrayAct->setCheckable(true);
    //转为位平面图
    changeToBitplaneAct[0] = processMenu->addAction(tr("&位平面-1"), this, &ImageProcesser::changeToBitplane1);
    changeToBitplaneAct[0]->setEnabled(false);
    changeToBitplaneAct[1] = processMenu->addAction(tr("&位平面-2"), this, &ImageProcesser::changeToBitplane2);
    changeToBitplaneAct[1]->setEnabled(false);
    changeToBitplaneAct[2] = processMenu->addAction(tr("&位平面-3"), this, &ImageProcesser::changeToBitplane3);
    changeToBitplaneAct[2]->setEnabled(false);
    changeToBitplaneAct[3] = processMenu->addAction(tr("&位平面-4"), this, &ImageProcesser::changeToBitplane4);
    changeToBitplaneAct[3]->setEnabled(false);
    changeToBitplaneAct[4] = processMenu->addAction(tr("&位平面-5"), this, &ImageProcesser::changeToBitplane5);
    changeToBitplaneAct[4]->setEnabled(false);
    changeToBitplaneAct[5] = processMenu->addAction(tr("&位平面-6"), this, &ImageProcesser::changeToBitplane6);
    changeToBitplaneAct[5]->setEnabled(false);
    changeToBitplaneAct[6] = processMenu->addAction(tr("&位平面-7"), this, &ImageProcesser::changeToBitplane7);
    changeToBitplaneAct[6]->setEnabled(false);
    changeToBitplaneAct[7] = processMenu->addAction(tr("&位平面-8"), this, &ImageProcesser::changeToBitplane8);
    changeToBitplaneAct[7]->setEnabled(false);

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

    toGray();//生成灰度图
    toBitplane();//生成位平面图

    setImage(image);

    setWindowFilePath(fileName);

    const QString message = tr("已打开 \"%1\", %2x%3, Depth: %4")
        .arg(QDir::toNativeSeparators(fileName)).arg(image.width()).arg(image.height()).arg(image.depth());
    statusBar()->showMessage(message);
    return true;
}
void ImageProcesser::setImage(const QImage &newImage){
    //将图像显示在imageLabel上

    imageLabel->setPixmap(QPixmap::fromImage(newImage));
    //以下控件可见或启用
    scrollArea->setVisible(true);
    saveAsAct->setEnabled(true);
    changeToGrayAct->setEnabled(true);
    for(int i=0; i<8; i++)
        changeToBitplaneAct[i]->setEnabled(true);

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
void ImageProcesser::toGray(){
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
}
void ImageProcesser::changeToGray(){
    bool isChecked = changeToGrayAct->isChecked();
    if(!isChecked)//若未被选择
        imageLabel->setPixmap(QPixmap::fromImage(image));//显示原图

    else//若被选择
        imageLabel->setPixmap(QPixmap::fromImage(grayimage));//显示灰度图
}

//将8位灰度图像转换为8幅位平面二值图
void ImageProcesser::toBitplane(){
    int width =  image.width();
    int height = image.height();

    QImage tmp(width, height, QImage::Format_Mono);
    for(int k=0;k<8;k++){
        bitplaneimage[k] = tmp;
        for(int i =0;i<width;i++) {
             for(int j=0;j<height;j++)  {
                int pixel = grayimage.pixelIndex(i, j);
                bitplaneimage[k].setPixel(i,j,((pixel>>k) & 1));
             }
        }
    }
}
void ImageProcesser::changeToBitplane1(){
    imageLabel->setPixmap(QPixmap::fromImage(bitplaneimage[0]));
}
void ImageProcesser::changeToBitplane2(){
    imageLabel->setPixmap(QPixmap::fromImage(bitplaneimage[1]));
}
void ImageProcesser::changeToBitplane3(){
    imageLabel->setPixmap(QPixmap::fromImage(bitplaneimage[2]));
}
void ImageProcesser::changeToBitplane4(){
    imageLabel->setPixmap(QPixmap::fromImage(bitplaneimage[3]));
}
void ImageProcesser::changeToBitplane5(){
    imageLabel->setPixmap(QPixmap::fromImage(bitplaneimage[4]));
}
void ImageProcesser::changeToBitplane6(){
    imageLabel->setPixmap(QPixmap::fromImage(bitplaneimage[5]));
}
void ImageProcesser::changeToBitplane7(){
    imageLabel->setPixmap(QPixmap::fromImage(bitplaneimage[6]));
}
void ImageProcesser::changeToBitplane8(){
    imageLabel->setPixmap(QPixmap::fromImage(bitplaneimage[7]));
}
