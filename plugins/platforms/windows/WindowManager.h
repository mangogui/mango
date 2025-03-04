#pragma once
#include "Windows.h"
#include <map>
#include <optional>
#include "Object.h"

class WindowManager {
public:
    static std::map<HWND, Object*> windowMapper;

    static void insertWidget(HWND hWnd, Object* widget) {
        windowMapper.insert(std::pair(hWnd, widget));
    }

    static std::optional<Object*> findWidget(HWND hWnd) {
        auto it = windowMapper.find(hWnd);
        if (it == windowMapper.end() || it->second == nullptr) {
            return std::nullopt;
        }
        return it->second;
    }

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
