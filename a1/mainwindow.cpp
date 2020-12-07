#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QImage>
#include <iostream>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintDevice>
#include <QPoint>
#include <iostream>
#include <vector>
#include <QMessageBox>

//// to draw point at (x, y) assuming upper left box as (0, 0) point(x*drawSize-1, y*drawSize-1)

using namespace std;

QImage img = QImage(450, 400, QImage::Format_RGB888);
QRgb lineColor = qRgb(200,200,200);
QRgb frameColor = qRgb(30,30,30);
QRgb unsetPoint = qRgb(245, 224, 80);
QRgb setPoint = 0xFF1DE9B6;

int MainWindow::scale(int X) {
    if (gridSize == 0) return X;
    if (X % gridSize) return X/gridSize;
    return (X/gridSize) - 1;
}

int MainWindow::transformX(int X) {
    X = scale(X);
    return (X-scale(img.width()/2));
}

int MainWindow::transformY(int Y) {
    Y = scale(Y);
    return -(Y-scale(img.height()/2));
}

void MainWindow::point(int x, int y, int r, int g, int b) {
//    if (gridSize == 0) {
//        img.setPixel(x, y, qRgb(r,g,b));
//    } else {
//        int startX = (x/gridSize) * gridSize;
//        int startY = (y/gridSize) * gridSize;

//        for (int i = startX+1; i < startX + gridSize; i++) {
//            if (i >= img.width() || i < 0) break;
//            for (int j = startY+1; j < startY + gridSize; j++) {
//                if (j >= img.height() || j < 0) break;
//                img.setPixel(i,j, qRgb(r,g,b));
//            }
//        }
////        ui->frame->setPixmap(QPixmap::fromImage(img));
//    }
    point(x, y, qRgb(r, g, b));
}

void MainWindow::point(int x, int y, QRgb color) {
    if (gridSize == 0) {
        img.setPixel(x, y, color);
    } else {
        int startX = (x/gridSize) * gridSize;
        int startY = (y/gridSize) * gridSize;

        for (int i = startX+1; i < startX + gridSize; i++) {
            if (i >= img.width() || i < 0) break;
            for (int j = startY+1; j < startY + gridSize; j++) {
                if (j >= img.height() || j < 0) break;
                img.setPixel(i,j, color);
            }
        }
    }
}

void MainWindow::draw_axes() {

    if(gridSize == 0) {
        // X axis
        for(int i=0;i<img.width();i++)
            if (img.pixel(i, img.height()/2) == qRgb(30, 30, 30)) point(i,img.height()/2);

        // Y axis
        for(int i=0;i<img.height();i++)
            if (img.pixel(img.width()/2, i) == qRgb(30, 30, 30)) point(img.width()/2,i);
    } else {
        // X axis
        for(int i=0;i<img.width();i+=gridSize)
            if (img.pixel(i+1, img.height()/2) == qRgb(30, 30, 30)) point(i,img.height()/2);
        // Y axis
        for(int i=0;i<img.height();i+=gridSize)
            if (img.pixel(img.width()/2, i+1) == qRgb(30, 30, 30)) point(img.width()/2,i);
    }

    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::remove_axes() {
    if(gridSize == 0) {
        // X axis
        for(int i=0;i<img.width();i++) {
            if (img.pixel(i, img.height()/2) == qRgb(200, 200, 200)) point(i,img.height()/2, 30, 30, 30);
        }
        // Y axis
        for(int i=0;i<img.height();i++) {
            if (img.pixel(img.width()/2, i) == qRgb(200, 200, 200)) point(img.width()/2,i, 30, 30, 30);
        }
    } else {
        // X axis
        for(int i=0;i<img.width();i+=gridSize) {
            if (img.pixel(i + 1, img.height()/2) == qRgb(200, 200, 200)) point(i,img.height()/2, 30, 30, 30);
        }

        // Y axis
        for(int i=0;i<img.height();i+=gridSize) {
            if (img.pixel(img.width()/2, i+1) == qRgb(200, 200, 200)) point(img.width()/2,i, 30, 30, 30);
        }
    }

    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::clear_frame() {
    for (int i = 0; i < img.width(); i++) {
        for (int j = 0; j < img.height(); j++) {
            img.setPixel(i,j, qRgb(30,30,30));
        }
    }
    ui->frame->setPixmap((QPixmap::fromImage(img)));
}

void MainWindow::reset() {
    clear_frame();
    ui->mouse_position->setText("");
    ui->locked_position->setText("");
    ui->show_axes->setCheckState(Qt::Unchecked);
}

/*-----------------------------------------------------------------*/

void MainWindow::dda(QPoint p1, QPoint p2)
{
    if(p1.x() == -1 && p1.y() == -1){
        if(p2.x() == -1 && p2.y() == -1){
            QMessageBox msgBox;
            msgBox.setText("Points not set!");
            msgBox.exec();
        }
    }else{
        //calculate the variation between (x1,x2) and (y1,y2)
        double dx = (double)(p2.x() - p1.x());
        double dy = (double)(p2.y() - p1.y());
        int x = 0;
        if(dx < 0.0){
            dx = dx * (-1);
            dy = dy * (-1);
            x = (int)p2.x();
        }else{
            x = (int)p1.x();
        }
        double m = dy/dx;
        double b = (double)(p1.y()) - m*(double)(p1.x());

        /*
        for(size_t i=0;i<dx;i++){
            img.setPixel(x+i,(int)(m*(x+i)+b),qRgb(100,100,100));
        }
        */

        if(gridSize == 0) {
            for(size_t i=0;i<=dx;i++){
                img.setPixel(x+i,(int)(m*(x+i)+b),qRgb(200,10,10));
            }
        } else {
            for(size_t i=0;i<=dx;i+=gridSize){
                //img.setPixel(x+i,(int)(m*(x+i)+b),qRgb(200,10,10));
                point(x+i,(int)(m*(x+i)+b),qRgb(200,10,10));
            }
        }
        ui->frame->setPixmap((QPixmap::fromImage(img)));
    }
}

/*---------------------------------------------------------------------*/

void MainWindow::bresenham(QPoint p1, QPoint p2)
{
    int x1 = p1.x(),x2 = p2.x();
    int y1 = p1.y(),y2 = p2.y();

     int dx = x2-x1;
     int dy = y2-y1;
     int d = 2*dy-dx;
     int incr_e = 2 * dy;
     int incr_ne = 2 * (dy-dx);
     int x = x1;
     int y = y1;
     // putPixel(x,y, myRGBA);
     point(x,y,qRgb(200,10,10));

      while(x < x2) {
        if(d < 0) {
          d += incr_e;
          x++;
        } else {
          d += incr_ne;
          x++;
          y++;
        }
        point(x,y,qRgb(200,10,10));
      }
    ui->frame->setPixmap((QPixmap::fromImage(img)));
}

/*---------------------------------------------------------------------*/

void MainWindow::draw_grid() {
    if (gridSize < 10) {
        return;
    }

    // vertical lines
    for (int i = 0; i < img.width(); i += gridSize) {
        for (int j = 0; j < img.height(); j++) {
            img.setPixel(i,j, qRgb(200, 200,200));
        }
    }

    // horizontal lines
    for (int i = 0; i < img.height(); i+= gridSize) {
        for (int j = 0; j < img.width(); j++) {
            img.setPixel(j, i, qRgb(200,200,200));
        }
    }

    ui->frame->setPixmap(QPixmap::fromImage(img));
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->frame,SIGNAL(mousePosLock()),this,SLOT(mousePressed()));
    connect(ui->frame,SIGNAL(sendMousePos(QPoint&)),this,SLOT(showCurrentMousePosition(QPoint&)));
    p1.setX(-1), p1.setY(-1);
    p2.setX(-1), p2.setY(-1);
    gridSize = ui->grid_size->value();
    clear_frame();
//    ui->locked_position->setText(QString::number(gridSize));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showCurrentMousePosition(QPoint& pos) {
//    ui->mouse_position->setText("X: " + QString::number(scale(pos.x())) + ", Y: " + QString::number(scale(pos.y())));
    ui->mouse_position->setText("X: " + QString::number(transformX(pos.x())) + ", Y: " + QString::number(transformY(pos.y())));
}

void MainWindow::mousePressed() {
    static int x = -1, y = -1;

    if (img.pixel(ui->frame->x, ui->frame->y) == setPoint) {
        return;
    }


    //////////// handles old unset point deleting if it's valid///////////////
    if ((x >= 0 && y >= 0) && img.pixel(x, y) != setPoint) {
        if (x % gridSize == 0) x++;
        if (y % gridSize == 0) y++;
        if ((scale(x) == scale(img.width()/2)  || scale(y) == scale(img.height()/2)) && ui->show_axes->checkState() == Qt::Checked) point(x, y);
        else point(x, y, 30, 30, 30);
    }


        /////////////handles diselecting unset point//////////////
    if (scale(x) == scale(ui->frame->x) && scale(y) == scale(ui->frame->y)) {
        if (x % gridSize == 0) x++;
        if (y % gridSize == 0) y++;
        if ((scale(x) == scale(img.width()/2)  || scale(y) == scale(img.height()/2)) && ui->show_axes->checkState() == Qt::Checked) point(x, y);
        else point(x, y, 30, 30, 30);
        ui->locked_position->setText("");
        x = -1, y = -1;
    }

        /////////////handles updating new point after clicking////////////
    else {
        x = ui->frame->x, y = ui->frame->y;
        ui->locked_position->setText("X: " + QString::number(scale(ui->frame->x)) + ", Y: " + QString::number(scale(ui->frame->y)));
        ui->locked_position->setText("X: " + QString::number(transformX(ui->frame->x)) + ", Y: " + QString::number(transformY(ui->frame->y)));
        point(ui->frame->x, ui->frame->y, 245, 224, 80);
    }

    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_show_axes_stateChanged(int arg1)
{
    if (arg1) draw_axes();
    else remove_axes();
}

void MainWindow::on_grid_size_valueChanged(int arg1)
{
    reset();
    if (gridSize > arg1) {
        gridSize -= 4;
        if (gridSize < 10) gridSize = 0;
        ui->grid_size->setValue(gridSize);
    }
    else if (gridSize < arg1) {
        gridSize += 4;
        if (gridSize < 10) gridSize = 10;
        ui->grid_size->setValue(gridSize);
    }

    draw_grid();
//    ui->locked_position->setText(QString::number(gridSize) + " " + QString::number(arg1) + " " + QString::number(ui->grid_size->value()));
}

void MainWindow::on_reset_clicked()
{
    reset();
    gridSize = 0;
    ui->grid_size->setValue(0);
    p1.setX(-1), p1.setY(-1);
    p2.setX(-1), p2.setY(-1);
}

void MainWindow::on_set_p1_clicked()
{
    if (ui->frame->x == p2.x() && ui ->frame->y == p2.y()) return;
    p1.setX(ui->frame->x), p1.setY(ui->frame->y);
    ui->p1_label->setText("X: " + QString::number(transformX(ui->frame->x)) + ", Y: " + QString::number(transformY(ui->frame->y)));
    point(ui->frame->x, ui->frame->y, setPoint);
    ui->frame->setPixmap(QPixmap::fromImage(img));
    ui->set_p1->setDisabled(true);
}

void MainWindow::on_set_p2_clicked()
{
    if (ui->frame->x == p1.x() && ui ->frame->y == p1.y()) return;
    p2.setX(ui->frame->x), p2.setY(ui->frame->y);
    ui->p2_label->setText("X: " + QString::number(transformX(ui->frame->x)) + ", Y: " + QString::number(transformY(ui->frame->y)));
    point(ui->frame->x, ui->frame->y, setPoint);
    ui->frame->setPixmap(QPixmap::fromImage(img));
    ui->set_p2->setDisabled(true);
}

void MainWindow::on_unset_p1_clicked()
{
    if ((scale(p1.x()) == scale(img.width()/2)  || scale(p1.y()) == scale(img.height()/2)) && ui->show_axes->checkState() == Qt::Checked) point(p1.x(), p1.y());
    else point(p1.x(), p1.y(), frameColor);
    p1.setX(-1), p1.setY(-1);
    ui->p1_label->setText("Not set");
    ui->frame->setPixmap(QPixmap::fromImage(img));
    ui->set_p1->setDisabled(false);
}

void MainWindow::on_unset_p2_clicked()
{
    if ((scale(p2.x()) == scale(img.width()/2)  || scale(p2.y()) == scale(img.height()/2)) && ui->show_axes->checkState() == Qt::Checked) point(p2.x(), p2.y());
    else point(p2.x(), p2.y(), frameColor);
    p2.setX(-1), p2.setY(-1);
    ui->p2_label->setText("Not set");
    ui->frame->setPixmap(QPixmap::fromImage(img));
    ui->set_p2->setDisabled(false);
}


void MainWindow::on_draw_clicked()
{
   //Draw using DDA algorithm
   // p1 and p2 are two points
    dda(p1,p2);

}

void MainWindow::on_draw_2_clicked()
{
    bresenham(p1,p2);
}

void MainWindow::on_reset_2_clicked()
{
    reset();
    gridSize = 0;
    ui->grid_size->setValue(0);
    p1.setX(-1), p1.setY(-1);
    p2.setX(-1), p2.setY(-1);
}

void MainWindow::on_frame_linkHovered(const QString &link)
{
  //does nothing
}
