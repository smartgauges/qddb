#ifndef QDDB_H
#define QDDB_H

#include <QString>
#include <QImage>

class qddb_t : public QObject
{
	Q_OBJECT

	public:
		QImage load(const QString &fileName);
		bool save(const QString & fileName, const QImage & image);
};

#endif

