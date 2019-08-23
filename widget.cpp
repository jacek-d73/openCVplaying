#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->labelImageDisplay->setMouseTracking(true);//track mouse movement
    this->imageScaling = 100;
    this->zoomFactor = 1.0;

    //update slider and label
    ui->verticalSliderZoomInOut->setValue(imageScaling);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        QPoint const p = ui->labelImageDisplay->mapFromParent(event->pos());//get cursor coords and maps them to image display coords
        if(p.x()>=0 && p.x()<= ui->labelImageDisplay->width()
        && p.y()>=0 && p.y()<= ui->labelImageDisplay->height())//check if within image display
        {
            cout << "In label: " << p.x() << ", " << p.y() << endl;

            //original image scale
            double const scale(zoomFactor * imageScaling);
            //cout << "In original image: " << static_cast<int>(p.x()/scale) << ", " << static_cast<int>(p.y()/scale) << endl;

            //define center of ROI
            centerOfROI.setX(p.x()/scale);
            centerOfROI.setY(p.y()/scale);
        }
    }
}

void Widget::on_imageOpenButton_clicked()
{
    cv::String imageFilePathName( "/home/jacdob/Pictures/7B0A4522.JPG" );
    imageOriginal = cv::imread( imageFilePathName, IMREAD_GRAYSCALE );

    if(imageOriginal.empty())
    {
        QMessageBox msgBox;
        msgBox.setText("Could not open or find the image.");
        msgBox.exec();
    }
    else
    {
        //define center of ROI
        centerOfROI.setX(imageOriginal.cols / 2);
        centerOfROI.setY(imageOriginal.rows / 2);

        //initial fitting image into label size
        imageScaling = ui->labelImageDisplay->maximumWidth()/imageOriginal.cols;

        //update slider and label -> this will fire Widget::on_verticalSliderZoomInOut_valueChanged
        ui->verticalSliderZoomInOut->setValue(zoomFactor);
    }
}


void Widget::on_verticalSliderZoomInOut_valueChanged(int value)
{
    cout<< "Slider value: " << value <<endl;
    zoomFactor = static_cast<double>(value)/10.0;//set actual zoom factor referenced to imageScaling
    QString s = QString::number(zoomFactor);
    ui->labelZoomSliderValue->setText(s);

    if(!imageOriginal.empty())
    {

        //create rectangle based on original image size and zoom factor
        //int rectXsize(imageOriginal.cols/static_cast<int>(zoomFactor));
        //int rectYsize(imageOriginal.rows/static_cast<int>(zoomFactor));
        int rectXsize(imageOriginal.cols/zoomFactor);
        int rectYsize(imageOriginal.rows/zoomFactor);

        //openCV Rect(x,y,width,hight)
        Rect myROI(centerOfROI.x() - rectXsize / 2 ,centerOfROI.y() - rectYsize / 2 ,rectXsize ,rectYsize);
        imageManipulated = imageOriginal(myROI);

        double scale(zoomFactor * imageScaling);

        cout << "Original image scaling: " << imageScaling << ", ROI scaling: " << scale << endl;

        Mat tempImage;

        cv::resize(imageManipulated, tempImage, Size(), scale, scale, INTER_CUBIC);

//        namedWindow("imageManipulated", WINDOW_AUTOSIZE);
//        imshow("imageManipulated", imageManipulated);
//        waitKey();

//        namedWindow("tempImage", WINDOW_AUTOSIZE);
//        imshow("tempImage", tempImage);
//        waitKey();

        cout << "Width: " << imageManipulated.cols << " Hight: " << imageManipulated.rows << endl;

        //cvtColor(tempImage, tempImage, CV_BGR2GRAY);
        QImage imgOut= QImage(static_cast<uchar*>(tempImage.data), tempImage.cols, tempImage.rows, tempImage.step, QImage::Format_Grayscale8);
        QPixmap pixmap = QPixmap::fromImage(imgOut);
        ui->labelImageDisplay->setFixedHeight(imgOut.height());
        ui->labelImageDisplay->setFixedWidth(imgOut.width());
        ui->labelImageDisplay->setPixmap(pixmap);//that's how to do it;
    }
}


