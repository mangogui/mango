#pragma once
#include <Windows.h>

#include <string>
#include <iostream>
#include <cassert>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM uParam, LPARAM lParam);
namespace GUI {

    class Win32Window {
    public:
        Win32Window();

        Win32Window &operator=(const Win32Window &) = delete;

        ~Win32Window();

        static bool ProcessMessages();

        void set_window_title(std::string title);

        const std::string get_window_title() const;

        int get_x() const noexcept;

        int get_y() const noexcept;

        void center();

        void display();

    private:
        HINSTANCE m_hInstance;
        HWND m_hWnd;
        HDC hdc;
        std::string window_title;
        DWORD style;

        void createWindow();
    };

}