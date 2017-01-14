#include <string>
#include "CImg.h"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
using namespace cimg_library;
using namespace std;
#include "haar.hpp"
#include "haar.cpp"

double* idwt(int rows,int cols,double myimage[], CImg<unsigned char> image)
{
	double matriz[rows+1][cols+1];
	double R,G,B;
	cout<<"muero"<<endl;
	for(int i = 0;i<=cols;i++)
	{	
   	 for(int j=0;j<=rows;j++)
   	 {
		
			cout<<i<<endl<<j<<endl;
    	    R=(float)image(i,j,0);
        	G=(float)image(i,j,1);
        	B=(float)image(i,j,2);
			cout<<"miau"<<endl;
        	matriz[j][i]=(0.299*R + 0.587*G + 0.114*B);
	
     }
	}
	cout<<"pase"<<endl;
	for(int i = 0;i<cols;i++)
	{
		for(int j = 0; j<rows; j++)
		{
			myimage[j*cols + i] = matriz[i][j];
			//cout<<myimage[i*cols + j];
		}
	}
	cout<<"vector hecho"<<endl;
	haar_2d(cols,rows,myimage);
	///convertirlo de nuevo a matriz para verificar(solo es para ver si esta bien)
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
	//double** myimage = new double*[165];
	char letra = 'a';
    string post;
    string pre;
    pre = "/home/eduardob/Desktop/Aed_Final/Face_Similarity/yalefaces/yalefaces/";	
	while(aux<165)
	{
		if(numero == 12)
		{
			numero = 1;
			letra++;
		}
		stringstream ss;
		string letrastring;
		ss << letra;
		ss >> letrastring;
		char numstr[21];
		sprintf(numstr, "%d", numero);
		post = letrastring + numstr +".pgm";
		string direccion = pre + post;
		cout<<direccion<<endl;
		CImg<unsigned char> image(direccion.c_str());//,visu(500,400,1,3,0);
		CImgDisplay main_disp(image,direccion.c_str());
		int height, width; 
		height = image.height();
		width = image.width();
		int rows =(int) height;
		int cols =(int) width;
		//
		//myimage[aux] = new double[rows*cols];
		//
		double myimage[(rows+1)*(cols+1)];

		idwt(rows,cols,myimage,image);	
		cout<<"ey"<<endl;	
		
		
		/*while(!main_disp.is_closed())
		{
			main_disp.wait();
		}*/
		
		//delete [] myimage;
		numero++;
		aux++;
	}
}

int main()
{
	indexar();
	return 0;
}
