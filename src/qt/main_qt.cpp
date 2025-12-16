#include "chip8emu.hpp"
#include "config_dialog.hpp"
#include "platform_qt.hpp"
#include <QApplication>
#include <QTimer>
#include <chrono>
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Show configuration dialog
    ConfigDialog configDialog;
    if (configDialog.exec() != QDialog::Accepted) {
        return 0;
    }

    QString romPath = configDialog.romPath();
    int scale = configDialog.scale();
    int cycleDelay = configDialog.delay();

    // Create platform and emulator
    PlatformQt platform(64, 32, scale);
    Chip8 chip8(&platform);
    chip8.LoadROM(romPath.toStdString().c_str());

    int videoPitch = sizeof(chip8.video[0]) * VIDEO_WIDTH;

    platform.show();

    // Use QTimer for the emulation loop
    auto lastCycleTime = std::chrono::high_resolution_clock::now();

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        if (platform.shouldQuit()) {
            app.quit();
            return;
        }

        platform.ProcessInput(chip8.keypad);

        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime -
                                                                                   lastCycleTime)
                       .count();

        if (dt > cycleDelay) {
            lastCycleTime = currentTime;
            chip8.Cycle();
            platform.Update(chip8.video, videoPitch);
        }
    });

    timer.start(1); // 1ms timer interval

    return app.exec();
}
