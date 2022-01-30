#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QtMath>
#include <QTimer>
#include <QFile>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,ui(new Ui::MainWindow)
    ,_scene(new QGraphicsScene)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(_scene);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fitIn()
{
    qDebug() << "FIT";
    ui->graphicsView->fitInView(_scene->sceneRect(),Qt::KeepAspectRatio);
}

void MainWindow::on_pushButton_clicked()
{
    ui->graphicsView->scale(1.1, 1.1);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->graphicsView->scale(0.9, 0.9);
}

void MainWindow::on_pushButton_3_clicked()
{
    QFile file;
    QString fileName = QFileDialog::getSaveFileName(0, "Save as", "export.csv","csv (*.csv)"); // export csv file
    file.setFileName(fileName);

    file.open(QFile::WriteOnly | QFile::Text);

    QTextStream txtStream(&file);

    QPen _penPoint;
    _penPoint.setBrush(QColorConstants::Black);
    _penPoint.setWidth(1);

    QBrush brush;
    brush.setColor(Qt::black);


    double rayon = ui->rayon_dsb->value();
    double hauteur = ui->hauteur_dsb->value();
    double step = ui->step_dsb->value();
    double min = ui->min_dsb->value();
    double max = ui->max_dsb->value();

    _scene->clear();
    _ellipseItem.clear();

    ui->progressBar->setEnabled(true);

    for (double t=min; t<max; t+=step)
    {
        double y = (rayon*qSqrt(acos(1-(2*t/hauteur))-((sin(2*(acos(1-(2*t/hauteur)))))/2)))/qSqrt(M_PI);


        if ( (!isnan(t)) && (!isnan(y)) )
        {
            _ellipseItem.append(_scene->addEllipse(t, y, 1, 1, _penPoint, brush));
            _ellipseItem.append(_scene->addEllipse(t, -y, 1, 1, _penPoint, brush));
            qDebug() << t << y;
            txtStream << t << ";" << y << "\n";
        }

        ui->progressBar->setValue(t/max * 100);
    }

    ui->progressBar->setEnabled(false);
    ui->progressBar->setValue(100);

    _scene->addLine(0,0,hauteur,0,_penPoint);

    file.close();

    QTimer::singleShot(1000, this, SLOT(fitIn()));

}
