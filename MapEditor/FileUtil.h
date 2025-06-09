//=====================================================
//
//FileUtil.h
//
//=====================================================
#pragma once
#include "pch.h"

//引数にはフォルダのパス、ファイルの種類の順番で代入
std::vector<std::string> GetXFileNamesInDirectory(const std::string& folderPath, const std::string& extension);
