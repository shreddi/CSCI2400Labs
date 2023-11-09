#include <stdio.h>
#include "cs1300bmp.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "Filter.h"
#include <cstdint>

using namespace std;

#include "rdtsc.h"

//getsize, get,getDivisor
//
// Forward declare the functions
//
Filter * readFilter(string filename);
double applyFilter(Filter *filter, cs1300bmp *input, cs1300bmp *output);

int main(int argc, char **argv){

    if ( argc < 2) {
        fprintf(stderr,"Usage: %s filter inputfile1 inputfile2 .... \n", argv[0]);
    }

    //
    // Convert to C++ strings to simplify manipulation
    //
    string filtername = argv[1];

    //
    // remove any ".filter" in the filtername
    //
    string filterOutputName = filtername;
    string::size_type loc = filterOutputName.find(".filter");
    if (loc != string::npos) {
        //
        // Remove the ".filter" name, which should occur on all the provided filters
        //
        filterOutputName = filtername.substr(0, loc);
    }

    Filter *filter = readFilter(filtername);

    double sum = 0.0;
    int samples = 0;

    for (int inNum = 2; inNum < argc; inNum++) {
        string inputFilename = argv[inNum];
        string outputFilename = "filtered-" + filterOutputName + "-" + inputFilename;
        struct cs1300bmp *input = new struct cs1300bmp;
        struct cs1300bmp *output = new struct cs1300bmp;
        int ok = cs1300bmp_readfile( (char *) inputFilename.c_str(), input);

        if ( ok ) {
          double sample = applyFilter(filter, input, output);
          sum += sample;
          samples++;
          cs1300bmp_writefile((char *) outputFilename.c_str(), output);
        }
        
        delete input;
        delete output;
    }
    fprintf(stdout, "Average cycles per sample is %f\n", sum / samples);

}

class Filter *
readFilter(string filename)
{
  ifstream input(filename.c_str());

  if ( ! input.bad() ) {
    int size = 0;
    input >> size;
    Filter *filter = new Filter(size);
    int div;
    input >> div;
    filter -> setDivisor(div);
    for (int i=0; i < size; i++) {
      for (int j=0; j < size; j++) {
	int value;
	input >> value;
	filter -> set(i,j,value);
      }
    }
    return filter;
  } else {
    cerr << "Bad input in readFilter:" << filename << endl;
    exit(-1);
  }
}

















double
applyFilter(class Filter *filter, cs1300bmp *input, cs1300bmp *output)
{

    long long cycStart, cycStop;
    cycStart = rdtscll();
    output -> width = input -> width;
    output -> height = input -> height;
    
    //calculate divisor outside of loop. This reduces memory reads. 
    int div = filter -> divisor;

    //these are the filters that were formerly calculated in the inside of the (i, j) loop. Best to move function calls outside of loop.
    int f1 = filter -> get(0,0);
    int f2 = filter -> get(0,1);
    int f3 = filter -> get(0,2);
    int f4 = filter -> get(1,0);
    int f5 = filter -> get(1,1);
    int f6 = filter -> get(1,2);
    int f7 = filter -> get(2,0);
    int f8 = filter -> get(2,1);
    int f9 = filter -> get(2,2);
    
    //reorder. Before, it was col, row, plane, but plane,row,col is best. This is because of locality. In memory, columns are right next to eachother, then rows, then planes. If you evaluate a 3d array in this order, you are going sequentially in memory and always incrementing one forward.
    for(int plane = 0; plane < 3; plane++) {
        for(int row = 1; row < (input -> height) - 1 ; row = row + 1) {
            for(int col = 1; col < (input -> width) - 1; col = col + 1) {

                output -> color[plane][row][col] = 0;

                //same var for the output's current specified pixel. This is to reduce memory reads from constantly reading output -> color[][][].
                int temp = output -> color[plane][row][col];
                  
                //get rid of for loops, just do 9 computations manually
                temp += (input -> color[plane][row + 0 - 1][col + 0 - 1] * f1);
                temp += (input -> color[plane][row + 0 - 1][col + 1 - 1] * f2);
                temp += (input -> color[plane][row + 0 - 1][col + 2 - 1] * f3);
                temp += (input -> color[plane][row + 1 - 1][col + 0 - 1] * f4);
                temp += (input -> color[plane][row + 1 - 1][col + 1 - 1] * f5);
                temp += (input -> color[plane][row + 1 - 1][col + 2 - 1] * f6);
                temp += (input -> color[plane][row + 2 - 1][col + 0 - 1] * f7);
                temp += (input -> color[plane][row + 2 - 1][col + 1 - 1] * f8);
                temp += (input -> color[plane][row + 2 - 1][col + 2 - 1] * f9);
                    

                temp = temp / div;
                if ( temp < 0 ) {
                    temp = 0;
                }
                if ( temp > 255 ) { 
                   temp = 255;
                }      
                output -> color[plane][row][col] = temp;
            }
        }
    }
    
    cycStop = rdtscll();
    double diff = cycStop - cycStart;
    double diffPerPixel = diff / (output -> width * output -> height);
    fprintf(stderr, "Took %f cycles to process, or %f cycles per pixel\n",
    diff, diff / (output -> width * output -> height));
    return diffPerPixel;
    
}
