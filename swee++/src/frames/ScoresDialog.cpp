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
	auto grid = new wxGrid(this, wxID_ANY);
	
	grid->CreateGrid(sweepp::ScoresManager::scores()->size(), 7);
	
	grid->SetColLabelValue(0, "Score");
	grid->SetColLabelValue(1, "Field size");
	grid->SetColLabelValue(2, "Mines");
	grid->SetColLabelValue(3, "Openings");
	grid->SetColLabelValue(4, "3BV");
	grid->SetColLabelValue(5, "Opened cells");
	grid->SetColLabelValue(6, "Time");
	
	for(int i = 0; i < sweepp::ScoresManager::scores()->size(); i++)
	{
		sweepp::Field::Score s = sweepp::ScoresManager::scores()->at(i);
		
		grid->SetRowLabelValue(i, wxString::FromDouble(i + 1));
		grid->DisableRowResize(i);
		
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
	this->SetClientSize(grid->GetBestSize());
	
	this->Center();

#ifndef __APPLE__
	this->SetIcon(wxICON(sweepp));
#endif
	
	this->ShowModal();
	
	this->Destroy();
}