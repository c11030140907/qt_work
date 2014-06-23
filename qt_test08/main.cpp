#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QDebug>
#include <qdir.h>
#include <QtSql/qsqlerror.h>


int main(int argc, char *argv[]){


	QCoreApplication a(argc, argv);

	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(":memory:");
	qDebug() << "db open" << db.open();

	QSqlQuery q_res = db.exec("ATTACH DATABASE '.\\qdn.db' AS fileDB");
	qDebug() << "attach: " << q_res.lastError();

	q_res = db.exec("SELECT * from fileDB.qdn;");
	qDebug() << " select: " << q_res.lastError();

	while (q_res.next()) {
		int id = q_res.value(0).toInt();
		qDebug() << QString("id(%1)").arg(id);
	}

	return a.exec();
}



