#include "imagetransform.h"
#include <QPainter>

imagetransform::imagetransform(QWidget *parent) : QWidget(parent)
{
    setWindowTitle(QStringLiteral("幾何轉換"));
    QHBoxLayout *mainlayout = new QHBoxLayout(this);
    QVBoxLayout *leftlayout = new QVBoxLayout();

    // 鏡射區塊
    QGroupBox *mirrorgroup = new QGroupBox(QStringLiteral("鏡射"), this);
    QVBoxLayout *grouplayout = new QVBoxLayout(mirrorgroup);
    hcheckbox = new QCheckBox(QStringLiteral("水平"), mirrorgroup);
    vcheckbox = new QCheckBox(QStringLiteral("垂直"), mirrorgroup);
    mirrorbutton = new QPushButton(QStringLiteral("執行"), mirrorgroup);
    grouplayout->addWidget(hcheckbox);
    grouplayout->addWidget(vcheckbox);
    grouplayout->addWidget(mirrorbutton);
    leftlayout->addWidget(mirrorgroup);

    // 旋轉區塊
    rotatedial = new QDial(this);
    rotatedial->setNotchesVisible(true);
    leftlayout->addWidget(rotatedial);
    mainlayout->addLayout(leftlayout);

    // 圖片顯示區
    inwin = new QLabel(this);
    inwin->setScaledContents(true);
    inwin->setFixedSize(400, 300);
    mainlayout->addWidget(inwin);

    connect(mirrorbutton, &QPushButton::clicked, this, &imagetransform::mirroredimage);
    connect(rotatedial, &QDial::valueChanged, this, &imagetransform::rotatedimage);
}

void imagetransform::setimage(const QImage &image) {
    srcimg = image;
    inwin->setPixmap(QPixmap::fromImage(srcimg));
}

void imagetransform::mirroredimage() {
    if (srcimg.isNull()) return;
    // 修正 Qt 6 警告，使用新的 flipped 邏輯或維持 mirrored 但確保變數正確
    dstimg = srcimg.mirrored(hcheckbox->isChecked(), vcheckbox->isChecked());
    inwin->setPixmap(QPixmap::fromImage(dstimg));
    srcimg = dstimg; // 更新來源圖以支援連續操作
}

void imagetransform::rotatedimage() {
    if (srcimg.isNull()) return;
    QTransform tran;
    tran.rotate(rotatedial->value());
    dstimg = srcimg.transformed(tran);
    inwin->setPixmap(QPixmap::fromImage(dstimg));
}
