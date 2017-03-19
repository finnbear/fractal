#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

double lerp(double value, double leftMin, double leftMax, double rightMin, double rightMax) {
	return rightMin + (rightMax - rightMin) * (value - leftMin) / (leftMax - leftMin);
}

int render(char* fileName, int resolution) {
	// Declare a new output file stream
	ofstream image(fileName);
	
	// Write header to the stream
	image << "P3" << endl;
	image << resolution << " " << resolution << endl;
	image << "255" << endl;
	
	// Iterate through every pixel of the image
	for (int y = 0; y < resolution; y++) {
		for (int x = 0; x < resolution; x++) {
			// Calculate color values
			int r = x % 255;
			int g = y % 255;
			int b = y + x % 255;
			
			// Write color values to image
			image << r << " " << g << " " << b << endl;
		}
	}
	
	// Close the output file stream
	image.close();
}

int main() {
	render("output.ppm", 256);

	system("eog output.ppm");

	return 0;
}
