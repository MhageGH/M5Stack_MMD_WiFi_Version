#pragma once
#include "tstring.h"

// ファイルパス操作
class Path {
public:
	// パス名からディレクトリ名を取得
	// @param path パス名
	// @return ディレクトリ名
	static tstring GetDirectoryName(tstring path);
};
