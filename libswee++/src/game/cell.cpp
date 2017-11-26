#include <libswee++.h>
#include <game/cell.h>

namespace sweepp
{
	Cell::Type Cell::type()
	{
		return this->mType;
	}
	
	Cell::State Cell::state()
	{
		return this->mState;
	}
	
	bool Cell::is(Cell::Type type)
	{
		return this->type() == type;
	}
	
	bool Cell::in(Cell::State state)
	{
		return this->state() == state;
	}
	
	void Cell::setType(Cell::Type type)
	{
		this->mType = type;
	}
	
	void Cell::setState(Cell::State state)
	{
		this->mState = state;
	}
	
	Point Cell::pos()
	{
		return this->mPosition;
	}
	
	bool Cell::isAt(Point pos)
	{
		return this->pos() == pos;
	}
	
	void Cell::mark()
	{
		this->m3BVMarked = true;
	}
	
	bool Cell::isMarked()
	{
		return this->m3BVMarked;
	}
}