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
int histogramsize= 256;
struct imageindex
{
	CImg<unsigned char> histogram_haar_image;
	string myroute;
};
vector<imageindex> Index;
Dis dd(imageindex a,imageindex b )
{
	Dis sum = 0;
	for(int i=0;i<histogramsize;i++)
	{
		sum += pow(a.histogram_haar_image[i]-b.histogram_haar_image[i],2);
	}
	return sqrt(sum);
}
CImg<unsigned char> idwt(CImg<unsigned char> image)
{
	CImg<unsigned char> image1 = image.haar();
	CImg<unsigned char> image2 = image1.histogram(histogramsize);
	return image2;
}

void indexar()
{
	int aux = 0;
	int numero = 1;
	imageindex* recover = new imageindex[165];
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
		CImg<unsigned char> image(direccion.c_str());
		recover[aux].histogram_haar_image = idwt(image);
		recover[aux].myroute = direccion;
		Index.push_back(recover[aux]);
		numero++;
		aux++;
	}
	return;
}

void comparation()
{
	SATree<imageindex>::FunDis distance = dd;
	SATree<imageindex> tree(dd, Index);
	string direccionnueva="/home/eduardob/Desktop/Aed_Final/Face_Similarity/yalefaces/yalefaces/a1.pgm";
	CImg<unsigned char> image(direccionnueva.c_str());
	CImgDisplay main_disp(image,direccionnueva.c_str());
	while(!main_disp.is_closed())
		{
			main_disp.wait();
		}
	imageindex comparatebitch;
	comparatebitch.histogram_haar_image = idwt(image);
	comparatebitch.myroute = direccionnueva;
	auto resultado = tree.NNSearch(comparatebitch,10);
	for(auto it=resultado.begin();it!=resultado.end();it++)
	{
		string resultados = (*it).myroute;
		CImg<unsigned char> image(resultados.c_str());
		CImgDisplay main_disp(image,resultados.c_str());
		while(!main_disp.is_closed())
		{
			main_disp.wait();
		}
	}
}
int main()
{
	indexar();
	comparation();
	return 0;
}
