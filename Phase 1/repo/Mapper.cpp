/******************************************************************************************
 *CS 687                      
 *University of Syracuse      
 *Project 1
 *Shutong Liu
 *Mapper.cpp: implementation of mapper class to map the text from single file to vector of 
 *single word and export to a temporary file
 *****************************************************************************************/
#include "Mapper.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>

//constructor
Mapper::Mapper(int c)
	:totalWord(c)
{
	words;
}
//map function to split whole text
void Mapper::map(int key, std::vector<std::string> container, std::string dir)
{
	std::string text = container[key];
	//remove punctuation;
	text.erase(std::remove_if(text.begin(), text.end(), ::ispunct), text.end());

	std::transform(text.begin(), text.end(), text.begin(), std::tolower);
	//std::vector<std::string> words;
	
	size_t x = text.find(" ");
	while (x != std::string::npos) {
		words.push_back(text.substr(0, x)+",1");
		totalWord += 1;
		text = text.substr(x);
		x = text.find(" ");
	}

	//export;
	export(dir);
}


void Mapper::export(std::string dir)
{
	std::ofstream outputFile(dir+"/temp.txt");
	for (auto& itr : words)
		outputFile << itr << std::endl;
	//clear up vector words.
	words.clear();
}