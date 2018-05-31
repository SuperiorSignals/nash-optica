#include <QApplication>

#include "customlens.h"
#include "lenswindow.h"
#include "mainwindow.h"
#include "matrix.h"
#include "sphericallens.h"

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);
    MainWindow mainWindow;
    LensWindow lensWindow;
    CustomLens customLens;
    SphericalLens sphericalLens;

    mainWindow.setLensWindow(&lensWindow);
    mainWindow.initializeValues();
    mainWindow.show();
    lensWindow.customLens = matrix(GAUSSIAN, 0.5, 0.7);
    lensWindow.customLens.setFocalLength(0.1);
    lensWindow.customLens.setBaseThickness(0.1);
    lensWindow.show();
    lensWindow.resize(640, 480);

    return a.exec();
}
