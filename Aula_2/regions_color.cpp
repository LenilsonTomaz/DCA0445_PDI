#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv){

  Mat image;

  int p1, p2, p3, p4, aux;
  string path = "/home/dan/opencv-3.4.1/scratch/images/";
  path.append(argv[1]);

  
  image= imread(path,IMREAD_COLOR);
	

  if(!image.data){
  	cout << "Erro ao abrir imagem.png" << endl;
  } else {

  cout << "Digite as coordenadas x e y, uma por vez, do primeiro ponto:\n";
  cin >> p1;
  cin >> p2;

  cout << "Digite as coordenadas x e y, uma por vez, do segundo ponto:\n";
  cin >> p3;
  cin >> p4;
  
  if (p1>p3){
    aux = p3;
    p3 = p1;
    p1 = aux;
  } 

  if (p2>p4){
    aux = p4;
    p4 = p2;
    p2 = aux;
  } 

    	for(int i=p1;i<=p3;i++){
     	 for(int j=p2;j<=p4;j++){
       	  image.at<Vec3b>(i,j) = Vec3b(255,255,255) - image.at<Vec3b>(i,j);
     	}
   	 }
   
	namedWindow("negativo",WINDOW_KEEPRATIO);
	imshow("negativo", image);
  imwrite("imagem_neagtivada.jpg",image);
	waitKey();

  }

return 0;
}
 

