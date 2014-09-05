#include "VideoStreamer.h"

#include <opencv2/opencv.hpp>

#include <QDebug>

#include <QImage>
#include <QLabel>
#include <qmath.h>
#include <QPixmap>
#include <QThread>
#include <QTime>

//static cv::Mat _lut_matrix;

VideoStreamer::VideoStreamer()
{
	timer = new QTime();
	timer->start();
	frames = 0;
    onGammaChanged(50);
    changed = false;
	time_stamp = new QTime();
}

void VideoStreamer::onStarted()
{
	// Do Stuff
	//cv::VideoCapture cap("http://192.168.1.115:80/videostream.cgi?user=admin&pwd=irislab&video=vid.mjpeg");
	cv::VideoCapture cap(0);
	//cv::VideoCapture cap("http://192.168.1.110:8080/vid.mjpg");
	/*if (!cap.isOpened())
    {
        qDebug() << "Error opening!";
    }*/
	time_stamp->start();
	qDebug() << "Start time is " << time_stamp->toString("hh:mm:ss.zzz");
	//cv::VideoCapture cap("/home/irislab/ncc/output.avi");
    cv::Mat frame;
	cv::Mat last_frame;
	cv::Mat processed_frame;
    //cv::Mat frame_roi;
	double fps;
	double left = 0;
	double speed = 1000.0 / cap.get(CV_CAP_PROP_FPS);
	QTime single_frame;
	single_frame.start();
	qDebug() << "Width: " << width << "Height: " << height;
	cv::Rect roi(0,480-height,width,height);
    while(cap.isOpened())
	{
        //qDebug() << "Capturing";
        if (changed)
        {
            //qDebug() << "Changed " << gammaValue;
            changed = false;
            onGammaChanged(gammaValue);
        }
        //qDebug() << "Gamma value " << gammaValue;
        cap >> frame;
		if (!last_frame.empty())
		{
			if (cv::norm(frame, last_frame) < 5000) {
				qDebug() << "NO DIFFERENCE";
			}
		}
		else {
			qDebug() << "first";
		}
		//qDebug() << "CAP " << time_stamp->elapsed();
        frame = frame(roi);
        cv::cvtColor(frame, frame, CV_BGR2RGB);
        //qDebug() << "FRAME WIDTH: " << processed_frame.cols << endl;
        //processed_frame = frame.clone();
       // frame_roi = processed_frame(roi);
        cv::LUT( frame, _lut_matrix, processed_frame );

        left = speed - single_frame.elapsed();
		single_frame.restart();
        if (left > 0){
            QThread::msleep(left*2);
        }

        fps = (1000.0 / timer->elapsed() * ++frames);
		if (frames == 100) {
			frames = 0;
			timer->restart();
		}
        QImage input_image = QImage((unsigned char*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        QImage output_image = QImage((unsigned char*)processed_frame.data, processed_frame.cols, processed_frame.rows, processed_frame.step, QImage::Format_RGB888);
        QString new_fps_string = QString("FPS: %1").arg(fps);
        //qDebug() << new_fps_string;
        emit new_frame(input_image, output_image);
        emit new_fps(new_fps_string);
		last_frame = frame.clone();
	}
    //qDebug() << "After the while";
	emit finished();
}

void VideoStreamer::onGammaChanged(int progress)
{
    //qDebug() << "Setting the gamma " << progress;
    double inverse_gamma = 1 / qPow(10, progress / 50.0 - 1);
    //inverse_gamma = 1;
	_lut_matrix = cv::Mat(1, 256, CV_8UC1);
	uchar * ptr = _lut_matrix.ptr();
	for( int i = 0; i < 256; i++ )
		ptr[i] = (int)( pow( (double) i / 255.0, inverse_gamma ) * 255.0 );
}
