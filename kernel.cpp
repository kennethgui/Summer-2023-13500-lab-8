/*
  Author: kenneth Guillont

  Description:
    The program reads a PGM image from the file "inImage.pgm",
    and outputs a modified image to "outImage.pgm"
*/


#include <iostream>
#include <cassert>
#include <cstdlib>
#include <fstream>

using namespace std;

const int MAX_H = 512;
const int MAX_W = 512;

// Reads a PGM file.
// Notice that: height and width are passed by reference!
void readImage(int image[MAX_H][MAX_W], int &height, int &width) {
	char c;
	int x;
	ifstream instr;
	instr.open("inImage.pgm");

	// read the header P2
	instr >> c;
	assert(c == 'P');
	instr >> c;
	assert(c == '2');

	// skip the comments (if any)
	while ((instr>>ws).peek() == '#') {
		instr.ignore(4096, '\n');
	}

	instr >> width;
	instr >> height;

	assert(width <= MAX_W);
	assert(height <= MAX_H);
	int max;
	instr >> max;
	assert(max == 255);

	for (int row = 0; row < height; row++)
		for (int col = 0; col < width; col++)
			instr >> image[row][col];
	instr.close();
	return;
}


// Writes a PGM file
// Need to provide the array data and the image dimensions
void writeImage(int image[MAX_H][MAX_W], int height, int width) {
	ofstream ostr;
	ostr.open("outImage.pgm");
	if (ostr.fail()) {
		cout << "Unable to write file\n";
		exit(1);
	};

	// print the header
	ostr << "P2" << endl;
	// width, height
	ostr << width << ' ';
	ostr << height << endl;
	ostr << 255 << endl;

	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			assert(image[row][col] < 256);
			assert(image[row][col] >= 0);
			ostr << image[row][col] << ' ';
			ostr << endl;
		}
	}
	ostr.close();
	return;
}

int main() {
    int img[MAX_H][MAX_W];
    int h, w;

    readImage(img, h, w);

    int out[MAX_H][MAX_W];

    for (int row = 1; row < h - 1; row++) {
        for (int col = 1; col < w - 1; col++) {
            int a = img[row - 1][col - 1];
            int b = img[row - 1][col];
            int c = img[row - 1][col + 1];
            int d = img[row][col - 1];
            int e = img[row][col];
            int f = img[row][col + 1];
            int g = img[row + 1][col - 1];
            int h = img[row + 1][col];
            int i = img[row + 1][col + 1];

            int result = (g + 2 * h + i) - (a + 2 * b + c);

            result = max(0, min(result, 255));

            out[row][col] = result;
        }
    }


    for (int col = 0; col < w; col++) {
        out[0][col] = img[0][col];
        out[h - 1][col] = img[h - 1][col];
    }
    for (int row = 0; row < h; row++) {
        out[row][0] = img[row][0]; 
        out[row][w - 1] = img[row][w - 1];
    }

    writeImage(out, h, w);

    return 0;
}