#include "StdAfx.h"
#include "Path.h"

tstring Path::GetDirectoryName(tstring path) {
	tstring::size_type i = path.find_last_of(TEXT("\\/"));
	if (i == tstring::npos) i = 0;
	return path.substr(0, i);
}
