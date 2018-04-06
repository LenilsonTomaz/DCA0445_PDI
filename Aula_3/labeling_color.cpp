#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){

  Mat image, mask;
  int width, height;
  // Variaveis para contagem de buracos e objetos
  // "nobjects" tambem e usada para diferenciar cada objeto pelas componetes BGR
  unsigned long int nobjects=0; 
  unsigned long nburacos=0;

  image = imread(argv[1],CV_LOAD_IMAGE_COLOR);
  
  if(!image.data){
    std::cout << "Imagem nao carregou corretamente!\n";
    return(-1);
  }

  width=image.size().width;
  height=image.size().height;
  Vec3b white (255,255,255);
  Vec3b black (0,0,0);
  // Vetor de auxilio na contagem
  Vec3b label;

  // Eliminar obejtos nas bordas 
  image.row(0) = white;
  image.row(height-1) = white;  
  image.col(width-1) = white;
  image.col(0) = white;
  floodFill(image,CvPoint(0,0),black);
  // Pintar o fundo com BGR=253,255,25 para diferneciar dos buracos
  floodFill(image,CvPoint(0,0),Vec3b(253,255,255));

  // loop para testar cada pixel
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      

      if(image.at<Vec3b>(i,j) == white){
      // achou um objeto
              nobjects++;
              // Separa cada bloco de byte do contador para uma componente
              label[0] = (nobjects & 0x000000FF);
              label[1] = (nobjects & 0x0000FF00) >> 8;
              label[2] = (nobjects & 0x00FF0000) >> 16;
        floodFill(image,CvPoint(j,i),label);
      }


      if((image.at<Vec3b>(i,j) == black)){
      // achou um buraco
              nburacos++;
        // Pinta com BGR = 254,255,255 para nao contar o buraco duas vezes
        floodFill(image,CvPoint(j,i),Vec3b(254,255,255));    
      }
         
     }
   }

  cout << "Existem " << nobjects << " objetos, sendo " << nburacos 
  << " com buracos e "
  << nobjects - nburacos << " sem." << endl; 


  imshow("image", image);
  imwrite("image_labeled.png", image);
  waitKey();
  return 0;  
}
