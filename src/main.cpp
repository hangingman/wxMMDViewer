/* wxMMDViewer - VMD file or PMD file viewer with wxWidgets and OpenGL
 * Copyright (C) 2014 Hiroyuki Nagata
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Contributor:
 *	Hiroyuki Nagata <newserver002@gmail.com>
 */

#include <wx/wx.h>
#include <wx/snglinst.h>
#include "wxmmdviewer.hpp"

/*
 * wxAppを継承したwxMainを宣言
 */
class wxMain: public wxApp {

  wxLocale m_Locale;

public:
  wxMain() : m_Locale(wxLANGUAGE_DEFAULT){}
  virtual bool OnInit();
  virtual int OnExit();

private:
  wxSingleInstanceChecker* m_checker;
  MMDViewer* wxMMDViewer;
};

/**
 * wxMainの実装
 */
bool wxMain::OnInit() 
{
    if (!wxApp::OnInit())
	 return false;

    m_checker = new wxSingleInstanceChecker(name);
    if ( m_checker->IsAnotherRunning()) 
    {
	 wxMessageBox(wxT("誤作動防止のためwxMMDViewerは複数起動できません。終了します。"), 
		      wxT("wxMMDViewer起動"), wxOK | wxICON_ERROR);
	 return false;
    }

    wxMMDViewer = new MMDViewer(NULL, wxID_ANY, wxEmptyString);
    SetTopWindow(wxMMDViewer);
    wxMMDViewer->Show();
     
    return true;
}

/**
 * 終了後の後始末
 */
int wxMain::OnExit() {
     return 0;
}
