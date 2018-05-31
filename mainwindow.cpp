#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "lenswindow.h"

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

void MainWindow::setLensWindow(LensWindow *input)
{
    lensWindow = input;
}

void MainWindow::initializeValues()
{
    QString temporary;

    temporary = QString::number(lensWindow->getFirstParameter());
    ui->firstParameterLineEdit->setText(temporary);
    temporary = QString::number(lensWindow->getSecondParameter());
    ui->secondParameterLineEdit->setText(temporary);
    temporary = QString::number(lensWindow->getThickness());
    ui->baseThicknessLineEdit->setText(temporary);
    temporary = QString::number(lensWindow->getFocalLength());
    ui->focalLengthLineEdit->setText(temporary);
}

void MainWindow::on_enterPushButton_clicked()
{
    QString temporary;

    if (ui->lensTypeComboBox->currentText() == "Custom Lens") {
        lensWindow->setCurrentDisplay(CUSTOM_LENS);
    } else if (ui->lensTypeComboBox->currentText() == "Spherical Lens") {
        lensWindow->setCurrentDisplay(SPHERICAL_LENS);
    }

    if (lensWindow->getCurrentDisplay() == SPHERICAL_LENS) {
        temporary = ui->firstParameterLineEdit->text();
        lensWindow->setFirstParameter(temporary.toDouble());
        temporary = ui->secondParameterLineEdit->text();
        lensWindow->setSecondParameter(temporary.toDouble());
        temporary = ui->baseThicknessLineEdit->text();
        lensWindow->setThickness(temporary.toDouble());
    } else if (lensWindow->getCurrentDisplay() == CUSTOM_LENS) {
        temporary = ui->firstParameterLineEdit->text();
        lensWindow->setFirstParameter(temporary.toDouble());
        temporary = ui->secondParameterLineEdit->text();
        lensWindow->setSecondParameter(temporary.toDouble());
        temporary = ui->baseThicknessLineEdit->text();
        lensWindow->setThickness(temporary.toFloat());
        temporary = ui->focalLengthLineEdit->text();
        lensWindow->setFocalLength(temporary.toFloat());
    }

    temporary = QString::number(lensWindow->getFirstParameter());
    ui->firstParameterLineEdit->setText(temporary);
    temporary = QString::number(lensWindow->getSecondParameter());
    ui->secondParameterLineEdit->setText(temporary);
    temporary = QString::number(lensWindow->getThickness());
    ui->baseThicknessLineEdit->setText(temporary);
    temporary = QString::number(lensWindow->getFocalLength());
    ui->focalLengthLineEdit->setText(temporary);
}
