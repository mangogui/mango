#ifndef MANGO_WIN32APPLICATION_H
#define MANGO_WIN32APPLICATION_H

namespace GUI {

    class Win32Application {
    public:
        Win32Application();
        ~Win32Application();
        void run();
        void quit();
    private:
        bool isRunning = true;
    };

}

#endif //MANGO_WIN32APPLICATION_H
