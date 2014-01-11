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

#ifndef WXMMDVIEWER_HPP_
#define WXMMDVIEWER_HPP_

#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/aui/auibook.h>
#include <wx/dnd.h>
#include <wx/dataobj.h>
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/log.h>
#include <clsPMDFile.hpp>
#include "common.hpp"
#include "basicglpane.hpp"
#include "wxmmdutil.hpp"

const wxString mmdModelDir = ::wxGetHomeDir() + wxFS + WXMMD_DIR + wxFS + wxT("model");
const wxString mmdCSVDir   = ::wxGetHomeDir() + wxFS + WXMMD_DIR + wxFS + wxT("csv");

/**
 * メインのクラス
 */
class MMDViewer : public wxFrame 
{

public:
     MMDViewer(const wxString& title);
     ~MMDViewer();

private:
     // すべてのウィジェットが載るAuiマネージャー
     wxAuiManager m_mgr;
     // OpenGL描画用キャンバス
     BasicGLPane* glPane;
     // ログ出力用ウィンドウ
     //wxPanel* logPanel;
     wxTextCtrl* txtPane;
     wxLogTextCtrl* logPane;

     void SetProperties();
     void DoLayout();
     void SetAuiPaneInfo();
     void OnDropFile(wxDropFilesEvent &event);

     DECLARE_EVENT_TABLE()
};

#endif /** WXMMDVIEWER_HPP_ */
