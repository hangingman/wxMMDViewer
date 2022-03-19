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

#include "wxmmdviewer.hpp"
#include "main.hpp"

BEGIN_EVENT_TABLE(MMDViewer, wxFrame)
    //EVT_CLOSE(MMDViewer::OnClose)
    EVT_DROP_FILES(MMDViewer::OnDropFile)
END_EVENT_TABLE()

BasicGLPane* MMDViewer::GetBasicGLPane()
{
     return glPane;
}

MMDViewer::MMDViewer(const wxString& title) : wxFrame(NULL, wxID_ANY, title)
{

     // Auiマネージャーがどのフレームを管理するか示す
     m_mgr.SetManagedWindow(this);

     // OpenGL用描画用キャンバス
     glPane = new BasicGLPane(this, 0);
     // ログ出力用ウィンドウ
     txtPane = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
			      wxDefaultPosition,
			      wxDefaultSize,
			      wxTE_MULTILINE | wxTE_READONLY);
     wxLog::SetActiveTarget(new wxLogTextCtrl(txtPane));

     // 各種GUI設定を行う
     SetProperties();       // 前回までの設定を読み出す
     DoLayout();            // 実際にレイアウトに展開する

     // DnDできるように設定する
     this->DragAcceptFiles(true);

     // ステータスバーを設定する
     CreateStatusBar(2);
     SetStatusText(wxT("完了"));
     wxLogMessage(wxT("レイアウト完了..."));
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
     // レイアウトの設定
     SetAuiPaneInfo();
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
     glCanvas.CloseButton(false);
     glCanvas.Caption(wxT("ビュー部"));
     glCanvas.BestSize(400, 400);
     glCanvas.Center();

     // ログ出力用ウィンドウ
     wxAuiPaneInfo logWindow;
     logWindow.Name(wxT("wxLogWindow"));
     logWindow.Bottom();
     logWindow.CloseButton(true);
     logWindow.Caption(wxT("ログ表示"));
     logWindow.MaxSize(20, 20);

     // OpenGL描画用キャンバスを載せる
     m_mgr.AddPane(glPane, glCanvas);
     m_mgr.AddPane(txtPane, logWindow);
     m_mgr.Update();
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
	       else if ( filenames[n] != wxEmptyString && ext == wxT("pmd") )
	       {
		 /**
		    clsPMDFile pmdFile;
		    const bool openIsSuccess = pmdFile.Open(filenames[n].mb_str());

		    // 内部のデータをトレースする
		    const wxString ret = openIsSuccess ? wxT("成功") : wxT("失敗");
		    wxLogMessage(wxT("ファイル: %s 読み込み %s"), filenames[n].c_str(), ret.c_str());
		    wxLogMessage(wxT("Version: %f"), pmdFile.GetVersion());
		    wxLogMessage(wxT("Header1: %s"), pmdFile.GetHeaderString1());
		    // ActorにはShift_JISのモデル名が入るので文字コード変換が必要
		    const std::string input(pmdFile.GetActor());
		    const std::string output = babel::sjis_to_utf8(input);

/**
		    wxLogMessage(wxT("Actor: %s"), output.c_str());
		    wxLogMessage(wxT("VertexChunkSize: %lu"), pmdFile.GetVertexChunkSize());
		    wxLogMessage(wxT("GetIndexChunkSize: %lu"), pmdFile.GetIndexChunkSize());
		    wxLogMessage(wxT("BoneChunkSize: %lu"), pmdFile.GetBoneChunkSize());
		    wxLogMessage(wxT("IKChunkSize: %lu"), pmdFile.GetIKChunkSize());
		    wxLogMessage(wxT("MaterialChunkSize: %lu"), pmdFile.GetMaterialChunkSize());
		    wxLogMessage(wxT("MorpChunkSize: %lu"), pmdFile.GetMorpChunkSize());
		    wxLogMessage(wxT("CtrlChunkSize: %lu"), pmdFile.GetCtrlChunkSize());
		    wxLogMessage(wxT("GrpNameChunkSize: %lu"), pmdFile.GetGrpNameChunkSize());
		    wxLogMessage(wxT("GrpChunkSize: %lu"), pmdFile.GetGrpChunkSize());
		    // PMDファイルをwxGLCanvasに投入する
		    DrawPMDFile(pmdFile);
		    */
	       }
	  }
     }
}

//void MMDViewer::DrawPMDFile(clsPMDFile& pmdFile)
//{
//     glPane->SetPMDFile(pmdFile);
//}
//
//void MMDViewer::OnClose(wxCloseEvent& event)
//{
//     wxAppConsole* app = wxAppConsole::GetInstance();
//     if ( wxMain* main = dynamic_cast<wxMain*>(app) )
//     {
// 	  main->ActivateRenderLoop(false);
// 	  event.Skip(); // don't stop event, we still want window to close
//     }
//}
