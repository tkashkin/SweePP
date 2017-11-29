#pragma once

#include <libswee++.h>
#include <game/field.h>

#include <string>
#include <vector>

namespace sweepp
{
	
	class SWEEPP ScoresManager
	{
		public:
			static void load(std::string file = "./scores");
			
			static void save(std::string file = "./scores");
			
			static int add(Field::Score score);
			
			static std::vector<Field::Score>* scores();
		
		private:
			static std::vector<Field::Score> mScores;
	};
	
}