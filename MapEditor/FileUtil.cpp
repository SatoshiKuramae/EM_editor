//=================================================
//
//FileUtil.cpp
//
// Auther : SatoshiKuramae
//=================================================
#include "FileUtil.h"


//指定されたフォルダ内のｘファイル名を取得する処理
std::vector<std::string> GetXFileNamesInDirectory(const std::string& folderPath, const std::string& extension) {
    std::vector<std::string> filenames;
    std::string searchPath = folderPath + "\\*" + extension;

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            filenames.push_back(findData.cFileName);
        } while (FindNextFileA(hFind, &findData));
        FindClose(hFind);
    }

    return filenames;
}
