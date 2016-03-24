#include <QtWidgets>
#include <fstream>
#include <ctime>
#include <iostream>

#include "letterrecognition.h"

#include "neuralNetwork.h"
#include "neuralNetworkTrainer.h"
#include "dataReader.h"

LetterRecognition::LetterRecognition()
    :poTreningu(false)
    ,poWczytaniu(false)
{
    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    setCentralWidget(scrollArea);

    createActions();
    createMenus();

    setWindowTitle(tr("Rozpoznawanie znaków"));
    resize(422, 331);

    QMessageBox::about(this, tr("Rozpoznawanie znaków"), tr("<p>Witaj w programie umożliwijącym <b>rozpoznawanie znaków przy uzyciu sieci neuronowych</b>.</p>"));

    updateActions();

    QImage image("D:\\t1.bmp");
    imageLabel->setPixmap(QPixmap::fromImage(image));
    scrollArea->setWidgetResizable(true);

}

void LetterRecognition::train()
{
    srand( (unsigned int) time(0) );

    dataReader d;
    d.loadDataFile("D:\\dane_uczace_calosc_d.csv",100,6);
    d.setCreationApproach( STATIC, 10 );

    neuralNetwork nn(100,80,6);

    neuralNetworkTrainer nT( &nn );
    nT.setTrainingParameters(0.001, 0.9, false);
    nT.setStoppingConditions(150, 90);
    nT.enableLogging("D:\\log.csv", 5);

    for (int i=0; i < d.getNumTrainingSets(); i++ )
    {
        nT.trainNetwork( d.getTrainingDataSet() );
    }

    nn.saveWeights("D:\\weights.csv");

    poTreningu = true;
    updateActions();

    QMessageBox::information(this, tr("Rozpoznawanie znaków"), tr("<b>Trening przebiegł pomyślnie.</b> Możesz teraz wczytać obraz wybierając opcję <i>Analizuj nowy obraz</i> z menu <i>Obraz</i>."));

    QImage image2("D:\\t2.bmp");
    imageLabel->setPixmap(QPixmap::fromImage(image2));
    scrollArea->setWidgetResizable(true);
}

void LetterRecognition::analize()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Wybierz plik"), QDir::currentPath());

    if (!fileName.isEmpty()) {

        resize(150, 200);

        QImage image(fileName);
        try{
            if (image.isNull()) {
                throw 1;
                //QMessageBox::information(this, tr("Rozpoznawanie pisma"), tr("Błąd. Nie można otworzyć pliku graficznego.").arg(fileName));
                return;
            }
        }
        catch (int x){
            if(x==1) QMessageBox::information(this, tr("Rozpoznawanie pisma"), tr("Błąd. Nie można otworzyć pliku graficznego.").arg(fileName));
        }
        imageLabel->setPixmap(QPixmap::fromImage(image));

        scaleFactor = 1.0;

        fitToWindowAct->setEnabled(true);
        fitToWindowAct->setChecked(true);
        updateActions();

        if (!fitToWindowAct->isChecked())
            imageLabel->adjustSize();


        scrollArea->setWidgetResizable(true);



        neuralNetwork nn(100,80,6);
        nn.loadWeights("D:\\weights.csv");


        double* nn_inputs = new double[100];
        int* nn_outputs = new int[6];


        int pixel_color;

        for(int x=0, i = 0; x< image.width(); x++)
        {
            for(int y=0; y< image.height(); y++, i++)
            {

                QColor c = QColor::fromRgb (image.pixel(x,y) );

                if (c == Qt::white) pixel_color = 0;
                else if (c == Qt::black) pixel_color = 1;

                nn_inputs[i] = pixel_color;
            }
        }


        nn_outputs = nn.feedForwardPattern(nn_inputs);

        if(nn_outputs[0] == 1 && nn_outputs[1] == 1 && nn_outputs[2] == 1 && nn_outputs[3] == 0 &&nn_outputs[4] == 0 && nn_outputs[5] == 0)
        {
            QMessageBox::information(this, tr("Rozpoznawanie znaków"), tr("Rozpoznano literę: <b>i</b>"));
        }
        else if(nn_outputs[0] == 0 && nn_outputs[1] == 0 && nn_outputs[2] == 0 && nn_outputs[3] == 1 && nn_outputs[4] == 1 && nn_outputs[5] == 1)
        {
            QMessageBox::information(this, tr("Rozpoznawanie znaków"), tr("Rozpoznano literę: <b>o</b>"));
        }
        else if(nn_outputs[0] + nn_outputs[1] + nn_outputs[2] >= nn_outputs[3] + nn_outputs[4] + nn_outputs[5])
        {
            QMessageBox::information(this, tr("Rozpoznawanie znaków"), tr("Rozpoznano literę: <b>i</b> (wysokie ryzyko pomyłki)"));
        }
        else
        {
            QMessageBox::information(this, tr("Rozpoznawanie znaków"), tr("Rozpoznano literę: <b>o</b> (wysokie ryzyko pomyłki)").arg(fileName));
        }

        delete nn_inputs;
        delete nn_outputs;

    }
    poWczytaniu = true;
    updateActions();
}

LetterRecognition::~LetterRecognition()
{

}


void LetterRecognition::zoomIn()
{
    scaleImage(1.25);
}

void LetterRecognition::zoomOut()
{
    scaleImage(0.8);
}

void LetterRecognition::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void LetterRecognition::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}

void LetterRecognition::about()
{
    QMessageBox::about(this, tr("Rozpoznawanie znaków"), tr("<p>Autor <b>Grzegorz Zając</b> 200664</p>"));
}

void LetterRecognition::createActions()
{
    analizeAct = new QAction(tr("Analizuj nowy obraz"), this);
    analizeAct->setShortcut(tr("Ctrl+A"));
    connect(analizeAct, SIGNAL(triggered()), this, SLOT(analize()));

    trainAct = new QAction(tr("Trenuj"), this);
    trainAct->setShortcut(tr("Ctrl+T"));
    connect(trainAct, SIGNAL(triggered()), this, SLOT(train()));

    exitAct = new QAction(tr("Wyjście"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    zoomInAct = new QAction(tr("Przybliż"), this);
    zoomInAct->setShortcut(tr("Ctrl++"));
    zoomInAct->setEnabled(false);
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Oddal"), this);
    zoomOutAct->setShortcut(tr("Ctrl+-"));
    zoomOutAct->setEnabled(false);
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    normalSizeAct = new QAction(tr("Przywróć oryginalny rozmiar"), this);
    normalSizeAct->setShortcut(tr("Ctrl+R"));
    normalSizeAct->setEnabled(false);
    connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

    fitToWindowAct = new QAction(tr("Dopasuj do okna"), this);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+D"));
    connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

    aboutAct = new QAction(tr("O programie..."), this);
    aboutAct->setShortcut(tr("Ctrl+I"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void LetterRecognition::createMenus()
{
    plikMenu = new QMenu(tr("Plik"), this);
    plikMenu->addAction(aboutAct);
    plikMenu->addSeparator();
    plikMenu->addAction(exitAct);

    siecMenu = new QMenu(tr("Sieć"), this);
    siecMenu->addAction(trainAct);

    obrazMenu = new QMenu(tr("Obraz"), this);
    obrazMenu->addAction(analizeAct);
    obrazMenu->addSeparator();
    obrazMenu->addAction(zoomInAct);
    obrazMenu->addAction(zoomOutAct);
    obrazMenu->addAction(normalSizeAct);
    obrazMenu->addSeparator();
    obrazMenu->addAction(fitToWindowAct);


    menuBar()->addMenu(plikMenu);
    menuBar()->addMenu(siecMenu);
    menuBar()->addMenu(obrazMenu);
}

void LetterRecognition::updateActions()
{
    if(!poTreningu)
    {
        analizeAct->setEnabled(false);
        trainAct->setEnabled(true);
        exitAct->setEnabled(true);
        zoomInAct->setEnabled(false);
        zoomOutAct->setEnabled(false);
        normalSizeAct->setEnabled(false);
        fitToWindowAct->setEnabled(false);
        aboutAct->setEnabled(true);

    }
    else
    {
        if(!poWczytaniu)
        {
            analizeAct->setEnabled(true);
            trainAct->setEnabled(false);
            exitAct->setEnabled(true);
            zoomInAct->setEnabled(false);
            zoomOutAct->setEnabled(false);
            normalSizeAct->setEnabled(false);
            fitToWindowAct->setEnabled(false);
            aboutAct->setEnabled(true);
        }
        else
        {
            analizeAct->setEnabled(true);
            trainAct->setEnabled(false);
            exitAct->setEnabled(true);;
            fitToWindowAct->setEnabled(true);
            aboutAct->setEnabled(true);
            zoomInAct->setEnabled(!fitToWindowAct->isChecked());
            zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
            normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
        }
    }

}

void LetterRecognition::scaleImage(double factor)
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 30.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void LetterRecognition::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}
