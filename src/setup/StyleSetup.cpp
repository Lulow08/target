#include "StyleSetup.hpp"

#include <QFontDatabase>
#include <QDebug>

void StyleSetup::loadFont() {
    if (QFontDatabase::addApplicationFont(":/qt/qml/target/ui/assets/fonts/BDOGrotesk-Regular.ttf") == -1) {
        qWarning() << "Failed to load BDO Grotesk font";
    }
}