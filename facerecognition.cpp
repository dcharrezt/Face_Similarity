#include <string>
#include "CImg.h"
#include <iostream>

using namespace cimg_library;
using namespace std;
#include "haar.hpp"
#include "haar.cpp"

double* idwt(int rows,int cols,double myimage[], CImg<unsigned char> image)
{
	double matriz[cols][rows];
	double R,G,B;
	for(int i = 0;i<cols;i++)
	{	
   	 for(int j=0;j<rows;j++)
   	 {
    	    R=image(i,j,0);
        	G=image(i,j,1);
        	B=image(i,j,2);
        	matriz[i][j]=(0.299*R + 0.587*G + 0.114*B);

    	}
	}
	
	for(int i = 0;i<cols;i++)
	{
		for(int j = 0; j<rows; j++)
		{
			myimage[i*cols + j] = matriz[i][j];
			//cout<<myimage[i*cols + j];
		}
	}

	haar_2d(cols,rows,myimage);
	
	/*for(int i = 0;i<cols;i++)
	{
		for(int j = 0; j<rows; j++)
		{
			cout<<myimage[i*cols + j];
		}
	}*/
	//cout<<"aca estoy"<<endl;
	return myimage;
}

void indexar()
{
	int aux = 0;
	int numero = 1;
	char letra = 'a';	
	while(aux<165)
	{
		if(numero == 12)
		{
			numero = 1;
			letra++;
		}
		

		
		cout<<numero<<letra<<endl;
		numero++;
		aux++;
	}
}

int main()
{
	indexar();
	return 0;
}
