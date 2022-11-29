#ifndef MAIN_H
#define MAIN_H

#include <QMainWindow>
#include <QImage>
#include <QAction>
#include <QGraphicsPixmapItem>

namespace Ui
{
	class main;
}

class main_t : public QMainWindow
{
	Q_OBJECT

	public:
		main_t();
		bool loadFile(const QString &);

	private slots:
		void slt_open();
		void slt_saveAs();

	private:
		void createActions();
		bool saveFile(const QString &fileName);

	private:
		Ui::main * ui;

		QString fileName;
		QImage image;
		QGraphicsPixmapItem * gpm;

		QAction *save_act;
};
#endif
