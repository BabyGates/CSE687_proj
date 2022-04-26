

#ifndef MAPPER_HPP
#define MAPPER_HPP

#include <vector>
#include <string>

class Mapper
{
public:
	explicit Mapper(int count=0);
	void map(int k, std::vector<std::string> c, std::string dir);
	
private:

	void exports(std::string temp);
	std::vector<std::string> words;
	//int totalWord;
}
;





#endif MAPPER_H
