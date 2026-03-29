#include "SystemSetup.hpp"
#include "internals/LayerShellConfigurator.hpp"
#include <QStringView>

void SystemSetup::prepareEnvironment() {
    // Force Wayland platform with layer-shell integration
    qputenv("QT_QPA_PLATFORM", QByteArray("wayland"));
    qputenv("QT_WAYLAND_SHELL_INTEGRATION", QByteArray("layer-shell"));
}

void SystemSetup::initializeWindow(QWindow* window) {
    LayerShellConfigurator::apply(window);
}