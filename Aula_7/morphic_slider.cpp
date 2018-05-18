#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace cv;
using namespace std;

float gamaL, gamaH, D0, D=0, c=1;

int gamaL_slider = 25;
int gamaL_slider_max = 100;

int gamaH_slider = 1;
int gamaH_slider_max = 100;

int D0_slider = 80;
int D0_slider_max = 100;

char TrackbarName[50];


void deslocaDFT(Mat& image ){
  Mat tmp, A, B, C, D;

  // se a imagem tiver tamanho impar, recorta a regiao para
  // evitar cópias de tamanho desigual
  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols/2;
  int cy = image.rows/2;

  // reorganiza os quadrantes da transformada
  // A B   ->  D C
  // C D       B A
  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));

  // A <-> D
  A.copyTo(tmp);  D.copyTo(A);  tmp.copyTo(D);

  // C <-> B
  C.copyTo(tmp);  B.copyTo(C);  tmp.copyTo(B);
}


void on_trackbar_gamaL(int, void*){
  gamaL = (float) gamaL_slider/100;
}

void on_trackbar_gamaH(int, void*){
  gamaH = (float)  gamaH_slider/20;
  gamaH = gamaH +1;
}

void on_trackbar_D0(int, void*){
  D0 = (float) D0_slider;
}

int main(int argvc, char** argv){


  Mat imaginaryInput, complexImage, complexImage2;
  Mat padded;
  Mat image;
  Mat_<float> realInput, zeros;

  int dft_M, dft_N;
  Mat filter, tmp;
  vector<Mat> planos;

  char key;

  namedWindow("homomorphic",WINDOW_KEEPRATIO);
 
  sprintf(TrackbarName,"Gama L (0.01 - 1)");
  createTrackbar( TrackbarName, "homomorphic",
          &gamaL_slider,
          gamaL_slider_max,
          on_trackbar_gamaL );

  on_trackbar_gamaL(gamaL_slider, 0);
  
  sprintf(TrackbarName, "Gama H (1 - 6)");
  createTrackbar( TrackbarName, "homomorphic",
          &gamaH_slider,
          gamaH_slider_max,
          on_trackbar_gamaH);

  on_trackbar_gamaH(gamaH_slider, 0 );

  sprintf(TrackbarName, "D0 (1 - 100)");
  createTrackbar(TrackbarName, "homomorphic",
          &D0_slider,
          D0_slider_max,
          on_trackbar_D0);

  on_trackbar_D0(D0_slider, 0 );
 
  string path = "/home/dan/opencv-3.4.1/scratch/images/";
  path.append(argv[1]);

  image= imread(path,CV_LOAD_IMAGE_GRAYSCALE);

    if(!image.data){
      std::cout << "imagem nao carregou corretamente\n";
      return(-1);
    }

    dft_M = getOptimalDFTSize(image.rows);
    dft_N = getOptimalDFTSize(image.cols);

    // realiza o padding da imagem
    copyMakeBorder(image, padded, 0,
                   dft_M - image.rows, 0,
                   dft_N - image.cols,
                   BORDER_CONSTANT, Scalar::all(0));

    // prepara a matriz complexa para ser preenchida
    complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

    // componente real da matriz
    realInput = Mat_<float>(padded);

    for(int i=0; i<dft_M; i++){
      for(int j=0; j<dft_N; j++){

        realInput.at<float>(i,j) = log(realInput.at<float>(i,j)+1);

      }
    }

    // parte imaginaria da matriz complexa (preenchida com zeros)
    zeros = Mat_<float>::zeros(padded.size());
    
    planos.push_back(realInput);
    planos.push_back(zeros);

    // combinar componentes da imagem
    merge(planos, complexImage);

    while(1){

    planos.clear();
    complexImage2 = complexImage.clone();

    // preparar filtro
    filter = complexImage2.clone();

    // cria uma matriz temporária para criar as componentes real
    // e imaginaria do filtro ideal
    tmp = Mat(dft_M, dft_N, CV_32F);

    // prepara o filtro homomorfico
    for(int i=0; i<dft_M; i++){
      for(int j=0; j<dft_N; j++){

        D = sqrt(((i - dft_M/2)*(i - dft_M/2))+((j - dft_N/2)*(j - dft_N/2)));

        tmp.at<float>(i,j) = (gamaH - gamaL)*(1 - exp(-1*c*((D*D)/(D0*D0)))) + gamaL;

      }
    }

    // cria a matriz com as componentes do filtro e junta
    // ambas em uma matriz multicanal complexa
    Mat comps[]= {tmp, tmp};
    merge(comps, 2, filter);

    // calcula o dft
    dft(complexImage2, complexImage2);

    // realiza a troca de quadrantes
    deslocaDFT(complexImage2);

    // aplica o filtro frequencial
    mulSpectrums(complexImage2,filter,complexImage2,0);

    // troca novamente os quadrantes
    deslocaDFT(complexImage2);

    // calcula a DFT inversa
    idft(complexImage2, complexImage2);

    // limpa o array de planos
    planos.clear();

    // separa as partes real e imaginaria da
    // imagem filtrada
    split(complexImage2, planos);

    // normaliza para poder calcular a exponenical
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    exp(planos[0],planos[0]);
    // normaliza para a exibicao da imagem
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);

    imshow("homomorphic", planos[0]);

    key = (char) waitKey(10);

    if( key == 27 ){
    	imwrite("homomorphic.png",planos[0]);
    	break; // esc pressed!
    } 

  }

  return 0;
}
