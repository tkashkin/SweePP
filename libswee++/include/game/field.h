#pragma once

#include <libswee++.h>
#include <game/cell.h>
#include <chrono>

namespace sweepp
{
	/**
	 * Игровое поле
	 */
	class SWEEPP Field
	{
		public:
			struct SWEEPP Score
			{
				Size fieldSize = Size(0, 0);
				uint16_t mines = 0;
				uint16_t openings = 0;
				uint16_t openingsRequired = 0;
				uint16_t openedCells = 0;
				long long time = 0;
				
				Score() = default;
				
				explicit Score(Field* field);
				
				uint32_t score() const;
				
				uint32_t operator()();
				
				bool operator<(Score rhs);
				
				bool operator>(Score rhs);
				
				bool operator==(Score rhs);
			};
			
			/**
			 * @param width ширина
			 * @param height высота
			 * @param mines количество мин
			 */
			explicit Field(uint8_t width = 10, uint8_t height = 10, uint16_t mines = 10);
			
			/**
			 * Открыть клетку
			 * @param pos позиция
			 */
			void open(Point pos);
			
			/**
			 * Отметить клетку
			 * @param pos позиция
			 */
			void flag(Point pos);
			
			/**
			 * Вычислить количество мин вокруг клетки
			 * @param cell клетка
			 * @return количество мин
			 */
			uint8_t minesAround(Cell* cell);
			
			/**
			 * @param pos позиция
			 * @return клетка
			 */
			Cell*& cell(Point pos);
			
			/**
			 * @param x x
			 * @param y y
			 * @return клетка
			 */
			Cell*& cell(uint8_t x, uint8_t y);
			
			/**
			 * @param index индекс
			 * @return клетка
			 */
			Cell*& cell(uint16_t index);
			
			/**
			 * Получить размер поля
			 * @return размер поля
			 */
			Size size();
			
			/**
			 * @return победа?
			 */
			bool isGameWon();
			
			/**
			 * @return поражение?
			 */
			bool isGameLost();
			
			/**
			 * @return количество мин
			 */
			uint16_t mines();
			
			/**
			 * @return количество установленных флагов
			 */
			uint16_t flags();
			
			/**
			 * @return время игры
			 */
			long long time();
			
			/**
			 * @return выполненные открытия
			 */
			uint32_t openings();
			
			/**
			 * @return минимальное количество открытий (3BV)
			 */
			uint32_t openingsRequired();
			
			/**
			 * @return счёт
			 */
			Score score();
		
		private:
			void generateMines(Point firstClick);
			
			uint16_t* cellsAround(Cell* cell, bool diagonal = true);
			void openCellRecursive(Cell* cell);
			
			uint16_t calcOpeningsRequired();
			void markRecursive(Cell* cell);
			
			uint8_t mFieldWidth, mFieldHeight;
			uint16_t mMines, mFlags, mOpenedCells;
			Cell** mField;
			bool mMinesGenerated = false;
			bool mGameLost = false;
			
			uint16_t mOpenings = 0, mOpeningsRequired = 0;
			
			std::chrono::system_clock::time_point mStartTime, mEndTime;
	};

}