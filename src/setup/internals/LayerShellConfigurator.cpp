#include "LayerShellConfigurator.hpp"

#include <LayerShellQt/Window>

namespace {
    // Fraction of the screen height used as the top margin.
    // At 1080p this yields ~270px; scales naturally on HiDPI and non-standard
    // resolutions without manual adjustment
    constexpr double kTopOffsetRatio = 0.25;
} // namespace

void LayerShellConfigurator::apply(QWindow* window)
{
    if (!window) return;

    auto* layerShell = LayerShellQt::Window::get(window);
    if (!layerShell) return;

    layerShell->setLayer(LayerShellQt::Window::LayerTop);
    layerShell->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityExclusive);
    layerShell->setAnchors(LayerShellQt::Window::AnchorTop);

    // Derive top margin from the screen that currently contains the window.
    // Falls back to the primary screen if the window has no screen assigned yet
    const QScreen* screen = window->screen()
                                ? window->screen()
                                : QGuiApplication::primaryScreen();

    const int topMargin = screen
                              ? static_cast<int>(screen->availableGeometry().height() * kTopOffsetRatio)
                              : 300; // last-resort static fallback

    layerShell->setMargins(QMargins(0, topMargin, 0, 0));
}