#pragma once

class QWindow;

namespace LayerShellConfigurator {
    // Configures Wayland layer-shell properties for the given window.
    // Top margin is derived from the screen height to remain correct across
    // resolutions, DPI scales, and multi-monitor setups
    void apply(QWindow* window);
} // namespace LayerShell