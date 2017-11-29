#pragma once

#include <wx/frame.h>
#include <wx/dcbuffer.h>
#include <wx/spinctrl.h>

class ScoresDialog: public wxDialog
{
	public:
		ScoresDialog(wxWindow* parent, int pos = -1);
	
	private:
	DECLARE_EVENT_TABLE()
};
