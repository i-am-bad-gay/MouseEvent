#include "ipwindow.h"
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QStatusBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QScrollArea>

ipwindow::ipwindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("影像處理 - 完整整合版"));

    // 1. 初始化影像顯示標籤
    imgwin = new QLabel(this);
    imgwin->setAlignment(Qt::AlignCenter);
    imgwin->setScaledContents(true);
    imgwin->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    // 2. 建立捲軸區域：解決圖片縮放回彈與顯示不全的問題
    QScrollArea *scrollarea = new QScrollArea(this);
    scrollarea->setWidget(imgwin);
    scrollarea->setWidgetResizable(false);
    scrollarea->setAlignment(Qt::AlignCenter);
    setCentralWidget(scrollarea);

    // 3. 初始化子視窗
    gwindow = new imagetransform();

    // 4. 建立介面元件 (選單、工具列)
    createactions();
    createmenus();
    createtoolbars();

    // 5. 初始化狀態列標籤 (定位功能)
    statuslabel = new QLabel(QStringLiteral("指標位置："), this);
    poslabel = new QLabel("", this);
    poslabel->setFixedWidth(120);
    statusBar()->addPermanentWidget(statuslabel);
    statusBar()->addPermanentWidget(poslabel);

    // 6. 開啟追蹤 (需同時對視窗與標籤開啟，確保 MoveEvent 觸發)
    setMouseTracking(true);
    imgwin->setMouseTracking(true);
    scrollarea->setMouseTracking(true);

    statusBar()->showMessage(QStringLiteral("準備就緒"));
    resize(1100, 800);
}

ipwindow::~ipwindow() {}

// --- 滑鼠事件實作 ---

void ipwindow::mouseMoveEvent(QMouseEvent *event)
{
    // 使用 mapFromGlobal 將全域座標轉換成 imgwin 上的內部座標
    QPoint localpos = imgwin->mapFromGlobal(event->globalPosition().toPoint());

    if (imgwin->rect().contains(localpos)) {
        QString str = QString("(%1, %2)").arg(localpos.x()).arg(localpos.y());
        poslabel->setText(str);
    } else {
        poslabel->setText("");
    }
}

void ipwindow::mousePressEvent(QMouseEvent *event)
{
    // 取得相對於圖片像素的座標
    QPoint localpos = imgwin->mapFromGlobal(event->globalPosition().toPoint());

    if (imgwin->rect().contains(localpos)) {
        QString str = QString("(%1, %2)").arg(localpos.x()).arg(localpos.y());

        if (event->button() == Qt::LeftButton) {
            statusBar()->showMessage(QStringLiteral("左鍵點擊座標：") + str, 2000);
        } else if (event->button() == Qt::RightButton) {
            statusBar()->showMessage(QStringLiteral("右鍵點擊座標：") + str, 2000);
        }
    }
}

void ipwindow::mouseReleaseEvent(QMouseEvent *event)
{
    // 釋放事件目前保留，可依需求擴充
}

// --- 影像操作功能 ---

void ipwindow::showopenfile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open", ".", "Images (*.png *.jpg *.bmp)");
    if (!fileName.isEmpty()) {
        if (img.load(fileName)) {
            // 載入時預設為原始大小
            imgwin->resize(img.size());
            imgwin->setPixmap(QPixmap::fromImage(img));
            statusBar()->showMessage(QStringLiteral("已載入影像: ") + fileName);
        }
    }
}

void ipwindow::imgzoomin() {
    if (img.isNull()) return;
    // 每次放大 20%
    QSize newsize = imgwin->size() * 1.2;
    imgwin->resize(newsize);
    statusBar()->showMessage(QStringLiteral("目前比例: %1%").arg(int(float(imgwin->width())/img.width()*100)), 2000);
}

void ipwindow::imgzoomout() {
    if (img.isNull()) return;
    // 每次縮小 20%
    QSize newsize = imgwin->size() * 0.8;
    imgwin->resize(newsize);
    statusBar()->showMessage(QStringLiteral("目前比例: %1%").arg(int(float(imgwin->width())/img.width()*100)), 2000);
}

void ipwindow::showgeometry() {
    if (img.isNull()) {
        QMessageBox::warning(this, "Warning", QStringLiteral("請先開啟影像"));
        return;
    }
    gwindow->setimage(img);
    gwindow->show();
}

void ipwindow::showthreshold() {
    statusBar()->showMessage(QStringLiteral("開啟閾值化處理..."));
    // 此處可實作閾值化子視窗呼叫
}

// --- 介面建構實作 ---

void ipwindow::createactions() {
    openaction = new QAction(QStringLiteral("開啟檔案&O"), this);
    connect(openaction, &QAction::triggered, this, &ipwindow::showopenfile);

    exitaction = new QAction(QStringLiteral("結束&E"), this);
    connect(exitaction, &QAction::triggered, this, &ipwindow::close);

    zoominaction = new QAction(QStringLiteral("放大"), this);
    connect(zoominaction, &QAction::triggered, this, &ipwindow::imgzoomin);

    zoomoutaction = new QAction(QStringLiteral("縮小"), this);
    connect(zoomoutaction, &QAction::triggered, this, &ipwindow::imgzoomout);

    geoaction = new QAction(QStringLiteral("幾何轉換"), this);
    connect(geoaction, &QAction::triggered, this, &ipwindow::showgeometry);

    binaction = new QAction(QStringLiteral("二值化"), this);
    histaction = new QAction(QStringLiteral("直方圖"), this);
    threshaction = new QAction(QStringLiteral("閾值化"), this);
    connect(threshaction, &QAction::triggered, this, &ipwindow::showthreshold);
}

void ipwindow::createmenus() {
    QMenu *filemenu = menuBar()->addMenu(QStringLiteral("檔案&F"));
    filemenu->addAction(openaction);
    filemenu->addSeparator();
    filemenu->addAction(exitaction);

    QMenu *toolmenu = menuBar()->addMenu(QStringLiteral("工具&T"));
    toolmenu->addAction(zoominaction);
    toolmenu->addAction(zoomoutaction);
    toolmenu->addAction(geoaction);
}

void ipwindow::createtoolbars() {
    QToolBar *toolbar = addToolBar("MainToolbar");
    toolbar->setMovable(false);
    toolbar->addAction(openaction);
    toolbar->addSeparator();
    toolbar->addAction(zoominaction);
    toolbar->addAction(zoomoutaction);
    toolbar->addAction(geoaction);
    toolbar->addAction(binaction);
    toolbar->addAction(histaction);
    toolbar->addAction(threshaction);
}

void ipwindow::loadfile(QString filename) {
    // 預留給外部呼叫使用
}
