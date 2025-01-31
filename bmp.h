#ifndef BMP_H
#define BMP_H

#include <QMainWindow>
#include<QDir>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QPainter>
#include <QFont>
#include <QImage>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QMessageBox>
#include <QTimer>
#include <QApplication>
#include <QDateTime>
#include <QTimer>
#include <QMessageBox>
#include<QFontDatabase>


QT_BEGIN_NAMESPACE
namespace Ui { class bmp; }
QT_END_NAMESPACE

class bmp : public QMainWindow
{
    Q_OBJECT

public:
    bmp(QWidget *parent = nullptr);
    ~bmp();
    void TextToPng(QString pData,int iPixLen,int iFontSize,QString qsFontPath,QString qsSavePng,int ix,int iy,int iMode,bool _iItalic,bool _iBold);


private:
    Ui::bmp *ui;
};
#endif // BMP_H
