#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QDebug>
#include <qdir.h>
#include <QtSql/qsqlerror.h>

//2014.06.21 iida 
//
//DB接続テスト用。07は作業継続、、Qt以外のライブラリと
//連携しようとすると途端に面倒になる。。
//
//本来の目的はin-memoryでsqliteを使うこと、起動終了時に
//save,loadすることだったんだけどload時に失敗する。。
//sqlite側で発生しているようで、sqliteコンパイルしないと
//原因不明。。あるいは、デバッグ実行時に別のsqliteと競合
//したりする？
//

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



