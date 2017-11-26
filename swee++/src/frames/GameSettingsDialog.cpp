#include <game/field.h>

#include "wx_pch.h"

#include "frames/GameSettingsDialog.h"

#ifdef __LINUX__
#include "../swee++.xpm"
#endif

BEGIN_EVENT_TABLE(GameSettingsDialog, wxDialog)

END_EVENT_TABLE()

static const int FieldSizeMin = 3;
static const int FieldSizeMax = 100;
static const int MinesMin = 1;
static const float MinesMaxMultiplier = 0.9;

GameSettingsDialog::GameSettingsDialog(wxWindow* parent, uint8_t fieldWidth, uint8_t fieldHeight, uint16_t fieldMines)
	: wxDialog(parent, -1, _("Game settings"))
{
	auto panel = new wxPanel(this, -1);
	
	auto sizer = new wxBoxSizer(wxVERTICAL);
	
	auto fieldSB = new wxStaticBox(panel, -1, _("Field size"), wxPoint(5, 5), wxSize(285, 50));
	auto minesSB = new wxStaticBox(panel, -1, _("Mines"), wxPoint(5, 55), wxSize(285, 50));
	
	bool isWindows = false;
    bool isMacOS = false;
	#ifdef __WIN32__
    isWindows = true;
	#endif
    #ifdef __APPLE__
    isMacOS = true;
    #endif
	
	int spinCtrlY = isWindows ? 20 : isMacOS ? 4 : 0;
	
	this->mFieldWidth = new wxSpinCtrl(fieldSB, -1, wxEmptyString, wxPoint(10, spinCtrlY), wxDefaultSize, wxSP_ARROW_KEYS, FieldSizeMin, FieldSizeMax, fieldWidth);
	this->mFieldHeight = new wxSpinCtrl(fieldSB, -1, wxEmptyString, wxPoint(145, spinCtrlY), wxDefaultSize, wxSP_ARROW_KEYS, FieldSizeMin, FieldSizeMax, fieldHeight);
	
	this->mFieldWidth->SetSize(130, this->mFieldWidth->GetSize().y);
	this->mFieldHeight->SetSize(130, this->mFieldHeight->GetSize().y);
	
	this->mMines = new wxSpinCtrl(minesSB, -1, wxEmptyString, wxPoint(10, spinCtrlY), wxDefaultSize, wxSP_ARROW_KEYS, MinesMin, static_cast<int>(fieldWidth * fieldHeight * MinesMaxMultiplier), fieldMines);
	
	this->mMines->SetSize(265, this->mMines->GetSize().y);
	
	sizer->Add(panel, 1);
	sizer->Add(this->CreateButtonSizer(wxOK), 1, wxALIGN_RIGHT | wxDOWN | wxRIGHT, isWindows ? 10 : 0);
	
	this->SetSizer(sizer);
	
	this->SetClientSize(295, 150);
	this->Center();

	#ifndef __APPLE__
	this->SetIcon(wxICON(sweepp));
	#endif
	
	this->mFieldWidth->Bind(wxEVT_SPINCTRL, &GameSettingsDialog::OnSpinCtrlUpdate, this);
	this->mFieldHeight->Bind(wxEVT_SPINCTRL, &GameSettingsDialog::OnSpinCtrlUpdate, this);

	this->ShowModal();
	
	this->Destroy();
}

uint8_t GameSettingsDialog::fieldWidth()
{
	return static_cast<uint8_t>(this->mFieldWidth->GetValue());
}

uint8_t GameSettingsDialog::fieldHeight()
{
	return static_cast<uint8_t>(this->mFieldHeight->GetValue());
}

uint16_t GameSettingsDialog::mines()
{
	return static_cast<uint16_t>(this->mMines->GetValue());
}

void GameSettingsDialog::OnSpinCtrlUpdate(wxSpinEvent& event)
{
	this->mMines->SetRange(MinesMin, static_cast<int>(this->mFieldWidth->GetValue() * this->mFieldHeight->GetValue() * MinesMaxMultiplier));
}
