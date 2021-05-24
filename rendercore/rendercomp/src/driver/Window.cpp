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

#include <rendercomp/driver/Window.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <algorithm>

#include <iostream>

namespace rendercomp
{

namespace
{
void key_callback(GLFWwindow* window, int key, int, int action, int mods)
{
    Window* engineWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if(engineWindow->_keyboardCb)
    {
        if(ImGui::GetIO().WantCaptureKeyboard)
            return;

        Key engineKey = Key::KEY_NONE;
        switch(key)
        {
            case GLFW_KEY_UNKNOWN:
                engineKey = Key::KEY_UNKNOWN;
                break;
            case GLFW_KEY_SPACE:
                engineKey = Key::KEY_SPACE;
                break;
            case GLFW_KEY_APOSTROPHE:
                engineKey = Key::KEY_APOSTROPHE;
                break;
            case GLFW_KEY_COMMA:
                engineKey = Key::KEY_COMMA;
                break;
            case GLFW_KEY_MINUS:
                engineKey = Key::KEY_MINUS;
                break;
            case GLFW_KEY_PERIOD:
                engineKey = Key::KEY_PERIOD;
                break;
            case GLFW_KEY_SLASH:
                engineKey = Key::KEY_SLASH;
                break;
            case GLFW_KEY_0:
                engineKey = Key::KEY_0;
                break;
            case GLFW_KEY_1:
                engineKey = Key::KEY_1;
                break;
            case GLFW_KEY_2:
                engineKey = Key::KEY_2;
                break;
            case GLFW_KEY_3:
                engineKey = Key::KEY_3;
                break;
            case GLFW_KEY_4:
                engineKey = Key::KEY_4;
                break;
            case GLFW_KEY_5:
                engineKey = Key::KEY_5;
                break;
            case GLFW_KEY_6:
                engineKey = Key::KEY_6;
                break;
            case GLFW_KEY_7:
                engineKey = Key::KEY_7;
                break;
            case GLFW_KEY_8:
                engineKey = Key::KEY_8;
                break;
            case GLFW_KEY_9:
                engineKey = Key::KEY_9;
                break;
            case GLFW_KEY_SEMICOLON:
                engineKey = Key::KEY_SEMICOLON;
                break;
            case GLFW_KEY_EQUAL:
                engineKey = Key::KEY_EQUAL;
                break;
            case GLFW_KEY_A:
                engineKey = Key::KEY_A;
                break;
            case GLFW_KEY_B:
                engineKey = Key::KEY_B;
                break;
            case GLFW_KEY_C:
                engineKey = Key::KEY_C;
                break;
            case GLFW_KEY_D:
                engineKey = Key::KEY_D;
                break;
            case GLFW_KEY_E:
                engineKey = Key::KEY_E;
                break;
            case GLFW_KEY_F:
                engineKey = Key::KEY_F;
                break;
            case GLFW_KEY_G:
                engineKey = Key::KEY_G;
                break;
            case GLFW_KEY_H:
                engineKey = Key::KEY_H;
                break;
            case GLFW_KEY_I:
                engineKey = Key::KEY_I;
                break;
            case GLFW_KEY_J:
                engineKey = Key::KEY_J;
                break;
            case GLFW_KEY_K:
                engineKey = Key::KEY_K;
                break;
            case GLFW_KEY_L:
                engineKey = Key::KEY_L;
                break;
            case GLFW_KEY_M:
                engineKey = Key::KEY_M;
                break;
            case GLFW_KEY_N:
                engineKey = Key::KEY_N;
                break;
            case GLFW_KEY_O:
                engineKey = Key::KEY_O;
                break;
            case GLFW_KEY_P:
                engineKey = Key::KEY_P;
                break;
            case GLFW_KEY_Q:
                engineKey = Key::KEY_Q;
                break;
            case GLFW_KEY_R:
                engineKey = Key::KEY_R;
                break;
            case GLFW_KEY_S:
                engineKey = Key::KEY_S;
                break;
            case GLFW_KEY_T:
                engineKey = Key::KEY_T;
                break;
            case GLFW_KEY_U:
                engineKey = Key::KEY_U;
                break;
            case GLFW_KEY_V:
                engineKey = Key::KEY_V;
                break;
            case GLFW_KEY_W:
                engineKey = Key::KEY_W;
                break;
            case GLFW_KEY_X:
                engineKey = Key::KEY_X;
                break;
            case GLFW_KEY_Y:
                engineKey = Key::KEY_Y;
                break;
            case GLFW_KEY_Z:
                engineKey = Key::KEY_Z;
                break;
            case GLFW_KEY_LEFT_BRACKET:
                engineKey = Key::KEY_LEFT_BRACKET;
                break;
            case GLFW_KEY_BACKSLASH:
                engineKey = Key::KEY_BACKSLASH;
                break;
            case GLFW_KEY_RIGHT_BRACKET:
                engineKey = Key::KEY_RIGHT_BRACKET;
                break;
            case GLFW_KEY_GRAVE_ACCENT:
                engineKey = Key::KEY_GRAVE_ACCENT;
                break;
            case GLFW_KEY_WORLD_1:
                engineKey = Key::KEY_WORLD_1;
                break;
            case GLFW_KEY_WORLD_2:
                engineKey = Key::KEY_WORLD_2;
                break;
            case GLFW_KEY_ESCAPE:
                engineKey = Key::KEY_ESCAPE;
                break;
            case GLFW_KEY_ENTER:
                engineKey = Key::KEY_ENTER;
                break;
            case GLFW_KEY_TAB:
                engineKey = Key::KEY_TAB;
                break;
            case GLFW_KEY_BACKSPACE:
                engineKey = Key::KEY_BACKSPACE;
                break;
            case GLFW_KEY_INSERT:
                engineKey = Key::KEY_INSERT;
                break;
            case GLFW_KEY_DELETE:
                engineKey = Key::KEY_DELETE;
                break;
            case GLFW_KEY_RIGHT:
                engineKey = Key::KEY_RIGHT;
                break;
            case GLFW_KEY_LEFT:
                engineKey = Key::KEY_LEFT;
                break;
            case GLFW_KEY_DOWN:
                engineKey = Key::KEY_DOWN;
                break;
            case GLFW_KEY_UP:
                engineKey = Key::KEY_UP;
                break;
            case GLFW_KEY_PAGE_UP:
                engineKey = Key::KEY_PAGE_UP;
                break;
            case GLFW_KEY_PAGE_DOWN:
                engineKey = Key::KEY_PAGE_DOWN;
                break;
            case GLFW_KEY_HOME:
                engineKey = Key::KEY_HOME;
                break;
            case GLFW_KEY_END:
                engineKey = Key::KEY_END;
                break;
            case GLFW_KEY_CAPS_LOCK:
                engineKey = Key::KEY_CAPS_LOCK;
                break;
            case GLFW_KEY_SCROLL_LOCK:
                engineKey = Key::KEY_SCROLL_LOCK;
                break;
            case GLFW_KEY_NUM_LOCK:
                engineKey = Key::KEY_NUM_LOCK;
                break;
            case GLFW_KEY_PRINT_SCREEN:
                engineKey = Key::KEY_PRINT_SCREEN;
                break;
            case GLFW_KEY_PAUSE:
                engineKey = Key::KEY_PAUSE;
                break;
            case GLFW_KEY_F1:
                engineKey = Key::KEY_F1;
                break;
            case GLFW_KEY_F2:
                engineKey = Key::KEY_F2;
                break;
            case GLFW_KEY_F3:
                engineKey = Key::KEY_F3;
                break;
            case GLFW_KEY_F4:
                engineKey = Key::KEY_F4;
                break;
            case GLFW_KEY_F5:
                engineKey = Key::KEY_F5;
                break;
            case GLFW_KEY_F6:
                engineKey = Key::KEY_F6;
                break;
            case GLFW_KEY_F7:
                engineKey = Key::KEY_F7;
                break;
            case GLFW_KEY_F8:
                engineKey = Key::KEY_F8;
                break;
            case GLFW_KEY_F9:
                engineKey = Key::KEY_F9;
                break;
            case GLFW_KEY_F10:
                engineKey = Key::KEY_F10;
                break;
            case GLFW_KEY_F11:
                engineKey = Key::KEY_F11;
                break;
            case GLFW_KEY_F12:
                engineKey = Key::KEY_F12;
                break;
            case GLFW_KEY_F13:
                engineKey = Key::KEY_F13;
                break;
            case GLFW_KEY_F14:
                engineKey = Key::KEY_F14;
                break;
            case GLFW_KEY_F15:
                engineKey = Key::KEY_F15;
                break;
            case GLFW_KEY_F16:
                engineKey = Key::KEY_F16;
                break;
            case GLFW_KEY_F17:
                engineKey = Key::KEY_F17;
                break;
            case GLFW_KEY_F18:
                engineKey = Key::KEY_F18;
                break;
            case GLFW_KEY_F19:
                engineKey = Key::KEY_F19;
                break;
            case GLFW_KEY_F20:
                engineKey = Key::KEY_F20;
                break;
            case GLFW_KEY_F21:
                engineKey = Key::KEY_F21;
                break;
            case GLFW_KEY_F22:
                engineKey = Key::KEY_F22;
                break;
            case GLFW_KEY_F23:
                engineKey = Key::KEY_F23;
                break;
            case GLFW_KEY_F24:
                engineKey = Key::KEY_F24;
                break;
            case GLFW_KEY_F25:
                engineKey = Key::KEY_F25;
                break;
            case GLFW_KEY_KP_0:
                engineKey = Key::KEY_KP_0;
                break;
            case GLFW_KEY_KP_1:
                engineKey = Key::KEY_KP_1;
                break;
            case GLFW_KEY_KP_2:
                engineKey = Key::KEY_KP_2;
                break;
            case GLFW_KEY_KP_3:
                engineKey = Key::KEY_KP_3;
                break;
            case GLFW_KEY_KP_4:
                engineKey = Key::KEY_KP_4;
                break;
            case GLFW_KEY_KP_5:
                engineKey = Key::KEY_KP_5;
                break;
            case GLFW_KEY_KP_6:
                engineKey = Key::KEY_KP_6;
                break;
            case GLFW_KEY_KP_7:
                engineKey = Key::KEY_KP_7;
                break;
            case GLFW_KEY_KP_8:
                engineKey = Key::KEY_KP_8;
                break;
            case GLFW_KEY_KP_9:
                engineKey = Key::KEY_KP_9;
                break;
            case GLFW_KEY_KP_DECIMAL:
                engineKey = Key::KEY_KP_DECIMAL;
                break;
            case GLFW_KEY_KP_DIVIDE:
                engineKey = Key::KEY_KP_DIVIDE;
                break;
            case GLFW_KEY_KP_MULTIPLY:
                engineKey = Key::KEY_KP_MULTIPLY;
                break;
            case GLFW_KEY_KP_SUBTRACT:
                engineKey = Key::KEY_KP_SUBTRACT;
                break;
            case GLFW_KEY_KP_ADD:
                engineKey = Key::KEY_KP_ADD;
                break;
            case GLFW_KEY_KP_ENTER:
                engineKey = Key::KEY_KP_ENTER;
                break;
            case GLFW_KEY_KP_EQUAL:
                engineKey = Key::KEY_KP_EQUAL;
                break;
            case GLFW_KEY_LEFT_SHIFT:
                engineKey = Key::KEY_LEFT_SHIFT;
                break;
            case GLFW_KEY_LEFT_CONTROL:
                engineKey = Key::KEY_LEFT_CONTROL;
                break;
            case GLFW_KEY_LEFT_ALT:
                engineKey = Key::KEY_LEFT_ALT;
                break;
            case GLFW_KEY_LEFT_SUPER:
                engineKey = Key::KEY_LEFT_SUPER;
                break;
            case GLFW_KEY_RIGHT_SHIFT:
                engineKey = Key::KEY_RIGHT_SHIFT;
                break;
            case GLFW_KEY_RIGHT_CONTROL:
                engineKey = Key::KEY_RIGHT_CONTROL;
                break;
            case GLFW_KEY_RIGHT_ALT:
                engineKey = Key::KEY_RIGHT_ALT;
                break;
            case GLFW_KEY_RIGHT_SUPER:
                engineKey = Key::KEY_RIGHT_SUPER;
                break;
            case GLFW_KEY_MENU:
                engineKey = Key::KEY_MENU;
                break;
        }

        KeyAction engineAction = KeyAction::ACTION_NONE;
        switch(action)
        {
            case GLFW_PRESS:
                engineAction = KeyAction::ACTION_PRESS;
                break;
            case GLFW_RELEASE:
                engineAction = KeyAction::ACTION_RELEASE;
                break;
            case GLFW_REPEAT:
                engineAction = KeyAction::ACTION_REPEAT;
                break;
        }

        InputMod engineMod = InputMod::MOD_NONE;
        switch(mods)
        {
            case GLFW_MOD_SHIFT:
                engineMod = InputMod::MOD_SHIFT;
                break;
            case GLFW_MOD_CONTROL:
                engineMod = InputMod::MOD_CONTROL;
                break;
            case GLFW_MOD_ALT:
                engineMod = InputMod::MODL_ALT;
                break;
            case GLFW_MOD_SUPER:
                engineMod = InputMod::MOD_SUPER;
                break;
            case GLFW_MOD_CAPS_LOCK:
                engineMod = InputMod::MOD_CAPS_LOCK;
                break;
            case GLFW_MOD_NUM_LOCK:
                engineMod = InputMod::MOD_NUM_LOCK;
                break;
        }

        engineWindow->_keyboardCb(engineKey, engineAction, engineMod);
    }
}


void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    Window* engineWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if(engineWindow->_cursorPosCb)
    {
        if(ImGui::GetIO().WantCaptureMouse)
            return;
        engineWindow->_cursorPosCb(xpos, ypos);
    }
}

void cursor_enter_callback(GLFWwindow* window, int entered)
{
    Window* engineWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if(engineWindow->_cursorEnterCb)
    {
        if(ImGui::GetIO().WantCaptureMouse)
            return;
        engineWindow->_cursorEnterCb(entered);
    }
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    Window* engineWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if(engineWindow->_mouseInputCb)
    {
        if(ImGui::GetIO().WantCaptureMouse)
            return;

        MouseButton engineButton = MouseButton::MOUSE_BUTTON_NONE;
        switch(button)
        {
            case GLFW_MOUSE_BUTTON_LEFT:
                engineButton = MouseButton::MOUSE_BUTTON_LEFT;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                engineButton = MouseButton::MOUSE_BUTTON_RIGHT;
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                engineButton = MouseButton::MOUSE_BUTTON_MIDDLE;
                break;
        }

        MouseButtonAction engineAction = MouseButtonAction::ACTION_NONE;
        switch(action)
        {
            case GLFW_PRESS:
                engineAction = MouseButtonAction::ACTION_PRESS;
                break;
            case GLFW_RELEASE:
                engineAction = MouseButtonAction::ACTION_RELEASE;
                break;
        }

        InputMod engineMod = InputMod::MOD_NONE;
        switch(mods)
        {
            case GLFW_MOD_SHIFT:
                engineMod = InputMod::MOD_SHIFT;
                break;
            case GLFW_MOD_CONTROL:
                engineMod = InputMod::MOD_CONTROL;
                break;
            case GLFW_MOD_ALT:
                engineMod = InputMod::MODL_ALT;
                break;
            case GLFW_MOD_SUPER:
                engineMod = InputMod::MOD_SUPER;
                break;
            case GLFW_MOD_CAPS_LOCK:
                engineMod = InputMod::MOD_CAPS_LOCK;
                break;
            case GLFW_MOD_NUM_LOCK:
                engineMod = InputMod::MOD_NUM_LOCK;
                break;
        }

        engineWindow->_mouseInputCb(engineButton, engineAction, engineMod);
    }
}


void scroll_callback(GLFWwindow* window, double, double yoffset)
{
    Window* engineWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if(engineWindow->_scrollCb)
    {
        if(ImGui::GetIO().WantCaptureMouse)
            return;
        engineWindow->_scrollCb(yoffset);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    Window* engineWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if(engineWindow->_fbResizeCb)
        engineWindow->_fbResizeCb(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
}


}
Window::Window(const WindowConfiguration& config) RC_NOEXCEPT
 : _window(nullptr)
{
    // INITIALIZE GLFW
    const int res = glfwInit();
    (void)res;
#ifdef RENDERCOMP_DEBUG
    if (!res)
        throw std::runtime_error("OpenGLWIndow: Could not initialize GLFW");
#endif

    glfwWindowHint(GLFW_SAMPLES, 4);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    glfwWindowHint(GLFW_RESIZABLE, config.resizable? GLFW_TRUE : GLFW_FALSE);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = glfwCreateWindow(config.width, config.height, config.title.c_str(), NULL, NULL);
    if (!_window)
    {
        glfwTerminate();
#ifdef RENDERCOMP_DEBUG
        throw std::runtime_error("OpenGLWindow: Could not create GLFW window");
#endif
    }

    glfwSetWindowUserPointer(_window, this);

    glfwSetKeyCallback(_window, key_callback);
    glfwSetCursorPosCallback(_window, cursor_position_callback);
    glfwSetCursorEnterCallback(_window, cursor_enter_callback);
    glfwSetScrollCallback(_window, scroll_callback);
    glfwSetMouseButtonCallback(_window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

    glfwMakeContextCurrent(_window);

    glfwSetWindowPos(_window, config.xPos, config.yPos);

    glfwSwapInterval(static_cast<int>(config.maxFPS));

    // INITIALIZE GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
#ifdef RENDERCOMP_DEBUG
        throw std::runtime_error("OpenGLWindow: Could not initialize GLAD");
#endif
    }

    // INITIALIZE IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init("#version 460 core");

    // OPENGL SPECIFIC CONFIGURATION
    DRIVER_CALL(glClearColor(config.clearColor.r,
                             config.clearColor.g,
                             config.clearColor.b,
                             config.clearColor.a));

    DRIVER_CALL(glViewport(0, 0, config.width, config.height));
}

Window::~Window()
{
    glfwTerminate();
}

void Window::removeWidget(const String& name)
{
    auto it = _widgets.find(name);
    if(it == _widgets.end())
        throw std::runtime_error("Unknown name given to remove widget: "
                                 + std::string(name.c_str()));

    _widgetList.erase(std::remove_if(_widgetList.begin(), _widgetList.end(),
    [ptr = it->second](std::unique_ptr<Widget>& widget)
    {
        return widget.get() == ptr;
    }));

    _widgets.erase(it);
}

void Window::renderLoop() RC_NOEXCEPT
{
    while(!glfwWindowShouldClose(_window))
    {
        glfwPollEvents();

        _drawCb();

        if(!_widgetList.empty())
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            for(const auto& widget : _widgetList)
                widget->draw();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        glfwSwapBuffers(_window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(_window);
}

}
