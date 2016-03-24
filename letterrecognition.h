#ifndef LETTERRECOGNITION_H
#define LETTERRECOGNITION_H

#include <QMainWindow>


#include "neuralNetwork.h"
#include "neuralNetworkTrainer.h"
#include "dataReader.h"

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
QT_END_NAMESPACE

class LetterRecognition : public QMainWindow
{
    Q_OBJECT
    
public:
    LetterRecognition();
    ~LetterRecognition();


private slots:
    void train();
    void analize();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void about();

private:
    bool poTreningu;
    bool poWczytaniu;

    void createActions();
    void createMenus();
    void updateActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    QLabel *imageLabel;
    QScrollArea *scrollArea;
    double scaleFactor;

    QAction *analizeAct;
    QAction *trainAct;
    QAction *exitAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
    QAction *aboutAct;

    QMenu *plikMenu;
    QMenu *siecMenu;
    QMenu *obrazMenu;
};

#endif // LETTERRECOGNITION_H
