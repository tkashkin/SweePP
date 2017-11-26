#include "wx_pch.h"
#include "frames/MainFrame.h"

#include <ColorScheme.h>

#include <ctime>

#ifdef __LINUX__
#include "../swee++.xpm"
#endif

static const int Padding = 64;
static const int MaxCellSize = 32;
static const int MinCellSize = 18;

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_PAINT(MainFrame::OnPaint)
	EVT_SIZE(MainFrame::OnResize)
	
	EVT_KEY_UP(MainFrame::OnKeyUp)
	
	EVT_MOTION(MainFrame::OnMouseMove)
	EVT_LEAVE_WINDOW(MainFrame::OnMouseLeave)
	
	EVT_LEFT_DOWN(MainFrame::OnLeftPreClick)
	EVT_LEFT_UP(MainFrame::OnLeftClick)
	EVT_RIGHT_DOWN(MainFrame::OnRightPreClick)
	EVT_RIGHT_UP(MainFrame::OnRightClick)
END_EVENT_TABLE()

MainFrame::MainFrame(wxWindow* parent, wxWindowID id)
	: wxFrame(parent, id, _("Swee++ - C++ minesweeper game"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE|wxWANTS_CHARS)
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
    this->SetClientSize(wxSize(500, 500));
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
	this->Center();
	this->SetFocus();
	this->SetFocusFromKbd();

	#ifndef __APPLE__
	this->SetIcon(wxICON(sweepp));
	#endif
	
	auto font = this->GetFont();
	font.SetWeight(wxFONTWEIGHT_BOLD);
	this->SetFont(font);
	
	this->GetEventHandler()->Connect(id, wxID_ANY, wxNotifyEventHandler(MainFrame::OnShow));
	wxPostEvent(this->GetEventHandler(), wxNotifyEvent(wxID_ANY, id));
}

void MainFrame::OnShow(wxNotifyEvent& event)
{
	this->startGame();
}

void MainFrame::startGame(bool askForSettings)
{
	uint8_t w = 10, h = 10;
	uint16_t m = 10;
	
	if(this->mField != nullptr)
	{
		w = this->mField->size().x;
		h = this->mField->size().y;
		m = this->mField->mines();
	}
	
	if(askForSettings)
	{
		auto dlg = new GameSettingsDialog(this, w, h, m);
		
		w = dlg->fieldWidth();
		h = dlg->fieldHeight();
		m = dlg->mines();
	}
	
	this->mField = new sweepp::Field(w, h, m);
	
	this->SetMinClientSize(wxSize(this->mField->size().x * MinCellSize, this->mField->size().y * MinCellSize + Padding));
	this->Refresh();
	this->SetFocus();
	this->SetFocusFromKbd();
}

void MainFrame::OnPaint(wxPaintEvent& event)
{
	if(this->mField == nullptr) return;
	
    wxAutoBufferedPaintDC dc(this);
    this->PrepareDC(dc);

    wxSize sz = this->GetClientSize();
    int w = sz.GetWidth();
    int h = sz.GetHeight();
	
	this->mGameWon = this->mField->isGameWon();
	this->mGameLost = this->mField->isGameLost();
	
	dc.Clear();
	
	wxBrush brush;
	
	if(this->mGameWon)
	{
		brush.SetColour(wxColour(96, 255, 96));
	}
	else if(this->mGameLost)
	{
		brush.SetColour(wxColour(255, 96, 96));
	}
	else if(this->mDebugEnabled)
	{
		brush.SetColour(this->mDebugColor);
	}
	else
	{
		brush.SetColour(wxColour(211, 211, 211));
	}
	
	dc.SetBrush(brush);
	dc.DrawRectangle(-1, -1, w + 2, h + 2);
	
	sweepp::Point fsz = this->mField->size();
	
	int cellsz = __max(__min(__min(w / fsz.x, (h - Padding) / fsz.y), MaxCellSize), MinCellSize);
	
	int containerWidth = cellsz * fsz.x;
	int containerHeight = cellsz * fsz.y;
	int containerX = (w - containerWidth) / 2;
	int containerY = (h - containerHeight) / 2;
	
	dc.SetPen(*wxBLACK_PEN);
	dc.SetBrush(*wxWHITE_BRUSH);
	
	dc.DrawRectangle(containerX, containerY, containerWidth, containerHeight);
	
	for(uint16_t i = 0; i < fsz.x * fsz.y; i++)
	{
		sweepp::Cell* cell = this->mField->cell(i);
		
		this->drawCell(&dc, cell, containerX, containerY, cellsz);
	}
	
	dc.SetTextForeground(*wxBLACK);
	
	wxRect restart = this->drawRestartButton(&dc, _("[R] - Restart"), false, w, h, containerX, containerY, containerWidth, containerHeight);
	this->drawRestartButton(&dc, _("[^R] - Settings"), true, w, h, restart.x + restart.width + 8, containerY, containerWidth, containerHeight);
	
	dc.DrawText(wxString::Format(_("%i mines"), this->mField->mines()), __max(containerX, 8), containerY - 8 - dc.GetFontMetrics().height);
	
	wxString flags = wxString::Format(_("%i flags"), this->mField->flags());
	wxSize flagsSz = dc.GetTextExtent(flags);
	int flagsX = w - containerX - flagsSz.x;
	dc.DrawText(flags, __min(flagsX, w - flagsSz.x - 8), containerY - 8 - dc.GetFontMetrics().height);
	
	this->mLeftClick = this->mRightClick = false;
	
	if(this->mDebugEnabled)
	{
		this->SetTitle(wxString::Format(_("Swee++ - C++ minesweeper game - [%i, 3BV: %i, openings: %i]"),
		                                this->mField->score(),
		                                this->mField->openingsRequired(),
		                                this->mField->openings()));
	}
}

wxRect MainFrame::drawRestartButton(wxAutoBufferedPaintDC* dc, wxString text, bool askForSettings, int w, int h, int cx, int cy, int cw, int ch)
{
	wxSize sz = dc->GetTextExtent(text);
	
	int rx = __max(cx, 8);
	int ry = cy + ch + 8;
	
	wxRect rect = wxRect(rx - 8, ry - 8, sz.x + 16, sz.y + 16);
	
	dc->SetBrush(wxBrush(wxColour(233, 233, 233)));
	
	if (rect.Contains(this->mMousePos))
	{
		if(mLeftPreClick)
		{
			dc->SetBrush(*wxWHITE_BRUSH);
		}
		
		dc->DrawRectangle(rect);
		
		if(this->mLeftClick)
		{
			this->mLeftClick = this->mRightClick = false;
			this->startGame(askForSettings);
		}
	}
	dc->DrawText(text, rx, ry);
	
	return rect;
}

ColorScheme::Scheme MainFrame::getCellColors(sweepp::Cell* cell, int cx, int cy, int size)
{
	auto stateCell = ColorScheme::State::Cell(cell->type(), cell->state());
	auto stateGame = ColorScheme::State::Game(this->mGameWon, this->mGameLost, {this->mDebugEnabled});
	
	bool normal = true, hover = false, active = false;
	
	if (this->mMousePos.x > cx && this->mMousePos.x < cx + size &&
	    this->mMousePos.y > cy && this->mMousePos.y < cy + size)
	{
		normal = false; hover = true; active = false;
		
		if(this->mLeftPreClick || this->mLeftClick || this->mRightPreClick || this->mRightClick)
		{
			normal = false; hover = false; active = true;
		}
	}
	
	auto stateInteraction = ColorScheme::State::Interaction(normal, hover, active);
	
	return ColorScheme::scheme(stateCell, stateInteraction, stateGame);
}

void MainFrame::drawCell(wxAutoBufferedPaintDC* dc, sweepp::Cell* cell, int startX, int startY, int size)
{
	uint8_t x = cell->pos().x, y = cell->pos().y;
	int cx = startX + x * size, cy = startY + y * size;
	
	wxPen pen;
	wxBrush brush;
	
	ColorScheme::Scheme colors = this->getCellColors(cell, cx, cy, size);
	
	brush.SetColour(colors.background);
	dc->SetBrush(brush);
	pen.SetColour(colors.border);
	dc->SetPen(pen);
	dc->SetTextForeground(colors.text);
	
	dc->DrawRectangle(cx, cy, size, size);
	
	if (this->mMousePos.x > cx && this->mMousePos.x < cx + size &&
	    this->mMousePos.y > cy && this->mMousePos.y < cy + size)
	{
		if(this->mLeftClick)
		{
			this->mField->open(cell->pos());
			this->mLeftClick = this->mRightClick = false;
			this->Refresh();
		}
		else if(this->mRightClick)
		{
			this->mField->flag(cell->pos());
			this->mLeftClick = this->mRightClick = false;
			this->Refresh();
		}
	}
	
	if((this->mDebugEnabled || this->mGameLost || this->mGameWon) && cell->is(sweepp::Cell::Type::Mine))
	{
		int mx = cx + size / 2;
		int my = cy + size / 2;
		
		brush.SetColour(colors.border);
		dc->SetBrush(brush);
		dc->DrawCircle(mx, my, 5);
	}
	else if(cell->in(sweepp::Cell::State::Opened))
	{
		uint8_t mines = this->mField->minesAround(cell);
		if(mines > 0)
		{
			dc->SetTextForeground(ColorScheme::mineCounterColor(mines));
			wxFontMetrics fm = dc->GetFontMetrics();
			dc->DrawText(wxString::FromDouble(mines), cx + (size - fm.averageWidth) / 2, cy + (size - fm.height) / 2);
		}
	}
	else if(cell->in(sweepp::Cell::State::Flagged))
	{
		int fx = cx + (size - 10) / 2;
		int fy = cy + (size - 14) / 2;
		
		dc->DrawLine(fx, fy, fx, fy + 14);
		
		brush.SetColour(colors.border);
		dc->SetBrush(brush);
		dc->DrawRectangle(fx, fy, 10, 8);
	}
}

void MainFrame::OnResize(wxSizeEvent& event)
{
	this->Refresh();
    event.Skip();
}

void MainFrame::OnMouseMove(wxMouseEvent& event)
{
	this->mMousePos = event.GetPosition();
	this->Refresh();
}

void MainFrame::OnMouseLeave(wxMouseEvent& event)
{
	this->mMousePos = wxPoint(-1, -1);
	this->Refresh();
}

void MainFrame::OnLeftPreClick(wxMouseEvent& event)
{
	this->mLeftPreClick = true;
	this->mLeftClick = false;
	this->mMousePos = event.GetPosition();
	this->Refresh();
}

void MainFrame::OnLeftClick(wxMouseEvent& event)
{
	this->mLeftPreClick = false;
	this->mLeftClick = true;
	this->mMousePos = event.GetPosition();
	this->Refresh();
}

void MainFrame::OnRightPreClick(wxMouseEvent& event)
{
	this->mRightPreClick = true;
	this->mRightClick = false;
	this->mMousePos = event.GetPosition();
	this->Refresh();
}

void MainFrame::OnRightClick(wxMouseEvent &event)
{
	this->mRightPreClick = false;
	this->mRightClick = true;
	this->mMousePos = event.GetPosition();
	this->Refresh();
}

void MainFrame::OnKeyUp(wxKeyEvent &event)
{
	switch(event.GetUnicodeKey())
	{
		case 'R':
			this->startGame(event.ControlDown());
			break;
		
		default:
			if(!this->mDebugEnabled && event.ControlDown() && event.AltDown() && event.ShiftDown() && event.GetUnicodeKey() == this->mDebugChars[this->mDebugCurrentChar])
			{
				this->mDebugCurrentChar++;
				
				if(this->mDebugCurrentChar >= wxStrlen(this->mDebugChars))
				{
					this->mDebugEnabled = true;
					std::srand(unsigned(std::time(nullptr)));
					this->mDebugColor = wxColour(std::rand() % 255, std::rand() % 255, std::rand() % 255);
					this->Refresh();
				}
			}
			else
			{
				this->mDebugCurrentChar = 0;
			}
			break;
	}
}