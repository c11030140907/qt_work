#ifndef APP_H
#define APP_H

#include <QTableView>
#include <QObject>
#include <QDateTime>
#include <qDebug.h>
#include <QEvent>
#include <QKeyEvent>

class AppData : public QObject
{
	Q_OBJECT
	public:
		Q_INVOKABLE QDateTime getCurrentDateTime() const {
			return QDateTime::currentDateTime();
		}
	public slots:
		void hoge(){
			qDebug() << "aaaaaaaaaaaaaaaaaaaaaaaaaaaa";
		}

	protected:
		bool eventFilter(QObject *obj, QEvent *event){
			if (event->type() == QEvent::KeyPress) {
				QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

                QVariantList lst;
                lst<<"a"<<"b"<<"c";
                QMetaObject::invokeMethod(obj, "setData",Q_ARG(QVariant, QVariant::fromValue(lst)));

				qDebug("Ate key press %d", keyEvent->key());
				return true;
			} else {
				// standard event processing
				return QObject::eventFilter(obj,event);
			}
		}

};


#endif // APP_H
