//=================================================
//
//FileUtil.cpp
//
//=================================================
#include "FileUtil.h"


//�w�肳�ꂽ�t�H���_���̂��t�@�C�������擾���鏈��
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