#include <string>
#include "CImg.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "SATree.h"
using namespace cimg_library;
using namespace std;
#include "haar.hpp"
#include "haar.cpp"
//template <typename T,unsigned S>
//inline unsigned arraysize(const T (&v)[S]) { return S; }

struct imageindex
{
	string myroute;
	double* importantPoints;
	int size;
	//int lenght[];
};
vector<imageindex> Index;
Dis dd(imageindex a,imageindex b )
{
	Dis sum = 0;
	for(int i=0;i<a.size;i++)
	{
		sum += pow(b.importantPoints[i]-a.importantPoints[i],2);
	}
	cout<<sqrt(sum);
	return sqrt(sum);
}
//SATree<vector<float>>::FunDis distance = dd;
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
	
	imageindex* recover = new imageindex[165];
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
		//cout<<direccion<<endl;
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
		//double myimage[(rows+1)*(cols+1)];
		double* myimage = new double[(rows+1)*(cols+1)];

		idwt(rows,cols,myimage,image);	
		recover[aux].myroute = direccion;
		cout<<recover[aux].myroute<<endl;
		recover[aux].importantPoints = myimage;
		recover[aux].size= (rows+1)*(cols+1);
		Index.push_back(recover[aux]);
		cout<<"ey"<<endl;	
		
		
		/*while(!main_disp.is_closed())
		{
			main_disp.wait();
		}*/
		
		delete [] myimage;
		numero++;
		aux++;
	}
	delete [] recover;
	return;
}

void comparation()
{
	///ACA ESTA EL ERROR SUPONGO QUE SERA LA FUNCION DISTANCIA DD
	SATree<imageindex>::FunDis distance = dd;
	SATree<imageindex> tree(dd, Index);
	/*string direccionnueva="/home/eduardob/Desktop/Aed_Final/Face_Similarity/yalefaces/yalefaces/a1.pgm";
	CImg<unsigned char> image(direccionnueva.c_str());
	CImgDisplay main_disp(image,direccionnueva.c_str());
	int height, width; 
	height = image.height();
	width = image.width();
	int rows =(int) height;
	int cols =(int) width;
	double* mynewimage = new double[(rows+1)*(cols+1)];
	idwt(rows,cols,mynewimage,image);
	imageindex comparatebitch;
	comparatebitch.myroute=direccionnueva;
	comparatebitch.size = (rows+1)*(cols+1);
	comparatebitch.importantPoints = mynewimage;
	auto res2 = tree.NNSearch(comparatebitch,10);
	cout<<res2.size();*/
}
int main()
{
	indexar();
	cout<<Index.size();
	comparation();
	return 0;
}
