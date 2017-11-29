#pragma once

#include <wx/frame.h>
#include <wx/dcbuffer.h>

#include <libswee++.h>
#include <game/field.h>
#include "ColorScheme.h"
#include "GameSettingsDialog.h"

class MainFrame: public wxFrame
{
    public:
        MainFrame(wxWindow* parent, wxWindowID id = -1);

    private:
		void startGame(bool askForSettings = true);
		
		ColorScheme::Scheme getCellColors(sweepp::Cell* cell, int cx, int cy, int size);
		
		void drawCell(wxAutoBufferedPaintDC* dc, sweepp::Cell* cell, int startX, int startY, int size);
		
		wxRect drawRestartButton(wxAutoBufferedPaintDC* dc, wxString text, bool askForSettings, int w, int h, int cx, int cy, int cw, int ch);
		
		void OnShow(wxNotifyEvent& event);
		
        void OnPaint(wxPaintEvent& event);
        void OnResize(wxSizeEvent& event);

        void OnKeyUp(wxKeyEvent& event);

        void OnMouseMove(wxMouseEvent& event);
        void OnMouseLeave(wxMouseEvent& event);
		
		void OnLeftPreClick(wxMouseEvent& event);
		void OnLeftClick(wxMouseEvent& event);
		void OnRightPreClick(wxMouseEvent& event);
		void OnRightClick(wxMouseEvent& event);
		
		void OnClose(wxCloseEvent &event);
		
		void OnGameWon();
		
		void OnGameLost();
		
		sweepp::Field* mField = nullptr;
		bool mGameWon = false, mGameLost = false;

        wxPoint mMousePos;
		bool mLeftPreClick = false, mLeftClick = false, mRightPreClick = false, mRightClick = false;
		
		bool mDebugEnabled = false;
		const char* mDebugChars = "DEBUG";
		int mDebugCurrentChar = 0;
		wxColour mDebugColor;

        DECLARE_EVENT_TABLE()
};
