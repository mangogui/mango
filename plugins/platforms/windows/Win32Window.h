#include <PlatformWindow.h>
#include <Direct2DGraphicsContext.h>
#include <Windows.h>


class Win32Window: public PlatformWindow {
private:
    Win32Window* m_parent;
    HINSTANCE m_hInstance;
    HWND m_hWnd;
    HDC hdc;
    std::unique_ptr<Direct2DGraphicsContext> graphics;
public:
    explicit Win32Window(Win32Window* parent = nullptr);
    ~Win32Window() override;

    // Setters
    void setTitle(const std::string &title) override;
    void setBackgroundColor(const std::string &hexColor) override;

    // Getters
    [[nodiscard]] void* getWinId() const { return m_hWnd; }
    void* getNativeObject() override;

    void create() override;
    void show() override;
    void hide() override;
    void maximize() override;
    void resize(int width, int height) override;
    void move(int x, int y) override;
    void update() override;

    GraphicsContext* getGraphicsContext() override {
        return graphics.get();
    }
};
