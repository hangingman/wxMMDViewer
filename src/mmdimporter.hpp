#ifndef MMD_IMPORTER_HPP
#define MMD_IMPORTER_HPP

#include <wx/wx.h>
#include <string>
#include <vector>

class MMDImporter {
public:
    struct TextureInfo {
        wxString internalPath; // Path specified in PMD (e.g., "tex/01.bmp")
        wxString sourcePath;   // Full source path to copy from
        bool exists;           // True if source file found
    };

    /**
     * Parse PMD to extract texture list and attempt auto-detection.
     */
    static std::vector<TextureInfo> AnalyzePMD(const wxString& pmdPath);

    /**
     * Perform the import: copy PMD and all textures into managed directory.
     * outImportedPath will contain the path to the newly imported PMD file.
     */
    static bool ExecuteImport(const wxString& pmdSourcePath, 
                              const std::vector<TextureInfo>& textures,
                              wxString& outImportedPath);
    
    /**
     * Helper to find a file path case-insensitively.
     * Returns the actual path on disk if found, otherwise an empty string.
     */
    static wxString FindFileCaseInsensitive(const wxString& fullPath);

private:
    /**
     * Helper to create subdirectories as needed and copy a file.
     */
    static bool CopyFileWithDir(const wxString& src, const wxString& destDir, const wxString& relativePath);
};

#endif
