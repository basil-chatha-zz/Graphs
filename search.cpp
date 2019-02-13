#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <stdexcept>
#include "search.h"

// HOW DO I MAKE IT SO IT SKIPS BLANK LINES IN INPUT_FILE.TXT ?????????????????????????????

void makeGraph(std::ifstream& myFile, std::vector< std::vector<Node*> >& countryGraph, std::vector< std::vector < std::vector<Node*> > >& adjList, int& numLetters, int& numRows, int& numColumns) {
	int lineCount = 0;
	int rowCount = 0;

	std::string myLine;
	char myLetter;
	while(std::getline(myFile, myLine)) {
		std::stringstream ss(myLine);
		switch(lineCount) {
			case 0: {
				ss >> numLetters;
				ss >> numRows;
				ss >> numColumns;
				break;
			}
			default: {
				countryGraph.push_back(std::vector<Node*>());
				adjList.push_back(std::vector< std::vector<Node*> >());
				int columnCount = 0;
				for(int i = 0; i < numColumns; i++) {
					Node *newNode = new Node;

					adjList.at(adjList.size() - 1).push_back(std::vector<Node*>());

					ss >> myLetter;
					newNode->letter = myLetter;
					newNode->row = rowCount;
					newNode->column = columnCount;

					countryGraph.at(countryGraph.size() - 1).push_back(newNode);
					columnCount++;
				} rowCount++;
			}
		} lineCount++;
	}
}

void makeAdjList(std::vector< std::vector<Node*> >& countryGraph, std::vector< std::vector < std::vector<Node*> > >& adjList) {
	for(size_t i = 0; i < countryGraph.size(); i++) {
		for(size_t j = 0; j < countryGraph.at(i).size(); j++) {
			try { // ADD NODE ABOVE
				adjList.at(i).at(j).push_back(countryGraph.at(i - 1).at(j));
				throw std::logic_error("Not possible");
			} catch(std::logic_error& e) {};
			try { // ADD NODE TO RIGHT
				adjList.at(i).at(j).push_back(countryGraph.at(i).at(j + 1));
				throw std::logic_error("Not possible");
			} catch(std::logic_error& e) {};
			try { // ADD NODE BELOW
				adjList.at(i).at(j).push_back(countryGraph.at(i + 1).at(j));
				throw std::logic_error("Not possible");
			} catch(std::logic_error& e) {};
			try { // ADD NODE TO LEFT
				adjList.at(i).at(j).push_back(countryGraph.at(i).at(j - 1));
				throw std::logic_error("Not possible");
			} catch(std::logic_error& e) {};
		}
	}
}

int BFS(std::vector< std::vector<Node*> >& countryGraph, std::vector< std::vector < std::vector<Node*> > >& adjList, Node* startNode) {
	int count = 0;

	std::queue<Node*> myQueue;
	std::set<Node*> encountered;

	myQueue.push(startNode);

	while(!myQueue.empty()) {
		Node* v = myQueue.front();
		myQueue.pop();

		count++;

		std::vector<Node*>::iterator it;
		for(it = adjList.at(v->row).at(v->column).begin(); it != adjList.at(v->row).at(v->column).end(); it++) {
			if(!(*it)->visited && (*it)->letter == startNode->letter) {
				bool is_in = encountered.find(*it) != encountered.end();
				if(!is_in) {
					myQueue.push(*it);
					encountered.insert(*it);
				}
			}
		} v->visited = true;
	}
	return count;
}

// MODIFIED CONNECTED COMPONENTS ALGORITHM
void largestCountrySize(std::vector< std::vector<Node*> >& countryGraph, std::vector< std::vector < std::vector<Node*> > >& adjList) {
	int maxCountrySize = 0;

	for(size_t i = 0; i < countryGraph.size(); i++) {
		for(size_t j = 0; j < countryGraph.at(i).size(); j++) {
			if(!countryGraph.at(i).at(j)->visited) {
				int countrySize = BFS(countryGraph, adjList, countryGraph.at(i).at(j));
				if(countrySize > maxCountrySize) maxCountrySize = countrySize;
			}
		}
	}
	std::cout << maxCountrySize << std::endl;
}

int main(int argc, char *argv[]) {
	std::ifstream myFile(argv[1]);
	
	int numLetters, numRows, numColumns;
	numLetters = 0; numRows = 0; numColumns = 0;
	std::vector< std::vector<Node*> > countryGraph;

	std::vector< std::vector < std::vector<Node*> > > adjList;

	makeGraph(myFile, countryGraph, adjList, numLetters, numRows, numColumns);

	makeAdjList(countryGraph, adjList);

	largestCountrySize(countryGraph, adjList);

	for(size_t i = 0; i < countryGraph.size(); i++) {
		for(size_t j = 0; j < countryGraph.at(i).size(); j++) {
			delete countryGraph.at(i).at(j);
		}
	}

	return 0;
}