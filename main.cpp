#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <math.h>
#include <stdlib.h>  
#include <array>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "vector2.h"
#include "triangle.h"
#include "delaunay.h"




void generate_points_normal(int rc, int d) {
	std::ofstream myOutputFile;
	myOutputFile.open("source.txt");

	for (int i = 0; i < rc; i++) {
		for (int j = 0; j < rc; j++) {
			float x = 100 + j*d;
			float y = 100 + i*d;
			myOutputFile << x << " " << y << std::endl;
		}
	}



	myOutputFile.close();
}

void generate_points_degenerate(int rc,int distance, float distortion) {
	std::ofstream myOutputFile;
	myOutputFile.open("source.txt");

	for (int i = 0; i < (rc/2); i++) {
		for (int j = 0; j < (rc/2); j++) {
			float cntr = rc / 2;
			float x = 150 + (cntr-1)*distance*(1-i)*distortion/cntr + j*distance*(1-i*distortion/cntr);
			float y = 100 + j*distance;
			myOutputFile << x << " " << y << std::endl;
			x = 100 + cntr*distance - (cntr - 1)*distance*(1 - i)*distortion / cntr - j*distance*(1-i*distortion / cntr);
			y = 100 +  j*distance;
			myOutputFile << x << " " << y << std::endl;
		}
	}
	myOutputFile.close();
}

void generate_random_points() {

	int x[75];
	int y[75];

	srand(time(NULL)); 

	for (int i = 0; i<75; i++)
	{
		bool check; 
		int n; 
		do
		{
			n = rand() % 550;
			check = true;
			for (int j = 0; j<i; j++)
				if (n == x[j])
				{
					check = false; 
					break; 
				}
		} while (!check); 
		x[i] = n;
	}

	for (int i = 0; i<75; i++)
	{
		bool check;
		int n;
		do
		{
			n = rand() % 750;
			check = true;
			for (int j = 0; j<i; j++)
				if (n == y[j])
				{
					check = false;
					break;
				}
		} while (!check);
		y[i] = n;
	}
	std::ofstream myOutputFile;
	myOutputFile.open("source.txt");
	for (int i = 0; i < 75; i++) {
		myOutputFile << x[i] << " " << y[i] << std::endl;

	}
	myOutputFile.close();
}

void menu(std::ifstream& myInputFile) {

	char choice;
	std::cout << "Choose a source method" << std::endl 
		<< "(1)Random nodes" << std::endl 
		<< "(2)Normal nodes" << std::endl 
		<< "(3)Distorted normal nodes" << std::endl 
		<< "(4)Premade .txt" << std::endl 
		<< "enter the number of the choosen method" << std::endl;
	std::cin >> choice;
	switch (choice) {
	case '1': {
		std::cout << "Generating random nodes" << std::endl;
		generate_random_points();
		myInputFile.open("source.txt");
		break;

	}

	case '2': {
		std::cout << "Generating normal nodes" << std::endl;
		generate_points_normal(12, 25);
		myInputFile.open("source.txt");
		break;
	}

	case '3': {
		std::cout << "Genearating distorted normal nodes" << std::endl;
		generate_points_degenerate(12, 25, 0.8);
		myInputFile.open("source.txt");
		break;
	}

	case '4': {
		std::cout << "Using premade.txt as a source" << std::endl;
		myInputFile.open("premade.txt");
		break;
	}

	default: {
		std::cout << "You've chosen neither";
		break;
	}
	}


}


int main()
{
	std::ifstream myInputFile;
	std::ofstream myOutputFile;
	menu(myInputFile);
	std::vector<Vector2<float>> points;



	float x;
	float y;
	myOutputFile.open("test_output.txt");



	while (myInputFile >> x >> y)
	{

		points.push_back(Vector2<float>(x, y));
	}
	myInputFile.close();
	Delaunay<float> triangulation;
	std::vector<Triangle<float>> triangles = triangulation.triangulate(points);
	std::cout << triangles.size() << " triangles generated\n";
	std::vector<Edge<float>> edges = triangulation.getEdges();

	std::cout << " ========= ";

	std::cout << "\nPoints : " << points.size() << std::endl;
	for (auto &p : points)
		std::cout << p << std::endl;

	std::cout << "\nTriangles : " << triangles.size() << std::endl;
	for (auto &t : triangles) {
		std::cout << t << std::endl;
		myOutputFile << t << std::endl;
	}

	std::cout << "\nEdges : " << edges.size() << std::endl;
	for (auto &e : edges)
		std::cout << e << std::endl;

	// SFML window
	sf::RenderWindow window(sf::VideoMode(600, 800), "Delaunay triangulation");

	// Transform each points of each vector as a rectangle
	std::vector<sf::RectangleShape*> squares;

	for (auto p = begin(points); p != end(points); p++) {
		sf::RectangleShape *c1 = new sf::RectangleShape(sf::Vector2f(4, 4));
		c1->setPosition(p->x, p->y);
		c1->setFillColor(sf::Color(150, 50, 250));
		squares.push_back(c1);
	}

	// Make the lines
	std::vector<std::array<sf::Vertex, 2> > lines;
	for (auto e = begin(edges); e != end(edges); e++) {
		lines.push_back({ {
				sf::Vertex(sf::Vector2f((*e).p1.x + 2, (*e).p1.y + 2)),
				sf::Vertex(sf::Vector2f((*e).p2.x + 2, (*e).p2.y + 2))
			} });
	}
	myOutputFile.close();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();




		// Draw the lines
		for (auto l = begin(lines); l != end(lines); l++) {
			window.draw((*l).data(), 2, sf::Lines);
		}
		// Draw the squares
		for (auto s = begin(squares); s != end(squares); s++) {
			window.draw(**s);
		}

		window.display();
	}

	return 0;
}
