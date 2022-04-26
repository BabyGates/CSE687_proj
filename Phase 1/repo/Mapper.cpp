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
#include <sstream>

//constructor
Mapper::Mapper(int c)
	/* :totalWord(c)*/
{
	words;
}
//map function to split whole text
void Mapper::map(int key, std::vector<std::string> container, std::string dir)
{
	std::string text = container[key];
	//remove punctuation;
	//text.erase(std::remove_if(text.begin(), text.end(), ::ispunct), text.end());

	//std::transform(text.begin(), text.end(), text.begin(), std::tolower);
	//std::vector<std::string> words;
	
	std::stringstream txtstr(text);
	std::string intermediate;

	while (getline(txtstr, intermediate, ' '))
	{
		intermediate.erase(std::remove_if(intermediate.begin(), intermediate.end(), ::ispunct), intermediate.end());

		std::transform(intermediate.begin(), intermediate.end(), intermediate.begin(), std::tolower);
		words.push_back(intermediate+",1");
	}


	//export;
	exports(dir);
}


void Mapper::exports(std::string dir)
{
	std::ofstream out(dir + "/temp.txt");
	std::streambuf* coutbuf = std::cout.rdbuf();
	std::cout.rdbuf(out.rdbuf());
	//std::ofstream outputFile(dir+"/temp.txt");
	for (auto& itr : words)
		std::cout << itr << std::endl;
	//clear up vector words.
	words.clear();

	std::cout.rdbuf(coutbuf);
	std::ofstream myfile;
}