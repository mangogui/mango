#ifndef APPLICATION_H
#define APPLICATION_H


struct CocoaApplicationWrapper;

class Application {
public:
    Application();

    ~Application();

    void run();

    // Ensure all instances share the same state
    static Application &instance();

    static CocoaApplicationWrapper *sharedWrapper;
};

#endif //APPLICATION_H
