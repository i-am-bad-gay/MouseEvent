#include "ipwindow.h"
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QStatusBar>
#include <QMessageBox>
#include <QMouseEvent> // 必須包含此檔以處理滑鼠事件

ipwindow::ipwindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("影像處理"));

    // 1. 初始化影像顯示標籤
    imgwin = new QLabel(this);
    imgwin->setAlignment(Qt::AlignCenter);
    imgwin->setScaledContents(true);
    setCentralWidget(imgwin);

    // 2. 初始化子視窗物件
    gwindow = new imagetransform();

    // 3. 建立介面元件
    createactions();
    createmenus();
    createtoolbars();

    // 4. 初始化鼠標定位標籤並加入狀態列
    statuslabel = new QLabel(QStringLiteral("指標位置："), this);
    statuslabel->setFixedWidth(100);

    poslabel = new QLabel("", this);
    poslabel->setFixedWidth(120);

    // 將定位標籤永久加入狀態列右側
    statusBar()->addPermanentWidget(statuslabel);
    statusBar()->addPermanentWidget(poslabel);

    // 5. 重要：開啟滑鼠追蹤 (Mouse Tracking)
    // 這樣滑鼠在視窗內移動時，不需按下按鍵也能觸發 mouseMoveEvent
    setMouseTracking(true);
    imgwin->setMouseTracking(true);

    statusBar()->showMessage(QStringLiteral("準備就緒"));
    resize(1000, 700);
}

ipwindow::~ipwindow() {}

// --- 滑鼠事件實作 (鼠標定位核心) ---

void ipwindow::mouseMoveEvent(QMouseEvent *event)
{
    // 取得當前座標並更新 poslabel
    // Qt 6 建議使用 event->position()，Qt 5 使用 event->pos()
    QString str = "(" + QString::number(event->position().x()) + ", " +
                  QString::number(event->position().y()) + ")";
    poslabel->setText(str);
}

void ipwindow::mousePressEvent(QMouseEvent *event)
{
    QString str = "(" + QString::number(event->position().x()) + ", " +
                  QString::number(event->position().y()) + ")";

    if (event->button() == Qt::LeftButton) {
        statusBar()->showMessage(QStringLiteral("左鍵按下：") + str, 2000);
    } else if (event->button() == Qt::RightButton) {
        statusBar()->showMessage(QStringLiteral("右鍵按下：") + str, 2000);
    }
}

void ipwindow::mouseReleaseEvent(QMouseEvent *event)
{
    // 可以在釋放時清除狀態列提示
    // statusBar()->clearMessage();
}

// --- 選單與動作實作 ---

void ipwindow::createactions() {
    // 檔案操作
    openaction = new QAction(QStringLiteral("開啟檔案&O"), this);
    connect(openaction, &QAction::triggered, this, &ipwindow::showopenfile);

    exitaction = new QAction(QStringLiteral("結束&E"), this);
    connect(exitaction, &QAction::triggered, this, &ipwindow::close);

    // 工具操作
    zoominaction = new QAction(QStringLiteral("放大"), this);
    connect(zoominaction, &QAction::triggered, this, &ipwindow::imgzoomin);

    zoomoutaction = new QAction(QStringLiteral("縮小"), this);
    connect(zoomoutaction, &QAction::triggered, this, &ipwindow::imgzoomout);

    geoaction = new QAction(QStringLiteral("幾何轉換"), this);
    connect(geoaction, &QAction::triggered, this, &ipwindow::showgeometry);

    // 其他功能標籤 (依圖示加入)
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

    // 依據順序加入工具列按鈕
    toolbar->addAction(openaction);
    toolbar->addSeparator();
    toolbar->addAction(zoominaction);
    toolbar->addAction(zoomoutaction);
    toolbar->addAction(geoaction);
    toolbar->addAction(binaction);
    toolbar->addAction(histaction);
    toolbar->addAction(threshaction);
}

// --- 功能邏輯實作 ---

void ipwindow::loadfile(QString filename) {
    if (img.load(filename)) {
        imgwin->setPixmap(QPixmap::fromImage(img));
        statusBar()->showMessage(QStringLiteral("已載入影像: ") + filename);
    }
}

void ipwindow::showopenfile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open", ".", "Images (*.png *.jfif *.bmp)");
    if (!fileName.isEmpty()) {
        loadfile(fileName);
    }
}

void ipwindow::imgzoomin() {
    if (img.isNull()) return;
    imgwin->resize(imgwin->width() * 1.2, imgwin->height() * 1.2);
}

void ipwindow::imgzoomout() {
    if (img.isNull()) return;
    imgwin->resize(imgwin->width() * 0.8, imgwin->height() * 0.8);
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
}
