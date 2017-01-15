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
    string post;
    string pre;
    pre = "/home/ms/AED/Face_Similarity/yalefaces/yalefaces/";	
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
		post = letrastring + numstr +".gif";
		string direccion = pre + post;
		cout<<direccion<<endl;
		CImg<unsigned char> image(direccion.c_str());
		int height, width;
		height = image.height();
		width = image.width();
		int rows =(int) height;
		int cols =(int) width;
		double myimage[rows*cols];

		idwt(rows,cols,myimage,image);

		/*
		CImgDisplay main_disp(image,direccion.c_str());
		while(!main_disp.is_closed())
		{
			main_disp.wait();
		}
		*/

		numero++;
		aux++;
	}
}

int main()
{
	indexar();
	return 0;
}
