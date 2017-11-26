#include "wx_pch.h"
#include "App.h"
#include "frames/MainFrame.h"

IMPLEMENT_APP(App);

BEGIN_EVENT_TABLE(App, wxApp)

END_EVENT_TABLE()

bool App::OnInit()
{
	auto appLocale = new wxLocale(wxLANGUAGE_DEFAULT);
	appLocale->AddCatalogLookupPathPrefix("./locales");
	appLocale->AddCatalog("swee++");
	
    wxInitAllImageHandlers();
	
    this->mFrame = new MainFrame(nullptr);
    this->mFrame->Show();
    SetTopWindow(this->mFrame);
	
    return true;
}