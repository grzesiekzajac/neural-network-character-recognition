#include "letterrecognition.h"
#include <QApplication>


#include "neuralNetwork.h"
#include "neuralNetworkTrainer.h"
#include "dataReader.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LetterRecognition w;
    w.show();
    
    return a.exec();
}
