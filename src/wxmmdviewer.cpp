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
 *  Hiroyuki Nagata <idiotpanzer@gmail.com>
 */

#include "wxmmdviewer.hpp"
#include "main.hpp"
#include <iostream>

enum {
    ID_IMPORT_MODEL = wxID_HIGHEST + 100,
    ID_MODEL_START  = wxID_HIGHEST + 200
};

BEGIN_EVENT_TABLE(MMDViewer, wxFrame)
//EVT_CLOSE(MMDViewer::OnClose)
EVT_MENU(ID_IMPORT_MODEL, MMDViewer::OnImportModel)
EVT_MENU_RANGE(ID_MODEL_START, ID_MODEL_START + 1000, MMDViewer::OnModelSelect)
//EVT_DROP_FILES(MMDViewer::OnDropFile)
END_EVENT_TABLE()

#include "importdialog.hpp"

BasicGLPane* MMDViewer::GetBasicGLPane() {
    return glPane;
}

MMDViewer::MMDViewer(const wxString& title) : wxFrame(NULL, wxID_ANY, title) {

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

    // メニューバーを設定
    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(ID_IMPORT_MODEL, wxT("モデルをインポート...\tCtrl+I"));
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, wxT("終了\tAlt+X"));
    menuBar->Append(fileMenu, wxT("ファイル"));

    m_modelMenu = new wxMenu;
    menuBar->Append(m_modelMenu, wxT("モデルを選択"));

    SetMenuBar(menuBar);

    // 各種GUI設定を行う
    SetProperties();       // 前回までの設定を読み出す
    DoLayout();            // 実際にレイアウトに展開する

    PopulateModelMenu();

    // DnDできるように設定する
    // this->DragAcceptFiles(true);

    // ステータスバーを設定する
    CreateStatusBar(2);
    SetStatusText(wxT("完了"));
    wxLogMessage(wxT("レイアウト完了..."));
}

/**
 * SetProperties
 * 前回からのデータ引継ぎ等の処理を行う。
 */
void MMDViewer::SetProperties() {

    SetTitle(_("wxMMDViewer"));
    SetSize(wxSize(960, 640));

    const wxString wxmmd = ::wxGetHomeDir() + wxFS + WXMMD_DIR;
    wxDir wxMMDDir(wxmmd);

    /**
     * 必要な隠しディレクトリを作成する
     */
    if (!wxDir(::wxGetHomeDir()).HasSubDirs(WXMMD_DIR)) {
        ::wxMkdir(wxmmd);

        if (!wxMMDDir.HasSubDirs(wxT("model"))) {
            ::wxMkdir(wxmmd + wxFS + wxT("model"));
        }

        if (!wxMMDDir.HasSubDirs(wxT("csv"))) {
            ::wxMkdir(wxmmd + wxFS + wxT("csv"));
        }
    }
}

/**
 * DoLayout
 * ユーザーが触る前のアプリのレイアウトを設定する
 * 前回の起動時にレイアウトに変更があった場合はそれを反映する
 */
void MMDViewer::DoLayout() {
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
void MMDViewer::SetAuiPaneInfo() {
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
    glCanvas.BestSize(30, 30);
    logWindow.MaxSize(30, 30);

    // OpenGL描画用キャンバスを載せる
    m_mgr.AddPane(glPane, glCanvas);
    m_mgr.AddPane(txtPane, logWindow);
    m_mgr.Update();
}

/**
 * デストラクタ
 */
MMDViewer::~MMDViewer() {
    // Auiマネージャーを削除する
    m_mgr.UnInit();
}

/**
 * ファイルがドラッグ＆ドロップされた時の処理
 */
void MMDViewer::OnDropFile(wxDropFilesEvent &event) {
    int nFiles = event.GetNumberOfFiles();
    const wxString* filenames = event.GetFiles();

    for ( int n = 0; n < nFiles; n++ ) {

        if (!wxFile::Exists(filenames[n])) {
            continue;
        }

        wxFileName file(filenames[n]);
        wxString ext      = file.GetExt();
        wxString filename = file.GetName();

        if ( filenames[n] != wxEmptyString && ext == wxT("vmd") ) {
            const wxString outputPath = mmdModelDir + wxFS + filename + wxFS + filename + wxT(".csv");
            ::wxMkdir(mmdModelDir + wxFS + filename);
            wxMMDViewerUtil::VMD2CSV( filenames[n].mb_str(), outputPath.mb_str() );
        } else if ( filenames[n] != wxEmptyString && ext == wxT("csv") ) {
            const wxString outputPath = mmdCSVDir + wxFS + filename + wxFS + filename + wxT(".vmd");
            ::wxMkdir(mmdCSVDir + wxFS + filename);
            wxMMDViewerUtil::CSV2VMD( filenames[n].mb_str(), outputPath.mb_str() );
        } else if ( filenames[n] != wxEmptyString && ext == wxT("pmd") ) {
            LoadModel(filenames[n]);
        }
    }
}

void MMDViewer::LoadModel(const wxString& path) {
    std::ifstream ifs(path.fn_str(), std::ifstream::binary);
    if (!ifs.is_open()) {
        wxLogError(wxT("Failed to open file: %s"), path.c_str());
        return;
    }
    kaitai::kstream ks(&ifs);

    try {
        auto pmd = std::make_unique<pmd_t>(&ks);

        // 内部のデータをトレースする
        wxLogMessage(wxT("ファイル: %s 読み込み"), path.c_str());
        wxLogMessage(wxT("Version: %f"), pmd->header()->version());
        wxLogMessage(wxT("モデル名: %s"), wxString::FromUTF8(pmd->header()->model_name().c_str()));
        wxLogMessage(wxT("コメント: %s"), wxString::FromUTF8(pmd->header()->comment().c_str()));

        wxLogMessage(wxT("vertex size: %u"), pmd->vertex()->num_vertex());
        wxLogMessage(wxT("face-vertex size: %u"), pmd->face_vertex()->num_face_vert_index());
        wxLogMessage(wxT("material size: %u"), pmd->material()->num_material());

        DrawPMDFile(pmd.get());

    } catch (const std::exception& e) {
        wxLogMessage(wxT("実行時例外: %s"), e.what());
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        wxLogMessage(wxT("未知の例外が発生しました"));
        std::cerr << "Unknown exception occurred" << std::endl;
    }
}

void MMDViewer::OnImportModel(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, wxT("インポートするPMDファイルを選択"), wxEmptyString, wxEmptyString,
                                 wxT("PMD files (*.pmd)|*.pmd"),
                                 wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_OK) {
        wxString pmdPath = openFileDialog.GetPath();
        ImportDialog dialog(this, pmdPath);
        if (dialog.ShowModal() == wxID_OK) {
            wxString importedPath = dialog.GetImportedPath();
            LoadModel(importedPath);
            PopulateModelMenu(); // Re-populate menu after import
        }
    }
}

void MMDViewer::PopulateModelMenu() {
    // Clear existing menu items
    wxMenuItemList& items = m_modelMenu->GetMenuItems();
    while (!items.IsEmpty()) {
        m_modelMenu->Delete(items.GetFirst()->GetData());
    }
    m_modelPaths.clear();

    const wxString modelRoot = ::wxGetHomeDir() + wxFS + WXMMD_DIR + wxFS + wxT("model");
    if (!wxDir::Exists(modelRoot)) return;

    wxDir dir(modelRoot);
    if (!dir.IsOpened()) return;

    wxString subDirName;
    bool cont = dir.GetFirst(&subDirName, wxEmptyString, wxDIR_DIRS);
    int menuId = ID_MODEL_START;

    while (cont) {
        wxString subDirPath = modelRoot + wxFS + subDirName;
        wxDir subDir(subDirPath);
        if (subDir.IsOpened()) {
            wxString pmdFileName;
            bool foundPmd = subDir.GetFirst(&pmdFileName, wxT("*.pmd"), wxDIR_FILES);
            if (foundPmd) {
                wxString pmdPath = subDirPath + wxFS + pmdFileName;
                m_modelMenu->Append(menuId, subDirName);
                m_modelPaths[menuId] = pmdPath;
                menuId++;
                if (menuId >= ID_MODEL_START + 1000) break; // Limit to 1000 models
            }
        }
        cont = dir.GetNext(&subDirName);
    }
}

void MMDViewer::OnModelSelect(wxCommandEvent& event) {
    int id = event.GetId();
    if (m_modelPaths.count(id)) {
        LoadModel(m_modelPaths[id]);
    }
}

void MMDViewer::DrawPMDFile(pmd_t* pmdFile) {
     glPane->SetPMDData(pmdFile);
}

void MMDViewer::OnClose(wxCloseEvent& event) {
     wxAppConsole* app = wxAppConsole::GetInstance();
     if ( wxMain* main = dynamic_cast<wxMain*>(app) ) {
        main->ActivateRenderLoop(false);
        event.Skip(); // don't stop event, we still want window to close
     }
}
