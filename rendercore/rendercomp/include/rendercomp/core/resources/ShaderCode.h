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

#include "../Resources.h"

namespace rendercomp
{

namespace data
{
class ShaderCode
{
public:
    ShaderCode(const String& code);

    const String& getRawCode() const;
    String parseUBERCode(const Vector<String>& defines);
private:

    String _code;
};
}

template<>
std::unique_ptr<data::ShaderCode>
Resources::get<data::ShaderCode>(const String& path);
}
