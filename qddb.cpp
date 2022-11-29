#include <QtEndian>
#include <QFile>
#include <QBuffer>

#include <QDebug>
#include "qddb.h"

#define DDB_MAGIC_1   0x00048002
#define DDB_MAGIC_2   0x00048082
#define DDB_MAGIC_3   0x00058002
#define DDB_MAGIC_4   0x00068003
#define DDB_MAGIC_5   0x00078003

QImage qddb_t::load(const QString & fileName)
{
	if (fileName.isEmpty())
		return QImage();

	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return QImage();

	QDataStream input(&file);
	quint32 magic;
	quint16 width, height;
	input >> width >> height;
	input >> magic;

	height = qFromBigEndian<quint16>(height);
	width = qFromBigEndian<quint16>(width);
	magic = qFromBigEndian<quint32>(magic);

	qDebug() << hex << width << "x" << height << " " << magic;
	if (width < 16)
		width = 16;
	else if (width > 16 && width < 32)
		width = 32;
	else if (width > 32 && width < 64)
		width = 64;
	else if (width > 64 && width < 128)
		width = 128;
	else if (width > 128 && width < 256)
		width = 256;
	else if (width > 256 && width < 512)
		width = 512;

	qDebug() << width << "x" << height <<  " size:" << file.bytesAvailable() << " WxH:" << width * height * 2;

	if (input.status() != QDataStream::Ok)
		return QImage();

	bool compress = false;
	if (((magic >> 16) & 0xf) > 4)
		compress = true;

	if (file.bytesAvailable() >= width * height * 2)
		compress = false;

	if (!compress) {

		//QImage result(width, height, QImage::Format_RGB555);
		QImage result(width, height, QImage::Format_ARGB32);

		for (quint32 y = 0; y < height; ++y) {

			for (quint32 x = 0; x < width; ++x) {

				quint16 pixel;
				input >> pixel;
				pixel = qFromBigEndian<quint16>(pixel);

				//ARGB1555
				quint16 a = (pixel & 0x8000) ? 255 : 0;
				quint16 r = (pixel >> 10) & 0x1ff;
				quint16 g = (pixel >> 5) & 0x1ff;
				quint16 b = pixel & 0x1ff;
				result.setPixel(x, y, qRgba(r * 8, g * 8, b * 8, a));
			}
		}

		return result;
	}
	else {

		QVector<quint16> vec;

		int cnt = 8;
		while (input.status() == QDataStream::Ok) {

			qint16 sz;
			input >> sz;
			sz = qFromBigEndian<qint16>(sz);
			if (sz > 0) {

				quint16 pixel;
				input >> pixel;
				for (quint16 i = 0; i < sz; i++)
					vec.append(pixel);

				//qDebug() << hex << cnt << dec << sz << pixel << vec.size();
				cnt += 4;
			}
			else {
				sz = abs(sz);
				//qDebug() << "-" << sz << "->" << sz;
				for (qint16 i = 0; i < sz; i++) {
					quint16 pixel;
					input >> pixel;
					vec.append(pixel);

					qDebug() << hex << cnt << dec << sz << pixel << vec.size();
				}
				cnt += 2 + sz*2;
			}

			if (vec.size() >= width*height)
				break;
		}

		if (vec.size() < width*height)
			return QImage();

		QImage result(width, height, QImage::Format_ARGB32);

		for (quint32 y = 0; y < height; ++y) {

			for (quint32 x = 0; x < width; ++x) {

				quint16 pixel = vec[y * width + x];
				pixel = qFromBigEndian<quint16>(pixel);

				//ARGB1555
				quint16 a = (pixel & 0x8000) ? 255 : 0;
				quint16 r = (pixel >> 10) & 0x1ff;
				quint16 g = (pixel >> 5) & 0x1ff;
				quint16 b = pixel & 0x1ff;
				result.setPixel(x, y, qRgba(r * 8, g * 8, b * 8, a));
			}
		}

		return result;
	}

	return QImage();
}

