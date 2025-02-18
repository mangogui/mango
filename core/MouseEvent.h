#pragma once


class MouseEvent {
public:
    enum Type {
        LeftButtonDown,
        LeftButtonUp,
        RightMouseDown,
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

    void setType(MouseEvent::Type type) {
        this->_type = type;
    }

    MouseEvent::Type type() { return this->_type; }

private:
    MouseEvent::Type _type;
};

