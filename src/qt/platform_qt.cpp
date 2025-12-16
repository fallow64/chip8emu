#include "platform_qt.hpp"
#include <QApplication>
#include <QPainter>

PlatformQt::PlatformQt(int textureWidth, int textureHeight, int scale, QWidget *parent)
    : QWidget(parent), framebuffer(textureWidth, textureHeight, QImage::Format_RGBA8888),
      textureWidth(textureWidth), textureHeight(textureHeight), scale(scale) {
    setFixedSize(textureWidth * scale, textureHeight * scale);
    setWindowTitle("chip8emu");
    setFocusPolicy(Qt::StrongFocus);
    framebuffer.fill(Qt::black);
}

PlatformQt::~PlatformQt() {}

void PlatformQt::Update(void const *buffer, int pitch) {
    const uint8_t *src = static_cast<const uint8_t *>(buffer);
    for (int y = 0; y < textureHeight; ++y) {
        const uint32_t *row = reinterpret_cast<const uint32_t *>(src + y * pitch);
        for (int x = 0; x < textureWidth; ++x) {
            uint32_t pixel = row[x];
            // CHIP-8 video buffer is RGBA8888 (same as SDL)
            // Extract components: pixel is 0xRRGGBBAA
            uint8_t r = (pixel >> 24) & 0xFF;
            uint8_t g = (pixel >> 16) & 0xFF;
            uint8_t b = (pixel >> 8) & 0xFF;
            uint8_t a = pixel & 0xFF;
            framebuffer.setPixelColor(x, y, QColor(r, g, b, a));
        }
    }
    update();
}

bool PlatformQt::ProcessInput(uint8_t *keys) {
    keysPtr = keys;
    QApplication::processEvents();
    return quitRequested;
}

void PlatformQt::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    QImage scaled = framebuffer.scaled(textureWidth * scale, textureHeight * scale,
                                       Qt::IgnoreAspectRatio, Qt::FastTransformation);
    painter.drawImage(0, 0, scaled);
}

void PlatformQt::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        quitRequested = true;
        return;
    }
    handleKey(event->key(), true);
}

void PlatformQt::keyReleaseEvent(QKeyEvent *event) {
    handleKey(event->key(), false);
}

void PlatformQt::closeEvent(QCloseEvent *) {
    quitRequested = true;
}

void PlatformQt::handleKey(int key, bool pressed) {
    if (!keysPtr)
        return;

    // CHIP-8 keypad mapping (same as SDL version):
    // 1 2 3 C    ->   1 2 3 4
    // 4 5 6 D    ->   Q W E R
    // 7 8 9 E    ->   A S D F
    // A 0 B F    ->   Z X C V
    switch (key) {
    case Qt::Key_1:
        keysPtr[0x1] = pressed;
        break;
    case Qt::Key_2:
        keysPtr[0x2] = pressed;
        break;
    case Qt::Key_3:
        keysPtr[0x3] = pressed;
        break;
    case Qt::Key_4:
        keysPtr[0xC] = pressed;
        break;

    case Qt::Key_Q:
        keysPtr[0x4] = pressed;
        break;
    case Qt::Key_W:
        keysPtr[0x5] = pressed;
        break;
    case Qt::Key_E:
        keysPtr[0x6] = pressed;
        break;
    case Qt::Key_R:
        keysPtr[0xD] = pressed;
        break;

    case Qt::Key_A:
        keysPtr[0x7] = pressed;
        break;
    case Qt::Key_S:
        keysPtr[0x8] = pressed;
        break;
    case Qt::Key_D:
        keysPtr[0x9] = pressed;
        break;
    case Qt::Key_F:
        keysPtr[0xE] = pressed;
        break;

    case Qt::Key_Z:
        keysPtr[0xA] = pressed;
        break;
    case Qt::Key_X:
        keysPtr[0x0] = pressed;
        break;
    case Qt::Key_C:
        keysPtr[0xB] = pressed;
        break;
    case Qt::Key_V:
        keysPtr[0xF] = pressed;
        break;

    default:
        break;
    }
}
