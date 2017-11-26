#include <libswee++.h>
#include <game/cell.h>
#include <game/field.h>
#include <cstdlib>
#include <chrono>
#include <cmath>

namespace sweepp
{
	
	Field::Field(uint8_t width, uint8_t height, uint16_t mines)
	{
		this->mFieldWidth = width;
		this->mFieldHeight = height;
		this->mMines = mines;
		this->mFlags = 0;
		this->mOpenedCells = 0;
		
		this->mField = new Cell*[width * height];
		
		for(uint8_t y = 0; y < height; y++)
		{
			for(uint8_t x = 0; x < width; x++)
			{
				this->cell(x, y) = new Cell(x, y);
			}
		}
	}
	
	Cell*& Field::cell(Point pos)
	{
		return this->cell(pos.x, pos.y);
	}
	Cell*& Field::cell(uint8_t x, uint8_t y)
	{
		return this->cell(y * this->mFieldWidth + x);
	}
	Cell*& Field::cell(uint16_t index)
	{
		return this->mField[index];
	}
	
	void Field::generateMines(Point firstClick)
	{
		this->mStartTime = std::chrono::high_resolution_clock::now();
		std::srand(static_cast<unsigned int>(this->mStartTime.time_since_epoch().count()));
		
		uint16_t m = 0;
		while(m < this->mMines)
		{
			auto index = static_cast<uint16_t>(std::rand() % (this->mFieldWidth * this->mFieldHeight));
			Cell* cell = this->cell(index);
			
			if(cell->isAt(firstClick) || cell->is(Cell::Type::Mine)) continue;
			
			cell->setType(Cell::Type::Mine);
			m++;
		}
		
		this->mMinesGenerated = true;
		
		this->calcOpeningsRequired();
	}
	
	void Field::open(Point pos)
	{
		if(!this->mMinesGenerated) this->generateMines(pos);
		
		if(this->isGameLost() || this->isGameWon()) return;
		
		Cell* target = this->cell(pos);
		
		if(!target->in(Cell::State::Closed) || this->mGameLost) return;
		
		if(target->is(Cell::Type::Mine))
		{
			this->mGameLost = true;
			target->setState(Cell::State::Opened);
			return;
		}
		
		this->mOpenings++;
		
		this->openCellRecursive(target);
	}
	
	void Field::flag(Point pos)
	{
		if(this->isGameLost() || this->isGameWon()) return;
		
		Cell* target = this->cell(pos);
		
		if(target->in(Cell::State::Opened) || this->mGameLost) return;
		
		bool flagged = target->in(Cell::State::Flagged);
		this->mFlags += flagged ? -1 : 1;
		target->setState(flagged ? Cell::State::Closed : Cell::State::Flagged);
	}
	
	void Field::openCellRecursive(Cell* cell)
	{
		cell->setState(Cell::State::Opened);
		this->mOpenedCells++;
		
		if(this->minesAround(cell) > 0) return;
		
		auto ca = this->cellsAround(cell, true);
		
		for(uint8_t i = 0; i < 8; i++)
		{
			uint16_t index = ca[i];
			
			if(index >= 0 && index < this->mFieldWidth * this->mFieldHeight)
			{
				Cell* c = this->cell(index);
				if(abs(cell->pos().x - c->pos().x) > 1) continue;
				
				if(c->is(Cell::Type::Empty) && c->in(Cell::State::Closed))
				{
					this->openCellRecursive(c);
				}
			}
		}
	}
	
	uint16_t* Field::cellsAround(Cell* cell, bool diagonal)
	{
		uint16_t ci = cell->pos().y * this->mFieldWidth + cell->pos().x;
		if(diagonal)
		{
			return new uint16_t[8] {
                static_cast<uint16_t>(ci - this->mFieldWidth - 1),
                static_cast<uint16_t>(ci - this->mFieldWidth),
                static_cast<uint16_t>(ci - this->mFieldWidth + 1),
                static_cast<uint16_t>(ci - 1),
                static_cast<uint16_t>(ci + 1),
                static_cast<uint16_t>(ci + this->mFieldWidth - 1),
                static_cast<uint16_t>(ci + this->mFieldWidth),
                static_cast<uint16_t>(ci + this->mFieldWidth + 1)
			};
		}
		
		return new uint16_t[4] {
            static_cast<uint16_t>(ci - this->mFieldWidth),
            static_cast<uint16_t>(ci - 1),
            static_cast<uint16_t>(ci + 1),
            static_cast<uint16_t>(ci + this->mFieldWidth)
		};
	}
	
	uint8_t Field::minesAround(Cell* cell)
	{
		uint8_t m = 0;
		
		auto ca = this->cellsAround(cell, true);
		
		for(uint8_t i = 0; i < 8; i++)
		{
			uint16_t index = ca[i];
			
			if(index >= 0 && index < this->mFieldWidth * this->mFieldHeight)
			{
				Cell* c = this->cell(index);
				if(abs(cell->pos().x - c->pos().x) > 1) continue;
				
				if(c->is(Cell::Type::Mine)) m++;
			}
		}
		
		return m;
	}
	
	Point Field::size()
	{
		return Point(this->mFieldWidth, this->mFieldHeight);
	}
	
	bool Field::isGameLost()
	{
		return this->mGameLost;
	}
	
	bool Field::isGameWon()
	{
		bool win = false;
		
		if (this->mFlags == this->mMines)
		{
			for (uint16_t i = 0; i < this->mFieldWidth * this->mFieldHeight; i++)
			{
				Cell* c = this->cell(i);
				
				if (c->is(Cell::Type::Mine))
				{
					if (c->in(Cell::State::Flagged))
					{
						win = true;
					}
					else
					{
						win = false;
						break;
					}
				}
			}
		}
		else
		{
			for (uint16_t i = 0; i < this->mFieldWidth * this->mFieldHeight; i++)
			{
				Cell* c = this->cell(i);
				
				if (c->is(Cell::Type::Empty))
				{
					if (c->in(Cell::State::Opened))
					{
						win = true;
					}
					else
					{
						win = false;
						break;
					}
				}
			}
		}
		
		return win;
	}
	
	uint16_t Field::mines()
	{
		return this->mMines;
	}
	
	uint16_t Field::flags()
	{
		return this->mFlags;
	}
	
	long long Field::time()
	{
		if(!this->isGameLost() && !this->isGameWon()) this->mEndTime = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::seconds>(this->mEndTime - this->mStartTime).count();
	}
	
	uint32_t Field::score()
	{
		double size = (double) this->mFieldWidth * (double) this->mFieldHeight / (double) 50;
		double difficulty = (double) this->mMines / ((double) this->mFieldWidth * (double) this->mFieldHeight);
		double openings = (double) this->mOpenings / (double) this->mOpeningsRequired;
		double time = ((double) this->mOpenings / (double) this->time()) * (double) (this->mOpeningsRequired * 2);
		
		return static_cast<uint32_t>(sqrt(
				(double) 100000 *
				((double) this->mOpenedCells / (double) ((this->mFieldWidth * this->mFieldHeight) - this->mMines))
				* difficulty * size * openings * time));
	}
	
	// https://gamedev.stackexchange.com/questions/63046/
	uint16_t Field::calcOpeningsRequired()
	{
		this->mOpeningsRequired = 0;
		
		if(!this->mMinesGenerated) return 0;
		
		for(uint16_t i = 0; i < this->mFieldWidth * this->mFieldHeight; i++)
		{
			Cell* cell = this->cell(i);
			
			if(cell->isMarked() || cell->is(Cell::Type::Mine) || this->minesAround(cell) > 0) continue;
			
			cell->mark();
			this->mOpeningsRequired++;
			
			this->markRecursive(cell);
		}
		
		for(uint16_t i = 0; i < this->mFieldWidth * this->mFieldHeight; i++)
		{
			Cell* cell = this->cell(i);
			
			if(!cell->isMarked() && cell->is(Cell::Type::Empty)) this->mOpeningsRequired++;
		}
		
		return this->mOpeningsRequired;
	}
	
	void Field::markRecursive(Cell* cell)
	{
		if(this->minesAround(cell) > 0) return;
		
		auto ca = this->cellsAround(cell, true);
		
		for(uint8_t i = 0; i < 8; i++)
		{
			uint16_t index = ca[i];
			
			if(index >= 0 && index < this->mFieldWidth * this->mFieldHeight)
			{
				Cell* c = this->cell(index);
				if(c->isMarked() || abs(cell->pos().x - c->pos().x) > 1) continue;
				
				c->mark();
				
				if(c->is(Cell::Type::Empty))
				{
					this->markRecursive(c);
				}
			}
		}
	}
	
	uint32_t Field::openings()
	{
		return this->mOpenings;
	}
	
	uint32_t Field::openingsRequired()
	{
		return this->mOpeningsRequired;
	}
}