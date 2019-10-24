/*构造函数*/

#include "imageprocesser.h"

//构造函数
ImageProcesser::ImageProcesser():
    scaleFactor(1),
    widget(new QWidget),
    imageLabel(new QLabel),
    scrollArea(new QScrollArea),
    chartview(new QChartView),
    GrayInfo(new QLabel),
    radioButton0(new QRadioButton("显示灰度图")),
    radioButton4(new QRadioButton("传统均衡")),
    radioButton5(new QRadioButton("优化均衡")),
    spinbox1(new QSpinBox),
    radioButton1(new QRadioButton("根据阈值灰度生成二值图：")),
    spinbox2_1(new QDoubleSpinBox),
    spinbox2_2(new QSpinBox),
    radioButton2(new QRadioButton("线性变换：")),    
    spinbox3_1(new QDoubleSpinBox),
    spinbox3_2(new QDoubleSpinBox),
    radioButton3(new QRadioButton("非线性变换：")),
    spinbox6_1(new QSpinBox),
    spinbox6_2(new QSpinBox),
    radioButton6(new QRadioButton("图像平移：")),
    spinbox7(new QDoubleSpinBox),
    radioButton7(new QRadioButton("图像旋转：")),
    spinbox8_1(new QDoubleSpinBox),
    spinbox8_2(new QDoubleSpinBox),
    radioButton8(new QRadioButton("图像缩放：")),
    radioButton9(new QRadioButton("均值平滑")),
    radioButton10(new QRadioButton("中值平滑")),
    radioButton11(new QRadioButton("高斯平滑"))
{
//初始化控件
    setCentralWidget(widget);//设置窗口中心部件
    //图片Label
    imageLabel->setBackgroundRole(QPalette::Base);//设置背景颜色
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);//设置大小可调整
    imageLabel->setScaledContents(true);//设置窗口自适应
    //滚动框
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);//将scrollArea中的内容设置为imageLabel
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setVisible(false);//初始不可见
    //直方图    
    chartview->setVisible(false);
    GrayInfo->setVisible(false);
    //“显示灰度图”按钮
    radioButton0->setVisible(false);
    connect(radioButton0, SIGNAL(clicked()), this, SLOT(showGrayImage()));
    //“均衡处理”按钮
    radioButton4->setVisible(false);
    connect(radioButton4, SIGNAL(clicked()), this, SLOT(showBalanceImage()));
    radioButton5->setVisible(false);
    connect(radioButton5, SIGNAL(clicked()), this, SLOT(showNewBalanceImage()));
    //根据阈值灰度生成二值图
    radioButton1->setVisible(false);
    spinbox1->setVisible(false);
    connect(radioButton1, SIGNAL(clicked()), this, SLOT(showSpinBox1()));
    connect(spinbox1, SIGNAL(valueChanged(int)), this, SLOT(showBinaryImage(int)));
    //线性变换
    radioButton2->setVisible(false);
    spinbox2_1->setVisible(false);
    spinbox2_2->setVisible(false);
    connect(radioButton2, SIGNAL(clicked()), this, SLOT(showSpinBox2()));
    connect(spinbox2_1, SIGNAL(valueChanged(double)), this, SLOT(showlinearImage1(double)));
    connect(spinbox2_2, SIGNAL(valueChanged(int)), this, SLOT(showlinearImage2(int)));
    //非线性变换
    radioButton3->setVisible(false);
    spinbox3_1->setVisible(false);
    spinbox3_2->setVisible(false);
    connect(radioButton3, SIGNAL(clicked()), this, SLOT(showSpinBox3()));
    connect(spinbox3_1, SIGNAL(valueChanged(double)), this, SLOT(showUnlinearImage1(double)));
    connect(spinbox3_2, SIGNAL(valueChanged(double)), this, SLOT(showUnlinearImage2(double)));
    //平移
    radioButton6->setVisible(false);
    spinbox6_1->setVisible(false);
    spinbox6_2->setVisible(false);
    connect(radioButton6, SIGNAL(clicked()), this, SLOT(showSpinBox6()));
    connect(spinbox6_1, SIGNAL(valueChanged(int)), this, SLOT(showTranslation1(int)));
    connect(spinbox6_2, SIGNAL(valueChanged(int)), this, SLOT(showTranslation2(int)));
    //旋转
    radioButton7->setVisible(false);
    spinbox7->setVisible(false);
    connect(radioButton7, SIGNAL(clicked()), this, SLOT(showSpinBox7()));
    connect(spinbox7, SIGNAL(valueChanged(double)), this, SLOT(showRotation(double)));
    //缩放
    radioButton8->setVisible(false);
    spinbox8_1->setVisible(false);
    spinbox8_2->setVisible(false);
    connect(radioButton8, SIGNAL(clicked()), this, SLOT(showSpinBox8()));
    connect(spinbox8_1, SIGNAL(valueChanged(double)), this, SLOT(nearstInterpolation(double)));
    connect(spinbox8_2, SIGNAL(valueChanged(double)), this, SLOT(bilinearInterpolation(double)));
    //平滑
    radioButton9->setVisible(false);
    connect(radioButton9, SIGNAL(clicked()), this, SLOT(meanFilter()));
    radioButton10->setVisible(false);
    connect(radioButton10, SIGNAL(clicked()), this, SLOT(medianFilter()));
    radioButton11->setVisible(false);
    connect(radioButton11, SIGNAL(clicked()), this, SLOT(gaussFilter()));
//设置布局
    QGridLayout *mainLayout = new QGridLayout;
    //左侧
    mainLayout->addWidget(scrollArea, 0, 0, 2, 3);
    mainLayout->addWidget(radioButton0, 2, 0, 1, 1);
    mainLayout->addWidget(radioButton4, 2, 1, 1, 1);
    mainLayout->addWidget(radioButton5, 2, 2, 1, 1);
    mainLayout->addWidget(radioButton1, 3, 0, 1, 1);
    mainLayout->addWidget(spinbox1, 3, 2, 1, 1);
    mainLayout->addWidget(radioButton2, 4, 0, 1, 1);
    mainLayout->addWidget(spinbox2_1, 4, 1, 1, 1);
    mainLayout->addWidget(spinbox2_2, 4, 2, 1, 1);
    mainLayout->addWidget(radioButton3, 5, 0, 1, 1);
    mainLayout->addWidget(spinbox3_1, 5, 1, 1, 1);
    mainLayout->addWidget(spinbox3_2, 5, 2, 1, 1);

    //右侧
    mainLayout->addWidget(chartview, 0, 3, 2, 3);
    mainLayout->addWidget(GrayInfo, 1, 3, 1, 3);
    mainLayout->addWidget(radioButton9, 2, 3, 1, 1);
    mainLayout->addWidget(radioButton10, 2, 4, 1, 1);
    mainLayout->addWidget(radioButton11, 2, 5, 1, 1);
    mainLayout->addWidget(radioButton6, 3, 3, 1, 1);
    mainLayout->addWidget(spinbox6_1, 3, 4, 1, 1);
    mainLayout->addWidget(spinbox6_2, 3, 5, 1, 1);
    mainLayout->addWidget(radioButton7, 4, 3, 1, 1);
    mainLayout->addWidget(spinbox7, 4, 5, 1, 1);
    mainLayout->addWidget(radioButton8, 5, 3, 1, 1);
    mainLayout->addWidget(spinbox8_1, 5, 4, 1, 1);
    mainLayout->addWidget(spinbox8_2, 5, 5, 1, 1);
    //设置行列比例
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(1, 1);
    mainLayout->setColumnStretch(2, 1);
    mainLayout->setColumnStretch(3, 1);
    mainLayout->setColumnStretch(4, 1);
    mainLayout->setColumnStretch(5, 1);
    mainLayout->setRowStretch(0, 19);
    mainLayout->setRowStretch(1, 1);
    mainLayout->setRowStretch(2, 1);
    widget->setLayout(mainLayout);

    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);//将窗口设置为默认的3/5大小

    createActions();
}

//创建菜单动作
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
    //分隔符
    fileMenu->addSeparator();
    //关闭按钮
    QAction *exitAct = fileMenu->addAction(tr("&关闭"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

//菜单栏-处理
    QMenu *processMenu = menuBar()->addMenu(tr("&处理"));
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
    QAction *about1Act = aboutMenu->addAction(tr("&关于作者..."), this, &ImageProcesser::about1);
    QAction *about2Act = aboutMenu->addAction(tr("&项目介绍..."), this, &ImageProcesser::about2);
}

//更新画面
void ImageProcesser::showImage(QImage img){
    showingImage = img;
    imageLabel->setPixmap(QPixmap::fromImage(img));
    imageLabel->adjustSize();//imageLabel的大小可调整

    showHistogram(img);//显示直方图
}

//隐藏其它输入框
void ImageProcesser::hideSpinBoxes(){
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
}
