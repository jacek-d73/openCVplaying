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
    this->actualOriginX = 0;
    this->actualOriginY = 0;
    //update slider and label
    ui->verticalSliderZoomInOut->setValue(imageScaling);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
//    cout << "In widget: " << event->pos().x() << ", " << event->pos().y() << endl;
//    QPoint p = ui->labelImageDisplay->mapFromParent(event->pos());
//    cout << "In label: " << p.x() << ", " << p.y() << endl;

    if(event->buttons() & Qt::LeftButton)
    {
        if(event->pos().x()>=0 && event->pos().x()<= ui->labelImageDisplay->geometry().width()
        && event->pos().y()>=0 && event->pos().y()<= ui->labelImageDisplay->geometry().height())
        {
            cout << "In widget: " << event->pos().x() << ", " << event->pos().y() << endl;
        }
        else
        {
            cout << "Out of the borders." << endl;
        }
    }
}

void Widget::on_imageOpenButton_clicked()
{
    String imageFilePathName( "/home/jacdob/Pictures/7B0A4522.JPG" );
    imageOriginal = imread( imageFilePathName, IMREAD_UNCHANGED );
    if(imageOriginal.empty())
    {
        cout << "Could not open or find the image." << std::endl ;
        // !!!handle invalid input
    }
    else
    {
        cout << "Image size: " << imageOriginal.cols << "x" << imageOriginal.rows <<endl;
        cout << "Image aspect: " << static_cast<double>(imageOriginal.cols)/static_cast<double>(imageOriginal.rows) <<endl;

        //initial fitting image into label size
        imageScaling = static_cast<double>(ui->labelImageDisplay->maximumWidth())/static_cast<double>(imageOriginal.cols);

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
        int rectXsize(imageOriginal.cols/zoomFactor);
        int rectYsize(imageOriginal.rows/zoomFactor);
        Rect myROI(0,0,rectXsize,rectYsize);
        imageManipulated = imageOriginal(myROI);

        double scale(zoomFactor * imageScaling);

        cout << "Original image scaling: " << imageScaling << ", ROI scaling: " << scale << endl;

        Mat tempImage;
        cv::resize(imageManipulated, tempImage, Size(), scale, scale, CV_INTER_AREA );

//        namedWindow("imageManipulated", WINDOW_AUTOSIZE);
//        imshow("imageManipulated", imageManipulated);
//        waitKey();

//        namedWindow("tempImage", WINDOW_AUTOSIZE);
//        imshow("tempImage", tempImage);
//        waitKey();

        cout << "Width: " << imageManipulated.cols << " Hight: " << imageManipulated.rows << endl;

        cvtColor(tempImage, tempImage, CV_BGR2GRAY);
        QImage imgOut= QImage(static_cast<uchar*>(tempImage.data), tempImage.cols, tempImage.rows, tempImage.step, QImage::Format_Grayscale8);
        QPixmap pixmap = QPixmap::fromImage(imgOut);
        ui->labelImageDisplay->setFixedHeight(imgOut.height());
        ui->labelImageDisplay->setFixedWidth(imgOut.width());
        ui->labelImageDisplay->setPixmap(pixmap);//that's how to do it;
    }
}

void Widget::on_verticalSliderZoomInOut_sliderMoved(int position)
{
    cout << "Slider position: " << position <<endl;


}
