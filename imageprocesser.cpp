/*构造函数*/

#include "imageprocesser.h"
#include <QtWidgets>
#include <QtCharts>

//构造函数
ImageProcesser::ImageProcesser():
    widget(new QWidget),
    imageLabel(new QLabel),
    scrollArea(new QScrollArea),
    chartview(new QChartView),
    GrayInfo(new QLabel),
    spinbox1(new QSpinBox),
    radioButton1(new QRadioButton("根据阈值灰度生成二值图：")),
    spinbox2_1(new QDoubleSpinBox),
    spinbox2_2(new QSpinBox),
    radioButton2(new QRadioButton("线性变换：")),
    radioButton3(new QRadioButton("显示原图"))
{
    setCentralWidget(widget);//设置窗口中心部件
    //图片Label
    imageLabel->setBackgroundRole(QPalette::Base);//设置背景颜色
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);//设置大小可调整
    imageLabel->setScaledContents(true);//设置窗口自适应
    //滚动框
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);//将scrollArea中的内容设置为imageLabel
    scrollArea->setVisible(false);//初始不可见
    //直方图
    chartview->setVisible(false);
    GrayInfo->setVisible(false);

    radioButton3->setChecked(true);
    radioButton3->setVisible(false);
    connect(radioButton3, SIGNAL(clicked()), this, SLOT(changeToGray()));
    //调节框1
    radioButton1->setVisible(false);
    spinbox1->setVisible(false);
    connect(radioButton1, SIGNAL(clicked()), this, SLOT(showSpinBox1()));
    connect(spinbox1, SIGNAL(valueChanged(int)), this, SLOT(showBinaryImage(int)));
    //调节框2
    radioButton2->setVisible(false);
    spinbox2_1->setVisible(false);
    spinbox2_2->setVisible(false);
    connect(radioButton2, SIGNAL(clicked()), this, SLOT(showSpinBox2()));
    connect(spinbox2_1, SIGNAL(valueChanged(double)), this, SLOT(showlinearImage1(double)));
    connect(spinbox2_2, SIGNAL(valueChanged(int)), this, SLOT(showlinearImage2(int)));
    //设置布局
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(scrollArea, 0, 0, 1, 3);
    mainLayout->addWidget(chartview, 0, 3, 1, 3);
    mainLayout->addWidget(GrayInfo, 1, 3, 1, 3);
    mainLayout->addWidget(radioButton3, 1, 0, 1, 1);
    mainLayout->addWidget(radioButton1, 2, 0, 1, 1);
    mainLayout->addWidget(spinbox1, 2, 2, 1, 1);
    mainLayout->addWidget(radioButton2, 3, 0, 1, 1);
    mainLayout->addWidget(spinbox2_1, 3, 1, 1, 1);
    mainLayout->addWidget(spinbox2_2, 3, 2, 1, 1);
    //行列比例
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(1, 1);
    mainLayout->setColumnStretch(2, 1);
    mainLayout->setRowStretch(0, 5);
    mainLayout->setRowStretch(1, 1);
    widget->setLayout(mainLayout);

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
    //分隔符
    fileMenu->addSeparator();
    //关闭按钮
    QAction *exitAct = fileMenu->addAction(tr("&关闭"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    //菜单栏-处理
    QMenu *processMenu = menuBar()->addMenu(tr("&处理"));
    //转为灰度图
    changeToGrayAct = processMenu->addAction(tr("&灰度图/RGB图"), this, &ImageProcesser::changeToGray);
    changeToGrayAct->setEnabled(false);
    changeToGrayAct->setCheckable(true);
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

    //菜单栏-关于
    QMenu *aboutMenu = menuBar()->addMenu(tr("&关于"));
    QAction *about1Act = aboutMenu->addAction(tr("&关于作者..."), this, &ImageProcesser::about1);
    QAction *about2Act = aboutMenu->addAction(tr("&项目介绍..."), this, &ImageProcesser::about2);
}

