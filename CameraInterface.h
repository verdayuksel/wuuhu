#ifndef CAMERA_INTERFACE_H
#define CAMERA_INTERFACE_H

#include <GLFW/glfw3.h>

class CameraInterface {
public:
    CameraInterface();
    ~CameraInterface();
    void Initialize();
    void Shutdown();
    void RenderUI();
    GLFWwindow* getWindow() const { return window; }

private:
    void DrawCrosshair();
    void DrawSettingsMenu();
    void DrawCalibrationScreen();

    GLFWwindow* window;
    bool showCalibration; // Kalibrasyon ekranını göster/gizle değişkeni
};

#endif // CAMERA_INTERFACE_H
