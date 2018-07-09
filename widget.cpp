#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_imageOpenButton_clicked()
{
    String imageFilePathName( "/home/jacdob/Pictures/7B0A4522.JPG" );
    imageOriginal = imread( imageFilePathName, IMREAD_UNCHANGED );
    if(imageOriginal.empty())
    {
        cout << "Could not open or find the image." << std::endl ;
        //handle invalid input
    }
    else
    {
        cv::resize(imageOriginal, imageManipulated, Size(), 0.1, 0.1, CV_INTER_AREA );
        Mat tempImage;
        cvtColor(imageManipulated, tempImage, CV_BGR2GRAY);
        QImage imgOut= QImage((uchar*) tempImage.data, tempImage.cols, tempImage.rows, tempImage.step, QImage::Format_Grayscale8);
        QPixmap pixmap = QPixmap::fromImage(imgOut);
        ui->labelImageDisplay->setFixedHeight(imgOut.height());
        ui->labelImageDisplay->setFixedWidth(imgOut.width());
        ui->labelImageDisplay->setPixmap(pixmap);//that's how to do it

    }

}
