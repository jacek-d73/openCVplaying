#ifndef WIDGET_H
#define WIDGET_H


#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>

#include <QWidget>
#include <QMouseEvent>


using namespace cv;
using namespace std;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void on_imageOpenButton_clicked();

    void on_verticalSliderZoomInOut_valueChanged(int value);

    void on_verticalSliderZoomInOut_sliderMoved(int position);

private:
    Ui::Widget *ui;
    Mat imageOriginal, imageManipulated;

    double imageScaling;//scaling to fit the label after open
    double zoomFactor;//1.0 - as scaled after open
    int actualOriginX, actualOriginY;//actual left, upper corner of ROI
};

#endif // WIDGET_H
