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

#include <rendercomp/ui/Widget.h>

#include <imgui.h>

namespace rendercomp
{
Widget::Widget(const String& title, const Vec2i pos, const Vec2i size)
 : _title(title)
{
    setPosition(pos);
    setSize(size);
}

void Widget::setPosition(const Vec2i &pos)
{
    _pos = ImVec2(pos.x, pos.y);
}

void Widget::setSize(const Vec2i &size)
{
    _size = ImVec2(size.x, size.y);
}

void Widget::draw() const noexcept
{
    ImGui::SetNextWindowPos(_pos);
    ImGui::SetNextWindowSize(_size);
    ImGui::Begin(_title.c_str());

    drawImpl();

    ImGui::End();
}
}
