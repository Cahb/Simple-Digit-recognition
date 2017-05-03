#include <SFML/Graphics.hpp>
#include <fann.h> 
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
/*
	warning! in the fann.h you have to modify the #include section:
	change 
	#include "floatfann.h"
	to
	#include "doublefann.h"
*/

const double *DEFINED_VALUES;

void calculateValueFromImage(const sf::Image &im, double *values)
{
    double count = 0;
    int numb=0;
    for(int y =0; y < im.getSize().y ;y++)
    {
        for(int x =0; x < im.getSize().x;x++)
        {
            count += (im.getPixel(x,y).toInteger()==(255)?(1):(0));
            numb += (im.getPixel(x,y).toInteger()==(255)?(1):(0));
        }
        values[y] = count * 0.1;
        if(y == 9)
        {
            values[y] = 0.15;
        }
        numb = 0;
        count = 0;
    }

}

void trainFann()
{
    const unsigned int num_input = 10;
    const unsigned int num_output = 1;
    const unsigned int num_layers = 3;
    const unsigned int num_neurons_hidden = 10;
    const double desired_error = (const float) 0.0000000178; // Warning: desired error is very low - possible long time taking net training 
    const unsigned int max_epochs = 4490000;                 // process
    const unsigned int epochs_between_reports = 1000;
    struct fann *ann = fann_create_standard(num_layers, num_input,num_neurons_hidden, num_output);
    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_LINEAR);
    fann_train_on_file(ann, "/NumberSamples/Numbers.data", max_epochs,epochs_between_reports, desired_error);
    fann_save(ann, "/NumberSamples/digitRecognition.net");
    fann_destroy(ann);
}

double useFann(const sf::Image &num)
{
    fann_type *calc_out;
    fann_type input[10] = {0};
    struct fann *ann = fann_create_from_file("/NumberSamples/digitRecognition.net");
    calculateValueFromImage(num,input);
    calc_out = fann_run(ann, input);
    double a = calc_out[0];
    fann_destroy(ann);
    return a;
}

void testOnReadyImages()
{
    double VALUES[10];
    double temp=0;
    char fName[50]={0};
    for(int i =0; i < 10; i++)
    {
        sf::Image im1;
        sprintf(fName, "%s%d%s", "/NumberSamples/", i, ".png");
        im1.loadFromFile(fName);
        VALUES[i] = useFann(im1);
    }
    DEFINED_VALUES = new double[10]{VALUES[0],VALUES[1],VALUES[2],VALUES[3],VALUES[4],VALUES[5],VALUES[6],VALUES[7],VALUES[8],VALUES[9]};

}

double useFannByValues(fann_type *input)
{
    fann_type *calc_out;
    struct fann *ann = fann_create_from_file("/NumberSamples/digitRecognition.net");
    calc_out = fann_run(ann, input);
    double a = calc_out[0];
    fann_destroy(ann);
    return a;
}

int whatValueIsPrinted(sf::Image &im)
{
    double res = useFann(im);
    if(res == DEFINED_VALUES[0]) return 0;
    else if(res == DEFINED_VALUES[1]) return 1;
    else if(res == DEFINED_VALUES[2]) return 2;
    else if(res == DEFINED_VALUES[3]) return 3;
    else if(res == DEFINED_VALUES[4]) return 4;
    else if(res == DEFINED_VALUES[5]) return 5;
    else if(res == DEFINED_VALUES[6]) return 6;
    else if(res == DEFINED_VALUES[7]) return 7;
    else if(res == DEFINED_VALUES[8]) return 8;
    else if(res == DEFINED_VALUES[9]) return 9;
	else{return -1;}
}

int main(void)
{
	short number=0;
	char filePath[25]={0};
	sf::Image Img;
	while(1)
	{
		scanf("%d", &number);
		if(number < 0 || number > 9)
		{
			continue;
		}
		sprintf(filePath, "%s%d%s", "/NumberSamples/", number, ".png");
		Img.loadFromFile(filePath);
		pritnf("\nDesired number:%d \t have: %d", number, whatValueIsPrinted(Img));
	}
	/*
	short number=0;
	char fName[50]={0};
	FILE *f1;
	sf::Image im;
	f1 = fopen("/NumberSamples/digitRecognition.net", "r");
	if(!f1)
	{
		printf("No training file found! Now training net...\n");
	}
	fclose(f1);
	trainFann();
	printf("Training ended!");
	testOnReadyImages2();
    srand(time(null));
	while(1)
    {
       Number = rand() % 10;
       sprintf(fName, "%s%d%s", "D:/NumberSamples/", Number, ".png");
       im.loadFromFile(fName);
       printf("DESIRED NMB:%d\tHAVE:%d", Number,whatValueIsPrinted(im));
       ::Sleep(200);
    }
	
	*/
	return 0;
}