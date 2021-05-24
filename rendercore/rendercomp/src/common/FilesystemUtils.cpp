/*
 * This file is part of the RenderEngine distribution (https://github.com/NadirRoGue/RenderEngine).
 * Copyright (c) 2021 Nadir Román Guerrero.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <rendercomp/common/FilesystemUtils.h>

#include <algorithm>

#ifdef _WIN32
#include <winbase.h>
#else
#include <unistd.h>
#endif

namespace rendercomp
{
String FilesystemUtils::_currentPath = String();

void FilesystemUtils::init(char *executableCall)
{
    String executablePath (executableCall);
    executablePath = getParentDirectory(executablePath);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    char buf[256];
    getcwd(buf, 256);
#pragma GCC diagnostic pop
    _currentPath = String(buf) + "/" + executablePath;
}

String FilesystemUtils::getFileExtension(const String& path)
{
    auto dotPos = path.find(".");
    if(dotPos == path.npos)
        return String();

    String ext = path.substr(dotPos + 1);

    std::transform(ext.begin(), ext.end(), ext.begin(), [](const char c)
    {
        return tolower(c);
    });

    return ext;
}

String FilesystemUtils::getFileName(const String& path)
{
    String unixPath = path;
#ifdef _WIN32
    std::transform(unixPath.begin(), unixPath.end(), unixPath.begin(), [](const char c)
    {
        if(c == '\\')
            return '/';
        return c;
    });
#endif
    auto lastSlashPos = unixPath.find_last_of('/');
    if(lastSlashPos == unixPath.npos)
        return unixPath;
    return unixPath.substr(lastSlashPos + 1);
}

String FilesystemUtils::getParentDirectory(const String& path)
{
    String unixPath = path;
#ifdef _WIN32
    std::transform(unixPath.begin(), unixPath.end(), unixPath.begin(), [](const char c)
    {
        if(c == '\\')
            return '/';
        return c;
    });
#endif
    auto lastSlashPos = unixPath.find_last_of('/');
    String resultPath = unixPath;
    if(lastSlashPos != unixPath.npos)
        return resultPath = unixPath.substr(0, lastSlashPos);

// Dont think this is necessary since Win10
/*
#ifdef _WIN32
    std::transform(resultPath.begin(), resultPath.end(), resultPath.begin(), [](const char c)
    {
        if(c == '/')
            return '\\';
        return c;
    });
#endif
*/
    return resultPath;
}

const String& FilesystemUtils::currentPath()
{
    return _currentPath;
}

bool FilesystemUtils::exists(const String& path)
{
#ifdef _WIN32
    return !(GetFileAttributes("C:\\MyFile.txt") == INVALID_FILE_ATTRIBUTES
                && GetLastError() == ERROR_FILE_NOT_FOUND);
#else
    return access(path.c_str(), F_OK) == 0;
#endif
}
}
