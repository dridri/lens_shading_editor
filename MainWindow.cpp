#include <cmath>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ui_OpenDialog.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow()
	: mOpenDialog( nullptr )
	, uiOpenDialog( nullptr )
{
	mDialogPath = getenv( "HOME" );
	ui = new Ui::MainWindow;
	ui->setupUi(this);

	mGLWidget = new GLWidget();
	mGLWidget->setVisible( false );
	ui->gridLayout_9->addWidget( mGLWidget, 0, 0, 1, 1 );

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
	connect( ui->r_tilt_h, SIGNAL(valueChanged(double)), this, SLOT(RepaintShader()) );
	connect( ui->r_tilt_v, SIGNAL(valueChanged(double)), this, SLOT(RepaintShader()) );
	connect( ui->gr_tilt_h, SIGNAL(valueChanged(double)), this, SLOT(RepaintShader()) );
	connect( ui->gr_tilt_v, SIGNAL(valueChanged(double)), this, SLOT(RepaintShader()) );
	connect( ui->gb_tilt_h, SIGNAL(valueChanged(double)), this, SLOT(RepaintShader()) );
	connect( ui->gb_tilt_v, SIGNAL(valueChanged(double)), this, SLOT(RepaintShader()) );
	connect( ui->b_tilt_h, SIGNAL(valueChanged(double)), this, SLOT(RepaintShader()) );
	connect( ui->b_tilt_v, SIGNAL(valueChanged(double)), this, SLOT(RepaintShader()) );
	connect( ui->groupBox, SIGNAL(toggled(bool)), this, SLOT(RepaintShader()) );
	connect( ui->groupBox_1, SIGNAL(toggled(bool)), this, SLOT(RepaintShader()) );
	connect( ui->groupBox_2, SIGNAL(toggled(bool)), this, SLOT(RepaintShader()) );
	connect( ui->groupBox_3, SIGNAL(toggled(bool)), this, SLOT(RepaintShader()) );

	connect( ui->r_radius, &QSlider::valueChanged, [=]( int v ) {
		ui->r_v_radius->setText( QString::number( (float)v / 100.0f ) );
	});
	connect( ui->gr_radius, &QSlider::valueChanged, [=]( int v ) {
		ui->gr_v_radius->setText( QString::number( (float)v / 100.0f ) );
	});
	connect( ui->gb_radius, &QSlider::valueChanged, [=]( int v ) {
		ui->gb_v_radius->setText( QString::number( (float)v / 100.0f ) );
	});
	connect( ui->b_radius, &QSlider::valueChanged, [=]( int v ) {
		ui->b_v_radius->setText( QString::number( (float)v / 100.0f ) );
	});

	connect( ui->r_base, SIGNAL(valueChanged(int)), this, SLOT(r_base_changed(int)) );
	connect( ui->r_strength, SIGNAL(valueChanged(int)), this, SLOT(r_strength_changed(int)) );
	connect( ui->gr_base, SIGNAL(valueChanged(int)), this, SLOT(gr_base_changed(int)) );
	connect( ui->gr_strength, SIGNAL(valueChanged(int)), this, SLOT(gr_strength_changed(int)) );
	connect( ui->gb_base, SIGNAL(valueChanged(int)), this, SLOT(gb_base_changed(int)) );
	connect( ui->gb_strength, SIGNAL(valueChanged(int)), this, SLOT(gb_strength_changed(int)) );
	connect( ui->b_base, SIGNAL(valueChanged(int)), this, SLOT(b_base_changed(int)) );
	connect( ui->b_strength, SIGNAL(valueChanged(int)), this, SLOT(b_strength_changed(int)) );

	connect( ui->separate_gr_gb, &QCheckBox::toggled, [=]( bool ch ){
		ui->groupBox_2->setVisible( ch );
		ui->groupBox_1->setTitle( ch ? "GR" : "G" );
	});
	connect( ui->view2d, &QCheckBox::toggled, [=]( bool ch ){
		if ( ch ) {
			mGLWidget->setVisible( false );
			ui->shader->setVisible( true );
			ui->view->repaint();
		}
	});
	connect( ui->view3d, &QCheckBox::toggled, [=]( bool ch ){
		if ( ch ) {
			ui->shader->setVisible( false );
			mGLWidget->setVisible( true );
			ui->view->repaint();
			mGLWidget->RepaintShader( mChannels, ui->separate_gr_gb->isChecked(), ui->groupBox->isChecked(), ui->groupBox_1->isChecked(), ui->groupBox_2->isChecked(), ui->groupBox_3->isChecked() );
		}
	});

	connect( ui->loadBtn, SIGNAL(pressed()), this, SLOT(Load()) );
	connect( ui->exportBtn, SIGNAL(pressed()), this, SLOT(Export()) );

	memset( mBaseChannels, 0, sizeof(mBaseChannels) );

	int32_t default_value = 32;
	ui->r_base->setValue( default_value );
	ui->gr_base->setValue( default_value );
	ui->gb_base->setValue( default_value );
	ui->b_base->setValue( default_value );

	ui->groupBox_2->setVisible( false );
	ui->groupBox_1->setTitle( "G" );
	adjustSize();
	setGeometry( x(), y(), width() + height() * 45 / 39, height() );

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

	if ( mGLWidget ) {
		mGLWidget->RepaintShader( mChannels, ui->separate_gr_gb->isChecked(), ui->groupBox->isChecked(), ui->groupBox_1->isChecked(), ui->groupBox_2->isChecked(), ui->groupBox_3->isChecked() );
		if ( mGLWidget->isVisible() ) {
			mGLWidget->repaint();
		}
	}
}


QImage MainWindow::GenerateImage( bool force_enable_all_channels, int32_t width, int32_t height )
{
	QImage image = QImage( width, height, QImage::Format_RGB32 );

	auto apply = [=]( uint16_t* channel, int x, int y, float dist, int dotradius, int dotstrength, float tilt_h, float tilt_v )
	{
		int32_t ret = ui->r_base->value() + (int32_t)( channel[y*width+x] );

		if ( dotradius > 0 ) {
			float radius = (float)dotradius * (float)std::max( width, height ) / 100.0f;
			float dist_norm = std::min( 1.0f, dist / radius );
			float dot = ( std::cos(dist_norm * M_PI ) + 1.0f ) / 2.0f;
			ret = std::max( 0, std::min( 255, ret + (int32_t)( dot * dotstrength ) ) );
		}

		if ( tilt_h != 0.0f ) {
			float dist_h = x - width / 2;
			ret -= (int32_t)( 2 * 256.0f * std::sin( tilt_h * M_PI / 180.0f ) * dist_h / (float)width / 2.0f );
		}

		if ( tilt_v != 0.0f ) {
			float dist_v = y - height / 2;
			ret += (int32_t)( 2 * 256.0f * std::sin( tilt_v * M_PI / 180.0f ) * dist_v / (float)width / 2.0f );
		}

		return ret;
	};

	for ( int32_t y = 0; y < height; y++ ) {
		for ( int32_t x = 0; x < width; x++ ) {
			int32_t dist = std::sqrt( ( x - width / 2 ) * ( x - width / 2 ) + ( y - height / 2 ) * ( y - height / 2 ) );

			int32_t r = ui->r_base->value() + apply( mBaseChannels[0], x, y, (float)dist, ui->r_radius->value(), ui->r_strength->value(), ui->r_tilt_h->value(), ui->r_tilt_v->value() );
			int32_t gr = ui->gr_base->value() + apply( mBaseChannels[1], x, y, (float)dist, ui->gr_radius->value(), ui->gr_strength->value(), ui->gr_tilt_h->value(), ui->gr_tilt_v->value() );
			int32_t gb = ui->gb_base->value() + apply( mBaseChannels[2], x, y, (float)dist, ui->gb_radius->value(), ui->gb_strength->value(), ui->gb_tilt_h->value(), ui->gb_tilt_v->value() );
			int32_t b = ui->b_base->value() + apply( mBaseChannels[3], x, y, (float)dist, ui->b_radius->value(), ui->b_strength->value(), ui->b_tilt_h->value(), ui->b_tilt_v->value() );

			r = std::min( std::max( r, 32 ), 255 );
			gr = std::min( std::max( gr, 32 ), 255 );
			gb = std::min( std::max( gb, 32 ), 255 );
			b = std::min( std::max( b, 32 ), 255 );

			mChannels[0][y * width + x] = r;
			mChannels[1][y * width + x] = gr;
			mChannels[2][y * width + x] = gb;
			mChannels[3][y * width + x] = b;

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

			if ( ui->separate_gr_gb->isChecked() ) {
				image.setPixel( x, y, QColor( r, (gr+gb)/2, b ).rgba() );
			} else {
				image.setPixel( x, y, QColor( r, gr, b ).rgba() );
			}
		}
	}

	return image;
}


void MainWindow::Load()
{
	if ( not mOpenDialog ) {
		mOpenDialog = new QDialog( this );
	}
	if ( not uiOpenDialog ) {
		uiOpenDialog = new Ui::OpenDialog;
		uiOpenDialog->setupUi( mOpenDialog );
		mOpenDialog->setModal( true );
		QLineEdit* lines[5] = { nullptr, uiOpenDialog->ch1, uiOpenDialog->ch2, uiOpenDialog->ch3, uiOpenDialog->ch4 };
		auto openFile = [=]( uint32_t channel ) {
			QString selfilter = "All supported files (.jpeg, .jpg, .bin) (*.jpeg *.jpg *.bin)";
			QString fileName = QFileDialog::getOpenFileName( this, "Open 16 bits raw 4-channels image", mDialogPath, "JPEG raw RasperryPi image (.jpeg, .jpg) (*.jpeg *.jpg);;Raw lens correction channel (.bin) (*.bin);;All supported files (.jpeg, .jpg, .bin) (*.jpeg *.jpg *.bin);;All files (*.*)", &selfilter, QFileDialog::HideNameFilterDetails );
			if ( fileName == "" ) {
				return;
			}
			mDialogPath = QFileInfo(fileName).path();
			lines[channel]->setText( fileName );
			if ( fileName.endsWith( ".jpg", Qt::CaseInsensitive ) or fileName.endsWith( ".jpeg", Qt::CaseInsensitive ) ) {
				uiOpenDialog->ch2->setEnabled( false );
				uiOpenDialog->ch3->setEnabled( false );
				uiOpenDialog->ch4->setEnabled( false );
				uiOpenDialog->ch2Btn->setEnabled( false );
				uiOpenDialog->ch3Btn->setEnabled( false );
				uiOpenDialog->ch4Btn->setEnabled( false );
				uiOpenDialog->ch2->clear();
				uiOpenDialog->ch3->clear();
				uiOpenDialog->ch4->clear();
			} else {
				uiOpenDialog->ch2->setEnabled( true );
				uiOpenDialog->ch3->setEnabled( true );
				uiOpenDialog->ch4->setEnabled( true );
				uiOpenDialog->ch2Btn->setEnabled( true );
				uiOpenDialog->ch3Btn->setEnabled( true );
				uiOpenDialog->ch4Btn->setEnabled( true );
				if ( uiOpenDialog->autofill->isChecked() ) {
					QString fileNameBase = fileName.mid( 0, fileName.lastIndexOf( '.' ) - 1 );
					uint32_t lastdigit = fileName.mid( fileName.lastIndexOf( '.' ) - 1, 1 ).toInt();
					if ( lastdigit == channel ) {
						for ( uint32_t ch = 1; ch <= 4; ch++ ) {
							if ( lines[ch]->text() != "" ) continue;
							lines[ch]->setText( fileNameBase + ( '0' + ch ) + ".bin" );
						}
					}
				}
			}
		};
		connect( uiOpenDialog->ch1Btn, &QPushButton::pressed, [=](){ openFile( 1 ); } );
		connect( uiOpenDialog->ch2Btn, &QPushButton::pressed, [=](){ openFile( 2 ); } );
		connect( uiOpenDialog->ch3Btn, &QPushButton::pressed, [=](){ openFile( 3 ); } );
		connect( uiOpenDialog->ch4Btn, &QPushButton::pressed, [=](){ openFile( 4 ); } );
		connect( uiOpenDialog->clear, &QPushButton::pressed, [=](){
			uiOpenDialog->ch1->clear();
			uiOpenDialog->ch2->clear();
			uiOpenDialog->ch3->clear();
			uiOpenDialog->ch4->clear();
		} );
		connect( mOpenDialog, SIGNAL(accepted()), this, SLOT(LoadChannels()) );
	}

	uiOpenDialog->ch2->setEnabled( false );
	uiOpenDialog->ch3->setEnabled( false );
	uiOpenDialog->ch4->setEnabled( false );
	uiOpenDialog->ch2Btn->setEnabled( false );
	uiOpenDialog->ch3Btn->setEnabled( false );
	uiOpenDialog->ch4Btn->setEnabled( false );
	mOpenDialog->show();
}


void MainWindow::LoadChannels()
{
	QMessageBox messageBox;
	QString filenames[5] = { "", uiOpenDialog->ch1->text(), uiOpenDialog->ch2->text(), uiOpenDialog->ch3->text(), uiOpenDialog->ch4->text() };
	uint16_t buf[3280 * 2464 / 2 / sizeof(uint16_t)];
	uint64_t final[52 * 39];
	uint32_t accum[52 * 39];

	for ( uint32_t ch = 1; ch <= 4; ch++ ) {
		QFile inputFile( filenames[ch] );
		if ( not inputFile.open( QIODevice::ReadOnly ) ) {
			messageBox.critical( 0, "Error", "Cannot open file \"" + filenames[ch] + "\" for reading :\n" + inputFile.errorString() );
			return;
		}
		qint64 ret = inputFile.read( (char*)buf, sizeof(buf) );
		inputFile.close();

		int32_t width = 2592 / 2;
		int32_t height = 1944 / 2;
		if ( ret == 4040960 ) { // v2 camera
			width = 3280 / 2;
			height = 2464 / 2;
		} else if ( ret != 2519424 ) {
			messageBox.critical( 0, "Error", "Image is not a full-sensor picture comming from Raspberry Pi v1 or v2 Camera\nFile size is : " + QString::number(ret) + ", should be 2519424 or 4040960" );
			return;
		}

		//Average out the centre 64 pixels.
		int32_t mid_value_avg = 0;
		int32_t count = 0;
		for ( int32_t y = height / 2 - 4; y < height / 2 + 4; y++ ) {
			for ( int32_t x = width / 2 - 4; x < width / 2 + 4; x++ ) {
				mid_value_avg += buf[y * width + x];
				count++;
			}
		}
		uint16_t middle_val = ( mid_value_avg / count ) << 5;

		memset( final, 0, sizeof(final) );
		memset( accum, 0, sizeof(accum) );
		for ( int32_t y = 0; y < height; y++ ) {
			int32_t y1 = ( y * 2 / 64 );
			for ( int32_t x = 0; x < width; x++ ) {
				int32_t x1 = ( x * 2 / 64 );
				final[y1 * 52 + x1] += buf[y * width + x];
				accum[y1 * 52 + x1]++;
			}
		}
		for ( int32_t i = 0; i < 52*39; i++ ) {
			int32_t gain = ( middle_val * accum[i] ) / final[i];
			mBaseChannels[ch-1][i] = gain;
		}
	}

	ui->r_base->setValue( 0 );
	ui->gr_base->setValue( 0 );
	ui->gb_base->setValue( 0 );
	ui->b_base->setValue( 0 );
	ui->separate_gr_gb->setChecked( true );
	RepaintShader();
}


void MainWindow::Save()
{
}
