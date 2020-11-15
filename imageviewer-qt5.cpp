
/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <vector>
#include <QtWidgets>
#ifndef QT_NO_PRINTER
#include <QPrintDialog>
#endif
#include <iostream>
#include <sstream>
#include <string.h>
#include <string>
#include <cstring>
#include <QString>
#include "imageviewer-qt5.h"
#include <QtWidgets>
#include "qcustomplot.h"
#include <QVector>

using namespace std;

int	spinbox1Value	= 8;
int	spinbox2Value	= 0;
int	spinbox3Value	= 255;


int	slider1Value	= 10;
int	slider2Value	= 10;
int	slider3Value	= 10;

int	x_filter_size	= 1;
int	y_filter_size	= 1;

double spinboxDoubleValue = 0.5;

int		avh	= 0;
int		vari	= 0;
vector<double>	coe;
QImage		originImage;
QImage		originGrayImage;


ImageViewer::ImageViewer()
{
    image = NULL;
    resize( 1600, 600 );

    startLogging();

    generateMainGui();
    renewLogging();

    generateControlPanels();
    createActions();
    createMenus();

    resize( QGuiApplication::primaryScreen()->availableSize() * 0.65 );
}


void ImageViewer::applyExampleAlgorithm()
{
    if ( image != NULL )
    {
        int	w	= image->width();
        int	h	= image->height();
        /* *image = image->convertToFormat(QImage::Format_Grayscale8); */
        for ( int i = 0; i < w; i++ )
        {
            for ( int j = 0; j < h; j++ )
            {
                QRgb	colorful_pixel	= image->pixel( i, j );
                int	gray		= qGray( colorful_pixel );
                QRgb	gray_pixel	= qRgb( gray, gray, gray );
                image->setPixel( i, j, gray_pixel );
            }
        }
        originImage = image->copy();


        updateImageDisplay();
        logFile << "example algorithm applied " << std::endl;
        renewLogging();
    } else {
        alert();
    }
}


void ImageViewer::bresenham( int x0, int x1, int y0, int y1, int config )
{
/*
 *    int deltax = x1 - x0;
 *    int deltay = abs(y1 - y0);
 *    int error = deltax / 2;
 *    int ystep;
 *    int y = y0;
 *    int p=2*deltay -deltax;
 *    int x = x0;
 *    while(x<x1){
 *        if(p>=0){
 *            plot(x,y);
 *            y=y+1;
 *            p=p+2*deltay-2*deltax;
 *        }
 *        else{
 *            plot(x,y);
 *            p=p+2*deltay;
 *        }
 *        x=x+1;
 *    }
 */

    boolean steep = (abs( y1 - y0 ) > abs( x1 - x0 ) );
    if ( steep )
    {
        /* swap(x0,y0) */
        int temp = x0;
        x0	= y0;
        y0	= temp;
        /* swap(x1, y1) */
        temp	= x1;
        x1	= y1;
        y1	= temp;
    }
    if ( x0 > x1 )
    {
        /* swap(x0, x1) */
        int temp = x0;
        x0	= x1;
        x1	= temp;
        /* swap(y0, y1) */
        temp	= y0;
        y0	= y1;
        y1	= temp;
    }
    int	deltax	= x1 - x0;
    int	deltay	= abs( y1 - y0 );
    int	error	= deltax / 2;
    int	ystep;
    int	y = y0;
    if ( y0 < y1 )
    {
        ystep = 1;
    } else { ystep = -1; }
    for ( int x = x0; x < x1; x++ )
    {
        if ( steep )
        {
            if ( config == 0 )
            {
                plot( y, x );
            } else { plotO( y, x ); }
        } else { if ( config == 0 )
             {
                 plot( x, y );
             } else { plotO( x, y ); } }
        error = error - deltay;
        if ( error < 0 )
        {
            y	= y + ystep;
            error	= error + deltax;
        }
    }
}


void ImageViewer::plot( int x, int y )
{
    image->setPixel( x, y, qRgb( 255, 0, 0 ) );
}


void ImageViewer::plotO( int x, int y )
{
    image->setPixel( x, y, originImage.pixel( x, y ) );
}


void ImageViewer::setSlider3Value( int value )
{
    if ( image != NULL )
    {
        slider3Value = value;
        label_contrast_value->setText( QString::number( value ) );
        button2->click();
        button3->click();
        int		cont	= value;
        QImage		copie	= originImage.copy();
        unsigned int	*img	= (unsigned int *) image->bits();
        {
            int		pixels	= image->width() * image->height();
            unsigned int	*data	= (unsigned int *) originImage.bits();

            int red, green, blue, nRed, nGreen, nBlue;

            if ( cont > 0 && cont < 100 )
            {
                float param = 1 / (1 - cont / 100.0) - 1;

                for ( int i = 0; i < pixels; ++i )
                {
                    nRed	= qRed( data[i] );
                    nGreen	= qGreen( data[i] );
                    nBlue	= qBlue( data[i] );

                    red	= nRed + (nRed - 127) * param;
                    red	= (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
                    green	= nGreen + (nGreen - 127) * param;
                    green	= (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
                    blue	= nBlue + (nBlue - 127) * param;
                    blue	= (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;

                    img[i] = qRgba( red, green, blue, qAlpha( data[i] ) );
                }
            }else  {
                for ( int i = 0; i < pixels; ++i )
                {
                    nRed	= qRed( data[i] );
                    nGreen	= qGreen( data[i] );
                    nBlue	= qBlue( data[i] );

                    red	= nRed + (nRed - 127) * cont / 100.0;
                    red	= (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
                    green	= nGreen + (nGreen - 127) * cont / 100.0;
                    green	= (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
                    blue	= nBlue + (nBlue - 127) * cont / 100.0;
                    blue	= (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;

                    img[i] = qRgba( red, green, blue, qAlpha( data[i] ) );
                }
            }
        }

        updateImageDisplay();
    } else {
        alert();
    }
}


void ImageViewer::setSlider2Value( int value )
{
    if ( image != NULL )
    {
        slider2Value = value;
        slider2->setValue( value );
        button2->click();
        button3->click();
        label_brightness_value->setText( QString::number( value ) );

        int		bri = value - avh;
        int		red, green, blue;
        int		pixels	= image->width() * image->height();
        QImage		copie	= originImage.copy();
        unsigned int	*data	= (unsigned int *) copie.bits();
        unsigned int	*img	= (unsigned int *) image->bits();

        for ( int i = 0; i < pixels; ++i )
        {
            red	= qRed( data[i] ) + bri;
            red	= (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green	= qGreen( data[i] ) + bri;
            green	= (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue	= qBlue( data[i] ) + bri;
            blue	= (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
            img[i]	= qRgba( red, green, blue, qAlpha( data[i] ) );
        }

        updateImageDisplay();
    } else {
        alert();
    }
}

void ImageViewer::readAndParCoTable()
{
    coe.clear();

    int	x	= slider_size_x_filter->value();
    int	y	= slider_size_y_filter->value();

        m_coefficients->setRowCount( y );
        m_coefficients->setColumnCount( x );
        double	sum	= 0;
        for ( int i = 0; i < x; i++ )
        {
            for ( int j = 0; j < y; j++ )
            {
                if(m_coefficients->item(j,i)!=NULL){
                    double v = m_coefficients->item( j, i )->text().toDouble();
                    //cout<<m_coefficients->item( j, i )->text().toUtf8().constData()<<endl;
                    sum += v;
                } else {
                    //cout<<i<<"---"<<j<<"---error---"<<endl;
                }
            }
        }

        //cout<<"sum:"<<sum<<endl;

        for ( int i = 0; i < x; i++ )
        {
            for ( int j = 0; j < y; j++ )
            {
                if(m_coefficients->item(j,i)!=NULL){
                  double v = m_coefficients->item( j, i )->text().toDouble() / sum;
                 // cout<<"value of v:"<<v<<endl;
                  coe.push_back( v );
                } else {
                   // cout<<i<<"---"<<j<<"---error2---"<<endl;
                    coe.push_back( (double)0 );
                }
            }
        }


}

void ImageViewer::button_without_border()
{
    if ( image != NULL )
    {
        int	_x	= slider_size_x_filter->value();
        int	_y	= slider_size_y_filter->value();
        if(_x % 2 == 0 || _y == 0) {
            QMessageBox::information( this, QString::fromLocal8Bit( "Error" ), QString::fromLocal8Bit( "X and Y must be odd number!" ) );
        } else {
            readAndParCoTable();
            for (auto iter = coe.cbegin(); iter != coe.cend(); iter++)
            {
                cout << (*iter) << endl;
            }
            int	x	= (slider_size_x_filter->value() - 1) / 2; /* z.B 3->1 */
            int	y	= (slider_size_y_filter->value() - 1) / 2;
            int	fy	= 2 * y + 1;
            //cout<<"fy"<<fy<<endl;
            int	w	= image->width()-1;
            int	h	= image->height()-1;

            for ( int i = 0; i < w; i++ )
            {
                for ( int j = 0; j < h; j++ )                                                                  /* pixel loop */
                {
                    //cout<<"i-x:"<<i-x<<endl;
                    //cout<<"j-y:"<<j-y<<endl;
                    if ( ( (i - x) < 0) && ( (j - y) < 0) )                                            /* left-top */
                    { /* mach nichts hier */ //cout<<"lt"<<endl;
                    } if ( ( (i - x) < 0) && ( (j - y) >= 0) && ( (j + y) <= h) )                      /* left-border */
                    { /* mach nichts hier */ //cout<<"lb"<<endl;
                    } if ( ( (i - x) < 0) && ( (j + y) > h) )                                          /* left-bottom */
                    { /* mach nichts hier */ //cout<<"lbb"<<endl;
                    } if ( ( (i + x) > w) && ( (j - y) < 0) )                                          /* right-top */
                    { /* mach nichts hier */ //cout<<"rt"<<endl;
                    } if ( ( (i + x) > w) && ( (j - y) >= 0) && ( (j + y) <= h) )                      /* right-border */
                    { /* mach nichts hier */ //cout<<"rb"<<endl;
                    } if ( ( (i + x) > w) && ( (j + y) > h) )                                          /* right-bottom */
                    { /* mach nichts hier */ //cout<<"rbb"<<endl;
                    } if ( ( (i + x) <= w) && ( (i - x) >= 0) && ( (j - y) < 0) )                      /* top-border */
                    { /* mach nichts hier */ //cout<<"tb"<<endl;
                    } if ( ( (i + x) <= w) && ( (i - x) >= 0) && ( (j + y) > h) )                      /* bottom-border */
                    { /* mach nichts hier */ //cout<<"bb"<<endl;
                    } if ( ( (i + x) <= w) && ( (i - x) >= 0) && ( (j - y) >= 0) && ( (j + y) <= h) )  /* inside of image */
                    {
                        double	r	= 0;
                        double	g	= 0;
                        double	b	= 0;

                        for ( int filterx = -x; filterx <= x; filterx++ )
                        {
                            for ( int filtery = -y; filtery <= y; filtery++ )
                            {
                                int	index	= (filtery + y) + (filterx + x) * fy;
                                double	ws	= coe[ index ];
                                r	+= originImage.pixelColor( i+filterx, j+filtery ).red() * ws;
                                g	+= originImage.pixelColor( i+filterx, j+filtery ).green() * ws;
                                b	+= originImage.pixelColor( i+filterx, j+filtery ).blue() * ws;
                            }
                        }
                        //  if(i==200 && j == 100){
                        //    cout<<"or:"<<originImage.pixelColor(i,j).red()<<endl<<r<<endl;
                        //  }
                        image->setPixel( i, j, qRgb( (int) r, (int) g, (int) b ) );
                    }
                }
            }
            updateImageDisplay();
        }
    } else {
        alert();
    }
}


void ImageViewer::button_zero_padding()
{
    if ( image != NULL )
    {
        int	_x	= slider_size_x_filter->value();
        int	_y	= slider_size_y_filter->value();
        if(_x % 2 == 0 || _y == 0) {
            QMessageBox::information( this, QString::fromLocal8Bit( "Error" ), QString::fromLocal8Bit( "X and Y must be odd number!" ) );
        } else {
            readAndParCoTable();
            for (auto iter = coe.cbegin(); iter != coe.cend(); iter++)
            {
                cout << (*iter) << endl;
            }
            int	x	= (slider_size_x_filter->value() - 1) / 2; /* z.B 3->1 */
            int	y	= (slider_size_y_filter->value() - 1) / 2;
            int	fy	= 2 * y + 1;

            //cout<<"fy"<<fy<<endl;
            int	w	= image->width()-1;
            int	h	= image->height()-1;

            for ( int i = 0; i <= w; i++ )
            {
                for ( int j = 0; j <= h; j++ )                                                                  /* pixel loop */
                {
                        double	r	= 0;
                        double	g	= 0;
                        double	b	= 0;
                        for ( int filterx = -x; filterx <= x; filterx++ )
                        {
                            for ( int filtery = -y; filtery <= y; filtery++ )
                            {
                                int	index	= (filtery + y) + (filterx + x) * fy;
                                double	ws	= coe[ index ];
                                if ( ( (i + filterx) <= w) && ( (i + filterx) >= 0) && ( (j + filtery) >= 0) && ( (j + filtery) <= h) ){
                                    r	+= originImage.pixelColor( i+filterx, j+filtery ).red() * ws;
                                    g	+= originImage.pixelColor( i+filterx, j+filtery ).green() * ws;
                                    b	+= originImage.pixelColor( i+filterx, j+filtery ).blue() * ws;
                                }  // else color = color + 0
                            }
                        }
                        image->setPixel( i, j, qRgb( (int) r, (int) g, (int) b ) );
                 }
             }
            updateImageDisplay();
        }
    } else {
        alert();
    }
}


void ImageViewer::button_constant_border()
{
    if ( image != NULL )
    {
        cout<<"log"<<endl;
        int	_x	= slider_size_x_filter->value();
        int	_y	= slider_size_y_filter->value();
        if(_x % 2 == 0 || _y == 0) {
            QMessageBox::information( this, QString::fromLocal8Bit( "Error" ), QString::fromLocal8Bit( "X and Y must be odd number!" ) );
        } else {
            readAndParCoTable();
            int	x	= (slider_size_x_filter->value() - 1) / 2; /* z.B 3->1 */
            int	y	= (slider_size_y_filter->value() - 1) / 2;
            int	fy	= 2 * y + 1;
            int	w	= image->width()-1;
            int	h	= image->height()-1;
            for ( int i = 0; i <= w; i++ )
            {
                for ( int j = 0; j <= h; j++ )                                                                  /* pixel loop */
                {
                        double	r	= 0;
                        double	g	= 0;
                        double	b	= 0;
                        for ( int filterx = -x; filterx <= x; filterx++ )
                        {
                            for ( int filtery = -y; filtery <= y; filtery++ )
                            {
                                int	index	= (filtery + y) + (filterx + x) * fy;
                                double	ws	= coe[ index ];
                                if ( ( (i + filterx) <= w) && ( (i + filterx) >= 0) && ( (j + filtery) >= 0) && ( (j + filtery) <= h) ){
                                    r	+= originImage.pixelColor( i+filterx, j+filtery ).red() * ws;
                                    g	+= originImage.pixelColor( i+filterx, j+filtery ).green() * ws;
                                    b	+= originImage.pixelColor( i+filterx, j+filtery ).blue() * ws;
                                } else if ( ( (i + filterx) < 0) && ( (j + filtery) < 0) ){         // left-top
                                    r	+= originImage.pixelColor( 0,0 ).red() * ws;
                                    g	+= originImage.pixelColor( 0,0 ).green() * ws;
                                    b	+= originImage.pixelColor( 0,0 ).blue() * ws;
                                } else if( ( (i + filterx) > w) && ( (j + filtery) < 0) ){          // right-top
                                    r	+= originImage.pixelColor( w,0 ).red() * ws;
                                    g	+= originImage.pixelColor( w,0 ).green() * ws;
                                    b	+= originImage.pixelColor( w,0 ).blue() * ws;
                                } else if ( ( (i + filterx) < 0) && ( (j + filtery) > h) ){         // left-bottom
                                    r	+= originImage.pixelColor( 0,h ).red() * ws;
                                    g	+= originImage.pixelColor( 0,h ).green() * ws;
                                    b	+= originImage.pixelColor( 0,h ).blue() * ws;
                                } else if ( ( (i + filterx) > w) && ( (j + filtery) > h) ){   // right-bottom
                                    r	+= originImage.pixelColor( w,h ).red() * ws;
                                    g	+= originImage.pixelColor( w,h ).green() * ws;
                                    b	+= originImage.pixelColor( w,h ).blue() * ws;
                                } else if (( (i + filterx) < 0) && ( (j + filtery) >= 0) && ( (j + filtery) <= h)) { // left-border
                                    r	+= originImage.pixelColor( 0,j + filtery ).red() * ws;
                                    g	+= originImage.pixelColor( 0,j + filtery ).green() * ws;
                                    b	+= originImage.pixelColor( 0,j + filtery ).blue() * ws;
                                } else if ( ( (i + filterx) > w) && ( (j + filtery) >= 0) && ( (j + filtery) <= h) ) { // right-border
                                    r	+= originImage.pixelColor( w,j + filtery ).red() * ws;
                                    g	+= originImage.pixelColor( w,j + filtery ).green() * ws;
                                    b	+= originImage.pixelColor( w,j + filtery ).blue() * ws;
                                } else if ( ( (i + filterx) <= w) && ( (i + filterx) >= 0) && ( (j + filtery) < 0) )   { // top-border
                                    r	+= originImage.pixelColor( i + filterx,0 ).red() * ws;
                                    g	+= originImage.pixelColor( i + filterx,0 ).green() * ws;
                                    b	+= originImage.pixelColor( i + filterx,0 ).blue() * ws;
                                } else if ( ( (i + filterx) <= w) && ( (i + filterx) >= 0) && ( (j + filtery) > h) )     { // bottom-border
                                    r	+= originImage.pixelColor( i + filterx,h ).red() * ws;
                                    g	+= originImage.pixelColor( i + filterx,h ).green() * ws;
                                    b	+= originImage.pixelColor( i + filterx,h ).blue() * ws;
                                }
                             }
                        }
                        image->setPixel( i, j, qRgb( (int) r, (int) g, (int) b ) );
                 }
             }
            updateImageDisplay();
        }
    } else {
        alert();
    }
}


void ImageViewer::button_mirror_border()
{
    if ( image != NULL )
    {
        int	_x	= slider_size_x_filter->value();
        int	_y	= slider_size_y_filter->value();
        if(_x % 2 == 0 || _y == 0) {
            QMessageBox::information( this, QString::fromLocal8Bit( "Error" ), QString::fromLocal8Bit( "X and Y must be odd number!" ) );
        } else {
            readAndParCoTable();
            for (auto iter = coe.cbegin(); iter != coe.cend(); iter++)
            {
                cout << (*iter) << endl;
            }
            int	x	= (slider_size_x_filter->value() - 1) / 2; /* z.B 3->1 */
            int	y	= (slider_size_y_filter->value() - 1) / 2;
            int	fy	= 2 * y + 1;
            //cout<<"fy"<<fy<<endl;
            int	w	= image->width()-1;
            int	h	= image->height()-1;

            for ( int i = 0; i <= w; i++ )
            {
                for ( int j = 0; j <= h; j++ )                                                                  /* pixel loop */
                {
                        double	r	= 0;
                        double	g	= 0;
                        double	b	= 0;
                        for ( int filterx = -x; filterx <= x; filterx++ )
                        {
                            for ( int filtery = -y; filtery <= y; filtery++ )
                            {
                                int	index	= (filtery + y) + (filterx + x) * fy;
                                double	ws	= coe[ index ];
                                if ( ( (i + filterx) <= w) && ( (i + filterx) >= 0) && ( (j + filtery) >= 0) && ( (j + filtery) <= h) ){
                                    r	+= originImage.pixelColor( i+filterx, j+filtery ).red() * ws;
                                    g	+= originImage.pixelColor( i+filterx, j+filtery ).green() * ws;
                                    b	+= originImage.pixelColor( i+filterx, j+filtery ).blue() * ws;
                                } else if ( ( (i + filterx) < 0) && ( (j + filtery) < 0) ){         // left-top
                                    r	+= originImage.pixelColor( -i - filterx, -j - filtery ).red() * ws;
                                    g	+= originImage.pixelColor( -i - filterx, -j - filtery  ).green() * ws;
                                    b	+= originImage.pixelColor( -i - filterx, -j - filtery  ).blue() * ws;
                                } else if( ( (i + filterx) > w) && ( (j + filtery) < 0) ){          // right-top
                                    r	+= originImage.pixelColor( 2*w-i-filterx, -j - filtery  ).red() * ws;
                                    g	+= originImage.pixelColor( 2*w-i-filterx, -j - filtery ).green() * ws;
                                    b	+= originImage.pixelColor( 2*w-i-filterx, -j - filtery ).blue() * ws;
                                } else if ( ( (i + filterx) < 0) && ( (j + filtery) > h) ){                     // left-bottom
                                    r	+= originImage.pixelColor( -i - filterx , 2*h-j - filtery).red() * ws;
                                    g	+= originImage.pixelColor( -i - filterx , 2*h-j - filtery ).green() * ws;
                                    b	+= originImage.pixelColor( -i - filterx , 2*h-j - filtery ).blue() * ws;
                                } else if ( ( (i + filterx) > w) && ( (j + filtery) > h) ){   // right-bottom
                                    r	+= originImage.pixelColor( 2*w-i-filterx,2*h-j - filtery ).red() * ws;
                                    g	+= originImage.pixelColor( 2*w-i-filterx,2*h-j - filtery ).green() * ws;
                                    b	+= originImage.pixelColor( 2*w-i-filterx,2*h-j - filtery ).blue() * ws;
                                } else if (( (i + filterx) < 0) && ( (j + filtery) >= 0) && ( (j + filtery) <= h)) { // left-border
                                    r	+= originImage.pixelColor( -i - filterx,j + filtery ).red() * ws;
                                    g	+= originImage.pixelColor( -i - filterx,j + filtery ).green() * ws;
                                    b	+= originImage.pixelColor( -i - filterx,j + filtery ).blue() * ws;
                                } else if ( ( (i + filterx) > w) && ( (j + filtery) >= 0) && ( (j + filtery) <= h) ) { // right-border
                                    r	+= originImage.pixelColor( 2*w-i-filterx,j + filtery ).red() * ws;
                                    g	+= originImage.pixelColor( 2*w-i-filterx,j + filtery ).green() * ws;
                                    b	+= originImage.pixelColor( 2*w-i-filterx,j + filtery ).blue() * ws;
                                } else if ( ( (i + filterx) <= w) && ( (i + filterx) >= 0) && ( (j + filtery) < 0) )   { // top-border
                                    r	+= originImage.pixelColor( i + filterx,-j - filtery ).red() * ws;
                                    g	+= originImage.pixelColor( i + filterx,-j - filtery ).green() * ws;
                                    b	+= originImage.pixelColor( i + filterx,-j - filtery ).blue() * ws;
                                } else if ( ( (i + filterx) <= w) && ( (i + filterx) >= 0) && ( (j + filtery) > h) )     { // bottom-border
                                    r	+= originImage.pixelColor( i + filterx,2*h-j - filtery ).red() * ws;
                                    g	+= originImage.pixelColor( i + filterx,2*h-j - filtery ).green() * ws;
                                    b	+= originImage.pixelColor( i + filterx,2*h-j - filtery ).blue() * ws;
                                }
                             }
                        }
                        image->setPixel( i, j, qRgb( (int) r, (int) g, (int) b ) );
                 }
             }
            updateImageDisplay();
        }
    } else {
        alert();
    }
}


void ImageViewer::button_gauss()
{
    if ( image != NULL )
    {
//        int x_filter_size = slider_size_x_filter->value();
//        int y_filter_size = slider_size_y_filter->value();
//        cout << x_filter_size << endl;
//        cout << y_filter_size << endl;


        double sum=0.0;
        int height =originGrayImage.height();
        int width = originGrayImage.width();
        vector<vector<double>> kernel(height, vector<double>(width));
        double sigma = spinbox_filter_gauss->value();
        cout << sigma << endl;
        for (int i=0 ; i< height ; i++) {
            for (int j=0 ; j< width ; j++) {
                kernel[i][j] = exp(-(i*i+j*j)/(2*sigma*sigma))/(2*M_PI*sigma*sigma);
                sum += kernel[i][j];
            }
        }

        for (int i=0 ; i<height ; i++) {
            for (int j=0 ; j<width ; j++) {
                kernel[i][j] /= sum;
            }
        }

        for (int i=0 ; i<height ; i++) {
            for (int j=0 ; j<width ; j++) {
                cout << "i: " << i << " j: " << j << " :" << kernel[i][j] << endl;
            }
        }

    } else {
        alert();
    }
}


void ImageViewer::setSlider1Value( int value )
{
    slider1Value = value;
    std::cout << slider1Value << std::endl;
    if ( image != NULL )
    {
        int	w	= image->width();
        int	h	= image->height();
        button2->click();
        button3->click();
        /*
         * set it back to origin* status
         * bresenham(0,w,0,h,1);
         */


        for ( int i = 0; i < w; i++ )
        {
            for ( int j = 0; j < h; j++ )
            {
                if ( ( ( ( (i * 1.0) / (j * 1.0) ) - (w * 1.0) / (h * 1.0) ) < 0.1) && ( ( ( (i * 1.0) / (j * 1.0) ) - (w * 1.0) / (h * 1.0) ) > -0.1) )
                {
                    image->setPixel( i, j, originImage.pixel( i, j ) );
                    image->setPixel( (w - i), j, originImage.pixel( (w - i), j ) );
                }

/*
 *             if((((w-i)*1.0)/(j*1.0)) == ((w*1.0)/(h*1.0))){
 *                 QRgb colorful_pixel = originImage.pixel(i,j);
 *                 int gray = qGray(colorful_pixel);
 *                 QRgb gray_pixel = qRgb(gray,gray,gray);
 *                 image->setPixel((w-i),j,gray_pixel);
 *             }
 */
            }
        }

        /* draw red cross */
/*
 *     float i=w/2;
 *     float j=h/2;
 *     while(i<((float)(w/2)*(1+(slider1Value*1.0)/100))) {
 *         while(j<(float)(h/2)*(1+(slider1Value*1.0)/100)){
 *             if(((((i*1.0)/(j*1.0)) - (w*1.0)/(h*1.0)) < 0.001) && ((((i*1.0)/(j*1.0)) - (w*1.0)/(h*1.0)) > -0.001)){
 *                    int it=(int)i;
 *                    int jt=(int)j;
 *                    image->setPixel(it,jt,qRgb(255,0,0));
 *                    image->setPixel(it,(h-jt),qRgb(255,0,0));
 *                    image->setPixel((w-it),jt,qRgb(255,0,0));
 *                    image->setPixel((w-it),(h-jt),qRgb(255,0,0));
 *               }
 *                j=j+1;
 *           }
 *           i=i+1;
 *           j=h/2;
 *        }
 */

        float	sv100	= (slider1Value * 1.0) / 100;
        float	w2	= w / 2;
        float	h2	= h / 2;
        bresenham( (1 - sv100) * w2, (sv100 + 1) * w2, (1 - sv100) * h2, (sv100 + 1) * h2, 0 );
        bresenham( (1 - sv100) * w2, (sv100 + 1) * w2, (h - (1 - sv100) * h2), (h - (sv100 + 1) * h2), 0 );
        updateImageDisplay();
        /*
         * logFile << "example algorithm applied " << std::endl;
         * renewLogging();
         */
    } else {
        alert();
    }
}


void ImageViewer::setSliderFilterYSizeValue( int value )
{
    if ( image != NULL )
    {
        if ( value % 2 == 1 )
        {
            label_size_filter_y->setText( QString::number( value ) );
            m_coefficients->setRowCount( value );
        } else {
            label_size_filter_y->setText( QString( "Not an odd number" ) );
        }
    } else {
        alert();
    }
}


void ImageViewer::setSliderFilterXSizeValue( int value )
{
    if ( image != NULL )
    {
        if ( value % 2 == 1 )
        {
            label_size_filter_x->setText( QString::number( value ) );
            m_coefficients->setColumnCount( value );
        } else {
            label_size_filter_x->setText( QString( "Not an odd number" ) );
        }
    } else {
        alert();
    }
}


string ImageViewer::Int_to_String( int n )
{
    ostringstream stream;
    stream << n;
    return(stream.str() );
}


void ImageViewer::initDataTab2()
{
    if ( image != NULL )
    {
        QImage i2;
        avh	= 0;
        vari	= 0;
        i2	= image->convertToFormat( QImage::Format_Grayscale8 );
        for ( int i = 0; i < i2.height(); i++ )
        {
            for ( int j = 0; j < i2.width(); j++ )
            {
                avh = avh + qGray( i2.pixel( i, j ) );
            }
        }
        avh = (avh * 1.0) / (i2.width() * i2.height() );
        float f = (float) (avh);
        for ( int i = 0; i < i2.height(); i++ )
        {
            for ( int j = 0; j < i2.width(); j++ )
            {
                vari = vari + pow( qGray( i2.pixel( i, j ) ) - f, 2 );
            }
        }
        vari = vari / (i2.width() * i2.height() );
        QString mater = "";
        mater.append( "<p><b>Mittlere Helligkeit</b> = " );
        mater.append( QString::number( avh ) );
        mater.append( "</p>\n<b>Varianz</b> = " );
        mater.append( QString::number( vari ) );
        HV->setText( mater );
    } else {
        alert();
    }
}


/****************************************************************************************
 *
 *  mit dieser Methode können sie sich pro Aufgabe ein  Tab anlegen, in der die Ein-
 *  stellungen per Slider, Button, Checkbox etc. gemacht werden und die zu implemen-
 *  tierenden Algorithmen gestatet werden.
 *
 *****************************************************************************************/

void ImageViewer::changeDynamik( int value )
{
    spinbox1Value = value;
    cout << spinbox1Value << endl;
}


void ImageViewer::changeSpinbox2( int value )
{
    spinbox2Value = value;
    cout << spinbox2Value << endl;
}


void ImageViewer::changeSpinbox3( int value )
{
    spinbox3Value = value;
    cout << spinbox3Value << endl;
}


void ImageViewer::histogram()
{
    if ( image != NULL )
    {
        int		w	= image->width();
        int		h	= image->height();
        QVector<double> vecX;
        QVector<double> vecY( 256, 0 );
        int		i = 0;
        while ( 256 != i )
        {
            vecX.append( i );
            ++i;
        }

        for ( int i = 0; i < w; i++ )
        {
            for ( int j = 0; j < h; j++ )
            {
                int nIndex = int(image->bits()[i * w + j]);
                vecY[nIndex] = vecY.at( nIndex ) + 1;
            }
        }
        double yMax = 0;
        for ( int j = 0; j < 255; j++ )
        {
            if ( yMax < vecY.at( j ) )
            {
                yMax = vecY.at( j );
            }
        }
        m_histogram->yAxis->setRange( 0, yMax );
        m_histogram->graph( 0 )->setPen( QPen( QColor( 178, 34, 34 ) ) );
        m_histogram->graph( 0 )->setData( vecX, vecY );
        m_histogram->replot();
    } else {
        alert();
    }
}


void ImageViewer::dynamic( int dit )
{
    if ( image != NULL )
    {
        int	w	= image->width();
        int	h	= image->height();
        for ( int i = 0; i < w; i++ )
        {
            for ( int j = 0; j < h; j++ )
            {
                QColor pixel_rgb = originGrayImage.pixelColor( i, j );
                pixel_rgb.setBlue( pixel_rgb.blue() >> dit << dit );
                pixel_rgb.setRed( pixel_rgb.red() >> dit << dit );
                pixel_rgb.setGreen( pixel_rgb.green() >> dit << dit );
                image->setPixel( i, j, pixel_rgb.rgb() );
            }
        }
        updateImageDisplay();
    } else {
        alert();
    }
}



void ImageViewer::alert()
{
    QMessageBox::information( this, QString::fromLocal8Bit( "Error" ), QString::fromLocal8Bit( "Please select a image first!" ) );
}


void ImageViewer::confirmDynamik()
{
    if ( image != NULL )
    {
        dynamic( 8 - spinbox1Value );
        button3->click();
        button2->click();
    } else {
        alert();
    }
}


void ImageViewer::automaticContrast()
{
    if ( image != NULL )
    {
/*
 *      if(slider2Value>=slider3Value){
 *        alert();
 *      } else {
 */
        int	aMin	= spinbox2Value;
        int	aMax	= spinbox3Value;
        int	w	= image->width();
        int	h	= image->height();
        button2->click();
        button3->click();
        double		aHigh	= 0;
        double		aLow	= 255;
        QVector<double> vecY( 256, 0 );
        for ( int i = 0; i < w; i++ )
        {
            for ( int j = 0; j < h; j++ )
            {
                int t = int(originImage.bits()[i * w + j]);
                if ( t > aHigh )
                {
                    aHigh = t;
                }
                if ( t < aLow )
                {
                    aLow = t;
                }
            }
        }

        int		red, green, blue;
        QImage		copie	= originGrayImage.copy();
        unsigned int	*data	= (unsigned int *) copie.bits();
        unsigned int	*data2	= (unsigned int *) originImage.bits();
        unsigned int	*img	= (unsigned int *) image->bits();
        int		pixels	= image->width() * image->height();
        for ( int k = 0; k < pixels; k++ )
        {
            int	fa	= 0;
            int	a	= qRed( data[k] );
            if ( a < aMin )
            {
                fa = aMin;
            } else {
                if ( a > aMax )
                {
                    fa = aMax;
                } else {
                    fa = aMin + (a - aLow) * ( (aMax - aMin) / (aHigh - aLow) );
                }
            }
            int bri = fa - a;
            red	= qRed( data2[k] ) + bri;
            red	= (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green	= qGreen( data2[k] ) + bri;
            green	= (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue	= qBlue( data2[k] ) + bri;
            blue	= (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
            img[k]	= qRgba( red, green, blue, qAlpha( data2[k] ) );
        }
/*      } */

        updateImageDisplay();
    } else {
        alert();
    }
}

void ImageViewer::button_edge() { // sobel algo
    if(image!=NULL) {
        int sobel_x[3][3] = {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1}
        };
        int sobel_y[3][3] = {
            { 1,   2,   1},
            { 0,   0,   0},
            {-1,  -2,  -1}
        };
        QImage img = originGrayImage;

        int width = img.width();
        int height = img.height();

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if(y == 0 || x== 0 || y == height - 1 || x == width - 1) { // zero - padding
                    image->setPixel(x,y,0);
                } else {
                    int i1 = (QColor(img.pixel(x-1,y-1)).red() + QColor(img.pixel(x-1,y-1)).blue() + QColor(img.pixel(x-1,y-1)).green())/3;
                    int i2 = (QColor(img.pixel(x,y-1)).red() + QColor(img.pixel(x,y-1)).blue() + QColor(img.pixel(x,y-1)).green())/3;
                    int i3 = (QColor(img.pixel(x+1,y-1)).red() + QColor(img.pixel(x+1,y-1)).green() + QColor(img.pixel(x+1,y-1)).blue())/3;
                    int i4 = (QColor(img.pixel(x-1,y)).blue() + QColor(img.pixel(x-1,y)).green() + QColor(img.pixel(x-1,y)).red())/3;
                    int i5 = (QColor(img.pixel(x,y)).green() + QColor(img.pixel(x,y)).red() + QColor(img.pixel(x,y)).blue())/3;
                    int i6 = (QColor(img.pixel(x+1,y)).blue() + QColor(img.pixel(x+1,y)).red() + QColor(img.pixel(x+1,y)).green())/3;
                    int i7 = (QColor(img.pixel(x-1,y+1)).green() + QColor(img.pixel(x-1,y+1)).red() + QColor(img.pixel(x-1,y+1)).blue())/3;
                    int i8 = (QColor(img.pixel(x,y+1)).blue() + QColor(img.pixel(x,y+1)).green() + QColor(img.pixel(x,y+1)).red())/3;
                    int i9 = (QColor(img.pixel(x+1,y+1)).red() + QColor(img.pixel(x+1,y+1)).green() + QColor(img.pixel(x+1,y+1)).blue())/3;

                    int sumX = 0;
                    int sumY = 0;

                    int newMatrix[3][3] = {
                        {i1,i2,i3},
                        {i4,i5,i6},
                        {i7,i8,i9}
                    };

                    for(int v = 0; v < 3; v++) {
                        for(int w = 0; w < 3; w++) {
                            sumX += newMatrix[v][w] * sobel_x[v][w];
                            sumY += newMatrix[v][w] * sobel_y[v][w];
                        }
                    }

                    int newValue = sqrt(pow(sumX, 2) + pow(sumY, 2));
    //                cout << newValue << endl;
                    if(newValue < 0){
                        newValue = 0;
                    }
                    if(newValue > 255){
                        newValue = 255;
                    }

                    QColor imgColor = QColor(image->pixel(x,y));

                    imgColor.setRed(newValue);
                    imgColor.setBlue(newValue);
                    imgColor.setGreen(newValue);

                    image->setPixel(x,y,imgColor.rgb());
                }
            }
        }
        updateImageDisplay();
    } else {
        alert();
    }
}


void ImageViewer::generateControlPanels()
{
    auto lineA = new QFrame;
    lineA->setFrameShape( QFrame::HLine );
    lineA->setFrameShadow( QFrame::Sunken );

    auto lineB = new QFrame;
    lineB->setFrameShape( QFrame::HLine );
    lineB->setFrameShadow( QFrame::Sunken );

    auto lineC = new QFrame;
    lineC->setFrameShape( QFrame::HLine );
    lineC->setFrameShadow( QFrame::Sunken );


    auto lineD = new QFrame;
    lineD->setFrameShape( QFrame::HLine );
    lineD->setFrameShadow( QFrame::Sunken );

    auto lineF = new QFrame;
    lineF->setFrameShape( QFrame::HLine );
    lineF->setFrameShadow( QFrame::Sunken );

    auto lineE = new QFrame;
    lineE->setFrameShape( QFrame::HLine );
    lineE->setFrameShadow( QFrame::Sunken );

    auto lineG = new QFrame;
    lineG->setFrameShape( QFrame::HLine );
    lineG->setFrameShadow( QFrame::Sunken );

    QFont ft;
    ft.setPointSize( 10 );
    QFont big_ft;
    big_ft.setPointSize( 14 );
    /* uebung1 */

    m_option_panel1		= new QWidget();
    m_option_layout1	= new QVBoxLayout();
    m_option_panel1->setLayout( m_option_layout1 );


    button1 = new QPushButton();
    button1->setText( "Apply algorithm" );

    slider1 = new QSlider( Qt::Horizontal );
    slider1->setMinimum( 0 );
    slider1->setMaximum( 100 );
    slider1->setValue( 10 );

    QObject::connect( button1, SIGNAL( clicked() ), this, SLOT( applyExampleAlgorithm() ) );
    QObject::connect( slider1, SIGNAL( valueChanged( int ) ), this, SLOT( setSlider1Value( int ) ) );

    m_option_layout1->addWidget( button1 );
    m_option_layout1->addWidget( slider1 );
    tabWidget->addTab( m_option_panel1, "Aufgabe 1" );


    /* uebung2 */
    m_option_panel2		= new QWidget();
    m_option_layout2	= new QVBoxLayout();
    m_change_dynamik	= new QHBoxLayout();
    QLabel* label_dynamic = new QLabel( "Dynamik: " );
    label_dynamic->setFont( ft );
    m_change_dynamik->addWidget( label_dynamic );
    spinbox1 = new QSpinBox( tabWidget );
    spinbox1->setMaximum( 8 );
    spinbox1->setMinimum( 1 );
    spinbox1->setValue( 8 );
    spinbox1->setMaximumWidth( 200 );
    QPushButton *confirm_dynamik = new QPushButton( "Change Dynamic" );
    QObject::connect( confirm_dynamik, SIGNAL( clicked() ), this, SLOT( confirmDynamik() ) );


    QObject::connect( spinbox1, SIGNAL( valueChanged( int ) ), this, SLOT( changeDynamik( int ) ) );
    m_change_dynamik->addWidget( spinbox1 );
    m_change_dynamik->addWidget( confirm_dynamik );


    m_option_panel2->setLayout( m_option_layout2 );

    button2 = new QPushButton();
    button2->setText( "Get Mittleren Helligkeit and Varianz" );
    HV = new QLabel( "Click the <b><u>button</u></b> to get <b>Mittleren Helligkeit</b> and <b>Varianz</b>" );

    HV->setFont( ft );
    QHBoxLayout* h_mv = new QHBoxLayout();
    QObject::connect( button2, SIGNAL( clicked() ), this, SLOT( initDataTab2() ) );
    h_mv->addWidget( HV );
    h_mv->addWidget( button2 );


    m_option_layout2->addLayout( h_mv );

    m_option_layout2->addWidget( lineA );
    m_option_layout2->addLayout( m_change_dynamik );
    m_option_layout2->addWidget( lineB );
    QHBoxLayout	* h_his			= new QHBoxLayout();
    QLabel		* label_histogram	= new QLabel( "Histogram:" );
    label_histogram->setFont( ft );
    h_his->addWidget( label_histogram );

    button3 = new QPushButton();
    button3->setText( "Calculate Histogram" );
    QObject::connect( button3, SIGNAL( clicked() ), this, SLOT( histogram() ) );
    h_his->addWidget( button3 );
    m_option_layout2->addLayout( h_his );
/*    m_option_layout2->addWidget(); */
    m_histogram = new QCustomPlot();
    m_histogram->setMaximumHeight( 300 );
    m_histogram->xAxis->setLabel( tr( "Grayscale" ) );
    m_histogram->yAxis->setLabel( tr( "Number" ) );
    m_histogram->addGraph();
    QCPTextElement *title = new QCPTextElement( m_histogram, tr( "Grayscale histogram" ) );

    m_histogram->plotLayout()->insertRow( 0 );
    m_histogram->plotLayout()->addElement( 0, 0, title );
    m_histogram->xAxis->setRange( -1, 255 );
    m_histogram->graph( 0 )->setLineStyle( QCPGraph::lsImpulse );
/*    m_histogram->resize(1,1); */
    m_option_layout2->addWidget( m_histogram );
    m_option_layout2->addWidget( lineC );

    QHBoxLayout	* h_brightness		= new QHBoxLayout();
    QLabel		* label_brightness	= new QLabel( "Brightness: " );
    label_brightness->setFont( ft );
    slider2 = new QSlider( Qt::Horizontal );
    slider2->setMinimum( -1 );
    slider2->setMaximum( 256 );
    slider2->setValue( slider2Value );
    slider2->setFixedWidth( 600 );
    label_brightness_value = new QLabel( QString::number( slider2Value ) );
    QObject::connect( slider2, SIGNAL( valueChanged( int ) ), this, SLOT( setSlider2Value( int ) ) );
    h_brightness->addWidget( label_brightness );
    h_brightness->addWidget( label_brightness_value );
    h_brightness->addWidget( slider2 );

    QHBoxLayout	* h_contrast		= new QHBoxLayout();
    QLabel		* label_contrast	= new QLabel( "Contrast: " );
    label_contrast->setFont( ft );
    slider3 = new QSlider( Qt::Horizontal );
    slider3->setMinimum( -1 );
    slider3->setMaximum( 256 );
    slider3->setValue( slider3Value );
    slider3->setFixedWidth( 600 );
    label_contrast_value = new QLabel( QString::number( slider3Value ) );
    QObject::connect( slider3, SIGNAL( valueChanged( int ) ), this, SLOT( setSlider3Value( int ) ) );
    h_contrast->addWidget( label_contrast );
    h_contrast->addWidget( label_contrast_value );
    h_contrast->addWidget( slider3 );

    m_option_layout2->addLayout( h_brightness );
    m_option_layout2->addLayout( h_contrast );
    m_option_layout2->addWidget( lineD );

    QHBoxLayout	* h_low		= new QHBoxLayout();
    QHBoxLayout	* h_high	= new QHBoxLayout();
    QLabel		* low		= new QLabel( "a<sub>low</sub>" );
    low->setFont( big_ft );

    QLabel* high = new QLabel( "a<sub>high</sub>" );
    high->setFont( big_ft );

    spinbox2 = new QSpinBox( tabWidget );
    spinbox2->setMaximum( 255 );
    spinbox2->setMinimum( 0 );
    spinbox2->setValue( 0 );
    spinbox2->setMaximumWidth( 200 );
    h_low->addWidget( low );
    h_low->addWidget( spinbox2 );

    spinbox3 = new QSpinBox( tabWidget );
    spinbox3->setMaximum( 255 );
    spinbox3->setMinimum( 0 );
    spinbox3->setValue( 255 );
    spinbox3->setMaximumWidth( 200 );
    h_high->addWidget( high );
    h_high->addWidget( spinbox3 );

    QPushButton* automatic_contrast_adjust = new QPushButton( "Automatic Contrast Adjust" );
    QObject::connect( spinbox2, SIGNAL( valueChanged( int ) ), this, SLOT( changeSpinbox2( int ) ) );
    QObject::connect( spinbox3, SIGNAL( valueChanged( int ) ), this, SLOT( changeSpinbox3( int ) ) );
    QObject::connect( automatic_contrast_adjust, SIGNAL( clicked() ), this, SLOT( automaticContrast() ) );
    m_option_layout2->addLayout( h_low );
    m_option_layout2->addLayout( h_high );
    m_option_layout2->addWidget( automatic_contrast_adjust );
/*    m_option_layout2->addWidget(button3); */


    tabWidget->addTab( m_option_panel2, "Aufgabe 2 - 3" );

    m_option_panel3		= new QWidget();
    m_option_layout3	= new QVBoxLayout();

    QVBoxLayout	* v_x_f_size	= new QVBoxLayout();
    QVBoxLayout	* v_y_f_size	= new QVBoxLayout();

    QHBoxLayout	* h_x_f_size	= new QHBoxLayout();
    QLabel		* label_x_size	= new QLabel( "X-Filter Size:" );
    label_x_size->setFont( ft );
    h_x_f_size->addWidget( label_x_size );
    label_size_filter_x = new QLabel( QString::number( x_filter_size ) );
    label_size_filter_x->setFont( ft );
    h_x_f_size->addWidget( label_size_filter_x );

    QHBoxLayout	* h_y_f_size	= new QHBoxLayout();
    QLabel		* label_y_size	= new QLabel( "Y-Filter Size:" );
    label_y_size->setFont( ft );
    h_y_f_size->addWidget( label_y_size );
    label_size_filter_y = new QLabel( QString::number( y_filter_size ) );
    label_size_filter_y->setFont( ft );
    h_y_f_size->addWidget( label_size_filter_y );

    slider_size_x_filter = new QSlider( Qt::Horizontal );
    slider_size_x_filter->setValue( 1 );

    slider_size_x_filter->setTickInterval( 2 );
    slider_size_x_filter->setRange( 1, 9 );
    slider_size_x_filter->setTickPosition( QSlider::TicksBothSides );
    QObject::connect( slider_size_x_filter, SIGNAL( valueChanged( int ) ), this, SLOT( setSliderFilterXSizeValue( int ) ) );

    slider_size_y_filter = new QSlider( Qt::Horizontal );
    slider_size_y_filter->setValue( 1 );
    slider_size_y_filter->setTickInterval( 2 );
    slider_size_y_filter->setRange( 1, 9 );
    slider_size_y_filter->setTickPosition( QSlider::TicksBothSides );
    QObject::connect( slider_size_y_filter, SIGNAL( valueChanged( int ) ), this, SLOT( setSliderFilterYSizeValue( int ) ) );

    v_x_f_size->addLayout( h_x_f_size );
    v_x_f_size->addWidget( slider_size_x_filter );

    v_y_f_size->addLayout( h_y_f_size );
    v_y_f_size->addWidget( slider_size_y_filter );

    m_option_layout3->addLayout( v_x_f_size );
    m_option_layout3->addLayout( v_y_f_size );

    QVBoxLayout* v_c = new QVBoxLayout();

    m_coefficients = new QTableWidget( 1, 1, this );
/*    m_coefficients->horizontalHeader()->setStretchLastSection(true); */

    m_option_layout3->addWidget( m_coefficients );
    m_option_panel3->setLayout( m_option_layout3 );
    QLabel* label_coefficient = new QLabel( "Coefficients:" );
    label_coefficient->setFont( ft );
    v_c->addWidget( label_coefficient );
    v_c->addWidget( m_coefficients );
    button_filter_without_border = new QPushButton( "Filter Without Border" );
    v_c->addWidget( button_filter_without_border );
    m_option_layout3->addLayout( v_c );
    m_option_layout3->addWidget( lineF );
    QObject::connect( button_filter_without_border, SIGNAL( clicked() ), this, SLOT( button_without_border() ) );
    button_filter_zero_padding	= new QPushButton( "Filter Zero-Padding" );
    QObject::connect( button_filter_zero_padding, SIGNAL( clicked() ), this, SLOT( button_zero_padding() ) );
    button_filter_constant_border	= new QPushButton( "Filter Constant Border" );
    QObject::connect(  button_filter_constant_border, SIGNAL( clicked() ), this, SLOT( button_constant_border() ) );
    button_filter_mirror_border	= new QPushButton( "Filter Mirror Border" );
    QObject::connect(  button_filter_mirror_border, SIGNAL( clicked() ), this, SLOT( button_mirror_border() ) );
    button_filter_gauss		= new QPushButton( "Filter 2D-Gauss" );
    QObject::connect(  button_filter_gauss, SIGNAL( clicked() ), this, SLOT( button_gauss()));
    button_filter_edge = new QPushButton("Filter Edge");
    QObject::connect(  button_filter_edge, SIGNAL( clicked() ), this, SLOT( button_edge()));

    m_option_layout3->addWidget( button_filter_zero_padding );
    m_option_layout3->addWidget( button_filter_constant_border );
    m_option_layout3->addWidget( button_filter_mirror_border );
    QHBoxLayout	* h_filter_gauss	= new QHBoxLayout();
    QLabel		* label_filter_gauss	= new QLabel( "σ for 2D-Gauss Filter:" );
    spinbox_filter_gauss = new QDoubleSpinBox();
    spinbox_filter_gauss->setValue( spinboxDoubleValue );
    h_filter_gauss->addWidget( label_filter_gauss );
    h_filter_gauss->addWidget( spinbox_filter_gauss );

    m_option_layout3->addWidget( lineE );
    m_option_layout3->addLayout( h_filter_gauss );
    m_option_layout3->addWidget( button_filter_gauss );
    m_option_layout3->addWidget(lineG);
    m_option_layout3->addWidget(button_filter_edge);

    tabWidget->addTab( m_option_panel3, "Aufgabe 4 - 5" );

    m_option_panel4		= new QWidget();
    m_option_layout4	= new QVBoxLayout();
//    QLabel* label_coefficient_2 = new QLabel( "Coefficients:" );
//    label_coefficient_2->setFont( ft );
//    m_option_layout4->addWidget(label_coefficient_2);
//    m_option_layout4->addWidget(m_coefficients_2);


    m_option_panel4->setLayout(m_option_layout4);



    tabWidget->addTab( m_option_panel4, "Aufgabe 6" );

    tabWidget->show();
    /* Hinweis: Es bietet sich an pro Aufgabe jeweils einen solchen Tab zu erstellen */
}


/****************************************************************************************
 *
 *   ab hier kommen technische Details, die nicht notwenig für das Verständnis und die
 *   Bearbeitung sind.
 *
 *
 *****************************************************************************************/


void ImageViewer::startLogging()
{
    /* LogFile */
    logFile.open( "log.txt", std::ios::out );
    logFile << "Logging: \n" << std::endl;
}


void ImageViewer::renewLogging()
{
    QFile	file( "log.txt" );              /* Create a file handle for the file named */
    QString line;
    file.open( QIODevice::ReadOnly );       /* Open the file */

    QTextStream stream( &file );            /* Set the stream to read from myFile */
    logBrowser->clear();
    while ( !stream.atEnd() )
    {
        line = stream.readLine();       /* this reads a line (QString) from the file */
        logBrowser->append( line );
    }
}


void ImageViewer::resizeEvent( QResizeEvent * event )
{
    QMainWindow::resizeEvent( event );
    centralwidget->setMinimumWidth( width() );
    centralwidget->setMinimumHeight( height() );
    centralwidget->setMaximumWidth( width() );
    centralwidget->setMaximumHeight( height() );
    logBrowser->setMinimumWidth( width() - 40 );
    logBrowser->setMaximumWidth( width() - 40 );
}


void ImageViewer::updateImageDisplay()
{
    imageLabel->setPixmap( QPixmap::fromImage( *image ) );
}


void ImageViewer::generateMainGui()
{
    /* Tab widget */
    tabWidget = new QTabWidget( this );
    tabWidget->setObjectName( QStringLiteral( "tabWidget" ) );


    /* Center widget */
    centralwidget = new QWidget( this );
    centralwidget->setObjectName( QStringLiteral( "centralwidget" ) );
    /*
     * centralwidget->setFixedSize(200,200);
     * setCentralWidget(centralwidget);
     */

    imageLabel = new QLabel;
    imageLabel->setBackgroundRole( QPalette::Base );
    imageLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    imageLabel->setScaledContents( true );


    /* Center widget */
    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole( QPalette::Dark );
    scrollArea->setWidget( imageLabel );


    setCentralWidget( scrollArea );

    /* HBox layout */
    QGridLayout* gLayout = new QGridLayout( centralwidget );
    gLayout->setObjectName( QStringLiteral( "hboxLayout" ) );
    gLayout->addWidget( new QLabel(), 1, 1 );
    gLayout->setVerticalSpacing( 50 );
    gLayout->addWidget( tabWidget, 2, 1 );
    gLayout->addWidget( scrollArea, 2, 2 );

    logBrowser = new QTextEdit( this );
    logBrowser->setMinimumHeight( 100 );
    logBrowser->setMaximumHeight( 200 );
    logBrowser->setMinimumWidth( width() );
    logBrowser->setMaximumWidth( width() );
    gLayout->addWidget( logBrowser, 3, 1, 1, 2 );
    gLayout->setVerticalSpacing( 50 );
}


bool ImageViewer::loadFile( const QString &fileName )
{
    if ( image != NULL )
    {
        delete image;
        image = NULL;
    }

    image		= new QImage( fileName );
    originImage	= image->copy();
    originGrayImage = originImage.copy();
    for ( int i = 0; i < originGrayImage.width(); i++ )
    {
        for ( int j = 0; j < originGrayImage.height(); j++ )
        {
            QRgb gray = qGray( originGrayImage.pixel( i, j ) );
            originGrayImage.setPixel( i, j, qRgb( gray, gray, gray ) );
        }
    }
    QImage grayImage = image->convertToFormat( QImage::Format_Grayscale8 );
    for ( int i = 0; i < grayImage.height(); i++ )
    {
        for ( int j = 0; j < grayImage.width(); j++ )
        {
            avh = avh + qGray( grayImage.pixel( i, j ) );
        }
    }
    avh = (avh * 1.0) / (grayImage.width() * grayImage.height() );
    setSlider2Value( avh );


    if ( image->isNull() )
    {
        QMessageBox::information( this, QGuiApplication::applicationDisplayName(),
                      tr( "Cannot load %1." ).arg( QDir::toNativeSeparators( fileName ) ) );
        setWindowFilePath( QString() );
        imageLabel->setPixmap( QPixmap() );
        imageLabel->adjustSize();
        return(false);
    }

    scaleFactor = 1.0;


    updateImageDisplay();

    printAct->setEnabled( true );
    fitToWindowAct->setEnabled( true );
    updateActions();

    if ( !fitToWindowAct->isChecked() )
        imageLabel->adjustSize();

    setWindowFilePath( fileName );
    logFile << "geladen: " << fileName.toStdString().c_str() << std::endl;
    renewLogging();
    return(true);
}


void ImageViewer::open()
{
    QStringList mimeTypeFilters;
    foreach( const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes() )
    mimeTypeFilters.append( mimeTypeName );
    mimeTypeFilters.sort();
    const QStringList	picturesLocations = QStandardPaths::standardLocations( QStandardPaths::PicturesLocation );
    QFileDialog		dialog( this, tr( "Open File" ),
                    picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.first() );
    dialog.setAcceptMode( QFileDialog::AcceptOpen );
    dialog.setMimeTypeFilters( mimeTypeFilters );
    dialog.selectMimeTypeFilter( "image/jpeg" );

    while ( dialog.exec() == QDialog::Accepted && !loadFile( dialog.selectedFiles().first() ) )
    {
    }
}


void ImageViewer::print()
{
    Q_ASSERT( imageLabel->pixmap() );
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
    QPrintDialog dialog( &printer, this );
    if ( dialog.exec() )
    {
        QPainter	painter( &printer );
        QRect		rect	= painter.viewport();
        QSize		size	= imageLabel->pixmap()->size();
        size.scale( rect.size(), Qt::KeepAspectRatio );
        painter.setViewport( rect.x(), rect.y(), size.width(), size.height() );
        painter.setWindow( imageLabel->pixmap()->rect() );
        painter.drawPixmap( 0, 0, *imageLabel->pixmap() );
    }
#endif
}


void ImageViewer::zoomIn()
{
    scaleImage( 1.25 );
}


void ImageViewer::zoomOut()
{
    scaleImage( 0.8 );
}


void ImageViewer::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}


void ImageViewer::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable( fitToWindow );
    if ( !fitToWindow )
    {
        normalSize();
    }
    updateActions();
}


void ImageViewer::about()
{
    QMessageBox::about( this, tr( "About Image Viewer" ),
                tr( "<p>The <b>Image Viewer</b> example shows how to combine QLabel "
                "and QScrollArea to display an image. QLabel is typically used "
                "for displaying a text, but it can also display an image. "
                "QScrollArea provides a scrolling view around another widget. "
                "If the child widget exceeds the size of the frame, QScrollArea "
                "automatically provides scroll bars. </p><p>The example "
                "demonstrates how QLabel's ability to scale its contents "
                "(QLabel::scaledContents), and QScrollArea's ability to "
                "automatically resize its contents "
                "(QScrollArea::widgetResizable), can be used to implement "
                "zooming and scaling features. </p><p>In addition the example "
                "shows how to use QPainter to print an image.</p>" ) );
}


void ImageViewer::createActions()
{
    openAct = new QAction( tr( "&Open..." ), this );
    openAct->setShortcut( tr( "Ctrl+O" ) );
    connect( openAct, SIGNAL( triggered() ), this, SLOT( open() ) );

    printAct = new QAction( tr( "&Print..." ), this );
    printAct->setShortcut( tr( "Ctrl+P" ) );
    printAct->setEnabled( false );
    connect( printAct, SIGNAL( triggered() ), this, SLOT( print() ) );

    exitAct = new QAction( tr( "E&xit" ), this );
    exitAct->setShortcut( tr( "Ctrl+Q" ) );
    connect( exitAct, SIGNAL( triggered() ), this, SLOT( close() ) );

    zoomInAct = new QAction( tr( "Zoom &In (25%)" ), this );
    zoomInAct->setShortcut( tr( "Ctrl++" ) );
    zoomInAct->setEnabled( false );
    connect( zoomInAct, SIGNAL( triggered() ), this, SLOT( zoomIn() ) );

    zoomOutAct = new QAction( tr( "Zoom &Out (25%)" ), this );
    zoomOutAct->setShortcut( tr( "Ctrl+-" ) );
    zoomOutAct->setEnabled( false );
    connect( zoomOutAct, SIGNAL( triggered() ), this, SLOT( zoomOut() ) );

    normalSizeAct = new QAction( tr( "&Normal Size" ), this );
    normalSizeAct->setShortcut( tr( "Ctrl+S" ) );
    normalSizeAct->setEnabled( false );
    connect( normalSizeAct, SIGNAL( triggered() ), this, SLOT( normalSize() ) );

    fitToWindowAct = new QAction( tr( "&Fit to Window" ), this );
    fitToWindowAct->setEnabled( false );
    fitToWindowAct->setCheckable( true );
    fitToWindowAct->setShortcut( tr( "Ctrl+F" ) );
    connect( fitToWindowAct, SIGNAL( triggered() ), this, SLOT( fitToWindow() ) );

    aboutAct = new QAction( tr( "&About" ), this );
    connect( aboutAct, SIGNAL( triggered() ), this, SLOT( about() ) );

    aboutQtAct = new QAction( tr( "About &Qt" ), this );
    connect( aboutQtAct, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );
}


void ImageViewer::createMenus()
{
    fileMenu = new QMenu( tr( "&File" ), this );
    fileMenu->addAction( openAct );
    fileMenu->addAction( printAct );
    fileMenu->addSeparator();
    fileMenu->addAction( exitAct );

    viewMenu = new QMenu( tr( "&View" ), this );
    viewMenu->addAction( zoomInAct );
    viewMenu->addAction( zoomOutAct );
    viewMenu->addAction( normalSizeAct );
    viewMenu->addSeparator();
    viewMenu->addAction( fitToWindowAct );

    helpMenu = new QMenu( tr( "&Help" ), this );
    helpMenu->addAction( aboutAct );
    helpMenu->addAction( aboutQtAct );

    menuBar()->addMenu( fileMenu );
    menuBar()->addMenu( viewMenu );
    menuBar()->addMenu( helpMenu );
}


void ImageViewer::updateActions()
{
    zoomInAct->setEnabled( !fitToWindowAct->isChecked() );
    zoomOutAct->setEnabled( !fitToWindowAct->isChecked() );
    normalSizeAct->setEnabled( !fitToWindowAct->isChecked() );
}


void ImageViewer::scaleImage( double factor )
{
    Q_ASSERT( imageLabel->pixmap() );
    scaleFactor *= factor;
    imageLabel->resize( scaleFactor * imageLabel->pixmap()->size() );

    adjustScrollBar( scrollArea->horizontalScrollBar(), factor );
    adjustScrollBar( scrollArea->verticalScrollBar(), factor );

    zoomInAct->setEnabled( scaleFactor < 10.0 );
    zoomOutAct->setEnabled( scaleFactor > 0.05 );
}


void ImageViewer::adjustScrollBar( QScrollBar *scrollBar, double factor )
{
    scrollBar->setValue( int(factor * scrollBar->value()
                 + ( (factor - 1) * scrollBar->pageStep() / 2) ) );
}


