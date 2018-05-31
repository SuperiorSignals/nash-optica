#ifndef LENSWINDOW_H
#define LENSWINDOW_H

#include <QMainWindow>
#include <QOpenGLwindow>
#include <QSurfaceFormat>
#include <QOpenGLFunctions>
#include <QtOpenGL>
#include <GL/GLU.h>

#include "customlens.h"
#include "sphericallens.h"

#define SAMPLE_COUNT 10240
#define STEP 6

enum DisplayType {CUSTOM_LENS, SPHERICAL_LENS};

void glPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
void display(float rotation);
void displayCustomLens(CustomLens input, float imageRotation);
void displaySphericalLens(SphericalLens input, float imageRotation);
void generateCustomLensVertices(CustomLens input, GLfloat *vertices);
void generateSphericalLensVertices(SphericalLens input, GLfloat *vertices);
void quickTransformCustomVertices(GLfloat vertices[SAMPLE_COUNT * STEP]);
void quickTransformVertices(GLfloat *vertices, int count);

class LensWindow : public QOpenGLWindow
{
    Q_OBJECT

public:
    LensWindow(QWidget *parent = 0);
    ~LensWindow();
    DisplayType getCurrentDisplay();
    double getFirstParameter();
    double getSecondParameter();
    GLfloat getThickness();
    GLfloat getFocalLength();
    void setCurrentDisplay(DisplayType input);
    void setFirstParameter(double input);
    void setSecondParameter(double input);
    void setThickness(GLfloat input);
    void setFocalLength(GLfloat input);

    CustomLens customLens;
    SphericalLens sphericalLens;

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

public slots:
    void updateAnimation();

private:
    QOpenGLContext *context;
    QOpenGLFunctions *openGLFunctions;
    float rotation;
    DisplayType currentDisplay;
    double firstParameter;
    double secondParameter;
    GLfloat thickness;
    GLfloat focalLength;
};

#endif // LENSWINDOW_H
