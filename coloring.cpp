#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <stdexcept>
#include "search.h"

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
				throw std::logic_error("Doesn't exist");
			} catch(std::logic_error& e) {};
			try { // ADD NODE TO RIGHT
				adjList.at(i).at(j).push_back(countryGraph.at(i).at(j + 1));
				throw std::logic_error("Doesn't exist");
			} catch(std::logic_error& e) {};
			try { // ADD NODE BELOW
				adjList.at(i).at(j).push_back(countryGraph.at(i + 1).at(j));
				throw std::logic_error("Doesn't exist");
			} catch(std::logic_error& e) {};
			try { // ADD NODE TO LEFT
				adjList.at(i).at(j).push_back(countryGraph.at(i).at(j - 1));
				throw std::logic_error("Doesn't exist");
			} catch(std::logic_error& e) {};
		}
	}
}

void BFS(std::vector< std::vector<Node*> >& countryGraph, std::vector< std::vector < std::vector<Node*> > >& adjList, Node* startNode, std::map<char, std::set<char>>& tempAdjMap) {
	std::queue<Node*> myQueue;
	std::set<Node*> encountered;

	myQueue.push(startNode);

	tempAdjMap[startNode->letter] = std::set<char>();

	while(!myQueue.empty()) {
		Node* v = myQueue.front();
		myQueue.pop();

		std::vector<Node*>::iterator it;
		for(it = adjList.at(v->row).at(v->column).begin(); it != adjList.at(v->row).at(v->column).end(); it++) {
			if((*it)->letter != startNode->letter) { // ADD ADJACENT COUNTRIES TO CURRENT COUNTRY'S LIST
				tempAdjMap[startNode->letter].insert((*it)->letter);
			}

			else if(!(*it)->visited && (*it)->letter == startNode->letter) {
				bool is_in = encountered.find(*it) != encountered.end();
				if(!is_in) {
					myQueue.push(*it);
					encountered.insert(*it);
				}
			}
		} v->visited = true;
	}
}

void connectedComponents(std::vector< std::vector<Node*> >& countryGraph, std::vector< std::vector < std::vector<Node*> > >& adjList, std::map<char, std::set<char>>& tempAdjMap) {
	for(size_t i = 0; i < countryGraph.size(); i++) {
		for(size_t j = 0; j < countryGraph.at(i).size(); j++) {
			if(!countryGraph.at(i).at(j)->visited) {
				BFS(countryGraph, adjList, countryGraph.at(i).at(j), tempAdjMap);
			}
		}
	}
}

void makeRealAdjMap(std::map<char, std::set<char>>& tempAdjMap, std::map<char, std::set<Node*>>& realAdjMap, std::map<char, Node*>& charNodeMappings) {
	std::map<char, std::set<char>>::iterator it1;

	for(it1 = tempAdjMap.begin(); it1!= tempAdjMap.end(); it1++) {
		Node *newNode = new Node;
		newNode->letter = it1->first;
		charNodeMappings[it1->first] = newNode;
	}
	std::map<char, std::set<char>>::iterator it2;
	for(it2 = tempAdjMap.begin(); it2 != tempAdjMap.end(); it2++) {
		std::set<char>::iterator it3;
		for(it3 = it2->second.begin(); it3 != it2->second.end(); it3++) {
			realAdjMap[it2->first].insert(charNodeMappings[*it3]);
		}
	}
}

void makeCharMappings(std::map<char, Node*>& charNodeMappings, std::map<char, size_t>& charIndexMapping, std::map<size_t, char>& indexCharMapping) {
	std::map<char, Node*>::iterator it1;
	size_t count = (size_t) 0;
	for(it1 = charNodeMappings.begin(); it1 != charNodeMappings.end(); it1++) {
		charIndexMapping[it1->first] = count;
		indexCharMapping[count] = it1->first;
		count++;
	}
}

void printColors(std::map<char, Node*>& charNodeMappings) {
	std::map<char, Node*>::iterator it;
	for(it = charNodeMappings.begin(); it != charNodeMappings.end(); it++) {
		std::cout << it->second->letter << " " << it->second->color << std::endl;
	}
}

bool isValidColoring(std::map<char, std::set<Node*>>& realAdjMap, char v, int color) {
	bool isValid = true;

	std::set<Node*>::iterator it;
	for(it = realAdjMap[v].begin(); it != realAdjMap[v].end(); it++) {
		if((*it)->color == 0) continue;
		else if(((*it)->color != 0) && ((*it)->color == color)) {
			isValid = false;
			break;
		}
	}
	return isValid;
}

bool graphColoring(std::map<char, std::set<Node*>>& realAdjMap, std::map<char, size_t> charIndexMapping, std::map<size_t, char>& indexCharMapping, std::map<char, Node*>& charNodeMappings, int v, int numLetters) {
	// BASE CASE WHEN THE ALL VERTICES HAVE BEEN VISITED
	if(v == numLetters) {
		return true;
	}

	for(int i = 1; i <= 4; i++) {
		if(isValidColoring(realAdjMap, indexCharMapping[v], i)) {
			charNodeMappings[indexCharMapping[v]]->color = i;

			if(graphColoring(realAdjMap, charIndexMapping, indexCharMapping, charNodeMappings, v+1, numLetters)) {
				return true;
			} else {
				charNodeMappings[indexCharMapping[v]]->color = 0;
			}
		}
	}
	return false;
}

int main(int argc, char *argv[]) {
	std::ifstream myFile(argv[1]);
	
	int numLetters, numRows, numColumns;
	numLetters = 0; numRows = 0; numColumns = 0;
	std::vector< std::vector<Node*> > countryGraph;

	std::vector< std::vector < std::vector<Node*> > > adjList;
	std::map<char, std::set<char>> tempAdjMap;
	std::map<char, std::set<Node*>> realAdjMap;
	std::map<char, Node*> charNodeMappings;
	std::map<char, size_t> charIndexMapping;
	std::map<size_t, char> indexCharMapping;

	makeGraph(myFile, countryGraph, adjList, numLetters, numRows, numColumns);

	makeAdjList(countryGraph, adjList);

	connectedComponents(countryGraph, adjList, tempAdjMap);

	makeRealAdjMap(tempAdjMap, realAdjMap, charNodeMappings);

	makeCharMappings(charNodeMappings, charIndexMapping, indexCharMapping);

	if(graphColoring(realAdjMap, charIndexMapping, indexCharMapping, charNodeMappings, 0, numLetters)) {
		printColors(charNodeMappings);
	}
	

	// DEALLOCATE MEMORY
	for(size_t i = 0; i < countryGraph.size(); i++) {
		for(size_t j = 0; j < countryGraph.at(i).size(); j++) {
			delete countryGraph.at(i).at(j);
		}
	}
	std::map<char, Node*>::iterator charIterator;
	for(charIterator = charNodeMappings.begin(); charIterator != charNodeMappings.end(); charIterator++) {
		delete charIterator->second;
	}


	return 0;
}