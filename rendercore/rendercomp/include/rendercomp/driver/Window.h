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

#include <rendercomp/common/Types.h>

#include <rendercomp/ui/Widget.h>

#include <functional>
#include <memory>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

namespace rendercomp
{
enum class FPS : uint8_t
{
    FPS_UNLIMITED = 0,
    FPS_60 = 1,
    FPS_30 = 2
};

struct WindowConfiguration
{
    String title;
    uint32_t width;
    uint32_t height;
    uint32_t xPos;
    uint32_t yPos;
    Vec4f clearColor;
    FPS maxFPS;
    bool resizable;
};

enum class Key: uint8_t
{
    KEY_NONE = 0,
    KEY_UNKNOWN,
    KEY_SPACE,
    KEY_APOSTROPHE,
    KEY_COMMA,
    KEY_MINUS,
    KEY_PERIOD,
    KEY_SLASH,
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_SEMICOLON,
    KEY_EQUAL,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_LEFT_BRACKET,
    KEY_BACKSLASH,
    KEY_RIGHT_BRACKET,
    KEY_GRAVE_ACCENT,
    KEY_WORLD_1,
    KEY_WORLD_2,
    KEY_ESCAPE,
    KEY_ENTER,
    KEY_TAB,
    KEY_BACKSPACE,
    KEY_INSERT,
    KEY_DELETE,
    KEY_RIGHT,
    KEY_LEFT,
    KEY_DOWN,
    KEY_UP,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    KEY_HOME,
    KEY_END,
    KEY_CAPS_LOCK,
    KEY_SCROLL_LOCK,
    KEY_NUM_LOCK,
    KEY_PRINT_SCREEN,
    KEY_PAUSE,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_F13,
    KEY_F14,
    KEY_F15,
    KEY_F16,
    KEY_F17,
    KEY_F18,
    KEY_F19,
    KEY_F20,
    KEY_F21,
    KEY_F22,
    KEY_F23,
    KEY_F24,
    KEY_F25,
    KEY_KP_0,
    KEY_KP_1,
    KEY_KP_2,
    KEY_KP_3,
    KEY_KP_4,
    KEY_KP_5,
    KEY_KP_6,
    KEY_KP_7,
    KEY_KP_8,
    KEY_KP_9,
    KEY_KP_DECIMAL,
    KEY_KP_DIVIDE,
    KEY_KP_MULTIPLY,
    KEY_KP_SUBTRACT,
    KEY_KP_ADD,
    KEY_KP_ENTER,
    KEY_KP_EQUAL,
    KEY_LEFT_SHIFT,
    KEY_LEFT_CONTROL,
    KEY_LEFT_ALT,
    KEY_LEFT_SUPER,
    KEY_RIGHT_SHIFT,
    KEY_RIGHT_CONTROL,
    KEY_RIGHT_ALT,
    KEY_RIGHT_SUPER,
    KEY_MENU,
    KEY_LAST_KEY_MENU
};

enum class InputMod: uint8_t
{
    MOD_NONE = 0,
    MOD_SHIFT,
    MOD_CONTROL,
    MODL_ALT,
    MOD_SUPER,
    MOD_CAPS_LOCK,
    MOD_NUM_LOCK
};

enum class KeyAction: uint8_t
{
    ACTION_NONE = 0,
    ACTION_PRESS,
    ACTION_RELEASE,
    ACTION_REPEAT
};

enum class MouseButton: uint8_t
{
    MOUSE_BUTTON_NONE = 0,
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTON_MIDDLE
};

enum class MouseButtonAction: uint8_t
{
    ACTION_NONE = 0,
    ACTION_PRESS,
    ACTION_RELEASE
};

class Window
{
public:
    Window(const WindowConfiguration&);
    ~Window();

    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    template<typename WidgetClass,
             typename ...Args,
             typename = std::enable_if_t<std::is_base_of<Widget, WidgetClass>::value>
            >
    WidgetClass* createWidget(const String& name, Args&&...args)
    {
        auto it = _widgets.find(name);
        if(it != _widgets.end())
            throw std::runtime_error("Duplicate widget name: " + std::string(name.c_str()));

        std::unique_ptr<Widget> newWidget =
                std::make_unique<WidgetClass>(std::forward<Args>(args)...);

        WidgetClass* result = static_cast<WidgetClass*>(newWidget.get());

        _widgets[name] = std::move(newWidget);

        return result;
    }

    void removeWidget(const String& name);

    template<typename WidgetClass,
             typename = std::enable_if_t<std::is_base_of<Widget, WidgetClass>::value>
            >
    WidgetClass* getWidget(const String& name)
    {
        auto it = _widgets.find(name);
        if(it != _widgets.end())
            return nullptr;

        return static_cast<WidgetClass*>(it->second.get());
    }

    /**
     * @brief setKeyboardInputHandler Sets a callback function that will be called everytime that
     *        a key is used within the window context.
     *
     * @param cb Callback function, following the signature:<br>
     *        void (const rendeng::Key key,<br>
     *              const rendeng::KeyAction action,<br>
     *              const rendeng::InputMod mod)<br>
     *        With:<br>
     *          - rendeng::Key key being the key used<br>
     *          - rendeng::KeyAction being the action applied on the key
     *          - rendeng::InputMod being the modifier applied to the action and key
     */
    void setKeyboardInputHandler(const std::function<void(const Key,
                                                          const KeyAction,
                                                          const InputMod)>& cb)
    {
        _keyboardCb = cb;
    }

    /**
     * @brief setCursorPositionHandler Sets a callback function that will be called everytime that
     *        the mouse cursor changes position within the window context
     *
     * @param cb Callback function, following the signature:<br>
     *        void (const double xPos, const double yPos)<br>
     *        With:<br>
     *          - xPos being the horizontal pixel position<br>
     *          - yPos being the vertical pixel position<br>
     *        The arguments being double allow, in those platforms that support it, to have
     *        subpixel position information.
     */
    void setCursorPositionHandler(const std::function<void(const double xPos,
                                                           const double yPos)>& cb)
    {
        _cursorPosCb = cb;
    }

    /**
     * @brief setCursorEnterWindowHandler Sets a callback function that will be called everytime
     *        that the cursor enters or leaves the window.
     * @param cb Callback function, following the signature:<br>
     *        void (const bool entered)<br>
     *        With:<br>
     *          - bool entered a flag that is true when the mouse enters the window, false
     *            otherwise.
     */
    void setCursorEnterWindowHandler(const std::function<void(const bool)>& cb)
    {
        _cursorEnterCb = cb;
    }

    /**
     * @brief setMouseInputHandler Sets a callback function that will be called everytime that
     *        a mouse button is used within the window context.
     * @param cb Callback function, with the signature:<br>
     *        void (const rendeng::MouseButton button,<br>
     *              const rendeng::MouseButtonAction action,<br>
     *              const rendeng::InputMod mod)<br>
     *        With:<br>
     *          - rendeng::MouseButton button being the mouse button used<br>
     *          - rendeng::MouseButtonAction being the action applied on the button
     *          - rendeng::InputMod being the modifier applied to the action and button
     */
    void setMouseInputHandler(const std::function<void(const MouseButton,
                                                       const MouseButtonAction,
                                                       const InputMod)>& cb)
    {
        _mouseInputCb = cb;
    }

    /**
     * @brief setScrollInputHandler Sets a callback function that will be called everytime that
     *        a scroll event happens. (mouse middle button)
     * @param cb Callback function, with the signature:<br>
     *        void (const double yOffset)<br>
     *        With:<br>
     *          - double yOffset movement produced by the scroll event
     */
    void setScrollInputHandler(const std::function<void(const double yOffset)>& cb)
    {
        _scrollCb = cb;
    }

    /**
     * @brief setFrameResizeHandler Sets a callback function that will be called everytime that
     *        the window framebuffer is resized.
     * @param cb Callback function, with the signature:<br>
     *        void (const uint32_t width, const uint32_t height)<br>
     *        With:<br>
     *          - uint32_t width being the new width in pixels of the framebuffer
     *          - uint32_t height being the new height in pixels of the framebuffer
     */
    void setFrameResizeHandler(const std::function<void(const uint32_t width,
                                                        const uint32_t height)>& cb)
    {
        _fbResizeCb = cb;
    }

    /**
     * @brief setDrawCallback sets a function callback which will be called on each
     *        iteration of the renderloop. This function implements the render logic.
     * @param drawFunc Callback function, with the signature<br>
     *        void ()
     */
    void setDrawCallback(const std::function<void()>& drawFunc)
    {
        _drawCb = drawFunc;
    }

    void renderLoop();

public:
    std::function<void(const Key, const KeyAction, const InputMod)> _keyboardCb;
    std::function<void(const double, const double)> _cursorPosCb;
    std::function<void(const bool)> _cursorEnterCb;
    std::function<void(const MouseButton, const MouseButtonAction, const InputMod)> _mouseInputCb;
    std::function<void(const double)> _scrollCb;
    std::function<void(const uint32_t, const uint32_t)> _fbResizeCb;

private:
    GLFWwindow* _window {nullptr};
    std::function<void()> _drawCb;
    HashTable<String, std::unique_ptr<Widget>> _widgets;

};
}

