#include <math.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

double dist(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

double lerp(double value, double leftMin, double leftMax, double rightMin, double rightMax) {
	//if (leftMin == leftMax) {
		
	//} else {
		return rightMin + (rightMax - rightMin) * (value - leftMin) / (leftMax - leftMin);
	//}
}

class Point {
	public:
		Point(double x, double y) {
			_x = x;
			_y = y;
		}
		
		static double distance(Point* point1, Point* point2) {
			return dist(point1->x(), point1->y(), point2->x(), point2->y());
		}
		
		double x() {
			return _x;
		}
		
		double y() {
			return _y;
		}
	private:
		double _x;
		double _y;
};

class Fractal {
	public:
		Fractal(vector<Point*> pattern) {
			_pattern = pattern;
		}
	
		Fractal* cast(Point* start2, Point* end2) {
			vector<Point*> pattern;
		
			Point* start1 = _pattern[0];
			Point* end1 = _pattern[_pattern.size() - 1];
	
			// Calculate angles between start and end points			
			double angle1 = atan2(start1->y() - end1->y(), start1->x() - end1->x());
			double angle2 = atan2(start2->y() - end2->y(), start2->x() - end2->x());

			double angle = angle2 - angle1;
		
			// Compute first unit vector
			double ix = cos(angle);
			double iy = sin(angle);
			
			// Normalize first unit vector
			double imag = sqrt(pow(ix, 2) + pow(iy, 2));
			ix /= imag;
			iy /= imag;
			
			// Copy to second unit vector
			double jx = iy;
			double jy = ix;
		
			for (int i = 0; i < _pattern.size(); i++) {
				double scale = 1/3;
				// Compute resulting point
				double x = _pattern[i]->x() + scale * (_pattern[i]->x() * ix + _pattern[i]->y() * iy);
				double y = _pattern[i]->y() + scale * (_pattern[i]->y() * ix + _pattern[i]->x() * iy);
				
				//double x = _pattern[i]->x() * ix + _pattern[i]->y() * jx;
				//double y = _pattern[i]->x() * iy + _pattern[i]->y() * jy;

				Point* point = new Point(x, y);

				pattern.push_back(point);
			}
		
			return new Fractal(pattern);
		}
	
		Point* sample(double t, int depth) {
			if (depth <= 0) {
				Point start = *_pattern[0];
				Point end = *_pattern[_pattern.size() - 1];
				double x = lerp(t, 0, 1, start.x(), end.x());
				double y = lerp(t, 0, 1, start.y(), end.y());
				
				return new Point(x, y);
			} else {
				double totalTraversal = t * length();
				double traversal = 0;
				
				int i;

				for (i = 0; i < _pattern.size() - 1; i++) {
					if (traversal > totalTraversal) {
						break;
					} else if (traversal == totalTraversal) {
						// If the value falls on a point, return it
						return _pattern[i];
					}
		
					traversal += Point::distance(_pattern[i], _pattern[i + 1]);
				}
				
				Fractal* subFractal = cast(_pattern[i - 1], _pattern[i]);
				
				double subT = lerp(traversal - totalTraversal, 0, Point::distance(_pattern[i - 1], _pattern[i]), 0, 1);

				return subFractal->sample(subT, depth - 1);
			}
		}
	private:
		vector<Point*> _pattern;
	
		double length() {
			double length = 0;
		
			for (int i = 0; i < _pattern.size() - 1; i++) {
				length += Point::distance(_pattern[i], _pattern[i + 1]);
			}
			
			return length;
		}
};

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
	vector<Point*> pattern;
	
	pattern.push_back(new Point(0, 0));
	pattern.push_back(new Point(1, 0));
	pattern.push_back(new Point(1, 1));
	pattern.push_back(new Point(2, 1));
	pattern.push_back(new Point(2, 0));
	pattern.push_back(new Point(3, 0));
	
	Fractal* fractal = new Fractal(pattern);

	for (double i = 0; i < 1; i += 0.1) {
		Point* point = fractal->sample(i, 1);
		cout << i << ": " << point->x() << ", " << point->y() << endl;
	}
	render("output.ppm", 256);

	system("eog output.ppm");

	return 0;
}
