#include "imageprocesser.h"
#include <QtWidgets>
#include <QtCharts>

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

    setImage();

    setWindowFilePath(fileName);

    const QString message = tr("已打开 \"%1\", %2x%3, Depth: %4")
        .arg(QDir::toNativeSeparators(fileName)).arg(image.width()).arg(image.height()).arg(image.depth());
    statusBar()->showMessage(message);
    return true;
}
void ImageProcesser::setImage(){
    //将图像显示在imageLabel上
    showingImage = image;
    imageLabel->setPixmap(QPixmap::fromImage(showingImage));
    //以下控件可见或启用
    scrollArea->setVisible(true);
    saveAsAct->setEnabled(true);
    changeToGrayAct->setEnabled(true);
    for(int i=0; i<8; i++)
        changeToBitplaneAct[i]->setEnabled(true);

    showHistogram();//构建直方图

    showGrayInfo();

    radioButton->setVisible(true);
    //showSpinBox();

    imageLabel->adjustSize();//imageLabel的大小可调整
}

//保存图片
bool ImageProcesser::saveFile(const QString &fileName)
{
    QImageWriter writer(fileName);

    if (!writer.write(showingImage)) {
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
    if(!isChecked){//若未被选择
        showingImage = image;
        imageLabel->setPixmap(QPixmap::fromImage(showingImage));//显示原图
    }
    else{//若被选择
        showingImage = grayimage;
        imageLabel->setPixmap(QPixmap::fromImage(showingImage));//显示灰度图
    }
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
    int index = 0;
    showingImage = bitplaneimage[index];
    imageLabel->setPixmap(QPixmap::fromImage(showingImage));
    for(int i=0; i<8; i++){
        if(i!=index) changeToBitplaneAct[i]->setChecked(false);
    }
}
void ImageProcesser::changeToBitplane2(){
    int index = 1;
    showingImage = bitplaneimage[index];
    imageLabel->setPixmap(QPixmap::fromImage(showingImage));
    for(int i=0; i<8; i++){
        if(i!=index) changeToBitplaneAct[i]->setChecked(false);
    }
}
void ImageProcesser::changeToBitplane3(){
    int index = 2;
    showingImage = bitplaneimage[index];
    imageLabel->setPixmap(QPixmap::fromImage(showingImage));
    for(int i=0; i<8; i++){
        if(i!=index) changeToBitplaneAct[i]->setChecked(false);
    }}
void ImageProcesser::changeToBitplane4(){
    int index = 3;
    showingImage = bitplaneimage[index];
    imageLabel->setPixmap(QPixmap::fromImage(showingImage));
    for(int i=0; i<8; i++){
        if(i!=index) changeToBitplaneAct[i]->setChecked(false);
    }}
void ImageProcesser::changeToBitplane5(){
    int index = 4;
    showingImage = bitplaneimage[index];
    imageLabel->setPixmap(QPixmap::fromImage(showingImage));
    for(int i=0; i<8; i++){
        if(i!=index) changeToBitplaneAct[i]->setChecked(false);
    }}
void ImageProcesser::changeToBitplane6(){
    int index = 5;
    showingImage = bitplaneimage[index];
    imageLabel->setPixmap(QPixmap::fromImage(showingImage));
    for(int i=0; i<8; i++){
        if(i!=index) changeToBitplaneAct[i]->setChecked(false);
    }}
void ImageProcesser::changeToBitplane7(){
    int index = 6;
    showingImage = bitplaneimage[index];
    imageLabel->setPixmap(QPixmap::fromImage(showingImage));
    for(int i=0; i<8; i++){
        if(i!=index) changeToBitplaneAct[i]->setChecked(false);
    }
}
void ImageProcesser::changeToBitplane8(){
    int index = 7;
    showingImage = bitplaneimage[index];
    imageLabel->setPixmap(QPixmap::fromImage(showingImage));
    for(int i=0; i<8; i++){
        if(i!=index) changeToBitplaneAct[i]->setChecked(false);
    }
}
