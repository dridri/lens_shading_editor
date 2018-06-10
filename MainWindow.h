#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdint.h>
#include <QWidget>
#include <QImage>
#include "GLWidget.h"

namespace Ui
{
	class MainWindow;
	class OpenDialog;
}

class MainWindow : public QWidget
{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();

	QImage GenerateImage( bool force_enable_all_channels = false, int32_t width = 52, int32_t height = 39 );

public slots:
	void RepaintShader();
	void Load();
	void LoadChannels();
	void Save();
	void Import();
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
	QDialog* mOpenDialog;
	Ui::OpenDialog* uiOpenDialog;
	QString mDialogPath;
	QImage mImage;
	uint16_t mChannels[4][52 * 39];
	uint16_t mBaseChannels[4][52 * 39];

	GLWidget* mGLWidget;

	void ExportC( char* ret, int32_t width, int32_t height );
	void ExportLUA( char* ret, int32_t width, int32_t height );
};

#endif // MAINWINDOW_H
