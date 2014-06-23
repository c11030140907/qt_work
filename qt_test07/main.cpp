#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QStringList>
#include <QDebug>
#include "sqlite3.h"
#include <qdir.h>
#include <QtSql/qsqlerror.h>

bool sqliteDBMemFile(QSqlDatabase memdb, QSqlDatabase filedb, bool save)
{
    bool state = false;
    QVariant v = memdb.driver()->handle();

	QVariant vf = filedb.driver()->handle();

	
	if (v.isValid() && qstrcmp(v.typeName(), "sqlite3*") == 0 && vf.isValid() && qstrcmp(vf.typeName(), "sqlite3*") == 0 )
    {
        // v.data() returns a pointer to the handle
		sqlite3 * handle = *static_cast<sqlite3 **>(v.data());
		sqlite3 * handlef = *static_cast<sqlite3 **>(vf.data());
		
		if (handle != 0 && handlef != 0) // check that it is not NULL
        {
            sqlite3 * pInMemory = handle;

			sqlite3 * pFile = handlef;

            int rc;                   /* Function return code */
            sqlite3_backup *pBackup;  /* Backup object used to copy data */
            sqlite3 *pTo;             /* Database to copy to (pFile or pInMemory) */
            sqlite3 *pFrom;           /* Database to copy from (pFile or pInMemory) */

			rc = SQLITE_OK;
			if( rc==SQLITE_OK ){

              pFrom = ( save ? pInMemory : pFile);
              pTo   = ( save ? pFile     : pInMemory);

              pBackup = sqlite3_backup_init(pTo, "main", pFrom, "main");
              if( pBackup ){
				try {
					(void)sqlite3_backup_step(pBackup, -1);

					(void)sqlite3_backup_finish(pBackup);
				}
				catch (std::exception &e) {
					qFatal("Error %s  ",
						e.what());
				}
				catch (...) {
					qFatal("Error <unknown> sending event ");
				}
				
              }
              rc = sqlite3_errcode(pTo);
            }
            (void)sqlite3_close(pFile);
            if( rc == SQLITE_OK ) state = true;
        }
    }
    return state;
}

void test01(){
	int rc;                   
	sqlite3 *pFile;           
	sqlite3_backup *pBackup;  
	sqlite3 *pTo;             
	sqlite3 *pFrom;           
	sqlite3 *pInMemory;           

	rc = sqlite3_open("test.db",&pFile);
	if (rc == SQLITE_OK){
		qDebug() << "xx";
	}

	rc = sqlite3_open(NULL,&pInMemory);
	if (rc == SQLITE_OK){
		pFrom = pInMemory;
		pTo = pFile;

		pBackup = sqlite3_backup_init(pTo, "main", pFrom, "main");
		if (pBackup){
			(void)sqlite3_backup_step(pBackup, -1);
			(void)sqlite3_backup_finish(pBackup);
		}
		rc = sqlite3_errcode(pTo);
	}
	(void)sqlite3_close(pFile);
}


void test02(){
	QSqlDatabase db1 = QSqlDatabase::addDatabase("QSQLITE","con1");
	db1.setDatabaseName(":memory:");

	qDebug() << "db open" << db1.open();

	QSqlDatabase db2 = QSqlDatabase::addDatabase("QSQLITE","con2");
	db2.setDatabaseName("test.db");
	qDebug() << "db open" << db2.open();

	sqlite3 * pMem1 = *static_cast<sqlite3 **>(db1.driver()->handle().data());
	sqlite3 * pFile1 = *static_cast<sqlite3 **>(db2.driver()->handle().data());

	sqlite3_backup *pBackup1 = sqlite3_backup_init(pMem1, "main", pFile1, "main");
	if (pBackup1){
		(void)sqlite3_backup_step(pBackup1, -1);
		(void)sqlite3_backup_finish(pBackup1);
	}
	int rc1 = sqlite3_errcode(pMem1);
		

	/*
	QSqlQuery("PRAGMA page_size = 2048");    
	QSqlQuery("PRAGMA cache_size = 16384");    
	QSqlQuery("PRAGMA temp_store = MEMORY");
	QSqlQuery("PRAGMA journal_mode = OFF");
	QSqlQuery("PRAGMA locking_mode = EXCLUSIVE");
	QSqlQuery("PRAGMA synchronous = OFF");
	*/
	
	//QString sqlAttach = QString("ATTACH DATABASE test.db AS test ");
	//db1.exec(sqlAttach);
	

	QSqlQuery query("CREATE TABLE aaa (b INTEGER)", db2);
	
	query.prepare("insert into aaa (b) "
		"values (?)");
	for (int i = 0; i < 20; i++) {
		query.bindValue(0, i);
		query.exec();
	}
	
		
	query.exec("select * from aaa");
	while (query.next()) {
		int id = query.value(0).toInt();
		qDebug() << QString("id(%1)").arg(id);
	}

	db1.close();
	db2.close();
}


int main(int argc, char *argv[]){


	QCoreApplication a(argc, argv);

	test01();
	test02();


	return 0;
}



