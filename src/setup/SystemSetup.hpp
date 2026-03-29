#pragma once

class QWindow;

class SystemSetup {
    public:
        // Set up the environment BEFORE creating QGuiApplication
        static void prepareEnvironment();

        // Set up the window AFTER QML has created it
        static void initializeWindow(QWindow* window);
};