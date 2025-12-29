#ifndef IMAGETRANSFORM_H
#define IMAGETRANSFORM_H

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QDial>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

class imagetransform : public QWidget
{
    Q_OBJECT

public:
    explicit imagetransform(QWidget *parent = nullptr);
    void setimage(const QImage &image); // 用來從主視窗傳入圖片

private slots:
    void mirroredimage();
    void rotatedimage();

public: // 讓主視窗方便存取的變數
    QImage srcimg, dstimg;
    QLabel *inwin;

private:
    QCheckBox *hcheckbox, *vcheckbox;
    QPushButton *mirrorbutton;
    QDial *rotatedial;
};

#endif
