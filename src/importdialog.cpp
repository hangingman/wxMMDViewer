#include "importdialog.hpp"
#include <wx/filename.h>

BEGIN_EVENT_TABLE(ImportDialog, wxDialog)
    EVT_BUTTON(ID_BROWSE, ImportDialog::OnBrowse)
    EVT_BUTTON(wxID_OK, ImportDialog::OnImport)
END_EVENT_TABLE()

ImportDialog::ImportDialog(wxWindow* parent, const wxString& pmdPath)
    : wxDialog(parent, wxID_ANY, wxT("モデルのインポート"), wxDefaultPosition, wxSize(700, 500), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
      m_pmdPath(pmdPath) {
    
    m_textures = MMDImporter::AnalyzePMD(pmdPath);
    InitUI();
    UpdateList();
}

void ImportDialog::InitUI() {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, wxT("モデルファイル: ") + m_pmdPath), 0, wxALL, 10);

    m_listCtrl = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    m_listCtrl->InsertColumn(0, wxT("状態"), wxLIST_FORMAT_LEFT, 100);
    m_listCtrl->InsertColumn(1, wxT("必要ファイル"), wxLIST_FORMAT_LEFT, 200);
    m_listCtrl->InsertColumn(2, wxT("参照先パス"), wxLIST_FORMAT_LEFT, 400);

    mainSizer->Add(m_listCtrl, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);

    wxBoxSizer* btnSizerRow = new wxBoxSizer(wxHORIZONTAL);
    btnSizerRow->Add(new wxButton(this, ID_BROWSE, wxT("選択したファイルを参照...")), 0, wxALL, 5);
    mainSizer->Add(btnSizerRow, 0, wxALIGN_RIGHT | wxRIGHT, 10);

    wxSizer* stdBtnSizer = CreateButtonSizer(wxOK | wxCANCEL);
    m_btnImport = (wxButton*)FindWindowById(wxID_OK, this);
    if (m_btnImport) {
        m_btnImport->SetLabel(wxT("インポート実行"));
    }
    
    mainSizer->Add(stdBtnSizer, 0, wxALIGN_RIGHT | wxALL, 10);

    SetSizer(mainSizer);
    UpdateImportButton();
}

void ImportDialog::UpdateList() {
    m_listCtrl->DeleteAllItems();
    for (size_t i = 0; i < m_textures.size(); ++i) {
        long item = m_listCtrl->InsertItem(i, m_textures[i].exists ? wxT("✅ OK") : wxT("⚠️ Missing"));
        m_listCtrl->SetItem(item, 1, m_textures[i].internalPath);
        m_listCtrl->SetItem(item, 2, m_textures[i].sourcePath);
    }
}

void ImportDialog::OnBrowse(wxCommandEvent& event) {
    long selected = m_listCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selected == -1) {
        wxMessageBox(wxT("リストから項目を選択してください。"), wxT("確認"), wxOK | wxICON_INFORMATION);
        return;
    }

    wxFileName fn(m_textures[selected].internalPath);
    wxFileDialog openFileDialog(this, wxT("テクスチャファイルを選択"), wxEmptyString, fn.GetFullName(),
                                 wxT("画像ファイル (*.bmp;*.png;*.tga;*.jpg)|*.bmp;*.png;*.tga;*.jpg"),
                                 wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_OK) {
        m_textures[selected].sourcePath = openFileDialog.GetPath();
        m_textures[selected].exists = true;
        UpdateList();
        UpdateImportButton();
    }
}

void ImportDialog::UpdateImportButton() {
    bool allOk = true;
    for (const auto& tex : m_textures) {
        if (!tex.exists) {
            allOk = false;
            break;
        }
    }
    if (m_btnImport) {
        m_btnImport->Enable(allOk);
    }
}

void ImportDialog::OnImport(wxCommandEvent& event) {
    if (MMDImporter::ExecuteImport(m_pmdPath, m_textures, m_importedPath)) {
        EndModal(wxID_OK);
    } else {
        wxMessageBox(wxT("インポートに失敗しました。"), wxT("エラー"), wxOK | wxICON_ERROR);
    }
}
