/*菜单动作*/

#include "imageprocesser.h"

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

//-----文件栏-----------------------------------------
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
void ImageProcesser::toGray(){
    //生成灰度图像
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
    calculateGrayInfo(grayimage, grayInfo);//计算并保存灰度图相关信息
}
void ImageProcesser::toBitplane(){
    //生成8幅位平面二值图
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
void ImageProcesser::setImage(){
    hideSpinBoxes();

    //将图像显示在imageLabel上
    scaleFactor = 1.0;
    showingImage = image;
    imageLabel->setPixmap(QPixmap::fromImage(showingImage));
    imageLabel->adjustSize();//imageLabel的大小可调整
    showHistogram(grayimage);//构建直方图

    //以下控件可见或启用
    scrollArea->setVisible(true);
    saveAsAct->setEnabled(true);
    changeToGrayAct->setEnabled(true);
    zoomInAct->setEnabled(true);
    zoomOutAct->setEnabled(true);
    normalSizeAct->setEnabled(true);
    for(int i=0; i<8; i++)
        changeToBitplaneAct[i]->setEnabled(true);
    radioButton0->setVisible(true);
    radioButton1->setVisible(true);
    radioButton2->setVisible(true);
    radioButton3->setVisible(true);
    radioButton4->setVisible(true);
    radioButton5->setVisible(true);
    radioButton6->setVisible(true);
    radioButton7->setVisible(true);
    radioButton8->setVisible(true);
    radioButton9->setVisible(true);
    radioButton10->setVisible(true);
    radioButton11->setVisible(true);
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

//------处理栏--------------------------------------
//显示原图
void ImageProcesser::showOriginal(){
    hideSpinBoxes();

    showingImage = image;
    imageLabel->setPixmap(QPixmap::fromImage(showingImage));//显示原图
    imageLabel->adjustSize();

    showHistogram(grayimage);
}
//显示位平面图
void ImageProcesser::changeToBitplane1(){
    int index = 0;
    showImage(bitplaneimage[index]);
    GrayInfo->setVisible(false);//隐藏灰度信息（因为二值图计算出来是错的）

    for(int i=0; i<8; i++){
        if(i!=index) changeToBitplaneAct[i]->setChecked(false);
    }
}
void ImageProcesser::changeToBitplane2(){
    int index = 1;
    showImage(bitplaneimage[index]);
    GrayInfo->setVisible(false);//隐藏灰度信息（因为二值图计算出来是错的）

    for(int i=0; i<8; i++){
        if(i!=index) changeToBitplaneAct[i]->setChecked(false);
    }
}
void ImageProcesser::changeToBitplane3(){
    int index = 2;
    showImage(bitplaneimage[index]);
    GrayInfo->setVisible(false);//隐藏灰度信息（因为二值图计算出来是错的）

    for(int i=0; i<8; i++){
        if(i!=index) changeToBitplaneAct[i]->setChecked(false);
    }
}
void ImageProcesser::changeToBitplane4(){
    int index = 3;
    showImage(bitplaneimage[index]);
    GrayInfo->setVisible(false);//隐藏灰度信息（因为二值图计算出来是错的）

    for(int i=0; i<8; i++){
        if(i!=index) changeToBitplaneAct[i]->setChecked(false);
    }
}
void ImageProcesser::changeToBitplane5(){
    int index = 4;
    showImage(bitplaneimage[index]);
    GrayInfo->setVisible(false);//隐藏灰度信息（因为二值图计算出来是错的）

    for(int i=0; i<8; i++){
        if(i!=index) changeToBitplaneAct[i]->setChecked(false);
    }
}
void ImageProcesser::changeToBitplane6(){
    int index = 5;
    showImage(bitplaneimage[index]);
    GrayInfo->setVisible(false);//隐藏灰度信息（因为二值图计算出来是错的）

    for(int i=0; i<8; i++){
        if(i!=index) changeToBitplaneAct[i]->setChecked(false);
    }
}
void ImageProcesser::changeToBitplane7(){
    int index = 6;
    showImage(bitplaneimage[index]);
    GrayInfo->setVisible(false);//隐藏灰度信息（因为二值图计算出来是错的）

    for(int i=0; i<8; i++){
        if(i!=index) changeToBitplaneAct[i]->setChecked(false);
    }
}
void ImageProcesser::changeToBitplane8(){
    int index = 7;
    showImage(bitplaneimage[index]);
    GrayInfo->setVisible(false);//隐藏灰度信息（因为二值图计算出来是错的）

    for(int i=0; i<8; i++){
        if(i!=index) changeToBitplaneAct[i]->setChecked(false);
    }
}

//------显示栏--------------------------------------
//调整显示大小
void ImageProcesser::scaleImage(double factor){
    //调整显示大小
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());
    //调整滚动框
    QScrollBar *scrollBar = scrollArea->horizontalScrollBar();
    scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)));;
    scrollBar = scrollArea->verticalScrollBar();
    scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)));;
    //最多放大或缩小5倍
    zoomInAct->setEnabled(scaleFactor < 5.0);
    zoomOutAct->setEnabled(scaleFactor > 0.2);
}
void ImageProcesser::zoomIn(){
    scaleImage(1.2);
}
void ImageProcesser::zoomOut(){
    scaleImage(0.8);
}
void ImageProcesser::normalSize(){
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

//------关于栏--------------------------------------
//关于
void ImageProcesser::about1(){
    QMessageBox:: about(this, tr("作者"),tr("<p>作者：郝正亮</p>"
                                          "<p>学号：1120172919</p>"
                                          ));
}
void ImageProcesser::about2(){
    QMessageBox:: about(this, tr("项目介绍"),tr("\
                                            <p>编程实现不同采样率和不同量化等级图像的显示效果。要求：</p>\
                                                                                        <ul>\
                                                                                        <li><del>只能使用C、C++、Java或Delphi等编程语言</del></li>\
                                                                                        <li><del>要求提供图形化显示界面</del></li>\
                                                                                        <li><del>要求有图像载入、保存和相关处理选项功能</del></li>\
                                                                                        <li><del>可以只考虑灰度图像</del></li>\
                                                                                        <li><del>编写代码，把8位灰度图转化为8幅位平面表示的二值图。</del></li>\
                                                                                        <li><del>在前两章的作业基础之上，添加直方图显示功能，并在直方图下方显示相关信息，如平均灰度、中值灰度、标准差和像素总数等</del></li>\
                                                                                        <li><del>同时，提供灵活交互方式确定阈值灰度，对输入图像进行阈值化产生一幅二值图像，要实时显示。（自己提供验证图像）</del></li>\
                                                                                        <li><del>在已有的程序框架上完成点运算，要求支持不少于两种线性和非线性的变换，要能显示变换前后的图像灰度直方图。</del></li>\
                                                                                        <li><del>对第二章之后的作业处理结果都支持直方图显示功能</del></li>\
                                                                                        <li>编写程序实现灰度直方图均衡算法，并参考相关文献，对传统的直方图均衡方法进行优化，给出优化前后的直方图比较。<li>\
                                                                                        <li>在原有程序框架上，根据输入条件，实现简单空间变换（包括指定任意参数的缩放、旋转和平移），要求提供最近邻和双线性至少两种灰度差值方法<li>\
                                                                                        <li>支持不少于3种模板的图像平滑（如均值、中值、K邻域、最小均方差等）<li>\
                                                                                        <li>支持不少于3种模板的图像锐化（如Roberts、Sobel、拉普拉斯等）<li>\
                                                                                        <li>给定任意模板（模板大小和权值可自由指定），计算其与图像的卷积并显示结果<li>\
                                                                                        <li>完成对彩色图像的灰度化处理。<li>\
                                                                                        <li>尝试编写代码实现把24位真彩色图像转换为256色彩色图像。<li>\
                                                                                        <li>在已有程序框架上，加入sobel算子、 Prewitt算子和拉普拉斯算子实现边缘检测，并利用边缘跟踪法实现对边缘点的闭合操作。（自己找示例图，最好是灰度图像）<li>\
                                                                                        <li>编写程序，实现霍夫变换检测直线。<li>\
                                                                                        </ul>"
                                            "<p>项目源码：https://github.com/DeltaHao/ImageProcesser</p>"));
}
