#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  // Objetos Mat para os frames normais e normalziados e seus histogramas
  Mat new_frame, old_frame, new_histogram, old_histogram;
  VideoCapture cap;
  
  // Configuracoes do histograma
  int nbins = 64;   // Numero de elementos da abscissa
  float range[] = {0, 256}; // Range do eixo y para o calculo do histograma
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;
  double corr, threshold = 0.996;

  //threshold = 0.992 : menos sensivel, melhro para pegar 
  //so obetos entrando na cena

  // Inicia a captura
  cap.open(0);
  
  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }


  while(1){

    cap >> new_frame;
    cvtColor(new_frame,new_frame,COLOR_BGR2GRAY);

    // aguarda 10 ms para pegar proximo frame
    usleep(50000);
    cap >> old_frame;
    cvtColor(old_frame,old_frame,COLOR_BGR2GRAY);
    
    // Calcula os histogramas
    calcHist(&new_frame, 1, 0, Mat(), new_histogram, 1,
             &nbins, &histrange,
             uniform, acummulate);

    calcHist(&old_frame, 1, 0, Mat(), old_histogram, 1,
             &nbins, &histrange,
             uniform, acummulate);

    //compara os histogramas usando uma medida de correlacao
    corr = compareHist(new_histogram,old_histogram,HISTCMP_CORREL);
    //Compara a correlacao entre os frames com o limiar
    if(corr <= threshold){
      cout << "WARNING!!! " << endl;
    } else {
      cout << "..." << endl;
    }
  
    imshow("original", old_frame);

    if(waitKey(30) >= 0) break;
  }
  return 0;
}