#ifndef VIDEOSTREAMER_H
#define VIDEOSTREAMER_H

#include <QObject>
#include <QImage>
#include <opencv2/highgui/highgui.hpp>
#include <QBuffer>
#include <QByteArray>
#include <QFile>
#include <QTimer>

class QTime;
class QLabel;

class VideoStreamer : public QObject
{
	Q_OBJECT
public:
    explicit VideoStreamer();
    int gammaValue;
    bool changed;
	int height;
	int width;
	char *buffer;
	QTime *time_stamp;

signals:
	void finished();
    void new_frame(QImage, QImage);
    void new_fps(QString);

public slots:
	void onStarted();
	void onGammaChanged(int progress);

private:
	QLabel* input;
	QLabel* output;
	QLabel* fps_label;
	QTime* timer;
	int frames;
    cv::Mat _lut_matrix;
};

#endif // VIDEOSTREAMER_H
