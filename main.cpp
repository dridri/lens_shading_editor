/*
 * BCFlight
 * Copyright (C) 2016 Adrien Aubry (drich)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#include <iostream>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtWidgets/QLabel>
#include <QGridLayout>
#include <QDebug>
#include "MainWindow.h"



int main( int ac, char** av )
{
	QApplication app( ac, av );
/*
	QImage image(52, 38, QImage::Format_RGB32 );
	for ( uint32_t y = 0; y < 38; y++ ) {
		for ( uint32_t x = 0; x < 52; x++ ) {
			uint32_t r = ls_grid[y*52+x];
			uint32_t g1 = ls_grid[(52*38*1)+y*52+x];
			uint32_t g2 = ls_grid[(52*38*2)+y*52+x];
			uint32_t b = ls_grid[(52*38*3)+y*52+x];
			image.setPixel( x, y, QColor( r, (g1+g2)/2, b ).rgba() );
		}
	}

	QPixmap pix = QPixmap::fromImage( image );
	QLabel* label = new QLabel();
	label->setScaledContents(true);
	label->setPixmap( pix );
// 	QGridLayout* layout = new QGridLayout();
	label->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
*/

	MainWindow win;
	win.show();

	return app.exec();
}
