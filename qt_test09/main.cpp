#include <QtCore>

#include <QDebug>
#include <QString>

#include <QList>
#include <QMap>
#include <QTextStream>
#include <QStringList>
#include <QDir>
#include <QTime>


#include <QtConcurrent>
#include <QThread>

#include <QtWidgets>
#include <QImage>

#include <QApplication>
#include <QGuiApplication>

#include <qtconcurrentrun.h>
#include <qtconcurrentmap.h>

#include <stdio.h>
#include <stdlib.h>




using namespace QtConcurrent;

const int iterations = 20;

void spin(int &iteration)
{
	const int work = 1000 * 1000 * 40*10;
	volatile int v = 0;
	for (int j = 0; j < work; ++j)
		++v;

	qDebug() << "iteration" << iteration << "in thread" << QThread::currentThreadId();
}

void test01(){
	// Prepare the vector.
	QVector<int> vector;
	for (int i = 0; i < iterations; ++i)
		vector.append(i);

	// Create a progress dialog.
	QProgressDialog dialog;
	dialog.setLabelText(QString("Progressing using %1 thread(s)...").arg(QThread::idealThreadCount()));

	// Create a QFutureWatcher and connect signals and slots.
	QFutureWatcher<void> futureWatcher;
	QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
	QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
	QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
	QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));

	// Start the computation.
	futureWatcher.setFuture(QtConcurrent::map(vector, spin));

	// Display the dialog and start the event loop.
	dialog.exec();

	futureWatcher.waitForFinished();

	// Query the future to check if was canceled.
	qDebug() << "Canceled?" << futureWatcher.future().isCanceled();
}

void hello(QString name)
{
	qDebug() << "Hello" << name << "from" << QThread::currentThread();
}

void test02(){
	QFuture<void> f1 = run(hello, QString("Alice"));
	QFuture<void> f2 = run(hello, QString("Bob"));
	f1.waitForFinished();
	f2.waitForFinished();
}



/*
	Utility function that recursivily searches for files.
*/
QStringList findFiles(const QString &startDir, QStringList filters)
{
	QStringList names;
	QDir dir(startDir);

	foreach (QString file, dir.entryList(filters, QDir::Files))
		names += startDir + "/" + file;

	foreach (QString subdir, dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot))
		names += findFiles(startDir + "/" + subdir, filters);
	return names;
}

typedef QMap<QString, int> WordCount;

/*
	Single threaded word counter function.
*/
WordCount singleThreadedWordCount(QStringList files)
{
	WordCount wordCount;
	foreach (QString file, files) {
		QFile f(file);
		f.open(QIODevice::ReadOnly);
		QTextStream textStream(&f);
		while (textStream.atEnd() == false)
			foreach(QString word, textStream.readLine().split(" "))
				wordCount[word] += 1;

	}
	return wordCount;
}


// countWords counts the words in a single file. This function is
// called in parallel by several threads and must be thread
// safe.
WordCount countWords(const QString &file)
{
	QFile f(file);
	f.open(QIODevice::ReadOnly);
	QTextStream textStream(&f);
	WordCount wordCount;

	while (textStream.atEnd() == false)
		foreach (QString word, textStream.readLine().split(" "))
			wordCount[word] += 1;

	return wordCount;
}

// reduce adds the results from map to the final
// result. This functor will only be called by one thread
// at a time.
void reduce(WordCount &result, const WordCount &w)
{
	QMapIterator<QString, int> i(w);
	while (i.hasNext()) {
		i.next();
		result[i.key()] += i.value();
	}
}

void test03()
{
	qDebug() << "finding files...";
	QStringList files = findFiles("../", QStringList() << "*.cpp" << "*.h");
	qDebug() << files.count() << "files";

	qDebug() << "warmup";
	{
		QTime time;
		time.start();
		WordCount total = singleThreadedWordCount(files);
	}

	qDebug() << "warmup done";

	int singleThreadTime = 0;
	{
		QTime time;
		time.start();
		WordCount total = singleThreadedWordCount(files);
		singleThreadTime = time.elapsed();
		qDebug() << "single thread" << singleThreadTime;
	}

	int mapReduceTime = 0;
	{
		QTime time;
		time.start();
		WordCount total = mappedReduced(files, countWords, reduce);
		mapReduceTime = time.elapsed();
		qDebug() << "MapReduce" << mapReduceTime;
	}
	qDebug() << "MapReduce speedup x" << ((double)singleThreadTime - (double)mapReduceTime) / (double)mapReduceTime + 1;
}


QImage scale(const QImage &image)
{
	qDebug() << "Scaling image in thread" << QThread::currentThread();
	return image.scaled(QSize(100, 100), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void test04()
{

	const int imageCount = 20;

	// Create a list containing imageCount images.
	QList<QImage> images;
	for (int i = 0; i < imageCount; ++i)
		images.append(QImage(1600, 1200, QImage::Format_ARGB32_Premultiplied));

	// Use QtConcurrentBlocking::mapped to apply the scale function to all the
	// images in the list.
	QList<QImage> thumbnails = QtConcurrent::blockingMapped(images, scale);
	return;
}





const int DataSize = 100000;
const int BufferSize = 8192;
char buffer[BufferSize];

//QWaitCondition bufferNotEmpty;
//QWaitCondition bufferNotFull;
//QMutex mutex;
//int numUsedBytes = 0;
//
//class Producer : public QThread {
//	public:
//		Producer(QObject *parent = NULL) : QThread(parent){}
//		void run(){
//			qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
//			for (int i = 0; i < DataSize; ++i) {
//				mutex.lock();
//				if (numUsedBytes == BufferSize){bufferNotFull.wait(&mutex);}
//				mutex.unlock();
//	
//				buffer[i % BufferSize] = "ACGT"[(int)qrand() % 4];
//	
//				mutex.lock();
//				++numUsedBytes;
//				bufferNotEmpty.wakeAll();
//				mutex.unlock();
//			}
//		}
//};
//class Consumer : public QThread{
//	Q_OBJECT
//	public:
//		Consumer(QObject *parent = NULL) : QThread(parent){}
//	
//		void run(){
//			for (int i = 0; i < DataSize; ++i) {
//				mutex.lock();
//				if (numUsedBytes == 0){bufferNotEmpty.wait(&mutex);}
//				mutex.unlock();
//	
//				fprintf(stderr, "%c", buffer[i % BufferSize]);
//	
//				mutex.lock();
//				--numUsedBytes;
//				bufferNotFull.wakeAll();
//				mutex.unlock();
//			}
//			fprintf(stderr, "\n");
//		}
//	
//	signals:
//		void stringConsumed(const QString &text);
//};


QSemaphore freeBytes(BufferSize);
QSemaphore usedBytes;
class Producer : public QThread
{
public:
	void run(){
		qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
		for (int i = 0; i < DataSize; ++i) {
			freeBytes.acquire();
			buffer[i % BufferSize] = "ACGT"[(int)qrand() % 4];
			//usedBytes.release();
		}
	}
};
class Consumer : public QThread{
	Q_OBJECT
public:
	void run(){
		for (int i = 0; i < DataSize; ++i) {
			usedBytes.acquire();
			fprintf(stderr, "%c", buffer[i % BufferSize]);
			freeBytes.release();
		}
		fprintf(stderr, "\n");
	}
signals:
	void stringConsumed(const QString &text);

protected:
	bool finish;
};

void test05()
{
	Producer producer;
	Consumer consumer;
	producer.start();
	consumer.start();
	producer.wait();
	consumer.wait();
}

#include "waitconditions.moc"


int main(int argc, char **argv)
{
	QApplication app(argc, argv);
//	test01();
//	test02();
//	test03();
//	test04();
	test05();


}
