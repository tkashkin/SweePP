#include <game/field.h>

#include "wx_pch.h"
#include <wx/grid.h>
#include <scoresmanager.h>

#include "frames/ScoresDialog.h"

#ifdef __LINUX__
#include "../swee++.xpm"
#endif

BEGIN_EVENT_TABLE(ScoresDialog, wxDialog)

END_EVENT_TABLE()

ScoresDialog::ScoresDialog(wxWindow* parent, int pos)
		: wxDialog(parent, -1, _("Scores"))
{
	this->setupGrid(pos);
	
	this->Center();

#ifndef __APPLE__
	this->SetIcon(wxICON(sweepp));
#endif
	
	this->ShowModal();
	
	this->Destroy();
}

void ScoresDialog::setupGrid(int pos)
{
	auto grid = new wxGrid(this, wxID_ANY);
	
	grid->CreateGrid(sweepp::ScoresManager::scores()->size(), 7);
	
	grid->SetColLabelValue(0, _("Score"));
	grid->SetColLabelValue(1, _("Field size"));
	grid->SetColLabelValue(2, _("Mines"));
	grid->SetColLabelValue(3, _("Openings"));
	grid->SetColLabelValue(4, _("3BV"));
	grid->SetColLabelValue(5, _("Opened cells"));
	grid->SetColLabelValue(6, _("Time"));
	
	grid->SetRowLabelSize(32);
	
	grid->SetSelectionMode(wxGrid::wxGridSelectRows);
	
	grid->SetDefaultCellAlignment(wxALIGN_CENTER, wxALIGN_CENTER);
	
	for(int i = 0; i < sweepp::ScoresManager::scores()->size(); i++)
	{
		sweepp::Field::Score s = sweepp::ScoresManager::scores()->at(i);
		
		grid->SetRowLabelValue(i, wxString::FromDouble(i + 1));
		grid->DisableRowResize(i);
		
		grid->SetCellBackgroundColour(i, 0, wxColour(172, 255, 172));
		
		grid->SetCellValue(i, 0, wxString::FromDouble(s()));
		grid->SetCellValue(i, 1, wxString::Format("%i x %i", s.fieldSize.x, s.fieldSize.y));
		grid->SetCellValue(i, 2, wxString::FromDouble(s.mines));
		grid->SetCellValue(i, 3, wxString::FromDouble(s.openings));
		grid->SetCellValue(i, 4, wxString::FromDouble(s.openingsRequired));
		grid->SetCellValue(i, 5, wxString::FromDouble(s.openedCells));
		grid->SetCellValue(i, 6, wxString::FromDouble(s.time));
		
		for(int j = 0; j < 7; j++)
		{
			grid->SetReadOnly(i, j);
			grid->DisableColResize(j);
		}
		
		if(i == pos)
		{
			grid->SelectRow(i);
		}
	}
	
	grid->InvalidateBestSize();
	grid->Fit();
	
	this->SetClientSize(wxSize(grid->GetBestSize().x + 20, __min(grid->GetBestSize().y, 768)));
}

void ScoresDialog::show(wxWindow* parent, int pos)
{
	if(!sweepp::ScoresManager::scores()->empty())
	{
		new ScoresDialog(parent, pos);
	}
	else
	{
		wxMessageBox(_("No scores saved"), _("Scores"));
	}
}
