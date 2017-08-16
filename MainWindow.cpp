#include <cmath>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>

MainWindow::MainWindow()
{
	ui = new Ui::MainWindow;
	ui->setupUi(this);

	connect( ui->r_base, SIGNAL(valueChanged(int)), this, SLOT(RepaintShader()) );
	connect( ui->r_radius, SIGNAL(valueChanged(int)), this, SLOT(RepaintShader()) );
	connect( ui->r_strength, SIGNAL(valueChanged(int)), this, SLOT(RepaintShader()) );
	connect( ui->gr_base, SIGNAL(valueChanged(int)), this, SLOT(RepaintShader()) );
	connect( ui->gr_radius, SIGNAL(valueChanged(int)), this, SLOT(RepaintShader()) );
	connect( ui->gr_strength, SIGNAL(valueChanged(int)), this, SLOT(RepaintShader()) );
	connect( ui->gb_base, SIGNAL(valueChanged(int)), this, SLOT(RepaintShader()) );
	connect( ui->gb_radius, SIGNAL(valueChanged(int)), this, SLOT(RepaintShader()) );
	connect( ui->gb_strength, SIGNAL(valueChanged(int)), this, SLOT(RepaintShader()) );
	connect( ui->b_base, SIGNAL(valueChanged(int)), this, SLOT(RepaintShader()) );
	connect( ui->b_radius, SIGNAL(valueChanged(int)), this, SLOT(RepaintShader()) );
	connect( ui->b_strength, SIGNAL(valueChanged(int)), this, SLOT(RepaintShader()) );
	connect( ui->groupBox, SIGNAL(toggled(bool)), this, SLOT(RepaintShader()) );
	connect( ui->groupBox_1, SIGNAL(toggled(bool)), this, SLOT(RepaintShader()) );
	connect( ui->groupBox_2, SIGNAL(toggled(bool)), this, SLOT(RepaintShader()) );
	connect( ui->groupBox_3, SIGNAL(toggled(bool)), this, SLOT(RepaintShader()) );

	connect( ui->r_radius, SIGNAL(valueChanged(int)), ui->r_v_radius, SLOT(setNum(int)) );
	connect( ui->gr_radius, SIGNAL(valueChanged(int)), ui->gr_v_radius, SLOT(setNum(int)) );
	connect( ui->gb_radius, SIGNAL(valueChanged(int)), ui->gb_v_radius, SLOT(setNum(int)) );
	connect( ui->b_radius, SIGNAL(valueChanged(int)), ui->b_v_radius, SLOT(setNum(int)) );

	connect( ui->r_base, SIGNAL(valueChanged(int)), this, SLOT(r_base_changed(int)) );
	connect( ui->r_strength, SIGNAL(valueChanged(int)), this, SLOT(r_strength_changed(int)) );
	connect( ui->gr_base, SIGNAL(valueChanged(int)), this, SLOT(gr_base_changed(int)) );
	connect( ui->gr_strength, SIGNAL(valueChanged(int)), this, SLOT(gr_strength_changed(int)) );
	connect( ui->gb_base, SIGNAL(valueChanged(int)), this, SLOT(gb_base_changed(int)) );
	connect( ui->gb_strength, SIGNAL(valueChanged(int)), this, SLOT(gb_strength_changed(int)) );
	connect( ui->b_base, SIGNAL(valueChanged(int)), this, SLOT(b_base_changed(int)) );
	connect( ui->b_strength, SIGNAL(valueChanged(int)), this, SLOT(b_strength_changed(int)) );

	connect( ui->exportBtn, SIGNAL(pressed()), this, SLOT(Export()) );

	int32_t default_value = 64;
	ui->r_base->setValue( default_value );
	ui->gr_base->setValue( default_value );
	ui->gb_base->setValue( default_value );
	ui->b_base->setValue( default_value );

	RepaintShader();
}


MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::r_base_changed( int value )
{
	ui->r_v_base->setText( QString::number( value >> 5 ) + "." + QString::number( ( value & 0b00011111 ) * 100 / 32 ) );
}


void MainWindow::r_strength_changed( int value )
{
	QString neg = "";
	if ( value < 0 ) {
		neg = "-";
		value = -value;
	}
	ui->r_v_strength->setText( neg + QString::number( value >> 5 ) + "." + QString::number( ( value & 0b00011111 ) * 100 / 32 ) );
}


void MainWindow::gr_base_changed( int value )
{
	ui->gr_v_base->setText( QString::number( value >> 5 ) + "." + QString::number( ( value & 0b00011111 ) * 100 / 32 ) );
}


void MainWindow::gr_strength_changed( int value )
{
	QString neg = "";
	if ( value < 0 ) {
		neg = "-";
		value = -value;
	}
	ui->gr_v_strength->setText( neg + QString::number( value >> 5 ) + "." + QString::number( ( value & 0b00011111 ) * 100 / 32 ) );
}


void MainWindow::gb_base_changed( int value )
{
	ui->gb_v_base->setText( QString::number( value >> 5 ) + "." + QString::number( ( value & 0b00011111 ) * 100 / 32 ) );
}


void MainWindow::gb_strength_changed( int value )
{
	QString neg = "";
	if ( value < 0 ) {
		neg = "-";
		value = -value;
	}
	ui->gb_v_strength->setText( neg + QString::number( value >> 5 ) + "." + QString::number( ( value & 0b00011111 ) * 100 / 32 ) );
}


void MainWindow::b_base_changed( int value )
{
	ui->b_v_base->setText( QString::number( value >> 5 ) + "." + QString::number( ( value & 0b00011111 ) * 100 / 32 ) );
}


void MainWindow::b_strength_changed( int value )
{
	QString neg = "";
	if ( value < 0 ) {
		neg = "-";
		value = -value;
	}
	ui->b_v_strength->setText( neg + QString::number( value >> 5 ) + "." + QString::number( ( value & 0b00011111 ) * 100 / 32 ) );
}


void MainWindow::RepaintShader()
{
	mImage = GenerateImage();
	QPixmap pix = QPixmap::fromImage( mImage );
	ui->shader->setPixmap( pix );
}


QImage MainWindow::GenerateImage( bool force_enable_all_channels )
{
	QImage image = QImage(52, 38, QImage::Format_RGB32 );

	for ( int32_t y = 0; y < 38; y++ ) {
		for ( int32_t x = 0; x < 52; x++ ) {
			int32_t r = ui->r_base->value();
			int32_t gr = ui->gr_base->value();
			int32_t gb = ui->gb_base->value();
			int32_t b = ui->b_base->value();

			int32_t dist = std::sqrt( ( x - 52 / 2 ) * ( x - 52 / 2 ) + ( y - 38 / 2 ) * ( y - 38 / 2 ) );
			if ( ui->r_radius->value() > 0 ) {
				float dot_r = (float)std::max( 0, ui->r_radius->value() - dist ) / (float)ui->r_radius->value();
				r = std::max( 0, std::min( 255, r + (int32_t)( dot_r * ui->r_strength->value() ) ) );
			}
			if ( ui->gr_radius->value() > 0 ) {
				float dot_gr = (float)std::max( 0, ui->gr_radius->value() - dist ) / (float)ui->gr_radius->value();
				gr = std::max( 0, std::min( 255, gr + (int32_t)( dot_gr * ui->gr_strength->value() ) ) );
			}
			if ( ui->gb_radius->value() > 0 ) {
				float dot_gb = (float)std::max( 0, ui->gb_radius->value() - dist ) / (float)ui->gb_radius->value();
				gb = std::max( 0, std::min( 255, gb + (int32_t)( dot_gb * ui->gb_strength->value() ) ) );
			}
			if ( ui->b_radius->value() > 0 ) {
				float dot_b = (float)std::max( 0, ui->b_radius->value() - dist ) / (float)ui->b_radius->value();
				b = std::max( 0, std::min( 255, b + (int32_t)( dot_b * ui->b_strength->value() ) ) );
			}

			if ( not force_enable_all_channels ) {
				if ( not ui->groupBox->isChecked() ) {
					r = 0;
				}
				if ( not ui->groupBox_1->isChecked() ) {
					gr = 0;
				}
				if ( not ui->groupBox_2->isChecked() ) {
					gb = 0;
				}
				if ( not ui->groupBox_3->isChecked() ) {
					b = 0;
				}
			}

// 			image.setPixel( x, y, QColor( r, (gr+gb)/2, b ).rgba() );
			image.setPixel( x, y, QColor( r, gr, b ).rgba() );
		}
	}

	return image;
}


void MainWindow::Export()
{
	QImage image = GenerateImage( true );
	char ret[1*1024*1024] = "";
	uint8_t grid[65535] = { 0 };
	uint32_t stride = image.width() * image.height();

	for ( int32_t y = 0; y < image.height(); y++ ) {
		for ( int32_t x = 0; x < image.width(); x++ ) {
			uint32_t color = image.pixelColor( x, y ).rgba();
			uint32_t r = ( color & 0x00FF0000 ) >> 16;
			uint32_t g = ( color & 0x0000FF00 ) >> 8;
			uint32_t b = color & 0x000000FF;
			grid[ stride*0 + image.width()*y + x ] = r;
			grid[ stride*1 + image.width()*y + x ] = g;
			grid[ stride*2 + image.width()*y + x ] = g;
			grid[ stride*3 + image.width()*y + x ] = b;
		}
	}

	sprintf( ret, "%s#include <stdint.h>\n\n// size : %d x %d\n", ret, image.width(), image.height() );
	sprintf( ret, "%suint8_t ls_grid[%d] = {\n", ret, stride * 4 );
	for ( uint32_t channel = 0; channel < 4; channel++ ) {
		switch ( channel ) {
			case 0 : sprintf( ret, "%s\t// R\n", ret ); break;
			case 1 : sprintf( ret, "%s\t// GR\n", ret ); break;
			case 2 : sprintf( ret, "%s\t// GB\n", ret ); break;
			case 3 : sprintf( ret, "%s\t// B\n", ret ); break;
		}
		for ( uint32_t y = 0; y < stride; y += image.width() ) {
			sprintf( ret, "%s\t", ret );
			for ( int32_t x = 0; x < image.width(); x++ ) {
				uint8_t value = grid[channel * stride + y + x];
				sprintf( ret, "%s%d,", ret, value );
				if ( x + 1 < image.width() ) {
					sprintf( ret, "%s ", ret );
				}
			}
			sprintf( ret, "%s\n", ret );
		}
	}
	sprintf( ret, "%s};\n", ret );

	QString fileName = QFileDialog::getSaveFileName( this, "Save C/C++ header", "", "*.h *.hpp" );
	if ( fileName == "" ) {
		return;
	}
	QFile outputFile( fileName );
	if ( not outputFile.open( QIODevice::WriteOnly ) ) {
		return;
	}
	outputFile.write( ret );
	outputFile.close();
}
