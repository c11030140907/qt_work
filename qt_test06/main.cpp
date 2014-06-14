
#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{

	Q_INIT_RESOURCE(app);

	QApplication app(argc, argv);
	app.setOrganizationName("qt");
	app.setApplicationName("test06");
	MainWindow mainWin;
	mainWin.show();
	return app.exec();
}

