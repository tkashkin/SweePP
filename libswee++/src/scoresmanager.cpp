#include <scoresmanager.h>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>

namespace sweepp
{
	std::vector<Field::Score> ScoresManager::mScores;
	
	void ScoresManager::load(std::string file)
	{
		ScoresManager::mScores.clear();
		
		std::ifstream stream(file, std::ios::in | std::ifstream::binary);
		
		if(!stream.is_open()) return;
		
		stream.seekg(0, std::ios::end);
		std::fpos<mbstate_t> size = stream.tellg();
		stream.seekg(0, std::ios::beg);
		
		ScoresManager::mScores.resize(size / sizeof(Field::Score));
		stream.read(reinterpret_cast<char*>(ScoresManager::mScores.data()), static_cast<std::streamsize>(size));
	}
	
	void ScoresManager::save(std::string file)
	{
		std::ofstream stream(file, std::ios::out | std::ofstream::binary);
		stream.write(reinterpret_cast<char*>(ScoresManager::mScores.data()), ScoresManager::mScores.size() * sizeof(Field::Score));
	}
	
	int ScoresManager::add(Field::Score score)
	{
		auto it = std::lower_bound(ScoresManager::mScores.begin(), ScoresManager::mScores.end(), score, std::greater<>());
		ScoresManager::mScores.insert(it, score);
		
		it = std::find(ScoresManager::mScores.begin(), ScoresManager::mScores.end(), score);
		return std::distance(ScoresManager::mScores.begin(), it);
	}
	
	std::vector<Field::Score>* ScoresManager::scores()
	{
		return &ScoresManager::mScores;
	}
	
}