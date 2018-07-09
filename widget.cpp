#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->labelImageDisplay->setMouseTracking(true);//track mouse movement
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    cout << "In widget: " << event->pos().x() << ", " << event->pos().y() << endl;
    QPoint p = ui->labelImageDisplay->mapFromParent(event->pos());
    cout << "In label: " << p.x() << ", " << p.y() << endl;
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
        Mat imageROI(imageOriginal,Rect(2000,1000,1500,1500));
        cv::resize(imageROI, imageManipulated, Size(), 0.3, 0.3, CV_INTER_AREA );

        cout << "Width: " << imageManipulated.cols << " Hight: " << imageManipulated.rows << endl;

        Mat tempImage;
        cvtColor(imageManipulated, tempImage, CV_BGR2GRAY);
        QImage imgOut= QImage((uchar*) tempImage.data, tempImage.cols, tempImage.rows, tempImage.step, QImage::Format_Grayscale8);
        QPixmap pixmap = QPixmap::fromImage(imgOut);
        ui->labelImageDisplay->setFixedHeight(imgOut.height());
        ui->labelImageDisplay->setFixedWidth(imgOut.width());
        ui->labelImageDisplay->setPixmap(pixmap);//that's how to do it

    }
}




