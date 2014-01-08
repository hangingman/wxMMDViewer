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

#include "wxmmdviewer.hpp"

BEGIN_EVENT_TABLE(MMDViewer, wxFrame)
    EVT_DROP_FILES(MMDViewer::OnDropFile)
END_EVENT_TABLE()

MMDViewer::MMDViewer(const wxString& title) : wxFrame(NULL, wxID_ANY, title)
{
     // 各種GUI設定を行う
     SetProperties();       // 前回までの設定を読み出す
     DoLayout();            // 実際にレイアウトに展開する

     // OpenGL用描画用キャンバス
     glPane = new BasicGLPane(this, 0);
     // DnDできるように設定する
     this->DragAcceptFiles(true);

     // ステータスバーを設定する
     CreateStatusBar(2);
     SetStatusText(wxT("完了"));
}

/**
 * SetProperties
 * 前回からのデータ引継ぎ等の処理を行う。
 */
void MMDViewer::SetProperties() 
{
     SetTitle(_("wxMMDViewer"));
     SetSize(wxSize(960, 640));

     const wxString wxmmd = ::wxGetHomeDir() + wxFS + WXMMD_DIR;
     wxDir wxMMDDir(wxmmd);

     /**
      * 必要な隠しディレクトリを作成する
      */
     if (!wxDir(::wxGetHomeDir()).HasSubDirs(WXMMD_DIR))
     {
	  ::wxMkdir(wxmmd);

	  if (!wxMMDDir.HasSubDirs(wxT("model")))
	  {
	       ::wxMkdir(wxmmd + wxFS + wxT("model"));
	  }

	  if (!wxMMDDir.HasSubDirs(wxT("csv")))
	  {
	       ::wxMkdir(wxmmd + wxFS + wxT("csv"));
	  }
     }
}

/**
 * DoLayout
 * ユーザーが触る前のアプリのレイアウトを設定する
 * 前回の起動時にレイアウトに変更があった場合はそれを反映する
 */
void MMDViewer::DoLayout() 
{
     // Auiマネージャーがどのフレームを管理するか示す
     m_mgr.SetManagedWindow(this);
     // Auiマネージャーの設定を反映する
     m_mgr.Update();
     // 初期設定はこのLayout()が呼ばれる前に行わなくてはいけない
     Layout();
}

/**
 * SetAuiPaneInfo
 * AuiManagerのPaneInfoを設定する
 */
void MMDViewer::SetAuiPaneInfo() 
{     
     // OpenGL描画用キャンバス
     wxAuiPaneInfo glCanvas;
     glCanvas.Name(wxT("wxGLCanvas"));
     glCanvas.Top();
     glCanvas.CloseButton(false);

     // OpenGL描画用キャンバスを載せる
     m_mgr.AddPane(glPane, glCanvas);
}

/**
 * デストラクタ
 */
MMDViewer::~MMDViewer() 
{
     // Auiマネージャーを削除する
     m_mgr.UnInit();
}

/**
 * ファイルがドラッグ＆ドロップされた時の処理
 */
void MMDViewer::OnDropFile(wxDropFilesEvent &event)
{
     int nFiles = event.GetNumberOfFiles();
     const wxString* filenames = event.GetFiles();

     for ( int n = 0; n < nFiles; n++ )
     {
	  if (wxFile::Exists(filenames[n]))
	  {
	       wxFileName file(filenames[n]);
	       wxString ext      = file.GetExt();
	       wxString filename = file.GetName();

	       if ( filenames[n] != wxEmptyString && ext == wxT("vmd") )
	       {
		    const wxString outputPath = mmdModelDir + wxFS + filename + wxFS + filename + wxT(".csv");
		    ::wxMkdir(mmdModelDir + wxFS + filename);
		    wxMMDViewerUtil::VMD2CSV( filenames[n].mb_str(), outputPath.mb_str() );
	       } 
	       else if ( filenames[n] != wxEmptyString && ext == wxT("csv") )
	       {
		    const wxString outputPath = mmdCSVDir + wxFS + filename + wxFS + filename + wxT(".vmd");
		    ::wxMkdir(mmdCSVDir + wxFS + filename);
		    wxMMDViewerUtil::CSV2VMD( filenames[n].mb_str(), outputPath.mb_str() );
	       }
	  }
     }
}
