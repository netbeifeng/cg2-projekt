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

#include <QtWidgets>
#ifndef QT_NO_PRINTER
#include <QPrintDialog>
#endif
#include<iostream>
#include<sstream>
#include<string.h>
#include<string>
#include<cstring>
#include<QString>
#include "imageviewer-qt5.h"
#include <QtWidgets>
#include "qcustomplot.h"
#include <QVector>

using namespace std;

int spinbox1Value = 8;

int slider1Value=10;
int avh=0;
int vari=0;
QImage originImage;
QImage originGrayImage;


ImageViewer::ImageViewer()
{

	image=NULL;
	resize(1600, 600);
	
	startLogging();

	generateMainGui();
	renewLogging();

	generateControlPanels();
    	createActions();
    	createMenus();

    	resize(QGuiApplication::primaryScreen()->availableSize() * 0.85 );

}

void ImageViewer::applyExampleAlgorithm()
{
	if(image!=NULL)
	{
        int w=image->width();
        int h=image->height();
        //*image = image->convertToFormat(QImage::Format_Grayscale8);
        for(int i=0;i<w;i++) {
            for(int j=0;j<h;j++) {
                QRgb colorful_pixel = image->pixel(i,j);
                int gray = qGray(colorful_pixel);
                QRgb gray_pixel = qRgb(gray,gray,gray);
                image->setPixel(i,j,gray_pixel);
            }
        }
        originImage = image->copy();


	updateImageDisplay();
 	logFile << "example algorithm applied " << std::endl;
	renewLogging();
	}
}

void ImageViewer::bresenham(int x0,int x1,int y0,int y1,int config){

//    int deltax = x1 - x0;
//    int deltay = abs(y1 - y0);
//    int error = deltax / 2;
//    int ystep;
//    int y = y0;
//    int p=2*deltay -deltax;
//    int x = x0;
//    while(x<x1){
//        if(p>=0){
//            plot(x,y);
//            y=y+1;
//            p=p+2*deltay-2*deltax;
//        }
//        else{
//            plot(x,y);
//            p=p+2*deltay;
//        }
//        x=x+1;
//    }

    boolean steep = (abs(y1 - y0) > abs(x1 - x0));
         if (steep){
            //swap(x0,y0)
            int temp=x0;
            x0=y0;
            y0=temp;
            // swap(x1, y1)
            temp=x1;
            x1=y1;
            y1=temp;
         }
         if (x0 > x1){
             //swap(x0, x1)
             int temp=x0;
             x0=x1;
             x1=temp;
             //swap(y0, y1)
             temp=y0;
             y0=y1;
             y1=temp;
         }
         int deltax = x1 - x0;
         int deltay = abs(y1 - y0);
         int error = deltax / 2;
         int ystep;
         int y = y0;
         if (y0 < y1){ystep = 1;} else {ystep = -1;}
         for (int x=x0; x<x1; x++){
             if (steep) {if(config == 0){plot(y,x);} else {plotO(y,x);}} else {if(config==0){plot(x,y);} else {plotO(x,y);}}
             error = error - deltay;
             if (error < 0){
                 y = y + ystep;
                 error = error + deltax;
             }
         }
}


void ImageViewer::plot(int x, int y){
    image->setPixel(x,y,qRgb(255,0,0));
}

void ImageViewer::plotO(int x, int y){
    image->setPixel(x,y,originImage.pixel(x,y));
}
void ImageViewer::setSlider1Value(int value){
    slider1Value = value;
    std::cout<<slider1Value<<std::endl;
    if(image!=NULL){
     int w=image->width();
     int h=image->height();
     //set it back to origin* status
     //bresenham(0,w,0,h,1);


     for(int i=0;i<w;i++) {
         for(int j=0;j<h;j++) {
             if(((((i*1.0)/(j*1.0)) - (w*1.0)/(h*1.0)) < 0.1) && ((((i*1.0)/(j*1.0)) - (w*1.0)/(h*1.0)) > -0.1)){
                image->setPixel(i,j,originImage.pixel(i,j));
                image->setPixel((w-i),j,originImage.pixel((w-i),j));
             }

//             if((((w-i)*1.0)/(j*1.0)) == ((w*1.0)/(h*1.0))){
//                 QRgb colorful_pixel = originImage.pixel(i,j);
//                 int gray = qGray(colorful_pixel);
//                 QRgb gray_pixel = qRgb(gray,gray,gray);
//                 image->setPixel((w-i),j,gray_pixel);
//             }
         }
     }

     //draw red cross
//     float i=w/2;
//     float j=h/2;
//     while(i<((float)(w/2)*(1+(slider1Value*1.0)/100))) {
//         while(j<(float)(h/2)*(1+(slider1Value*1.0)/100)){
//             if(((((i*1.0)/(j*1.0)) - (w*1.0)/(h*1.0)) < 0.001) && ((((i*1.0)/(j*1.0)) - (w*1.0)/(h*1.0)) > -0.001)){
//                    int it=(int)i;
//                    int jt=(int)j;
//                    image->setPixel(it,jt,qRgb(255,0,0));
//                    image->setPixel(it,(h-jt),qRgb(255,0,0));
//                    image->setPixel((w-it),jt,qRgb(255,0,0));
//                    image->setPixel((w-it),(h-jt),qRgb(255,0,0));
//               }
//                j=j+1;
//           }
//           i=i+1;
//           j=h/2;
//        }

     float sv100 = (slider1Value*1.0)/100;
     float w2 = w/2;
     float h2 = h/2;
     bresenham((1-sv100)*w2,(sv100+1)*w2,(1-sv100)*h2,(sv100+1)*h2,0);
     bresenham((1-sv100)*w2,(sv100+1)*w2,(h-(1-sv100)*h2),(h-(sv100+1)*h2),0);
     updateImageDisplay();
     //logFile << "example algorithm applied " << std::endl;
     //renewLogging();
   }

}

string ImageViewer::Int_to_String(int n){
    ostringstream stream;
    stream<<n;
    return stream.str();
}
void ImageViewer::initDataTab2(){
    if(image!=NULL) {
        QImage i2;
        avh=0;
        vari=0;
        i2 = image->convertToFormat(QImage::Format_Grayscale8);
        for(int i=0;i<i2.height();i++){
            for(int j=0;j<i2.width();j++){
                avh=avh+qGray(i2.pixel(i,j));
            }
        }
        avh =(avh*1.0)/(i2.width()*i2.height());
        float f = (float)(avh);
        for(int i=0;i<i2.height();i++){
            for(int j=0;j<i2.width();j++){
                vari=vari+pow(qGray(i2.pixel(i,j))-f,2);
            }
        }
        vari = vari/ (i2.width()*i2.height());
        QString mater = "";
        mater.append("<p><b>Mittlere Helligkeit</b> = ");
        mater.append(QString::number(avh));
        mater.append("</p>\n<b>Varianz</b> = ");
        mater.append(QString::number(vari));
        HV->setText(mater);
    }
}
/**************************************************************************************** 
*   
*  mit dieser Methode können sie sich pro Aufgabe ein  Tab anlegen, in der die Ein-
*  stellungen per Slider, Button, Checkbox etc. gemacht werden und die zu implemen-
*  tierenden Algorithmen gestatet werden.
*
*****************************************************************************************/

void ImageViewer::changeDynamik(int value){
    spinbox1Value = value;
    cout<<spinbox1Value<<endl;
}

void ImageViewer::histogram(){
    if(image!=NULL){
     int w=image->width();
     int h=image->height();
     QVector<double> vecX;
     QVector<double> vecY(256,0);
     int i = 0;
     while (256 != i) {
         vecX.append(i);
         ++i;
     }

     for(int i = 0; i < w; i++) {
        for(int j = 0; j < h; j++) {
            int nIndex = int(image->bits()[i*w + j]);
            vecY[nIndex] = vecY.at(nIndex) + 1;
        }
     }
     double yMax = 0;
     for(int j = 0; j < 255 ; j++)
     {
         if(yMax < vecY.at(j)){
            yMax = vecY.at(j);
         }
     }
    m_histogram->yAxis->setRange(0,yMax);
    m_histogram->graph(0)->setPen(QPen(QColor(178,34,34)));
    m_histogram->graph(0)->setData(vecX,vecY);
    m_histogram->replot();
   }
}
void ImageViewer::dynamic(int dit) {
    if(image!=NULL){
        int w=image->width();
        int h=image->height();
        for(int i = 0; i < w; i++) {
            for(int j = 0; j < h; j++) {
                QColor pixel_rgb = originGrayImage.pixelColor(i,j);
                pixel_rgb.setBlue(pixel_rgb.blue()>>dit<<dit);
                pixel_rgb.setRed(pixel_rgb.red() >>dit<<dit);
                pixel_rgb.setGreen(pixel_rgb.green() >>dit<<dit);
                image->setPixel(i,j,pixel_rgb.rgb());
            }
        }
        updateImageDisplay();
    }
}

void ImageViewer::confirmDynamik() {
    if(image!=NULL){
        dynamic(8-spinbox1Value);
    }

}

void ImageViewer::generateControlPanels()
{
    auto lineA = new QFrame;
    lineA->setFrameShape(QFrame::HLine);
    lineA->setFrameShadow(QFrame::Sunken);

    auto lineB = new QFrame;
    lineB->setFrameShape(QFrame::HLine);
    lineB->setFrameShadow(QFrame::Sunken);
    // uebung1

    m_option_panel1 = new QWidget();
	m_option_layout1 = new QVBoxLayout();
	m_option_panel1->setLayout(m_option_layout1);      


	button1 = new QPushButton();
	button1->setText("Apply algorithm");

    slider1 = new QSlider(Qt::Horizontal);
    slider1->setMinimum(0);
    slider1->setMaximum(100);
    slider1->setValue(10);

	QObject::connect(button1, SIGNAL (clicked()), this, SLOT (applyExampleAlgorithm()));
    QObject::connect(slider1, SIGNAL (valueChanged(int)), this, SLOT (setSlider1Value(int)));

    m_option_layout1->addWidget(button1);
    m_option_layout1->addWidget(slider1);
    tabWidget->addTab(m_option_panel1,"Aufgabe 1");


    // uebung2

    m_option_panel2 = new QWidget();
	m_option_layout2 = new QVBoxLayout();
    m_change_dynamik = new QHBoxLayout();
    m_change_dynamik->addWidget(new QLabel("Dynamik: "));
    spinbox1 = new QSpinBox(tabWidget);
    spinbox1->setMaximum(8);
    spinbox1->setMinimum(1);
    spinbox1->setValue(8);
    spinbox1->setMaximumWidth(200);
    QPushButton *confirm_dynamik = new QPushButton("Change Dynamic");
    QObject::connect(confirm_dynamik, SIGNAL (clicked()), this, SLOT (confirmDynamik()));


    QObject::connect(spinbox1, SIGNAL (valueChanged(int)), this, SLOT (changeDynamik(int)));
    m_change_dynamik->addWidget(spinbox1);
    m_change_dynamik->addWidget(confirm_dynamik);


	m_option_panel2->setLayout(m_option_layout2);      

    button2 = new QPushButton();
    button2->setText("Get Mittleren Helligkeit and Varianz");
    HV = new QLabel("Click <b><u>button</u></b> to get <b>Mittleren Helligkeit</b> and <b>Varianz</b>");

    QHBoxLayout* h_mv = new QHBoxLayout();
    QObject::connect(button2, SIGNAL (clicked()), this, SLOT (initDataTab2()));
    h_mv->addWidget(HV);
    h_mv->addWidget(button2);


    m_option_layout2->addLayout(h_mv);

    m_option_layout2->addWidget(lineA);

    m_option_layout2->addLayout(m_change_dynamik);
    m_option_layout2->addWidget(lineB);
    QHBoxLayout* h_his = new QHBoxLayout();
    h_his->addWidget(new QLabel("Histogram:"));

    button3 = new QPushButton();
    button3->setText("Calculate Histogram");
    QObject::connect(button3, SIGNAL (clicked()), this, SLOT (histogram()));
    h_his->addWidget(button3);
    m_option_layout2->addLayout(h_his);
//    m_option_layout2->addWidget();
    m_histogram = new QCustomPlot();
    m_histogram->setMinimumHeight(400);
    m_histogram->xAxis->setLabel(tr("Grayscale"));
    m_histogram->yAxis->setLabel(tr("Number"));
    m_histogram->addGraph();
    QCPTextElement *title = new QCPTextElement(m_histogram,tr("Grayscale histogram"));

    m_histogram->plotLayout()->insertRow(0);
    m_histogram->plotLayout()->addElement(0,0,title);
    m_histogram->xAxis->setRange(-1,255);
    m_histogram->graph(0)->setLineStyle(QCPGraph::lsImpulse);

    m_option_layout2->addWidget(m_histogram);


//    m_option_layout2->addWidget(button3);

    tabWidget->addTab(m_option_panel2,"Aufgabe 2");
	tabWidget->show();
	// Hinweis: Es bietet sich an pro Aufgabe jeweils einen solchen Tab zu erstellen
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
	//LogFile
	logFile.open("log.txt", std::ios::out);
	logFile << "Logging: \n" << std::endl;
}

void ImageViewer::renewLogging()
{
	QFile file("log.txt"); // Create a file handle for the file named
	QString line;
	file.open(QIODevice::ReadOnly); // Open the file

	QTextStream stream( &file ); // Set the stream to read from myFile
	logBrowser->clear();
	while(!stream.atEnd()){

		line = stream.readLine(); // this reads a line (QString) from the file
		logBrowser->append(line);
	}
}


void ImageViewer::resizeEvent(QResizeEvent * event)
{
	QMainWindow::resizeEvent(event);
	centralwidget->setMinimumWidth(width());	
    	centralwidget->setMinimumHeight(height());
	centralwidget->setMaximumWidth(width());	
    	centralwidget->setMaximumHeight(height());
 	logBrowser->setMinimumWidth(width()-40);
    	logBrowser->setMaximumWidth(width()-40);
}

void ImageViewer::updateImageDisplay()
{
	imageLabel->setPixmap(QPixmap::fromImage(*image));
}


void ImageViewer::generateMainGui()
{
	/* Tab widget */
        tabWidget = new QTabWidget(this);
	tabWidget->setObjectName(QStringLiteral("tabWidget"));
	


	/* Center widget */
	centralwidget = new QWidget(this);
	centralwidget->setObjectName(QStringLiteral("centralwidget"));
	//centralwidget->setFixedSize(200,200);
	//setCentralWidget(centralwidget);

    	imageLabel = new QLabel;
 	imageLabel->setBackgroundRole(QPalette::Base);
    	imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    	imageLabel->setScaledContents(true);
	

	/* Center widget */
	scrollArea = new QScrollArea;
   	scrollArea->setBackgroundRole(QPalette::Dark);
    	scrollArea->setWidget(imageLabel);
	
	
    	setCentralWidget(scrollArea);

	/* HBox layout */
	QGridLayout* gLayout = new QGridLayout(centralwidget);
	gLayout->setObjectName(QStringLiteral("hboxLayout"));
	gLayout->addWidget(new QLabel(),1,1);
	gLayout->setVerticalSpacing(50);
	gLayout->addWidget(tabWidget,2,1);
	gLayout->addWidget(scrollArea,2,2);
	
	logBrowser= new QTextEdit(this);
	logBrowser->setMinimumHeight(100);
	logBrowser->setMaximumHeight(200);
	logBrowser->setMinimumWidth(width());
	logBrowser->setMaximumWidth(width());
	gLayout->addWidget(logBrowser,3,1,1,2);
	gLayout->setVerticalSpacing(50);
}


bool ImageViewer::loadFile(const QString &fileName)
{
    if(image!=NULL)
    {
	delete image;
	image=NULL;
    }

    image = new QImage(fileName);
    originImage = image->copy();
    originGrayImage = originImage.copy();
    for(int i = 0; i < originGrayImage.width(); i++) {
        for(int j = 0; j < originGrayImage.height(); j++) {
            QRgb gray = qGray(originGrayImage.pixel(i,j));
            originGrayImage.setPixel(i,j,qRgb(gray,gray,gray));
        }
    }

    if (image->isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
        setWindowFilePath(QString());
        imageLabel->setPixmap(QPixmap());
        imageLabel->adjustSize();
        return false;
    }
    
    scaleFactor = 1.0;

   
    updateImageDisplay();

    printAct->setEnabled(true);
    fitToWindowAct->setEnabled(true);
    updateActions();

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();

    setWindowFilePath(fileName);
    logFile << "geladen: " << fileName.toStdString().c_str()  << std::endl;
    renewLogging();
    return true;
}




void ImageViewer::open()
{
    QStringList mimeTypeFilters;
    foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QFileDialog dialog(this, tr("Open File"),
                       picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.first());
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

void ImageViewer::print()
{
    Q_ASSERT(imageLabel->pixmap());
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
    QPrintDialog dialog(&printer, this);
    if (dialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = imageLabel->pixmap()->size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(imageLabel->pixmap()->rect());
        painter.drawPixmap(0, 0, *imageLabel->pixmap());
    }
#endif
}

void ImageViewer::zoomIn()
{
    scaleImage(1.25);
}

void ImageViewer::zoomOut()
{
    scaleImage(0.8);
}

void ImageViewer::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void ImageViewer::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}

void ImageViewer::about()
{
    QMessageBox::about(this, tr("About Image Viewer"),
            tr("<p>The <b>Image Viewer</b> example shows how to combine QLabel "
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
               "shows how to use QPainter to print an image.</p>"));
}

void ImageViewer::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    printAct = new QAction(tr("&Print..."), this);
    printAct->setShortcut(tr("Ctrl+P"));
    printAct->setEnabled(false);
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    zoomInAct = new QAction(tr("Zoom &In (25%)"), this);
    zoomInAct->setShortcut(tr("Ctrl++"));
    zoomInAct->setEnabled(false);
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Zoom &Out (25%)"), this);
    zoomOutAct->setShortcut(tr("Ctrl+-"));
    zoomOutAct->setEnabled(false);
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    normalSizeAct = new QAction(tr("&Normal Size"), this);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);
    connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

    fitToWindowAct = new QAction(tr("&Fit to Window"), this);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));
    connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void ImageViewer::createMenus()
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    viewMenu = new QMenu(tr("&View"), this);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(normalSizeAct);
    viewMenu->addSeparator();
    viewMenu->addAction(fitToWindowAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(helpMenu);
}

void ImageViewer::updateActions()
{
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void ImageViewer::scaleImage(double factor)
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 10.0);
    zoomOutAct->setEnabled(scaleFactor > 0.05);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}
