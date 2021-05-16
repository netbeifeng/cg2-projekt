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

#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#endif

#include "fstream"
#include "qcustomplot.h"
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
class QTextEdit;
class QVBoxLayout;
class QHBoxLayout;
class QTabWidget;
class QPushButton;
class QSpinBox;
class QSlider;
class QTableWidget;

class ImageViewer : public QMainWindow
{
    Q_OBJECT

 private:

     // Beispiel für GUI Elemente
     QWidget*	m_option_panel1;
     QHBoxLayout* m_change_dynamik;
     QVBoxLayout*    m_option_layout1;

     QWidget*	m_option_panel2;
     QVBoxLayout* m_option_layout2;

     QWidget* m_option_panel3;
     QVBoxLayout* m_option_layout3;

     QWidget* m_option_panel4;
     QVBoxLayout* m_option_layout4;

     QWidget* m_option_panel5;
     QVBoxLayout* m_option_layout5;

     QPushButton* button1;
     QPushButton* button2;
     QPushButton* button3;

     QPushButton* button_filter_without_border;
     QPushButton* button_filter_zero_padding;
     QPushButton* button_filter_constant_border;
     QPushButton* button_filter_mirror_border;
     QPushButton* button_filter_gauss;

     QPushButton* button_filter_edge;

     QPushButton* button_canny_edge;
     QPushButton* button_usm_dete;

     QPushButton* button_hough_tran;

     QSpinBox* spinbox1;
     QSpinBox* spinbox2;
     QSpinBox* spinbox3;
     QSpinBox* spinbox_sch_grad;
     QSpinBox* spinbox_mind_wert;
     QDoubleSpinBox* spinbox_filter_gauss;
     QDoubleSpinBox* spinbox_ce_sigma;
     QDoubleSpinBox* spinbox_usm_sigma;

     QSlider* slider1;
     QSlider* slider2;
     QSlider* slider3;

     QSlider* slider_size_x_filter;
     QSlider* slider_size_y_filter;

     QSlider* slider_size_xce_filter;
     QSlider* slider_size_yce_filter;

     QSlider* slider_theta;
     QSlider* slider_rho;
     QSlider* slider_ts;

     QLabel* label_contrast_value;
     QLabel* label_brightness_value;
     // hier können weitere GUI Objekte hin wie Buttons Slider etc. 

     QCustomPlot *m_histogram;

     QTableWidget* m_coefficients;
     QTableWidget* m_coefficients_2;
 private slots:

     // Beispiel für einen Algorithmus
     void applyExampleAlgorithm();
     void setSlider1Value(int);
     void setSlider2Value(int);
     void setSlider3Value(int);
     void initDataTab2();
     void automaticContrast();
     void changeDynamik(int);
     void changeSpinbox2(int);
     void changeSpinbox3(int);
     void confirmDynamik();
     void histogram();

     void setSliderFilterYSizeValue(int);
     void setSliderFilterXSizeValue(int);

     void setSliderCEYSizeValue(int);
     void setSliderCEXSizeValue(int);

     void setSliderTheta(int);
     void setSliderRho(int);
     void setSliderTS(int);
     // hier können weitere als SLOTS definierte Funktionen hin, die auf Knopfdruck etc. aufgerufen werden.


     void open();
     void print();
     void zoomIn();
     void zoomOut();
     void normalSize();
     void fitToWindow();
     void about();
     void button_without_border();
     void button_zero_padding();
     void button_constant_border();
     void button_mirror_border();
     void button_gauss();
     void button_edge();
     void button_canny();
     void button_usm();

     void button_hough();



public:
     ImageViewer();
     bool loadFile(const QString &);
     void updateImageDisplay();
     void readAndParCoTable();
     void TraceAndThreshold(QVector<int> &Gbin, QVector<double> Gnms, int x, int y, int lowThreshold);
protected:

    void resizeEvent(QResizeEvent * event);

 private:

    // in diesen Beiden Methoden sind Änderungen nötig bzw. sie dienen als
    // Vorlage für eigene Methoden.
    void generateControlPanels();
    // definiert von Bozhen*
    void plot(int x,int y);
    void plotO(int x,int y);
    void bresenham(int x0,int x1,int y0,int y1, int config); // config=0:red cross; config=1: origin
    std::string Int_to_String(int n);
    void alert();
    void dynamic(int);

    // Ab hier technische Details die nicht für das Verständnis notwendig sind.
    void startLogging();
    void generateMainGui(); 
    
    void createActions();
    void createMenus();
    void updateActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    void renewLogging();

    QTabWidget* tabWidget;
    QTextEdit* logBrowser;
    QWidget* centralwidget;
    QLabel* imageLabel;
    QLabel* HV;

    QLabel* label_size_filter_x;
    QLabel* label_size_filter_y;

    QLabel* label_size_ce_x;
    QLabel* label_size_ce_y;

    QLabel* label_theta_t;
    QLabel* label_rho_r;
    QLabel* label_ts_t;

    QScrollArea* scrollArea;
    double scaleFactor;
    QImage* image;

    std::fstream logFile;

#ifndef QT_NO_PRINTER
    QPrinter printer;
#endif

    QAction *openAct;
    QAction *printAct;
    QAction *exitAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
};

#endif
