#pragma once

#include "wx_pch.h"

#include <game/cell.h>

using namespace std;

#ifdef __APPLE__
#include <experimental/optional>
using namespace std::experimental;
#else
#include <optional>
#endif

class ColorScheme
{
	private:
		using CT = sweepp::Cell::Type;
		using CS = sweepp::Cell::State;
		
	public:
		struct Scheme
		{
			wxColour background;
			wxColour border;
			wxColour text;
		};
		struct State
		{
			struct Cell
			{
				optional<CT> type;
				optional<CS> state;
				
				explicit Cell(optional<CT> type, optional<CS> state)
					: type(type), state(state) {}
				
				explicit Cell(optional<CS> state)
					: Cell({}, state) {}
				
				bool matches(State::Cell cell)
				{
					return (!this->type || this->type == cell.type) &&
					       (!this->state || this->state == cell.state);
				}
			};
			struct Interaction
			{
				bool normal;
				bool hover;
				bool active;
				
				explicit Interaction(bool normal, bool hover, bool active)
					: normal(normal), hover(hover), active(active) {}
				
				bool matches(State::Interaction interaction)
				{
					return this->normal == interaction.normal &&
					       this->hover == interaction.hover &&
					       this->active == interaction.active;
				}
			};
			struct Game
			{
				bool gameWon;
				bool gameLost;
				optional<bool> debug;
				
				explicit Game(bool gameWon, bool gameLost, optional<bool> debug)
					: gameWon(gameWon), gameLost(gameLost), debug(debug) {}
				
				bool matches(State::Game game)
				{
					return this->gameWon == game.gameWon &&
					       this->gameLost == game.gameLost &&
					       (!this->debug || this->debug == game.debug);
				}
			};
			
			optional<State::Cell> cell;
			optional<State::Interaction> interaction;
			optional<State::Game> game;
			
			explicit State(optional<State::Cell> cell, optional<State::Interaction> interaction, optional<State::Game> game)
				: cell(cell), interaction(interaction), game(game) {}
			
			explicit State(optional<State::Interaction> interaction, optional<State::Game> game)
				: State({}, interaction, game) {}
			
			explicit State(optional<State::Game> game)
				: State({}, game) {}
			
			bool matches(State::Cell cell, State::Interaction interaction, State::Game game)
			{
				return (!this->cell || this->cell->matches(cell)) &&
				       (!this->interaction || this->interaction->matches(interaction)) &&
				       (!this->game || this->game->matches(game));
			}
		};
		
		static ColorScheme::Scheme scheme(State::Cell cell, State::Interaction interaction, State::Game game)
		{
			for(int i = 0; i < size(ColorScheme::schemes); i++)
			{
				SC container = ColorScheme::schemes[i];
				if(container.first.matches(cell, interaction, game))
				{
					return container.second;
				}
			}
			return ColorScheme::schemes[0].second;
		}
		
		static wxColour mineCounterColor(uint8_t mines)
		{
			return ColorScheme::mineCounterColors[mines - 1];
		}
		
	private:
		using SC = std::pair<ColorScheme::State, ColorScheme::Scheme>;
		
		inline static const SC schemes[] = {
			SC(State(State::Cell(CT::Mine, {}), {}, State::Game({}, {}, true)),
			   Scheme { wxColour(255, 96, 96), wxColour(128, 32, 32), wxColour(128, 32, 32) }),
			
			SC(State(State::Cell(CS::Closed), State::Interaction(true, false, false), {}),
			   Scheme { wxColour(200, 200, 200), wxColour(128, 128, 128), wxColour(0, 0, 0) }),
			SC(State(State::Cell(CS::Closed), State::Interaction(false, true, false), {}),
			   Scheme { wxColour(222, 222, 222), wxColour(64, 64, 64), wxColour(0, 0, 0) }),
			SC(State(State::Cell(CS::Closed), State::Interaction(false, false, true), {}),
			   Scheme { wxColour(244, 244, 244), wxColour(0, 0, 0), wxColour(0, 0, 0) }),
			
			SC(State(State::Cell(CT::Empty, CS::Opened), {}, {}),
			   Scheme { wxColour(255, 255, 255), wxColour(200, 200, 200), wxColour(0, 0, 0) }),
			
			SC(State(State::Cell(CT::Mine, {}), {}, State::Game(true, false, {})),
			   Scheme { wxColour(96, 255, 96), wxColour(32, 128, 32), wxColour(32, 128, 32) }),
			SC(State(State::Cell(CT::Mine, CS::Opened), {}, State::Game(false, true, {})),
			   Scheme { wxColour(255, 96, 96), wxColour(128, 32, 32), wxColour(128, 32, 32) }),
			
			SC(State(State::Cell(CS::Flagged), {}, {}),
			   Scheme { wxColour(96, 255, 96), wxColour(32, 128, 32), wxColour(32, 128, 32) })
		};
		
		inline static wxColour mineCounterColors[] =
		{
			*wxBLUE, wxColour(64, 192, 64), *wxRED, wxColour(112, 39, 195), wxColour(192, 192, 64), wxColour(255, 92, 0), wxColour(255, 92, 0), wxColour(255, 92, 0)
		};
};