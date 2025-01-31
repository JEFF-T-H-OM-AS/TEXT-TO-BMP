#include "bmp.h"
#include "ui_bmp.h"
#include <QPainter>
#include <QFontDatabase>
#include <QImage>
#include <QPainterPath>
#include <QApplication>

bmp::bmp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::bmp)
{
    ui->setupUi(this);

    bool italic, bold, semibold;
    italic = false;
    QString pngname = "croppedimage";
    QString fontpath = "/home/jeff/Videos/BMP/TiroTamil-Regular.ttf";
    QString final_str = "சென்னை";
    semibold = true;

    TextToPng(final_str, 450, 40, fontpath, pngname, 0, 42, 1, italic, semibold);
}

void bmp ::TextToPng(QString pData, int iPixLen, int iFontSize, QString qsFontPath, QString qsSavePng, int ix, int iy, int iMode, bool _iItalic, bool _iBold)
{
    QImage image(iPixLen, 60, QImage::Format_RGB888);
    image.fill(Qt::black);
    QPainterPath path;
    QFont font = QApplication::font();
    int id = QFontDatabase::addApplicationFont(qsFontPath);
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    font.setFamily(family);
    font.setPixelSize(iFontSize);
    font.setItalic(_iItalic);
    font.setBold(_iBold);
    font.setStrikeOut(false);
    path.addText(ix, iy, font, pData);
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);
    painter.drawPath(path);
    image.save(qsSavePng + ".bmp", "BMP");
}

bmp::~bmp()
{
    delete ui;
}
