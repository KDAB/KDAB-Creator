#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QmlApplicationViewer *viewer = QmlApplicationViewer::create();

    viewer->addImportPath(QLatin1String("modules")); // ADDIMPORTPATH
    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto); // ORIENTATION
    viewer->setMainQmlFile(QLatin1String("qml/app/main.qml")); // MAINQML
    viewer->show();

    return app.exec();
}
