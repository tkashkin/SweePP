#pragma once
#include <wx/app.h>
#include "frames/MainFrame.h"

class App : public wxApp
{
    public:
        virtual bool OnInit();
	
	private:
		MainFrame* mFrame;
		
		DECLARE_EVENT_TABLE()
};

