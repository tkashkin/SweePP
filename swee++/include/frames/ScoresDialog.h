#pragma once

#include <wx/frame.h>
#include <wx/dcbuffer.h>
#include <wx/spinctrl.h>

class ScoresDialog: public wxDialog
{
	public:
		static void show(wxWindow* parent, int pos = -1);
	
	private:
		ScoresDialog(wxWindow* parent, int pos = -1);
		
		void setupGrid(int pos);
	
	DECLARE_EVENT_TABLE()
};
