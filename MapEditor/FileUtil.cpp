//=================================================
//
//FileUtil.cpp
//
// Auther : SatoshiKuramae
//=================================================
#include "FileUtil.h"
#include <set>

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


std::vector<std::string> GetJsonFileList(const std::string& folderPath)
{
	std::set<std::string> uniqueFileSet;
	std::vector<std::string> fileList;

	std::string searchPath = folderPath + "\\*.json";
	WIN32_FIND_DATAA fd;
	HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fd);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				std::string filename = fd.cFileName;
				if (uniqueFileSet.insert(filename).second) {
					fileList.push_back(filename);
				}
			}
		} while (FindNextFileA(hFind, &fd));

		FindClose(hFind);
	}
	else
	{
		std::cerr << "フォルダが見つかりません: " << folderPath << std::endl;
	}

	return fileList;
}
