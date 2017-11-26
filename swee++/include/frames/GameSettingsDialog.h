#pragma once

#include <wx/frame.h>
#include <wx/dcbuffer.h>
#include <wx/spinctrl.h>

class GameSettingsDialog: public wxDialog
{
    public:
		GameSettingsDialog(wxWindow* parent, uint8_t fieldWidth = 10, uint8_t fieldHeight = 10, uint16_t mines = 10);
		
		uint8_t fieldWidth();
		uint8_t fieldHeight();
		uint16_t mines();

    private:
		wxSpinCtrl *mFieldWidth, *mFieldHeight, *mMines;
		
		void OnSpinCtrlUpdate(wxSpinEvent& event);

        DECLARE_EVENT_TABLE()
};
