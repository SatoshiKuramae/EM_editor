//=====================================================
//
//FileUtil.h
//
//Auther : Satoshi Kuramae
//=====================================================
#pragma once
#include "pch.h"
#include <filesystem>


//引数にはフォルダのパス、ファイルの種類の順番で代入(foldwePath：フォルダのパス、extension：ファイルの拡張子）
std::vector<std::string> GetXFileNamesInDirectory(const std::string& folderPath, const std::string& extension);

//JSONファイル名をフォルダから読み込む処理
std::vector<std::string> GetJsonFileList(const std::string& folderPath);
