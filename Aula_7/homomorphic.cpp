#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>

using namespace cv;
using namespace std;

// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image){
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

int main(int argc, char** argv){
  
  string path = "/home/dan/opencv-3.4.1/scratch/images/";
  path.append(argv[1]);

  Mat imaginaryInput, complexImage;
  Mat padded, filter;
  Mat image, tmp, image2;
  Mat_<float> realInput, zeros;
  vector<Mat> planos;

  image = imread(path,CV_LOAD_IMAGE_GRAYSCALE);

  namedWindow("original",WINDOW_KEEPRATIO);
  imshow("original", image);

  if(!image.data){
    std::cout << "imagem nao carregou corretamente\n";
    return(-1);
  }

  float gamaL = 0.25;
  float gamaH = 1;
  float D0 = 80;
  float c = 1;
  float D = 0;

  // valores ideais dos tamanhos da imagem
  // para calculo da DFT
  int dft_M, dft_N;

  // identifica os tamanhos otimos para
  // calculo do FFT
  dft_M = getOptimalDFTSize(image.rows);
  dft_N = getOptimalDFTSize(image.cols);

  // realiza o padding da imagem
  copyMakeBorder(image, padded, 0,
                 dft_M - image.rows, 0,
                 dft_N - image.cols,
                 BORDER_CONSTANT, Scalar::all(0));

  // prepara a matriz complexa para ser preenchida
  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

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

  // combina o array de matrizes em uma unica
  // componente complexa
  merge(planos, complexImage);

  // a função de transferência (filtro frequencial) deve ter o
  // mesmo tamanho e tipo da matriz complexa
  filter = complexImage.clone();

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
  dft(complexImage, complexImage);

  // realiza a troca de quadrantes
  deslocaDFT(complexImage);

  // aplica o filtro frequencial
  mulSpectrums(complexImage,filter,complexImage,0);

  // troca novamente os quadrantes
  deslocaDFT(complexImage);

  // calcula a DFT inversa
  idft(complexImage, complexImage);

  // limpa o array de planos
  planos.clear();

  // separa as partes real e imaginaria da
  // imagem filtrada
  split(complexImage, planos);
  normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
  exp(planos[0],planos[0]);
  normalize(planos[0], planos[0], 0, 1, CV_MINMAX);


    // normaliza a parte real para exibicao
    namedWindow("filtrada",WINDOW_KEEPRATIO);
    imshow("filtrada", planos[0]);
    imwrite("homomorphic.png", planos[0]);
    waitKey();

  return 0;
}
