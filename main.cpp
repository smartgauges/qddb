#include <QApplication>
#include <QFileDialog>
#include <QDir>

#include "main.h"
#include "ui_main.h"
#include "qddb.h"

main_t::main_t() : ui(new Ui::main)
{
	ui->setupUi(this);

	QGraphicsScene * scn = new QGraphicsScene(this);
	ui->gv->setScene(scn);
	gpm = scn->addPixmap(QPixmap::fromImage(image));
	ui->gv->fitInView(scn->itemsBoundingRect(), Qt::KeepAspectRatio);
	ui->gv->setBackgroundBrush(QColor("white"));

	createActions();
}

bool main_t::loadFile(const QString &fn)
{
	if (fn.isEmpty())
		return false;

	qddb_t qddb;

	QImage newImage = qddb.load(fn);
	if (!newImage.isNull()) {

		image = newImage;

		QPixmap pm = QPixmap::fromImage(image);
		gpm->setPixmap(pm);

		save_act->setEnabled(true);
	}
	else {

		const QString message = QString("Cannot load %1").arg(QDir::toNativeSeparators(fn));
		statusBar()->showMessage(message);
		return false;
	}

	const QString message = tr("Opened \"%1\", %2x%3, Depth: %4").arg(QDir::toNativeSeparators(fn)).arg(image.width()).arg(image.height()).arg(image.depth());
	statusBar()->showMessage(message);

	return true;
}

void main_t::slt_open()
{
	QString fn = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Ford Images(*.ddb)"));

	if (loadFile(fn)) {

		fileName = fn;
		QGuiApplication::setApplicationDisplayName(fileName);
	}
}

void main_t::slt_saveAs()
{
	QString fn = QFileDialog::getSaveFileName(this, tr("Save png image"), fileName + ".png", tr("png image (*.png)"));

	if (fn.isEmpty())
		return;

	image.save(fn);
}

void main_t::createActions()
{
	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

	fileMenu->addAction(tr("&Open..."), this, SLOT(slt_open()), QKeySequence::Open);
	save_act = fileMenu->addAction(tr("&Save As..."), this, SLOT(slt_saveAs()), QKeySequence::Save);
	save_act->setEnabled(false);
	fileMenu->addSeparator();
	fileMenu->addAction(tr("E&xit"), this, SLOT(close()), QKeySequence(tr("Ctrl+Q")));
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	main_t m;
	m.show();

	return app.exec();
}

