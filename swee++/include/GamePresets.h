#pragma once

#include "wx_pch.h"

class GamePresets
{
	public:
		struct Preset
		{
			int menuID = wxID_NONE;
			wxString name;
			
			uint8_t fieldWidth = 0;
			uint8_t fieldHeight = 0;
			uint16_t mines = 0;
			
			Preset()
			{}
			
			Preset(int menuID, const wxString &name, uint8_t fieldWidth, uint8_t fieldHeight, uint16_t mines)
					: menuID(wxID_HIGHEST + 10 + menuID), name(name), fieldWidth(fieldWidth), fieldHeight(fieldHeight), mines(mines)
			{}
		};
		
		inline static Preset presets[] =
				{
						Preset(0, _("Beginner"), 10, 10, 10),
						Preset(1, _("Skilled"), 10, 10, 20),
						Preset(2, _("Hardcore"), 10, 10, 50),
						
						Preset(),
						
						Preset(3, _("Experienced"), 26, 18, 50),
						Preset(4, _("No mistakes"), 26, 18, 100),
						
						Preset(),
						
						Preset(5, _("Pro"), 40, 26, 100),
						Preset(6, _("Impossible"), 40, 26, 150),
						Preset(7, _(_U("¯\\_(ツ)_/¯")), 40, 26, 300)
				};
};