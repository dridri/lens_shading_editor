#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qwidget.h>
#include <QImage>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QWidget
{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();

	QImage GenerateImage( bool force_enable_all_channels = false );

public slots:
	void RepaintShader();
	void Export();

	void r_base_changed( int value );
	void r_strength_changed( int value );
	void gr_base_changed( int value );
	void gr_strength_changed( int value );
	void gb_base_changed( int value );
	void gb_strength_changed( int value );
	void b_base_changed( int value );
	void b_strength_changed( int value );

private:
	Ui::MainWindow* ui;
	QImage mImage;
};

#endif // MAINWINDOW_H
