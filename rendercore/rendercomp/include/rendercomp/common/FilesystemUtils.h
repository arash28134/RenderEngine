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

#pragma once

#include "Types.h"

namespace rendercomp
{
class FilesystemUtils
{
public:
    /**
     * @brief Initializes the filesystem utilites by computing the absolute path to the executable
     *
     * @param executableCall: The first argument passed to main
     */
    static void init(char* executableCall);

    /**
     * @brief Extracts a file extension from a path by spliting at the dot (.) position. If no dot
     *        is found, it returns an empty string
     * @param path The path to the file
     * @return Extension of the file
     */
    static String getFileExtension(const String& path);

    /**
     * @brief getFileName Extracts the last element from a path
     * @param path Original path
     * @return Last element of the path, without slashes
     */
    static String getFileName(const String& path);

    /**
     * @brief Extracts the path to the directory of a file, or of the parent directory
     *        of a folder.
     * @param Original path
     * @return Path to the parent folder of the given path
     */
    static String getParentDirectory(const String& path);

    /**
     * @brief getCurrentPath Returns the path to the executable which was built using the
     *        RenderComponents library. A call to FilesystemUtils::init() is needed for
     *        this function to works properly, otherwise it will return an empty string.
     * @return Path to the exeutable, or empty string
     */
    static const String& currentPath();

    /**
     * @brief Tests a given path and returns true if the file or folder denoted by such
     *        path exists.
     * @param Path to a file or folder
     * @return True if the given path points to an existing file/directory, false otherwise
     */
    static bool exists(const String& path);

private:
    static String _currentPath;
};
}
