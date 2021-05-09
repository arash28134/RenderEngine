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

#include <memory>

#include "../common/Types.h"
#include "../common/FilesystemUtils.h"

namespace rendercomp
{
class Resources
{
public:
    template<class ResourceType>
    static std::unique_ptr<ResourceType> load(const String& path)
    {
        return get<ResourceType>(path);
    }

private:
    template<class ResourceType>
    static std::unique_ptr<ResourceType> get(const String& path)
    {
        const std::string className (typeid(ResourceType).name());
        throw std::runtime_error("There is not support to load "
                                 + className
                                 + " from path " + std::string(path.c_str()));
    }
};
}
