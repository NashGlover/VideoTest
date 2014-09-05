#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include "VideoStreamer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void startVideo();

signals:
	void valueChanged(int);
	void newDimensions(int, int);

private slots:
	void on_gamma_slider_valueChanged(int value);
    void update_frame(QImage, QImage);
    void update_fps(QString);
	void updateDimensions(int, int);

	void on_width_input_editingFinished();

	void on_height_input_editingFinished();

	void on_Capture_clicked();

	void on_pushButton_clicked();

private:
	void resizeEvent(QResizeEvent*);
	Ui::MainWindow *ui;
    VideoStreamer* video;
	QThread *thread;
};

#endif // MAINWINDOW_H
