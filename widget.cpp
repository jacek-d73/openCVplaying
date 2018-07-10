#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->labelImageDisplay->setMouseTracking(true);//track mouse movement
    this->imageScaling = 100;
    //update slider and label
    ui->verticalSliderZoomInOut->setValue(imageScaling);
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
        imageScaling = 100 * ui->labelImageDisplay->maximumWidth()/imageOriginal.cols;

        //update slider and label
        ui->verticalSliderZoomInOut->setValue(imageScaling);

        //Mat imageROI(imageOriginal,Rect(2000,1000,1500,1500));
        double scale(static_cast<double>(imageScaling)/100.0);
        cv::resize(imageOriginal, imageManipulated, Size(), scale, scale, CV_INTER_AREA );

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


void Widget::on_verticalSliderZoomInOut_valueChanged(int value)
{
    QString s = QString::number(value);
    ui->labelZoomSliderValue->setText(s);
}
