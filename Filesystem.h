#pragma once

#include <string>
#include <vector>

namespace Kernel
{

std::vector<std::string> FindFilesInDir(const std::string& dir, const std::string& pattern = "*");
bool DeleteFile(const std::string& path);

}