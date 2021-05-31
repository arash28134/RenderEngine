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

#include <exception>
#include <iostream>
#include <glad/glad.h>

#include "../common/Debug.h"

#ifndef RENDERCOMP_DEBUG
#define DRIVER_CALL(call) call
#define DRIVER_CALL_NOTHROW(call) call
#else
#define DRIVER_CALL(call) \
    call; \
    { \
        int rendercomp_gl_debug_error = glGetError(); \
        if (rendercomp_gl_debug_error != 0) \
        { \
            std::string debug_info; \
            debug_info += "OpenGL call " + std::string(#call); \
            debug_info += " returned " + std::to_string(rendercomp_gl_debug_error) + "\n"; \
            debug_info += "At:\n"; \
            debug_info += "\tFile: " + std::string(__FILE__) + "\n"; \
            debug_info += "\tFunction: " + std::string(__FUNCTION__) + "\n"; \
            debug_info += "\tLine: " + std::to_string(__LINE__) + "\n"; \
            throw std::runtime_error(debug_info); \
        } \
    }

#define DRIVER_CALL_NOTHROW(call) \
    call; \
    { \
        int rendercomp_gl_debug_error = glGetError(); \
        if (rendercomp_gl_debug_error != 0) \
        { \
            std::string debug_info; \
            debug_info += "OpenGL call " + std::string(#call); \
            debug_info += " returned " + std::to_string(rendercomp_gl_debug_error) + "\n"; \
            debug_info += "At:\n"; \
            debug_info += "\tFile: " + std::string(__FILE__) + "\n"; \
            debug_info += "\tFunction: " + std::string(__FUNCTION__) + "\n"; \
            debug_info += "\tLine: " + std::to_string(__LINE__) + "\n"; \
            std::cout << debug_info << std::endl; \
        } \
    }
#endif
