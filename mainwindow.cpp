#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::startVideo()
{
	thread = new QThread(this);
    video = new VideoStreamer();

	video->moveToThread(thread);
	connect(thread, SIGNAL(started()),  video,  SLOT(onStarted()));
	connect(video,  SIGNAL(finished()), thread, SLOT(quit()));
	connect(video,  SIGNAL(finished()), video,  SLOT(deleteLater()));
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	connect(this, SIGNAL(newDimensions(int, int)), this, SLOT(updateDimensions(int, int)));
	//connect(ui->gamma_slider, SIGNAL(valueChanged(int)), video, SLOT(onGammaChanged(int)));

    /* For updating fps and frame */
    connect(video, SIGNAL(new_frame(QImage, QImage)), this, SLOT(update_frame(QImage, QImage)), Qt::BlockingQueuedConnection);
    connect(video, SIGNAL(new_fps(QString)), this, SLOT(update_fps(QString)), Qt::BlockingQueuedConnection);
}

void MainWindow::update_frame(QImage input_image, QImage output_image)
{
    //qDebug() << "Updating Frame";
    ui->input_label->setPixmap(QPixmap::fromImage(input_image));
    ui->output_label->setPixmap(QPixmap::fromImage(output_image));
}

void MainWindow::update_fps(QString input_string)
{
    ui->fps_label->setText(input_string);
}

void MainWindow::on_gamma_slider_valueChanged(int value)
{
    //qDebug() << "Value changed";
    video->changed = true;
    video->gammaValue = value;
	//emit valueChanged(value);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   qDebug() << "Width is " << ui->input_label->width();
   emit newDimensions(ui->input_label->width(), ui->output_label->height());
   // Your code here.
}

void MainWindow::updateDimensions(int in_width, int in_height)
{
	qDebug() << "New width: " << in_width << "New height: " << in_height;
}

void MainWindow::on_width_input_editingFinished()
{
	video->width = ui->width_input->text().toInt();
}

void MainWindow::on_height_input_editingFinished()
{
	video->height = ui->height_input->text().toInt();
}

void MainWindow::on_Capture_clicked()
{
	thread->start();
}

void MainWindow::on_pushButton_clicked()
{
	qDebug() << ui->input_label->width() << ui->output_label->height();
}
