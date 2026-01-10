#include "mmdimporter.hpp"
#include "../vmdconv/clsPMDFile.hpp"
#include "common.hpp"
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/file.h>

std::vector<MMDImporter::TextureInfo> MMDImporter::AnalyzePMD(const wxString& pmdPath) {
    std::vector<TextureInfo> result;
    clsPMDFile pmd;
    if (!pmd.Open(pmdPath.fn_str())) {
        return result;
    }

    std::vector<std::string> paths = pmd.GetTexturePaths();
    wxFileName pmdFile(pmdPath);
    wxString baseDir = pmdFile.GetPathWithSep();

    for (const auto& p : paths) {
        TextureInfo info;
        // PMD internal path might use backslashes from Windows.
        // Convert to platform separator.
        wxString internalPath = wxString::FromUTF8(p.c_str());
        internalPath.Replace(wxT("\\"), wxFS); 
        
        info.internalPath = internalPath;
        
        // Attempt to auto-detect relative to the PMD file location
        wxString fullPath = baseDir + internalPath;
        wxString actualPath = FindFileCaseInsensitive(fullPath);
        
        if (!actualPath.IsEmpty()) {
            info.sourcePath = actualPath;
            info.exists = true;
        } else {
            info.sourcePath = wxEmptyString;
            info.exists = false;
        }
        result.push_back(info);
    }

    return result;
}

bool MMDImporter::ExecuteImport(const wxString& pmdSourcePath, 
                               const std::vector<TextureInfo>& textures,
                               wxString& outImportedPath) {
    wxFileName pmdFile(pmdSourcePath);
    wxString modelName = pmdFile.GetName();
    
    // Ensure base directory exists: ~/.wxmmd/model/
    wxString modelRoot = ::wxGetHomeDir() + wxFS + WXMMD_DIR + wxFS + wxT("model");
    if (!wxDir::Exists(modelRoot)) {
        if (!wxDir::Make(modelRoot)) return false;
    }

    // Determine target directory: ~/.wxmmd/model/<modelName>
    // Avoid collisions by adding a numeric suffix if needed.
    wxString targetDir = modelRoot + wxFS + modelName;
    int suffix = 1;
    while (wxDir::Exists(targetDir)) {
        targetDir = modelRoot + wxFS + modelName + wxString::Format(wxT("_%d"), suffix++);
    }

    if (!wxDir::Make(targetDir)) {
        return false;
    }

    // 1. Copy the PMD file itself
    wxString targetPmdPath = targetDir + wxFS + pmdFile.GetFullName();
    if (!wxCopyFile(pmdSourcePath, targetPmdPath)) {
        return false;
    }
    outImportedPath = targetPmdPath;

    // 2. Copy all texture dependencies
    for (const auto& tex : textures) {
        if (!CopyFileWithDir(tex.sourcePath, targetDir, tex.internalPath)) {
            // Failure here might leave an incomplete import.
            // For now, we return false.
            return false;
        }
    }

    return true;
}

bool MMDImporter::CopyFileWithDir(const wxString& src, const wxString& destDir, const wxString& relativePath) {
    if (src.IsEmpty()) return true; // Skip missing but allowed files if any (though UI should prevent this)

    wxFileName destFile(destDir + wxFS + relativePath);
    wxString destSubDir = destFile.GetPath();
    
    if (!wxDir::Exists(destSubDir)) {
        if (!wxFileName::Mkdir(destSubDir, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL)) {
            return false;
        }
    }
    
    return wxCopyFile(src, destFile.GetFullPath());
}

wxString MMDImporter::FindFileCaseInsensitive(const wxString& fullPath) {
    if (wxFileExists(fullPath)) return fullPath;

    wxFileName fn(fullPath);
    wxArrayString dirs = fn.GetDirs();
    wxString currentPath = fn.GetVolume();
    if (!currentPath.IsEmpty()) currentPath += fn.GetVolumeSeparator();
    
    if (fn.IsAbsolute()) {
        currentPath += wxFileName::GetPathSeparator();
    }

    // Resolve directories
    for (size_t i = 0; i < dirs.GetCount(); ++i) {
        wxString targetDir = dirs[i];
        if (targetDir.IsEmpty()) continue;

        wxDir dir(currentPath);
        if (!dir.IsOpened()) return wxEmptyString;

        wxString found;
        bool cont = dir.GetFirst(&found, wxEmptyString, wxDIR_DIRS);
        bool match = false;
        while (cont) {
            if (found.CmpNoCase(targetDir) == 0) {
                currentPath += found + wxFileName::GetPathSeparator();
                match = true;
                break;
            }
            cont = dir.GetNext(&found);
        }
        if (!match) return wxEmptyString;
    }

    // Resolve filename
    wxString targetFile = fn.GetFullName();
    wxDir dir(currentPath);
    if (!dir.IsOpened()) return wxEmptyString;

    wxString found;
    bool cont = dir.GetFirst(&found, wxEmptyString, wxDIR_FILES);
    while (cont) {
        if (found.CmpNoCase(targetFile) == 0) {
            return currentPath + found;
        }
        cont = dir.GetNext(&found);
    }

    return wxEmptyString;
}
