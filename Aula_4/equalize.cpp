#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  // Objetos Mat para os frames normais e normalziados e seus histogramas
  Mat image,image_equ,HistImg,HistImg_equ;
 
  VideoCapture cap;
  
  // Configuracoes do histograma
  int nbins = 64;   // Numero de elementos da abscissa
  float range[] = {0, 256}; // Range do eixo y para o calculo do histograma
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;
  
  // Inicia a captura
  cap.open(0);
  
  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }
  

  // Configuracao do tamanho do histograma que sera exibido
  int histw = nbins, histh = nbins/2;
  // Objeto Mat para a exibicao do histograma
  Mat display_h(histh, histw, CV_8UC1, Scalar(0));
  Mat display_h_equ(histh, histw, CV_8UC1, Scalar(0));
  while(1){
    cap >> image;
    cvtColor(image,image,COLOR_BGR2GRAY);
    equalizeHist(image,image_equ);  // Imagem com o histograma normalizado

    // Calcula os histogramas
    calcHist(&image, 1, 0, Mat(), HistImg, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&image_equ, 1, 0, Mat(), HistImg_equ, 1,
             &nbins, &histrange,
             uniform, acummulate);

    // Normaliza os histogramas para ficarem entre 0 e o valor maximo de tom de
    // cinza calcualdo
    normalize(HistImg, HistImg, 0, display_h.rows, NORM_MINMAX, -1, Mat());
    normalize(HistImg_equ, HistImg_equ, 0, display_h_equ.rows, NORM_MINMAX, -1, Mat());

    // "reseta" o histograma para o porximo frame
    display_h.setTo(Scalar(0));
    display_h_equ.setTo(Scalar(0));

    // desenha o histograma, coluna por coluna
    for(int i=0; i<nbins; i++){
      line(display_h,
           Point(i, histh),
           Point(i, histh-cvRound(HistImg.at<float>(i))),
           Scalar(255), 1, 8, 0);
      line(display_h_equ,
           Point(i, histh),
           Point(i, histh-cvRound(HistImg_equ.at<float>(i))),
           Scalar(255), 1, 8, 0);
    }

    // Desneha uma linha em preta em baixo do histograma, pra fazer uma borda
    //line(display_h_equ,Point(0, histh),Point(nbins-1, histh), Scalar(0), 1, 8, 0);


    // Exibe o histograma no topo da janela
    display_h.copyTo(image(Rect(0,0,nbins,histh)));
    display_h_equ.copyTo(image_equ(Rect(0,0,nbins,histh)));

    imshow("original", image);
    imshow("equalized", image_equ);
    if(waitKey(30) >= 0) break;
  }
  return 0;
}
