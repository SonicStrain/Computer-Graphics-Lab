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
#include <QString>
#include <QTime>
#include <QElapsedTimer>
#include <time.h>
#include <QDebug>
#include <QPainter>
#include <QThread>
#include <thread>
#include <cmath>
#include <algorithm>

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
    //ui->frame->setPixmap(QPixmap::fromImage(img));
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

void MainWindow::delay(int number_of_seconds)
{
    // Converting time into milli_seconds
        int milli_seconds = 1000 * number_of_seconds;

        // Storing start time
        clock_t start_time = clock();

        // looping till required time is not achieved
        while (clock() < start_time + milli_seconds)
            ;
}

QRgb MainWindow::getPointColor(int x, int y)
{
    QRgb color;
    if (gridSize == 0) {
        color = img.pixel(x,y);
    } else {
        int startX = (x/gridSize) * gridSize;
        int startY = (y/gridSize) * gridSize;

        color = img.pixel(startX+gridSize/2,startY+gridSize/2);
    }
    return color;
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
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

void MainWindow::bresenham(QPoint p1, QPoint p2)
{
    int x1 = p1.x(),y1 = p1.y(),x2 = p2.x(),y2 = p2.y();
    point(x1,y1,qRgb(200,10,10));
    point(x2,y2,qRgb(200,10,10));
    int x, y, dx, dy, s1, s2, interchange, e;
        x = x1;
        y = y1;
        dx = abs(x2 - x1);
        dy = abs(y2 - y1);
        // Initialze signs
        s1 = x1==x2?0:(x2>x1?1:-1);
        s2 = y1==y2?0:(y2>y1?1:-1);
        // Set interchange flag
        if(dy > dx ){
            interchange = 1;
            int temp = dx;
            dx = dy;
            dy = temp;
        }else{
            interchange=0;
        }
        e = 2 * dy - dx;

        int i;
        for(i=1;i<=dx;i++){
            point(x,y,qRgb(200,10,10));
            while(e>0){
                if(interchange){
                    x = x + s1;
                }else{
                    y = y + s2;
                }
                e = e - 2*dx;
            }
            if(interchange){
                y = y + s2;
            }else{
                x = x + s1;
            }
            e = e + 2*dy;
        }

}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
//@author : Sagen Soren
//@Computer Networks Lab
//@Assignment 2

void MainWindow::polarCircle(QPoint p1, int r)
{
    QElapsedTimer t;
    t.start();
    int xc = p1.x(),yc = p1.y();
    point(xc,yc,qRgb(200,10,10));

    if(gridSize != 0){
        r = r*gridSize;
    }

    float theta = 0.0;
    int x = 0,y = 0;

    for(int i=0;i<=360;i++){
        theta = (i*3.14)/180;
        x = (int)(xc + r*cos(theta));
        y = (int)(yc + r*sin(theta));

        point(x,y,qRgb(200,10,10));
    }
    ui->frame->setPixmap((QPixmap::fromImage(img)));
    QString esTime = QString::number(t.elapsed());
    QMessageBox msgBox;
    msgBox.setText("Total time: "+ esTime +" ms");
    msgBox.exec();
}

/*-----------------------------------------------------------------*/

void MainWindow::midCircle(QPoint p1, int r)
{
    QElapsedTimer t;
    t.start();
    if(gridSize != 0){
        r = r*gridSize;
    }
    int x=r,y=0,d=1-r;
    int xc = p1.x(),yc = p1.y();
    eightWayPlot(xc,yc,x,y);

    while(y<=x){
        if(d<=0){
            d=d+(2*y)+1;
          }
        else
          {
            d=d+(2*y)-(2*x)+1;
            x=x-1;
          }
          y=y+1;
          eightWayPlot(xc,yc,x,y);
    }

    ui->frame->setPixmap((QPixmap::fromImage(img)));
    QString esTime = QString::number(t.elapsed());
    QMessageBox msgBox;
    msgBox.setText("Total time: "+ esTime +" ms");
    msgBox.exec();
}

/*-----------------------------------------------------------------*/
void MainWindow::plotPoints(int xc, int yc, int x, int y)
{
    point(xc+x,yc+y,qRgb(200,10,10));
    point(xc-x,yc+y,qRgb(200,10,10));
    point(xc+x,yc-y,qRgb(200,10,10));
    point(xc-x,yc-y,qRgb(200,10,10));
}
//<---------------------------------------------------------------------------->
//<---------------------------------------------------------------------------->
//<---------------------------------------------------------------------------->

void MainWindow::boundaryFill4(int x, int y, QRgb fillColor, QRgb boundaryColor)
{
    if(gridSize == 0){
        gridSize = 1;
    }
    if(getPointColor(x,y) != fillColor && getPointColor(x,y)!=boundaryColor){
        point(x,y,fillColor);
        qDebug() << "filling at " << x;
        //ui->frame->setPixmap((QPixmap::fromImage(img)));
        //delay(100);
        //QThread::msleep(100);
        boundaryFill4(x+gridSize,y,fillColor,boundaryColor);
        boundaryFill4(x,y+gridSize,fillColor,boundaryColor);
        boundaryFill4(x-gridSize,y,fillColor,boundaryColor);
        boundaryFill4(x,y-gridSize,fillColor,boundaryColor);
    }else{
        qDebug() << "Skipping at " << x;
    }
}

void MainWindow::boundaryFill8(int x, int y, QRgb fillColor, QRgb boundaryColor)
{
    if(gridSize == 0){
        gridSize = 1;
    }
    if(getPointColor(x,y) != fillColor && getPointColor(x,y)!=boundaryColor){
        point(x,y,fillColor);
        qDebug() << "filling at " << x;
        //ui->frame->setPixmap((QPixmap::fromImage(img)));
        //delay(100);
        //QThread::msleep(100);
        boundaryFill8(x+gridSize,y,fillColor,boundaryColor);
        boundaryFill8(x,y+gridSize,fillColor,boundaryColor);
        boundaryFill8(x-gridSize,y,fillColor,boundaryColor);
        boundaryFill8(x,y-gridSize,fillColor,boundaryColor);
        boundaryFill8(x+gridSize,y+gridSize,fillColor,boundaryColor);
        boundaryFill8(x-gridSize,y+gridSize,fillColor,boundaryColor);
        boundaryFill8(x-gridSize,y-gridSize,fillColor,boundaryColor);
        boundaryFill8(x+gridSize,y-gridSize,fillColor,boundaryColor);
    }else{
        qDebug() << "Skipping at " << x;
    }
}

void MainWindow::floodfill4(int x, int y, QRgb previousColor, QRgb newColor)
{
    if(gridSize == 0){
        gridSize = 1;
    }
    if(getPointColor(x,y) != previousColor){
        return;
    }
    if(getPointColor(x,y)==newColor){
        return;
    }
    point(x,y,newColor);

    floodfill4( x+gridSize, y, previousColor, newColor);
    floodfill4( x-gridSize, y, previousColor, newColor);
    floodfill4( x, y+gridSize, previousColor, newColor);
    floodfill4( x, y-gridSize, previousColor, newColor);
    //floodfill4( x+gridSize, y+gridSize, previousColor, newColor);
    //floodfill4( x+gridSize, y-gridSize, previousColor, newColor);
    //floodfill4( x-gridSize, y+gridSize, previousColor, newColor);
    //floodfill4( x-gridSize, y-gridSize, previousColor, newColor);
}

//<---------------------------------------------------------------------------->
//<---------------------------------------------------------------------------->
//<---------------------------------------------------------------------------->

void MainWindow::drawPolygon(int n,QPoint ar[])
{
    QPoint last = ar[0];
    for(int i=0;i<n-1;i++){
        bresenham(ar[i],ar[i+1]);
    }
    bresenham(ar[n-1],last);
}

void MainWindow::scanFill(QPoint ar[])
{

        //anticlockwise input coordinates
        sortPoints(ar);
        int x1[]={ar[0].x(),ar[1].x(),ar[2].x(),ar[3].x(),ar[4].x(),ar[5].x()};
        int y1[]={ar[0].y(),ar[1].y(),ar[2].y(),ar[3].y(),ar[4].y(),ar[5].y()};
        int i=0;
        int n=6;
        int ymin = 400, ymax=0;
        int c,j,y;
        float m[50],a[50],dx,dy,t;
        x1[i]=x1[0];
        y1[i]=y1[0];
        for(i=0;i<n-1;i++)
        {
            bresenham(x1[i],y1[i],x1[i+1],y1[i+1]);
        }
        bresenham(x1[0],y1[0],x1[n-1],y1[n-1]);

        for(i=0;i<n;i++)
        {
            if(y1[i]>=ymax)
                ymax=y1[i];
            if(y1[i]<=ymin)
                ymin=y1[i];
            dx=x1[i+1]-x1[i];
            dy=y1[i+1]-y1[i];
            if(dx==0)
                m[i]=0;
            if(dy==0)
                m[i]=1;
            if(dx!=0 && dy!=0)
                m[i]=dx/dy;
        }
        for(y=ymax;y>=ymin;y--)
        {
            c=0;
            for(i=0;i<n;i++)
            {
                if((y1[i]>y && y1[i+1] <=y) || (y1[i]<=y && y1[i+1]>y))
    {
                    a[c]=x1[i]+(m[i]*(y-y1[i]));
                    c++;
            }
        }
    for(i=0;i<c-1;i++)      //Bubble sort
    {
        for(j=0;j<c-1;j++)
        {
            if(a[j]>a[j+1])
            {
                t=a[j];
                a[j]=a[j+1];
                a[j+1]=t;
            }
        }
    }
        for(i=0;i<c-1;i=i+3)
        {
            //drawline
            bresenham(a[i],y,a[i+1],y);
        }
    }

}

void MainWindow::bresenham(int x1, int y1, int x2, int y2)
{
    point(x1,y1,qRgb(200,10,10));
    point(x2,y2,qRgb(200,10,10));
    int x, y, dx, dy, s1, s2, interchange, e;
        x = x1;
        y = y1;
        dx = abs(x2 - x1);
        dy = abs(y2 - y1);
        // Initialze signs
        s1 = x1==x2?0:(x2>x1?1:-1);
        s2 = y1==y2?0:(y2>y1?1:-1);
        // Set interchange flag
        if(dy > dx ){
            interchange = 1;
            int temp = dx;
            dx = dy;
            dy = temp;
        }else{
            interchange=0;
        }
        e = 2 * dy - dx;

        int i;
        for(i=1;i<=dx;i++){
            point(x,y,qRgb(200,10,10));
            while(e>0){
                if(interchange){
                    x = x + s1;
                }else{
                    y = y + s2;
                }
                e = e - 2*dx;
            }
            if(interchange){
                y = y + s2;
            }else{
                x = x + s1;
            }
            e = e + 2*dy;
        }

}

double MainWindow::getClockwiseAngle(QPoint p)
{
    double angle = 0.0;

    angle = -1 * atan2(p.x(), -1 * p.y());
    return angle;
}

bool MainWindow::comparePoints(QPoint p1, QPoint p2)
{
    return getClockwiseAngle(p1) < getClockwiseAngle(p2);
}

void MainWindow::sortPoints(QPoint q[])
{
    int i, j,n=6;
    QPoint temp;
    for (i = 0; i < n-1; i++){
        // Last i elements are already in place
        for (j = 0; j < n-i-1; j++){
            if (!comparePoints(q[j],q[j+1])){
                temp = q[j];
                q[j] = q[j+1];
                q[j+1] = temp;
            }
        }
    }

}


void MainWindow::readSFL()
{
    p[0] = p1,p[1]=p2,p[2]=p3,p[3]=p4,p[4]=p5,p[5]=p6;
    p[6].setX(p[0].x());
    p[6].setY(p[0].y());
    xmin=xmax=p[0].x();
    ymin=ymax=p[0].y();
}

void MainWindow::calcs()
{
    for(int i=0;i<6;i++)
        {
            if(xmin>p[i].x())
            xmin=p[i].x();
            if(xmax<p[i].x())
            xmax=p[i].x();
            if(ymin>p[i].y())
            ymin=p[i].y();
            if(ymax<p[i].y())
            ymax=p[i].y();
    }
}

void MainWindow::ints(float z)
{
        int x1,x2,y1,y2,temp;
        c=0;
        for(int i=0;i<v;i++)
        {
            x1=p[i].x();
            y1=p[i].y();
            x2=p[i+1].x();
            y2=p[i+1].y();
            if(y2<y1)
            {
                temp=x1;
                x1=x2;
                x2=temp;
                temp=y1;
                y1=y2;
                y2=temp;
            }
            if(z<=y2&&z>=y1)
            {
                if((y1-y2)==0)
                x=x1;
                else // used to make changes in x. so that we can fill our polygon after cerain distance
                {
                    x=((x2-x1)*(z-y1))/(y2-y1);
                    x=x+x1;
                }
                if(x<=xmax && x>=xmin)
                inter[c++]=x;
            }
        }
}

void MainWindow::sortP(int z)
{
    int temp,j,i;

            for(i=0;i<v;i++)
            {
                bresenham(p[i].x(),p[i].y(),p[i+1].x(),p[i+1].y()); // used to make hollow outlines of a polygon
            }
            delay(100);
            for(i=0; i<c;i+=2)
            {
                delay(100);
                bresenham(inter[i],z,inter[i+1],z);  // Used to fill the polygon ....
            }
}

void MainWindow::drawFill()
{
    int s;
    s=ymin+gridSize;

    while(s<=ymax)
    {
     ints(s);
     sortP(s);
     s++;
    }
}

//<---------------------------------------------------------------------------->
//<---------------------------------------------------------------------------->
//<---------------------------------------------------------------------------->

void MainWindow::breshEllipse(QPoint p1, float rx, float ry)
{
    QElapsedTimer t;
    t.start();

    if(gridSize != 0){
        rx = rx*gridSize;
        ry = ry*gridSize;
    }

    /*variables*/
    float ry1 = ry*ry;
    float rx1 = rx*rx;
    float ry2 = 2*ry1;
    float rx2 = 2*rx1;

    /* REGION 1 */

    /*starting point*/
    float x=0;
    float y=ry;

    /*center*/
    float xcenter = (float)p1.x();
    float ycenter = (float)p1.y();

    plotPoints(xcenter,ycenter,x,y);

    float p=(ry1-rx1*ry+(0.25*rx1));

    /*initial partial derivatives*/
    float px=0.0;
    float py=rx2*y;

    while(px<py){
        x=x+1;
        px=px+ry2;
        if(p>=0){
            y=y-1;
            py=py-rx2;
        }
        if(p<0)
            p=p+ry1+px;
        else
            p=p+ry1+px-py;

        plotPoints(xcenter,ycenter,x,y);

     }


    /* REGION 2*/
    p=(ry1*(x+0.5)*(x+0.5)+rx1*(y-1)*(y-1)-rx1*ry1);

    while(y>0){
        y=y-1;
        py=py-rx2;
        if(p<=0)
        {
            x=x+1;
            px=px+ry2;
        }
        if(p>0)
            p=p+rx1-py;
        else
            p=p+rx1-py+px;
        plotPoints(xcenter,ycenter,x,y);
    }
    ui->frame->setPixmap((QPixmap::fromImage(img)));
    QString esTime = QString::number(t.elapsed());
    QMessageBox msgBox;
    msgBox.setText("Total time: "+ esTime +" ms");
    msgBox.exec();
}

/*-----------------------------------------------------------------*/

void MainWindow::eightWayPlot(int xc,int yc,int x,int y){
    point(xc+x, yc+y,qRgb(200,10,10));
    point(xc-x, yc+y,qRgb(200,10,10));
    point(xc+x, yc-y,qRgb(200,10,10));
    point(xc-x, yc-y,qRgb(200,10,10));
    point(xc+y, yc+x,qRgb(200,10,10));
    point(xc-y, yc+x,qRgb(200,10,10));
    point(xc+y, yc-x,qRgb(200,10,10));
    point(xc-y, yc-x,qRgb(200,10,10));
}

void MainWindow::breshCircle(QPoint p1,int r){

    QElapsedTimer t;
    t.start();

    if(gridSize != 0){
        r = r*gridSize;
    }
    int x=0,y=r,d=3-(2*r);
    int xc = p1.x(),yc = p1.y();
    eightWayPlot(xc,yc,x,y);

    while(x<=y){
        if(d<=0){
            d=d+(4*x)+6;
          }
        else
          {
            d=d+(4*x)-(4*y)+10;
            y=y-1;
          }
          x=x+1;
          eightWayPlot(xc,yc,x,y);
    }
    ui->frame->setPixmap((QPixmap::fromImage(img)));
    QString esTime = QString::number(t.elapsed());
    QMessageBox msgBox;
    msgBox.setText("Total time: "+ esTime +" ms");
    msgBox.exec();

}
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/


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
        else{
            point(x, y, 30,30,30);
        }
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
    //point(ui->frame->x, ui->frame->y, setPoint);
    ui->frame->setPixmap(QPixmap::fromImage(img));
    ui->set_p1->setDisabled(true);
}

void MainWindow::on_set_p2_clicked()
{
    if (ui->frame->x == p1.x() && ui ->frame->y == p1.y()) return;
    p2.setX(ui->frame->x), p2.setY(ui->frame->y);
    ui->p2_label->setText("X: " + QString::number(transformX(ui->frame->x)) + ", Y: " + QString::number(transformY(ui->frame->y)));
    //point(ui->frame->x, ui->frame->y, setPoint);
    ui->frame->setPixmap(QPixmap::fromImage(img));
    ui->set_p2->setDisabled(true);
}

void MainWindow::on_unset_p1_clicked()
{
    if ((scale(p1.x()) == scale(img.width()/2)  || scale(p1.y()) == scale(img.height()/2)) && ui->show_axes->checkState() == Qt::Checked) point(p1.x(), p1.y());
    //else point(p1.x(), p1.y(), frameColor);
    p1.setX(-1), p1.setY(-1);
    ui->p1_label->setText("Not set");
    ui->frame->setPixmap(QPixmap::fromImage(img));
    ui->set_p1->setDisabled(false);
}

void MainWindow::on_unset_p2_clicked()
{
    if ((scale(p2.x()) == scale(img.width()/2)  || scale(p2.y()) == scale(img.height()/2)) && ui->show_axes->checkState() == Qt::Checked) point(p2.x(), p2.y());
    //else point(p2.x(), p2.y(), frameColor);
    p2.setX(-1), p2.setY(-1);
    ui->p2_label->setText("Not set");
    ui->frame->setPixmap(QPixmap::fromImage(img));
    ui->set_p2->setDisabled(false);
}


void MainWindow::on_draw_clicked()
{
   //draw ellipse
    QString rx = ui->lineEdit_4->text();
    float a = (float)rx.toInt();
    QString ry = ui->lineEdit_5->text();
    float b = (float)ry.toInt();

    breshEllipse(p1,a,b);

}

void MainWindow::on_draw_2_clicked()
{
    //bresenham
    QString radius = ui->lineEdit_3->text();
    int r = (int)radius.toInt();
    breshCircle(p1,r);

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

void MainWindow::on_pushButton_clicked()
{
    QString radius = ui->lineEdit->text();
    int r = (int)radius.toInt();
    polarCircle(p1,r);
}



void MainWindow::on_pushButton_2_clicked()
{
    QString radius = ui->lineEdit_2->text();
    int r = (int)radius.toInt();
    midCircle(p1,r);
}

void MainWindow::on_pushButton_3_clicked()
{
    //draw polygon
    QElapsedTimer t;
    t.start();
    int n = 6;
    QPoint points[n];
    points[0]=p1,points[1]=p2,points[2]=p3,points[3]=p4,points[4]=p5,points[5]=p6;
    drawPolygon(n,points);
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_pushButton_4_clicked()
{
    QElapsedTimer t;
    t.start();
    //Do the task
    //<-------------------------------------------->
    QRgb fillColor = qRgb(10,200,10);
    QRgb boundaryColor = qRgb(200,10,10);
    int x = p1.x();
    int y = p1.y();
    boundaryFill4(x,y,fillColor,boundaryColor);
    ui->frame->setPixmap((QPixmap::fromImage(img)));
    //<-------------------------------------------->
    QString esTime = QString::number(t.elapsed());
    QMessageBox msgBox;
    msgBox.setText("Total time: "+ esTime +" ms");
    msgBox.exec();
}

void MainWindow::on_pushButton_5_clicked()
{
    QElapsedTimer t;
    t.start();
    //Do the task
    //<-------------------------------------------->
    QRgb fillColor = qRgb(10,200,10);
    QRgb boundaryColor = qRgb(200,10,10);
    int x = p1.x();
    int y = p1.y();
    boundaryFill8(x,y,fillColor,boundaryColor);
    ui->frame->setPixmap((QPixmap::fromImage(img)));
    //<-------------------------------------------->
    QString esTime = QString::number(t.elapsed());
    QMessageBox msgBox;
    msgBox.setText("Total time: "+ esTime +" ms");
    msgBox.exec();
}

void MainWindow::on_pushButton_6_clicked()
{
    QElapsedTimer t;
    t.start();
    //Do the task
    //<-------------------------------------------->
    QRgb newColor = qRgb(200,10,10);

    int x = p1.x();
    int y = p1.y();
    point(x,y,qRgb(30,30,30));
    QRgb previousColor = qRgb(30,30,30);

    floodfill4(x,y,previousColor,newColor);
    //QThread *thread = QThread::create(&&boundaryFill4, x,y,fillColor,boundaryColor);
    // extra setup...
    //thread->start();
    ui->frame->setPixmap((QPixmap::fromImage(img)));
    //<-------------------------------------------->
    QString esTime = QString::number(t.elapsed());
    QMessageBox msgBox;
    msgBox.setText("Total time: "+ esTime +" ms");
    msgBox.exec();
}

void MainWindow::on_pushButton_7_clicked()
{
    bresenham(p1,p2);
    ui->frame->setPixmap((QPixmap::fromImage(img)));
}

void MainWindow::on_set_p1_2_clicked()
{
    p3.setX(ui->frame->x), p3.setY(ui->frame->y);
    ui->p1_label_2->setText("X: " + QString::number(transformX(ui->frame->x)) + ", Y: " + QString::number(transformY(ui->frame->y)));
    //point(ui->frame->x, ui->frame->y, setPoint);
    ui->frame->setPixmap(QPixmap::fromImage(img));
    ui->set_p1_2->setDisabled(true);
}

void MainWindow::on_set_p1_3_clicked()
{
    p4.setX(ui->frame->x), p4.setY(ui->frame->y);
    ui->p1_label_3->setText("X: " + QString::number(transformX(ui->frame->x)) + ", Y: " + QString::number(transformY(ui->frame->y)));
    //point(ui->frame->x, ui->frame->y, setPoint);
    ui->frame->setPixmap(QPixmap::fromImage(img));
    ui->set_p1_3->setDisabled(true);
}

void MainWindow::on_set_p1_4_clicked()
{
    p5.setX(ui->frame->x), p5.setY(ui->frame->y);
    ui->p1_label_4->setText("X: " + QString::number(transformX(ui->frame->x)) + ", Y: " + QString::number(transformY(ui->frame->y)));
    //point(ui->frame->x, ui->frame->y, setPoint);
    ui->frame->setPixmap(QPixmap::fromImage(img));
    ui->set_p1_4->setDisabled(true);
}

void MainWindow::on_set_p1_5_clicked()
{
    p6.setX(ui->frame->x), p6.setY(ui->frame->y);
    ui->p1_label_5->setText("X: " + QString::number(transformX(ui->frame->x)) + ", Y: " + QString::number(transformY(ui->frame->y)));
    //point(ui->frame->x, ui->frame->y, setPoint);
    ui->frame->setPixmap(QPixmap::fromImage(img));
    ui->set_p1_5->setDisabled(true);
}

void MainWindow::on_unset_p1_2_clicked()
{
    if ((scale(p3.x()) == scale(img.width()/2)  || scale(p3.y()) == scale(img.height()/2)) && ui->show_axes->checkState() == Qt::Checked) point(p3.x(), p3.y());
    //else point(p3.x(), p3.y(), frameColor);
    p3.setX(-1), p3.setY(-1);
    ui->p1_label_2->setText("Not set");
    ui->frame->setPixmap(QPixmap::fromImage(img));
    ui->set_p1_2->setDisabled(false);
}

void MainWindow::on_unset_p1_3_clicked()
{
    if ((scale(p4.x()) == scale(img.width()/2)  || scale(p4.y()) == scale(img.height()/2)) && ui->show_axes->checkState() == Qt::Checked) point(p4.x(), p4.y());
    //else point(p3.x(), p3.y(), frameColor);
    p4.setX(-1), p4.setY(-1);
    ui->p1_label_3->setText("Not set");
    ui->frame->setPixmap(QPixmap::fromImage(img));
    ui->set_p1_3->setDisabled(false);
}

void MainWindow::on_unset_p1_4_clicked()
{
    if ((scale(p5.x()) == scale(img.width()/2)  || scale(p5.y()) == scale(img.height()/2)) && ui->show_axes->checkState() == Qt::Checked) point(p5.x(), p5.y());
    //else point(p3.x(), p3.y(), frameColor);
    p5.setX(-1), p5.setY(-1);
    ui->p1_label_4->setText("Not set");
    ui->frame->setPixmap(QPixmap::fromImage(img));
    ui->set_p1_4->setDisabled(false);
}

void MainWindow::on_unset_p1_5_clicked()
{
    if ((scale(p6.x()) == scale(img.width()/2)  || scale(p6.y()) == scale(img.height()/2)) && ui->show_axes->checkState() == Qt::Checked) point(p6.x(), p6.y());
    //else point(p3.x(), p3.y(), frameColor);
    p6.setX(-1), p6.setY(-1);
    ui->p1_label_5->setText("Not set");
    ui->frame->setPixmap(QPixmap::fromImage(img));
    ui->set_p1_5->setDisabled(false);
}

void MainWindow::on_pushButton_8_clicked()
{
    drawFills();
}
void MainWindow::drawFills()
{
    //int x = (p1.x()+p4.x())/2;
    //int y = (p1.y()+p4.y())/2;
    QElapsedTimer t;
    t.start();
    //Do the task
    delay(200);
    //<-------------------------------------------->
    QRgb newColor = qRgb(200,10,10);

    int x = (p1.x()+p4.x())/2;
    int y = (p1.y()+p4.y())/2;
    point(x,y,qRgb(30,30,30));
    QRgb previousColor = qRgb(30,30,30);

    floodfill4(x,y,previousColor,newColor);
    ui->frame->setPixmap((QPixmap::fromImage(img)));
    //<-------------------------------------------->
    QString esTime = QString::number(t.elapsed());
    QMessageBox msgBox;
    msgBox.setText("Total time: "+ esTime +" ms");
    msgBox.exec();
}
