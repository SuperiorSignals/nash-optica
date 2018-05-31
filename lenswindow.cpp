#include "lenswindow.h"
#include <iostream>
#include <vector>
#include "customlens.h"
#include "matrix.h"
#include "sphericallens.h"

int width = 640;
int height = 480;

LensWindow::LensWindow(QWidget *parent)
{
    QSurfaceFormat format;
    QTimer *timer;

    setSurfaceType(QWindow::OpenGLSurface);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setVersion(2, 1);
    setFormat(format);
    context = new QOpenGLContext;
    context->setFormat(format);
    context->create();
    context->makeCurrent(this);
    openGLFunctions = context->functions();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateAnimation()));
    timer->start();
    rotation = 0;

    currentDisplay = CUSTOM_LENS;
    firstParameter = 0.5;
    secondParameter = 0.7;
    thickness = 0.1;
    focalLength = 0.1;
}

LensWindow::~LensWindow()
{

}

DisplayType LensWindow::getCurrentDisplay()
{
    return currentDisplay;
}

double LensWindow::getFirstParameter()
{
    if (currentDisplay == CUSTOM_LENS) {
        return firstParameter;
    } else if (currentDisplay == SPHERICAL_LENS) {
        return (sphericalLens.getFront()).getRadius();
    }

    return 0;
}

double LensWindow::getSecondParameter()
{
    if (currentDisplay == CUSTOM_LENS) {
        return secondParameter;
    } else if (currentDisplay == SPHERICAL_LENS) {
        return (sphericalLens.getBack()).getRadius();
    }

    return 0;
}

GLfloat LensWindow::getThickness()
{
   if (currentDisplay == CUSTOM_LENS) {
       return thickness;
   } else if (currentDisplay == SPHERICAL_LENS) {
       return sphericalLens.getThickness();
   }

   return 0; // Should not reach
}

GLfloat LensWindow::getFocalLength()
{
    if (currentDisplay == CUSTOM_LENS) {
        return focalLength;
    } else if (currentDisplay == SPHERICAL_LENS) {
        return sphericalLens.getFocalLength();
    }

    return 0;   // Should not reach
}

void LensWindow::setCurrentDisplay(DisplayType input)
{
    currentDisplay = input;
}

void LensWindow::setFirstParameter(double input)
{
    firstParameter = input;
    if (currentDisplay == CUSTOM_LENS) {
        customLens = matrix(GAUSSIAN, firstParameter, secondParameter);
    } else if (currentDisplay == SPHERICAL_LENS) {
        sphericalLens.changeFrontRadius(firstParameter);
    }
}

void LensWindow::setSecondParameter(double input)
{
    secondParameter = input;
    if (currentDisplay == CUSTOM_LENS) {
        customLens = matrix(GAUSSIAN, firstParameter, secondParameter);
    } else if (currentDisplay == SPHERICAL_LENS) {
        sphericalLens.changeBackRadius(secondParameter);
    }
}

void LensWindow::setThickness(GLfloat input)
{
    thickness = input;
    if (currentDisplay == CUSTOM_LENS) {
        customLens.setBaseThickness(thickness);
    } else if (currentDisplay == SPHERICAL_LENS) {
        sphericalLens.setThickness(thickness);
    }
}

void LensWindow::setFocalLength(GLfloat input)
{
    focalLength = input;
    if (currentDisplay == CUSTOM_LENS) {
        customLens.setFocalLength(focalLength);
    } else if (currentDisplay == SPHERICAL_LENS) {
        // Do nothing; focal length is not adjustable.
    }
}

void LensWindow::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    resizeGL(this->width(), this->height());
}

void LensWindow::resizeGL(int w, int h)
{
    /*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    qreal aspectRatio = qreal(w) / qreal(h);
    glOrtho(-1 * aspectRatio, 1 * aspectRatio, -1, 1, 1, -1);
    */
    qreal aspectRatio;

    glViewport(0, 0, w, h); // Set viewport
    aspectRatio = qreal(w) / qreal(h);
    glMatrixMode(GL_PROJECTION); // Initialize projection matrix
    glLoadIdentity();
    //gluPerspective(75, aspectRatio, 0.1, 400000000);
    glPerspective(75, aspectRatio, 0.1, 400000000);
    glMatrixMode(GL_MODELVIEW); // Initialize model view matrix
    glLoadIdentity();
}

void LensWindow::paintGL()
{
    if (currentDisplay == CUSTOM_LENS) {
        displayCustomLens(customLens, rotation);
    } else if (currentDisplay == SPHERICAL_LENS) {
        displaySphericalLens(sphericalLens, rotation);
    } else {
        display(rotation);
    }
}

void LensWindow::resizeEvent(QResizeEvent *event)
{
    resizeGL(this->width(), this->height());
    this->update();
}

void LensWindow::paintEvent(QPaintEvent *event)
{
    paintGL();
}

void LensWindow::updateAnimation()
{
    rotation += 0.001;
    this->update();
}

void glPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;

    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan( fovY / 360 * pi ) * zNear;
    fW = fH * aspect;
    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

void display(float rotation)
{
    /*
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
        glColor3f(0.0f, 0.5f, 0.5f);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glVertex2f(0.5f, 0.5f);
        glVertex2f(-0.5f, 0.5f);
    glEnd();

    glBegin(GL_TRIANGLES);
        glColor3f(0.5f, 0.5f, 0.0f);
        glVertex2f(-0.25f, -0.25f);
        glVertex2f(0.5f, -0.125f);
        glVertex2f(-0.125f, -0.5f);
    glEnd();

    glFlush();
    */
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); // Reset model view matrix
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.0); // 3D transformation
    glRotatef(rotation, 1.0, 1.0, 1.0);

    // FRONT
    glBegin(GL_POLYGON);
        glColor3f(0.0,  0.0, 0.0);
        glVertex3f(0.5, -0.5, -0.5);
        glVertex3f(0.5, 0.5, -0.5);
        glVertex3f(-0.5, 0.5, -0.5);
        glVertex3f(-0.5, -0.5, -0.5);
    glEnd();

    // BACK
    glBegin(GL_POLYGON);
        glColor3f(0.0,  1.0, 0.0);
        glVertex3f(0.5, -0.5, 0.5);
        glVertex3f(0.5, 0.5, 0.5);
        glVertex3f(-0.5, 0.5, 0.5);
        glVertex3f(-0.5, -0.5, 0.5);
    glEnd();

    // RIGHT
    glBegin(GL_POLYGON);
        glColor3f(1.0, 0.0, 1.0);
        glVertex3f(0.5, -0.5, -0.5);
        glVertex3f(0.5, 0.5, -0.5);
        glVertex3f(0.5, 0.5, 0.5);
        glVertex3f(0.5, -0.5, 0.5);
    glEnd();

    // LEFT
    glBegin(GL_POLYGON);
        glColor3f(1.0, 1.0, 0.0);
        glVertex3f(-0.5, -0.5, 0.5);
        glVertex3f(-0.5, 0.5, 0.5);
        glVertex3f(-0.5, 0.5, -0.5);
        glVertex3f(-0.5, -0.5, -0.5);
    glEnd();

    // TOP
    glBegin(GL_POLYGON);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(0.5, 0.5, 0.5);
        glVertex3f(0.5, 0.5, -0.5);
        glVertex3f(-0.5, 0.5, -0.5);
        glVertex3f(-0.5, 0.5, 0.5);
    glEnd();

    // Red side - BOTTOM
    glBegin(GL_POLYGON);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(0.5, -0.5, -0.5);
        glVertex3f(0.5, -0.5, 0.5);
        glVertex3f(-0.5, -0.5, 0.5);
        glVertex3f(-0.5, -0.5, -0.5);
    glEnd();

    glFlush();

}

void displayCustomLens(CustomLens input, float imageRotation)
{
    GLfloat x, y, z;
    GLfloat vertices[SAMPLE_COUNT * STEP * 2];
    int sampleCount;
    const int DIVISION = 16;

    sampleCount = SAMPLE_COUNT / DIVISION;

    generateCustomLensVertices(input, vertices);
    quickTransformCustomVertices(vertices);

    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); // Reset model view matrix
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.0); // 3D transformation
    glRotatef(imageRotation, 0.0, 1.0, 0.0);

    glBegin(GL_LINE_STRIP);
        glColor3f(1.0f, 1.0f, 1.0f);
        for (int j = 0; j < DIVISION; j++) {
            for (int i = 0; i < sampleCount; i++) {
                x = vertices[j * sampleCount * STEP + i * STEP + 0];
                y = vertices[j * sampleCount * STEP + i * STEP + 1];
                z = vertices[j * sampleCount * STEP + i * STEP + 2];
                glVertex3f(x, y, z);
            }
        }
    glEnd();

    glFlush();
}

void displaySphericalLens(SphericalLens input, float imageRotation)
{
    //int i, j;
    GLfloat x, y, z;
    GLfloat vertices[SAMPLE_COUNT * STEP * 2];
    int sampleCount;
    const int DIVISION = 16;
    std::vector<Position<GLfloat>> output;

    sampleCount = SAMPLE_COUNT / DIVISION;
    generateSphericalLensVertices(input, vertices);
    quickTransformVertices(vertices, SAMPLE_COUNT * 2);

    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); // Reset model view matrix
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.0); // 3D transformation
    glRotatef(imageRotation, 0.0, 1.0, 0.0);

    glBegin(GL_LINE_STRIP);
        glColor3f(1.0f, 1.0f, 1.0f);
        for (int j = 0; j < DIVISION; j++) {
            for (int i = 0; i < sampleCount; i++) {
                x = vertices[j * sampleCount * STEP + i * STEP + 0];
                y = vertices[j * sampleCount * STEP + i * STEP + 1];
                z = vertices[j * sampleCount * STEP + i * STEP + 2];
                glVertex3f(x, y, z);
            }
        }
    glEnd();

    glFlush();
}

void generateCustomLensVertices(CustomLens input, GLfloat *vertices)
{
    int i, j;
    int profileCount;
    int profileNumber;
    const int DIVISION = 16;
    const float PI = 3.14159265359;
    bool isTruncated;
    GLfloat deltaHeight;
    GLfloat positionX, positionY;
    double startX, startY;
    double endX, endY;
    double deltaX, deltaY;
    double angle, lensAngle;
    double heightLimit;
    double widthLimit;
    double profileAngle; // Angle to rotate profile;
    std::vector<double> angles, lensAngles;
    //std::ofstream outputFile;

    angles = input.getAngles();
    lensAngles = input.getLensAngles();

    // Focal point is at origin (0, 0)
    positionX = input.getFocalLength();
    positionY = 0;

    widthLimit = input.getBaseThickness() + input.getFocalLength();
    profileCount = 0;
    profileAngle = 0;
    profileNumber = SAMPLE_COUNT / angles.size();
    std::cout << "Profile Number: " << profileNumber << std::endl;
    for (i = 0; i < profileNumber; i++) {
        isTruncated = false;
        startX = static_cast<double>(positionX);
        startY = static_cast<double>(positionY);
        for (j = 0; j < angles.size(); j++) {
            if (1 + (tan(angles[j]) * tan(lensAngles[j])) == 0.0) {
                if ((startX * tan(angles[j]) - startY) == 0.0) {
                    deltaY = 0.0;
                } else {
                    std::cout << "ERROR! DIVISION BY ZERO.\n";
                }
            } else {
                deltaY = (startX * tan(angles[j]) - startY) / (1 + tan(angles[j]) * tan(lensAngles[j]));
            }
            deltaX = deltaY * tan(lensAngles[j]);
            endX = startX - deltaX;
            endY = startY + deltaY;
            if (endX < widthLimit && isTruncated == false) {
                vertices[(i * angles.size() + j) * STEP] = endX;
                vertices[(i * angles.size() + j) * STEP + 1] = endY * cos(profileAngle);
                vertices[(i * angles.size() + j) * STEP + 2] = endY * sin(profileAngle);
                vertices[(i * angles.size() + j) * STEP + 3] = 1.0f;
                vertices[(i * angles.size() + j) * STEP + 4] = 1.0f;
                vertices[(i * angles.size() + j) * STEP + 5] = 1.0f;
            } else {
                isTruncated = true;
                vertices[(i * angles.size() + j) * STEP] = vertices[(i * angles.size() + j - 1) * STEP];
                vertices[(i * angles.size() + j) * STEP + 1] = vertices[(i * angles.size() + j - 1) * STEP + 1];
                vertices[(i * angles.size() + j) * STEP + 2] = vertices[(i * angles.size() + j - 1) * STEP + 2];
                vertices[(i * angles.size() + j) * STEP + 3] = 1.0f;
                vertices[(i * angles.size() + j) * STEP + 4] = 1.0f;
                vertices[(i * angles.size() + j) * STEP + 5] = 1.0f;
            }
            startX = endX;
            startY = endY;
        }
        vertices[(i * angles.size() + angles.size() - 1) * STEP + 1] = 0;
        vertices[(i * angles.size() + angles.size() - 1) * STEP + 2] = 0;
        profileAngle += 2 * PI / profileNumber;
    }
    /*
    outputFile.open("optics_output_2.txt", std::ios::out | std::ios::trunc);
    if (outputFile.is_open()) {
        outputFile << std::fixed << std::setprecision(5);
        for (i = 0; i < SAMPLE_COUNT; i++) {
            outputFile << vertices[i * STEP] << "\t\t" << vertices[i * STEP + 1] << std::endl;
        }
    } else {
        std::cout << "ERROR: Unable to open file optics_output_2.txt" << std::endl;
    }
    outputFile.close();
    */
}

void generateSphericalLensVertices(SphericalLens input, GLfloat *vertices)
{
    GLfloat x, y, z;
    GLfloat startAngle, stopAngle;
    GLfloat angle, interval;
    Position<GLfloat> vertex;
    GLfloat angles[4];
    int sampleCount;
    const int DIVISION = 16;
    const float PI = 3.14159265359;
    float rotation;
    std::vector<Position<GLfloat>> output;

    for (int i = 0; i < SAMPLE_COUNT * STEP * 2; i++)
    {
        vertices[i] = 0;
    }

    sampleCount = SAMPLE_COUNT / DIVISION;
    output = lensProfile(input, sampleCount);
    rotation = 0;

    for (int j = 0; j < DIVISION; j++) {
        //rotation = static_cast<float>(j * 2) * PI / static_cast<float>(DIVISION);
        rotation += 2.0 * PI / static_cast<float>(DIVISION);
        for (int i = 0; i < sampleCount; i++) {
            //sphereSurface(input.front, angle, vertex);
            vertices[j * sampleCount * STEP + i * STEP] = (output[i]).getX();
            vertices[j * sampleCount * STEP + i * STEP + 1] = (output[i]).getY() * cos(rotation);
            vertices[j * sampleCount * STEP + i * STEP + 2] = (output[i]).getY() * sin(rotation);
            vertices[j * sampleCount * STEP + i * STEP + 3] = 1.0f;
            vertices[j * sampleCount * STEP + i * STEP + 4] = 1.0f;
            vertices[j * sampleCount * STEP + i * STEP + 5] = 1.0f;
        }
    }
}

void quickTransformVertices(GLfloat *vertices, int count)
{
    int i;
    GLfloat aspectRatio;
    GLfloat horizontalScale;
    GLfloat horizontalShift;
    GLfloat verticalShift;
    GLfloat verticalScale;

    GLfloat horizontalMaximum;
    int hozMax = 0;
    GLfloat verticalMaximum;
    int verMax = 0;

    aspectRatio = static_cast<GLfloat>(height) / static_cast<GLfloat>(width);
    //horizontalShift = 4.5;
    horizontalShift = 0.0;
    //verticalShift = -5.0;
    verticalShift = 0.0;
    horizontalScale = 1.0 * aspectRatio;
    verticalScale = 1.0;
    // Actually, the horizontal and vertical scale should reflect
    // the range of values while a shift should put the center
    // of the values in the middle of the screen
    if (count > 0) {
        horizontalMaximum = abs(vertices[0]);
        verticalMaximum = abs(vertices[1]);
    }
    for (i = 0; i < count; i++) {
        if (horizontalMaximum < abs(vertices[i * STEP])) {
            horizontalMaximum = abs(vertices[i * STEP]);
            hozMax = i;
        }
        if (verticalMaximum < abs(vertices[i * STEP + 1])) {
            verticalMaximum = abs(vertices[i* STEP + 1]);
            verMax = i;
        }
    }
    if (verticalMaximum > 0) {
        verticalScale = 1 / verticalMaximum;
    }
    if (horizontalMaximum > 0) {
        horizontalScale = 1 / horizontalMaximum;
    }
    /*
    if (horizontalScale < verticalScale) {
        horizontalScale = verticalScale;
    } else {
        verticalScale = horizontalScale;
    }
    */
    verticalScale = verticalScale * 0.75;
    horizontalScale = horizontalScale * 0.75;
    //std::cout << "Horizontal maximum: " << horizontalMaximum;
    //std::cout << " at i = " << hozMax << std::endl;
    //std::cout << "Vertical maximum: " << verticalMaximum;
    //std::cout << " at i = " << verMax << std::endl;
    //std::cout << "Pre-processed Horizontal scale: " << horizontalScale << std::endl;
    //std::cout << "Pre-processed Vertical scale: " << verticalScale << std::endl;
    if (horizontalScale < verticalScale) {
        verticalScale = horizontalScale;
    } else {
        horizontalScale = verticalScale;
    }
    //std::cout << "Post-processed Horizontal scale: " << horizontalScale << std::endl;
    //std::cout << "Post-processed Vertical scale: " << verticalScale << std::endl;
    horizontalScale = horizontalScale * aspectRatio;
    for (i = 0; i < count; i++) {
        vertices[i * STEP] = horizontalScale * (vertices[i * STEP]) + horizontalShift;
        vertices[i * STEP + 1] = verticalScale * (vertices[i * STEP + 1]) + verticalShift;
        vertices[i * STEP + 2] = aspectRatio * verticalScale * (vertices[i * STEP + 2]);
    }
}

void quickTransformCustomVertices(GLfloat vertices[SAMPLE_COUNT * STEP])
{
    int i;
    GLfloat horizontalScale;
    GLfloat horizontalShift;
    GLfloat verticalScale;
    GLfloat verticalShift;
    GLfloat aspectRatio;
    GLfloat horizontalMaximum;
    GLfloat verticalMaximum;
    int hozMax = 0;
    int verMax = 0;

    //horizontalScale = static_cast<GLfloat>(HEIGHT) / static_cast<GLfloat>(WIDTH);
    aspectRatio = static_cast<GLfloat>(height) / static_cast<GLfloat>(width);

    horizontalShift = 0;
    verticalShift = 0;

    horizontalMaximum = abs(vertices[0]);
    verticalMaximum = abs(vertices[1]);
    for (i = 0; i < SAMPLE_COUNT; i++) {
        if (horizontalMaximum < abs(vertices[i * STEP])) {
            horizontalMaximum = abs(vertices[i * STEP]);
            hozMax = i;
        }
        if (verticalMaximum < abs(vertices[i * STEP + 1])) {
            verticalMaximum = abs(vertices[i* STEP + 1]);
            verMax = i;
        }
    }
    if (verticalMaximum > 0) {
        verticalScale = 1 / verticalMaximum;
    }
    if (horizontalMaximum > 0) {
        horizontalScale = 1 / horizontalMaximum;
    }

    if (horizontalScale > verticalScale) {
        horizontalScale = verticalScale;
    } else {
        verticalScale = horizontalScale;
    }    
    horizontalScale *= aspectRatio;

    std::cout << "Vertical scale: " << verticalScale << std::endl;
    std::cout << "Horizontal scale: " << horizontalScale << std::endl;

    for (i = 0; i < SAMPLE_COUNT; i++) {
        vertices[i * STEP] = (vertices[i * STEP] - horizontalShift) * horizontalScale;
        vertices[i * STEP + 1] = (vertices[i * STEP + 1] - verticalShift) * verticalScale;
        vertices[i * STEP + 2] = vertices[i * STEP + 2] * verticalScale * aspectRatio;
        //vertices[i * STEP + 3] = 1.0f;
        //vertices[i * STEP + 4] = 1.0f;
        //vertices[i * STEP + 5] = 1.0f;
    }
}
