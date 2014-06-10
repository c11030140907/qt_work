/****************************************************************************
****************************************************************************/

#include <QtQml/QQmlApplicationEngine>
#include <QApplication>
#include <QWindow>
#include <qDebug.h>
#include <QDate>
#include <QQmlContext>
#include "app.h"

void testBinding(QObjectList olst)
{

	olst.at(0)->dumpObjectTree();
	qDebug() << olst.at(0)->objectName();
	qDebug() << olst.at(0)->property("margins");


	QMetaObject::invokeMethod(olst.at(0), "test01");

	QVariantList list;
	list<<10<<QColor("green")<<"bottles";

	QVariantMap map;
	map.insert("language","QML");
	map.insert("released",QDate(2010,9,21));

	QMetaObject::invokeMethod(olst.at(0), "test02",
			Q_ARG(QVariant, QVariant::fromValue(list)),
			Q_ARG(QVariant, QVariant::fromValue(map)));

}


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QQmlApplicationEngine engine;

	AppData data;
	engine.rootContext()->setContextProperty("appData",&data);
	engine.load(QUrl("qrc:/main.qml"));
	app.installEventFilter(&data);


	testBinding(engine.rootObjects().at(0)->children());

	app.connect( &app, SIGNAL( lastWindowClosed() ), &data, SLOT( hoge() ) );
	return app.exec();

}
