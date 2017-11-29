#pragma once

#include <libswee++.h>

namespace sweepp
{
	/**
	 * Клетка
	 */
    class SWEEPP Cell
    {
	    public:
		    /**
		     * Тип клетки
		     */
		    enum class Type { Empty, Mine };
		    /**
		     * Состояние клетки
		     */
		    enum class State { Closed, Opened, Flagged };
        
        public:
		    /**
		     * @param x x
		     * @param y y
		     * @param type тип клетки
		     */
		    Cell(uint8_t x, uint8_t y, Cell::Type type = Cell::Type::Empty): Cell(Point(x, y), type) {}
		    
		    /**
		     * @param position позиция
		     * @param type тип клетки
		     */
		    explicit Cell(Point position, Cell::Type type = Cell::Type::Empty)
		        : mPosition(position), mType(type), mState(Cell::State::Closed) {}
		
		    /**
		     * @return позиция
		     */
		    Point pos();
		    
		    /**
		     * @param pos позиция
		     * @return клетка находится в позиции?
		     */
		    bool isAt(Point pos);
		
		    /**
		     * @return тип
		     */
		    Cell::Type type();
		    
		    /**
		     * @return состояние
		     */
		    Cell::State state();
		    
		    /**
		     * @param type тип
		     * @return клетка имеет тип?
		     */
		    bool is(Cell::Type type);
		    
		    /**
		     * @param state состояние
		     * @return клетка находится в состоянии?
		     */
		    bool in(Cell::State state);
		    
		    /**
		     * @param type тип
		     */
		    void setType(Cell::Type type);
		    
		    /**
		     * @param state состояние
		     */
		    void setState(Cell::State state);
		    
		    /**
		     * пометить клетку как пройденную (расчёт 3BV)
		     */
		    void mark();
		    
		    /**
		     * @return клетка помечена как пройденная? (расчёт 3BV)
		     */
		    bool isMarked();
	
	    private:
		    Point mPosition;
		    Cell::Type mType;
		    Cell::State mState;
		    bool m3BVMarked = false;
    };

}