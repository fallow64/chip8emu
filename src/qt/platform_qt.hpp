#pragma once

#include "platform.hpp"
#include <QImage>
#include <QKeyEvent>
#include <QWidget>

class PlatformQt : public QWidget, public IPlatform {
    Q_OBJECT

  public:
    PlatformQt(int textureWidth = 64, int textureHeight = 32, int scale = 10,
               QWidget *parent = nullptr);
    ~PlatformQt() override;

    void Update(void const *buffer, int pitch) override;
    bool ProcessInput(uint8_t *keys) override;

    bool shouldQuit() const { return quitRequested; }

  protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

  private:
    void handleKey(int key, bool pressed);

    QImage framebuffer;
    int textureWidth;
    int textureHeight;
    int scale;
    bool quitRequested = false;
    uint8_t *keysPtr = nullptr;
};
