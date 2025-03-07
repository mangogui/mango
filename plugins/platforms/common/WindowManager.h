#pragma once
#include <map>
#include <optional>
#include "Object.h"

#ifdef _WIN32
    #include <Windows.h>
#endif

class WindowManager {
public:
    static std::map<void*, Object*> windowMapper;

    static void insertWidget(void* hWnd, Object* widget) {
        windowMapper.insert(std::pair(hWnd, widget));
    }

    static std::optional<Object*> findWidget(void* hWnd) {
        auto it = windowMapper.find(hWnd);
        if (it == windowMapper.end() || it->second == nullptr) {
            return std::nullopt;
        }
        return it->second;
    }

#ifdef _WIN32
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
};
