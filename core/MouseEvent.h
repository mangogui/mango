#pragma once
#include <Event.h>

class MouseEvent: public Event {
public:
    enum Type {
        LeftButtonDown,
        LeftButtonUp,
        RightButtonDown,
        RightMouseUp,
        MouseMoved,
        LeftMouseDragged,
        RightMouseDragged,
        MouseEntered,
        MouseExited,
        KeyDown,
        KeyUp,
        FlagsChanged,
        AppKitDefined,
        SystemDefined,
        ApplicationDefined,
        Periodic,
        CursorUpdate,
        ScrollWheel,
        TabletPoint,
        TabletProximity,
        OtherMouseDown,
        OtherMouseUp,
        OtherMouseDragged
    };

    MouseEvent(MouseEvent::Type type): Event(Event::Type::MouseEvent) {
        this->_type = type;
    }

    void setType(MouseEvent::Type type) {
        this->_type = type;
    }

    MouseEvent::Type type() { return this->_type; }

private:
    MouseEvent::Type _type;
};

