#ifndef IMPORT_DIALOG_HPP
#define IMPORT_DIALOG_HPP

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "mmdimporter.hpp"

class ImportDialog : public wxDialog {
public:
    ImportDialog(wxWindow* parent, const wxString& pmdPath);

    wxString GetImportedPath() const { return m_importedPath; }

private:
    void InitUI();
    void UpdateList();
    void OnBrowse(wxCommandEvent& event);
    void OnImport(wxCommandEvent& event);
    void UpdateImportButton();

    wxString m_pmdPath;
    wxString m_importedPath;
    std::vector<MMDImporter::TextureInfo> m_textures;

    wxListCtrl* m_listCtrl;
    wxButton* m_btnImport;

    enum {
        ID_BROWSE = wxID_HIGHEST + 1
    };

    DECLARE_EVENT_TABLE()
};

#endif
