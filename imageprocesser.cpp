/*构造函数*/

#include "imageprocesser.h"

//构造函数
ImageProcesser::ImageProcesser():
    scaleFactor(1),
    widget(new QWidget),
    imageLabel(new QLabel),
    scrollArea(new QScrollArea),
    chartview(new QChartView),
    GrayInfo(new QLabel)
{
    setCentralWidget(widget);//设置窗口中心部件
    resize(1250, 730);//设置窗口大小  
    statusBar()->showMessage(" 请从\"文件-打开\"打开图片");//设置提示栏
    createMenuActions();//生成菜单栏
    showHistogram(image);//生成灰度直方图
//初始化控件
    //图片Label
    imageLabel->setBackgroundRole(QPalette::Base);//设置背景颜色
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);//设置大小可调整
    imageLabel->setScaledContents(true);//设置窗口自适应
    //滚动框
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);//将scrollArea中的内容设置为imageLabel
    scrollArea->setAlignment(Qt::AlignCenter); 


//设置布局
    //“显示灰度图”按钮
    QRadioButton *radioButton0 = new QRadioButton("灰度图");
    connect(radioButton0, SIGNAL(clicked()), this, SLOT(showGrayImage()));
    //根据阈值灰度生成二值图
    QRadioButton *radioButton1 = new QRadioButton("根据阈值灰度生成二值图：");
    spinbox1 = new QSpinBox;
    connect(radioButton1, SIGNAL(clicked()), this, SLOT(showSpinBox1()));
    connect(spinbox1, SIGNAL(valueChanged(int)), this, SLOT(showBinaryImage(int)));
    //线性变换
    QRadioButton *radioButton2 = new QRadioButton("线性变换：");
    spinbox2_1 = new QDoubleSpinBox;
    spinbox2_2 = new QSpinBox;
    connect(radioButton2, SIGNAL(clicked()), this, SLOT(showSpinBox2()));
    connect(spinbox2_1, SIGNAL(valueChanged(double)), this, SLOT(showlinearImage1(double)));
    connect(spinbox2_2, SIGNAL(valueChanged(int)), this, SLOT(showlinearImage2(int)));
    //非线性变换
    QRadioButton *radioButton3 = new QRadioButton("非线性变换：");
    spinbox3_1 = new QDoubleSpinBox;
    spinbox3_2 = new QDoubleSpinBox;
    connect(radioButton3, SIGNAL(clicked()), this, SLOT(showSpinBox3()));
    connect(spinbox3_1, SIGNAL(valueChanged(double)), this, SLOT(showUnlinearImage1(double)));
    connect(spinbox3_2, SIGNAL(valueChanged(double)), this, SLOT(showUnlinearImage2(double)));
    //“均衡处理”按钮
    QRadioButton *radioButton4 = new QRadioButton("传统均衡");//均衡处理
    QRadioButton *radioButton5 = new QRadioButton("优化均衡");//优化均衡
    connect(radioButton4, SIGNAL(clicked()), this, SLOT(showBalanceImage()));
    connect(radioButton5, SIGNAL(clicked()), this, SLOT(showNewBalanceImage()));
    //平移
    QRadioButton *radioButton6 = new QRadioButton("图像平移：");
    spinbox6_1 = new QSpinBox;
    spinbox6_2 = new QSpinBox;
    connect(radioButton6, SIGNAL(clicked()), this, SLOT(showSpinBox6()));
    connect(spinbox6_1, SIGNAL(valueChanged(int)), this, SLOT(showTranslation1(int)));
    connect(spinbox6_2, SIGNAL(valueChanged(int)), this, SLOT(showTranslation2(int)));
    //旋转
    QRadioButton *radioButton7 = new QRadioButton("图像旋转：");
    spinbox7 = new QDoubleSpinBox;
    connect(radioButton7, SIGNAL(clicked()), this, SLOT(showSpinBox7()));
    connect(spinbox7, SIGNAL(valueChanged(double)), this, SLOT(showRotation(double)));
    //缩放
    QRadioButton *radioButton8 = new QRadioButton("图像缩放：");
    spinbox8_1 = new QDoubleSpinBox;
    spinbox8_2 = new QDoubleSpinBox;
    connect(radioButton8, SIGNAL(clicked()), this, SLOT(showSpinBox8()));
    connect(spinbox8_1, SIGNAL(valueChanged(double)), this, SLOT(nearstInterpolation(double)));
    connect(spinbox8_2, SIGNAL(valueChanged(double)), this, SLOT(bilinearInterpolation(double)));
    //平滑
    QRadioButton *radioButton9 = (new QRadioButton("均值平滑"));
    QRadioButton *radioButton10 = (new QRadioButton("中值平滑"));
    QRadioButton *radioButton11 = (new QRadioButton("高斯平滑"));
    connect(radioButton9, SIGNAL(clicked()), this, SLOT(meanFilter()));
    connect(radioButton10, SIGNAL(clicked()), this, SLOT(medianFilter()));
    connect(radioButton11, SIGNAL(clicked()), this, SLOT(gaussFilter()));
    //锐化
    QRadioButton *radioButton12 = (new QRadioButton("锐化(Roberts)"));
    QRadioButton *radioButton13 = (new QRadioButton("锐化(Sobel)"));
    QRadioButton *radioButton14 = (new QRadioButton("锐化(Laplace)"));
    connect(radioButton12, SIGNAL(clicked()), this, SLOT(RobertsSharpen()));
    connect(radioButton13, SIGNAL(clicked()), this, SLOT(SobelSharpen()));
    connect(radioButton14, SIGNAL(clicked()), this, SLOT(LaplacianSharpen()));
    //给定模板求卷积
    QRadioButton *radioButton15 = new QRadioButton("根据模板求卷积：");
    confrimEdit = new QPushButton("确认");
    templateEdit = new QTextEdit;
    connect(radioButton15, SIGNAL(clicked()), this, SLOT(showTemplateEdit()));
    connect(confrimEdit, SIGNAL(clicked()), this, SLOT(toConvolution()));

    //显示256色彩图
    QRadioButton *radioButton16 = new QRadioButton("256色彩图");
    connect(radioButton16, SIGNAL(clicked()), this, SLOT(showIndex8Image()));

    //边缘跟踪
    QRadioButton *radioButton17 = new QRadioButton("边缘检测(Sobel)");
    connect(radioButton17, SIGNAL(clicked()), this, SLOT(SobelEdge()));
    QRadioButton *radioButton18 = new QRadioButton("边缘检测(Prewitt)");
    connect(radioButton18, SIGNAL(clicked()), this, SLOT(PrewittEdge()));
    QRadioButton *radioButton19 = new QRadioButton("边缘检测(Laplace)");
    connect(radioButton19, SIGNAL(clicked()), this, SLOT(LaplacianEdge()));
    QRadioButton *radioButton20 = new QRadioButton("霍夫变换");
    connect(radioButton20, SIGNAL(clicked()), this, SLOT(HoughTransform()));

    //原图
    QRadioButton *radioButton21 = new QRadioButton("原图");
    connect(radioButton21, SIGNAL(clicked()), this, SLOT(showOriginal()));

    //暂存、撤销
    QPushButton *confirm = new QPushButton("暂存(Ctrl+S)");
    QPushButton *revoke = new QPushButton("撤销暂存(Ctrl+Z)");
    connect(confirm, SIGNAL(clicked()), this, SLOT(confirmChange()));
    confirm->setShortcut(tr("Ctrl+S"));
    connect(revoke, SIGNAL(clicked()), this, SLOT(revokeChange()));
    revoke->setShortcut(tr("Ctrl+Z"));

    hideSpinBoxes();//隐藏各种输入框

//设置布局
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(chartview, 0, 3, 3, 3);
    mainLayout->addWidget(GrayInfo, 1, 3, 1, 3);
    int no = 3;

    mainLayout->addWidget(getSeparator(), no, 3, 2, 3);
    mainLayout->addWidget(radioButton21, no, 3, 1, 1);
    mainLayout->addWidget(radioButton16, no, 4, 1, 1);
    mainLayout->addWidget(radioButton0, no++, 5, 1, 1);


    mainLayout->addWidget(radioButton4, no, 3, 1, 1);
    mainLayout->addWidget(radioButton5, no++, 4, 1, 1);



    mainLayout->addWidget(radioButton1, no, 3, 1, 1);
    mainLayout->addWidget(spinbox1, no++, 5, 1, 1);

    mainLayout->addWidget(radioButton2, no, 3, 1, 1);
    mainLayout->addWidget(spinbox2_1, no, 4, 1, 1);
    mainLayout->addWidget(spinbox2_2, no++, 5, 1, 1);

    mainLayout->addWidget(getSeparator(), no, 3, 2, 3);
    mainLayout->addWidget(radioButton3, no, 3, 1, 1);
    mainLayout->addWidget(spinbox3_1, no, 4, 1, 1);
    mainLayout->addWidget(spinbox3_2, no++, 5, 1, 1);


    mainLayout->addWidget(radioButton6, no, 3, 1, 1);
    mainLayout->addWidget(spinbox6_1, no, 4, 1, 1);
    mainLayout->addWidget(spinbox6_2, no++, 5, 1, 1);

    mainLayout->addWidget(radioButton7, no, 3, 1, 1);
    mainLayout->addWidget(spinbox7, no++, 5, 1, 1);

    mainLayout->addWidget(getSeparator(), no, 3, 2, 3);
    mainLayout->addWidget(radioButton8, no, 3, 1, 1);
    mainLayout->addWidget(spinbox8_1, no, 4, 1, 1);
    mainLayout->addWidget(spinbox8_2, no++, 5, 1, 1);

    mainLayout->addWidget(radioButton9, no, 3, 1, 1);
    mainLayout->addWidget(radioButton10, no, 4, 1, 1);
    mainLayout->addWidget(radioButton11, no++, 5, 1, 1);


    mainLayout->addWidget(radioButton12, no, 3, 1, 1);
    mainLayout->addWidget(radioButton13, no, 4, 1, 1);
    mainLayout->addWidget(radioButton14, no++, 5, 1, 1);

    mainLayout->addWidget(radioButton15, no++, 3, 1, 1);
    //mainLayout->addWidget(getSeparator(), no, 3, 3, 3);
    mainLayout->addWidget(confrimEdit, no++, 3, 1, 1);
    mainLayout->addWidget(templateEdit, no-2, 4, 2, 2);

    mainLayout->addWidget(radioButton17, no, 3, 1, 1);
    mainLayout->addWidget(radioButton18, no, 4, 1, 1);
    mainLayout->addWidget(radioButton19, no++, 5, 1, 1);

    mainLayout->addWidget(getSeparator(), no, 3, 2, 3);
    mainLayout->addWidget(radioButton20, no++, 3, 1, 1);

    mainLayout->addWidget(confirm, no, 3, 1, 1);
    mainLayout->addWidget(revoke, no++, 5, 1, 1);

    mainLayout->addWidget(scrollArea, 0, 0, no, 3);

    //设置行列比例
    mainLayout->setColumnStretch(0, 2);
    for(int i=1; i<6; i++)
        mainLayout->setColumnStretch(i, 1);
    mainLayout->setRowStretch(0, 19);
    for(int i=1; i<no; i++)
        mainLayout->setRowStretch(i, 1);

    widget->setLayout(mainLayout);

}

//生成菜单栏
void ImageProcesser::createMenuActions(){
//菜单栏-文件
    QMenu *fileMenu = menuBar()->addMenu(tr("&文件"));
    //打开按钮
    QAction *openAct = fileMenu->addAction(tr("&打开..."), this, &ImageProcesser::open);
    openAct->setShortcut(QKeySequence::Open);//快捷键设置为"open"的快捷键
    //另存为按钮
    saveAsAct = fileMenu->addAction(tr("&另存为..."), this, &ImageProcesser::saveAs);
    saveAsAct->setShortcut(QKeySequence::SaveAs);
    saveAsAct->setEnabled(false);//初始不可用(未打开时)
    //分隔符
    fileMenu->addSeparator();
    //关闭按钮
    QAction *exitAct = fileMenu->addAction(tr("&关闭"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

//菜单栏-处理
    QMenu *processMenu = menuBar()->addMenu(tr("&位平面"));
    //显示原图
    changeToGrayAct = processMenu->addAction(tr("&原图"), this, &ImageProcesser::showOriginal);
    changeToGrayAct->setEnabled(false);
    //分隔符
    processMenu->addSeparator();
    //转为位平面图
    changeToBitplaneAct[0] = processMenu->addAction(tr("&位平面-1"), this, &ImageProcesser::changeToBitplane1);
    changeToBitplaneAct[1] = processMenu->addAction(tr("&位平面-2"), this, &ImageProcesser::changeToBitplane2);
    changeToBitplaneAct[2] = processMenu->addAction(tr("&位平面-3"), this, &ImageProcesser::changeToBitplane3);
    changeToBitplaneAct[3] = processMenu->addAction(tr("&位平面-4"), this, &ImageProcesser::changeToBitplane4);
    changeToBitplaneAct[4] = processMenu->addAction(tr("&位平面-5"), this, &ImageProcesser::changeToBitplane5);
    changeToBitplaneAct[5] = processMenu->addAction(tr("&位平面-6"), this, &ImageProcesser::changeToBitplane6);
    changeToBitplaneAct[6] = processMenu->addAction(tr("&位平面-7"), this, &ImageProcesser::changeToBitplane7);
    changeToBitplaneAct[7] = processMenu->addAction(tr("&位平面-8"), this, &ImageProcesser::changeToBitplane8);
    for(int i=0; i<8; i++){
        changeToBitplaneAct[i]->setEnabled(false);
        changeToBitplaneAct[i]->setCheckable(true);
    }

//菜单栏-显示
    QMenu *viewMenu = menuBar()->addMenu(tr("&显示"));
    zoomInAct = viewMenu->addAction(tr("&放大(20%)"), this, &ImageProcesser::zoomIn);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setEnabled(false);
    zoomOutAct = viewMenu->addAction(tr("&缩小(20%)"), this, &ImageProcesser::zoomOut);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setEnabled(false);
    viewMenu->addSeparator();
    normalSizeAct = viewMenu->addAction(tr("原始大小"), this, &ImageProcesser::normalSize);
    normalSizeAct->setEnabled(false);

//菜单栏-关于
    QMenu *aboutMenu = menuBar()->addMenu(tr("&关于"));
    QAction *about1Act = aboutMenu->addAction(tr("&帮助..."), this, &ImageProcesser::about1);
    QAction *about2Act = aboutMenu->addAction(tr("&项目介绍..."), this, &ImageProcesser::about2);
}
//生成边框
QLabel *ImageProcesser::getSeparator()
{
    QLabel *label = new QLabel();
    label->setFrameStyle(QFrame::HLine | QFrame::Raised);
    return label;
}

