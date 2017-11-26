#ifndef WX_PCH_H_INCLUDED
#define WX_PCH_H_INCLUDED

//#define _U(str) wxString::FromUTF8(str)

//#undef _
//#define _(s) wxT(s)

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#ifdef WX_PRECOMP
    // put here all your rarely-changing header files
#endif // WX_PRECOMP

#endif // WX_PCH_H_INCLUDED
