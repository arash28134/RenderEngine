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

#include "../common/Types.h"

#include <imgui.h>

namespace rendercomp
{
class Widget
{
public:
    Widget(const String& title,
           const Vec2i pos = Vec2i(0, 0),
           const Vec2i size = Vec2i(-1, -1));

    void setPosition(const Vec2i& pos);
    void setSize(const Vec2i& size);

    virtual ~Widget() = default;

    void draw() const noexcept;

protected:
    virtual void drawImpl() const noexcept = 0;
private:
    const String _title;
    ImVec2 _pos;
    ImVec2 _size;
};
}
