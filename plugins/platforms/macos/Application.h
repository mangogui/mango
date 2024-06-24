#ifndef APPLICATION_H
#define APPLICATION_H


struct CocoaApplicationWrapper;

class Application {
public:
    Application();

    ~Application();

    void run();

private:
    CocoaApplicationWrapper *wrapper;
};

#endif //APPLICATION_H
