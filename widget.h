#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QMessageBox>

#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/highgui.hpp>

#include <iostream>
#include <string>



namespace Ui {
class Widget;
}

using namespace cv;
using namespace std;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
     ~Widget() override;

protected:
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void on_imageOpenButton_clicked();
    void on_verticalSliderZoomInOut_valueChanged(int value);


private:
    Ui::Widget *ui;
    Mat imageOriginal, imageManipulated;

    double imageScaling;//scaling to fit the label after open
    double zoomFactor;//1.0 - as scaled after open
    QPoint centerOfROI;//ROI center coords in original image
};

#endif // WIDGET_H
