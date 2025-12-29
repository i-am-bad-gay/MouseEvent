#ifndef IPWINDOW_H
#define IPWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>
#include <QMouseEvent> // 新增：處理滑鼠事件所需的標頭檔
#include "imagetransform.h"

class ipwindow : public QMainWindow
{
    Q_OBJECT

public:
    ipwindow(QWidget *parent = nullptr);
    ~ipwindow();

    void createactions();
    void createmenus();
    void createtoolbars();
    void loadfile(QString filename);

protected:
    // 新增：重定義滑鼠事件以實現鼠標定位功能
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void showopenfile();
    void imgzoomout();
    void imgzoomin();
    void showgeometry();
    void showthreshold();

private:
    QWidget     *central;
    QLabel      *imgwin;
    QImage      img;

    // 子視窗指標
    imagetransform *gwindow;

    // Actions (工具列按鈕)
    QAction *openaction;
    QAction *zoomoutaction;
    QAction *zoominaction;
    QAction *geoaction;
    QAction *binaction;
    QAction *histaction;
    QAction *threshaction;
    QAction *exitaction;

    // 狀態列標籤 (已與您的定位邏輯融合)
    QLabel *statuslabel;    // 顯示「指標位置：」字樣
    QLabel *poslabel;       // 顯示即時座標數值，如 (x, y)
};

#endif // IPWINDOW_H
