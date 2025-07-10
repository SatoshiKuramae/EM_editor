//=====================================================
//
//FileUtil.h
//
//Auther : Satoshi Kuramae
//=====================================================
#pragma once
#include "pch.h"
#include <filesystem>


//引数にはフォルダのパス、ファイルの種類の順番で代入
std::vector<std::string> GetXFileNamesInDirectory(const std::string& folderPath, const std::string& extension);
std::vector<std::string> GetJsonFileList(const std::string& folderPath);
