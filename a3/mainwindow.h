#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <vector>
#include <utility>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void showCurrentMousePosition(QPoint&);
    void mousePressed();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_show_axes_stateChanged(int arg1);

    void on_grid_size_valueChanged(int arg1);

    void on_reset_clicked();

    void on_set_p1_clicked();

    void on_set_p2_clicked();

    void on_unset_p2_clicked();

    void on_unset_p1_clicked();

    void on_draw_clicked();

    void on_draw_2_clicked();

    void on_reset_2_clicked();

    void on_frame_linkHovered(const QString &link);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_set_p1_2_clicked();

    void on_set_p1_3_clicked();

    void on_set_p1_4_clicked();

    void on_set_p1_5_clicked();

    void on_unset_p1_2_clicked();

    void on_unset_p1_3_clicked();

    void on_unset_p1_4_clicked();

    void on_unset_p1_5_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::MainWindow *ui;
    int gridSize;
    QPoint p1, p2 , p3 , p4 , p5 , p6;
    int inter[20],x,y;
    int v,xmin,ymin,xmax,ymax,c;
    QPoint p[7];

    //////////////////// BASIC UTILITY FUNCTION////////////////////
    void point(int x, int y, int r=200, int g=200, int b=200);
    void point(int x, int y, QRgb color);
    void clear_frame();
    int scale(int X);
    int transformX(int X);
    int transformY(int Y);
    void reset();
    void delay(int number_of_seconds);
    QRgb getPointColor(int x,int y);
    void dda(QPoint p1,QPoint p2);
    void bresenham(QPoint p1,QPoint p2);
    void polarCircle(QPoint p1,int r);
    void midCircle(QPoint p1,int r);
    void breshCircle(QPoint p1,int r);
    void eightWayPlot(int xc,int yc,int x,int y);
    void breshEllipse(QPoint p1,float a,float b);
    void plotPoints(int xc,int yc,int x,int y);
    void boundaryFill4(int x,int y,QRgb fillColor,QRgb boundaryColor);
    void boundaryFill8(int x,int y,QRgb fillColor,QRgb boundaryColor);
    void floodfill4(int x,int y,QRgb previousColor,QRgb newColor);
    void drawPolygon(int n,QPoint ar[]);
    void scanFill(QPoint ar[]);
    void bresenham(int x1,int y1,int x2,int y2);
    double getClockwiseAngle(QPoint p);
    bool comparePoints(QPoint p1, QPoint p2);
    void sortPoints(QPoint q[]);
    void drawFills();
    //////////////////////////////////////////////////////////////////
    void readSFL();
    void calcs();
    void ints(float z);
    void sortP(int z);
    void drawFill();


    ////////////////// FUNCTIONS FOR GRID AND AXES////////////////////
    void draw_axes();
    void remove_axes();
    void draw_grid();
};
#endif // MAINWINDOW_H
