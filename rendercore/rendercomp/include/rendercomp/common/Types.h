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

#include <EASTL/hash_map.h>
#include <EASTL/hash_set.h>
#include <EASTL/map.h>
#include <EASTL/set.h>
#include <EASTL/string.h>
#include <EASTL/vector.h>

// GLM_FORCE_CTOR_INIT
// This can cause seg faults if added as submodule of a project which does not use the same
// directive (due to compiler optimizing away non initialized elements, glm objects have different
// sizes on each side)
#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>
#include <glm/gtx/io.hpp>

#include <string>

namespace rendercomp
{
template<typename K, typename V>
using HashTable = eastl::hash_map<K, V>;

template<typename K, typename V>
using Map = eastl::map<K, V>;

template<typename T>
using HashSet = eastl::hash_set<T>;

template<typename T>
using Set = eastl::set<T>;

template<typename T>
using Vector = eastl::vector<T>;

using String = eastl::string;
#define TOSTDSTR(str) std::string(str.c_str())

using Vec2i     = glm::ivec2;
using Vec2i64   = glm::vec<2, int64_t, glm::defaultp>;
using Vec2ui    = glm::vec<2, uint32_t, glm::defaultp>;
using Vec2ui64  = glm::vec<2, uint64_t, glm::defaultp>;
using Vec2f     = glm::vec2;
using Vec2f64   = glm::vec<2, double, glm::defaultp>;

using Vec3i     = glm::ivec3;
using Vec3i64   = glm::vec<3, int64_t, glm::defaultp>;
using Vec3ui    = glm::vec<3, uint32_t, glm::defaultp>;
using Vec3ui64  = glm::vec<3, uint64_t, glm::defaultp>;
using Vec3f     = glm::vec3;
using Vec3f64   = glm::vec<3, double, glm::defaultp>;

using Vec4i     = glm::ivec4;
using Vec4i64   = glm::vec<4, int64_t, glm::defaultp>;
using Vec4ui    = glm::vec<4, uint32_t, glm::defaultp>;
using Vec4ui64  = glm::vec<4, uint64_t, glm::defaultp>;
using Vec4f     = glm::vec4;
using Vec4f64   = glm::vec<4, double, glm::defaultp>;

using Mat3      = glm::mat3;
using Mat4      = glm::mat4;

using Quaternion = glm::quat;
}

std::ostream& operator<<(std::ostream& os, const rendercomp::String& str);
