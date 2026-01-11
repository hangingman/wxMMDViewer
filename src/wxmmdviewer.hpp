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
 *      Hiroyuki Nagata <idiotpanzer@gmail.com>
 */

#ifndef WXMMDVIEWER_HPP_
#define WXMMDVIEWER_HPP_

//#include <memory>
#include <fstream>
#include <map>


#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/aui/auibook.h>
#include <wx/dnd.h>
#include <wx/dataobj.h>
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/log.h>

#include <kaitai/kaitaistream.h>
#include "common.hpp"
#include "basicglpane.hpp"
#include "wxmmdutil.hpp"
#include "pmd.h"

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
     BasicGLPane* GetBasicGLPane();

private:
     // すべてのウィジェットが載るAuiマネージャー
     wxAuiManager m_mgr;
     // OpenGL描画用キャンバス
     BasicGLPane* glPane;
     // ログ出力用ウィンドウ
     wxTextCtrl* txtPane;
     // モデル選択メニュー
     wxMenu* m_modelMenu;
     // メニューIDとモデルのパスの対応表
     std::map<int, wxString> m_modelPaths;

     /** レイアウト  */
     void SetProperties();
     void DoLayout();
     void SetAuiPaneInfo();
     void SetAppIcons();
     void OnDropFile(wxDropFilesEvent &event);
     void DrawPMDFile(pmd_t* pmdFile, const wxString& modelPath);
     void LoadModel(const wxString& path);
     void PopulateModelMenu();

     /** イベント */
     void OnClose(wxCloseEvent& event);
     void OnImportModel(wxCommandEvent& event);
     void OnModelSelect(wxCommandEvent& event);

     /** ユーティリティ */
     wxString Dump(const std::string& dump) {
       // TODO
       return wxEmptyString;
       //return wxString(StringToHex(dump).c_str(), wxConvUTF8);
     };

     DECLARE_EVENT_TABLE()
};

#endif /** WXMMDVIEWER_HPP_ */
