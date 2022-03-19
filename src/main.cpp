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
 *	Hiroyuki Nagata <idiotpanzer@gmail.com>
 */

#include "main.hpp"

IMPLEMENT_APP(wxMain)

/**
 * wxMainの実装
 */
bool wxMain::OnInit() 
{
    if (!wxApp::OnInit())
	 return false;

    const wxString name = wxString::Format(_("wxMMDViewer-%s"), wxGetUserId().c_str());
    m_checker = new wxSingleInstanceChecker(name);
    if ( m_checker->IsAnotherRunning()) 
    {
	 wxMessageBox(wxT("誤作動防止のためwxMMDViewerは複数起動できません。終了します。"), 
		      wxT("wxMMDViewer起動"), wxOK | wxICON_ERROR);
	 return false;
    }

    render_loop_on = false;
    wxMMDViewer = new MMDViewer(wxT("wxMMDViewer"));
    SetTopWindow(wxMMDViewer);
    wxMMDViewer->Show();
     
    ActivateRenderLoop(true);
    return true;
}

/**
 * 終了後の後始末
 */
int wxMain::OnExit() {
     return 0;
}

void wxMain::ActivateRenderLoop(bool on)
{
     if(on && !render_loop_on)
     {
	  Connect( wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(wxMain::OnIdle) );
	  render_loop_on = true;
     }
     else if(!on && render_loop_on)
     {
	  Disconnect( wxEVT_IDLE, wxIdleEventHandler(wxMain::OnIdle) );
	  render_loop_on = false;
     }
}

void wxMain::OnIdle(wxIdleEvent& event)
{
     if(render_loop_on)
     {
	  wxThread::Sleep(5);
	  wxMMDViewer->GetBasicGLPane()->Refresh();
	  event.RequestMore(); // render continuously, not only once on idle
     }
}

/**
 * キーイベントをここで判断する
 */
int wxMain::FilterEvent(wxEvent& event) 
{

     if (event.GetEventType() == wxEVT_KEY_DOWN)
     {
	  wxLogDebug(wxT("Key pressed."));
	  wxMMDViewer->GetBasicGLPane()->KeyPressed(((wxKeyEvent&)event));
	  return true;
     }
 
     return -1;
}
