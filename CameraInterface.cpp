#include <GL/glew.h>        // GLEW başlık dosyası önce
#include <GLFW/glfw3.h>     // GLFW başlık dosyası
#include "CameraInterface.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <iostream>

CameraInterface::CameraInterface() : window(nullptr), showCalibration(false) {
    // Constructor işlemleri
}

CameraInterface::~CameraInterface() {
    Shutdown();
}

void CameraInterface::Initialize() {
    // GLFW başlat
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed!" << std::endl;
        return;
    }

    // GLFW pencere oluştur
    window = glfwCreateWindow(1280, 720, "Camera Interface", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW window creation failed!" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // V-Sync

    // GLEW'yi başlat
    glewExperimental = GL_TRUE; // Core profile destekle
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        std::cerr << "GLEW initialization failed: " << glewGetErrorString(glewError) << std::endl;
        return;
    }

    // Desteklenen GLSL sürümünü kontrol et
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::cout << "Supported GLSL version: " << glslVersion << std::endl;

    // ImGui'yi başlat
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // ImGui stilini ayarla
    ImGui::StyleColorsDark();

    // ImGui backend'lerini başlat
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 120"); // Sürüm 130 genelde daha geniş uyumluluk sağlar
}

void CameraInterface::Shutdown() {
    if (window) {
        // ImGui backend'lerini kapat
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        // GLFW kaynaklarını serbest bırak
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}

// RenderUI fonksiyonuna ekleyin
void CameraInterface::RenderUI() {
    // OpenGL ekran temizleme işlemleri
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Eğer showCalibration true ise kalibrasyon ekranını çiz
    if (showCalibration) {
        DrawCalibrationScreen();
    }

    // ImGui frame'ini başlat
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // UI Render işlemleri
    ImGui::Begin("Settings");

    static bool showSquare = false;
    if (ImGui::Button("Show Pink Square")) {
        showSquare = !showSquare; // Butona tıklandığında kareyi göster/gizle
    }

    if (ImGui::Button("Kalibrasyon")) {
        showCalibration = !showCalibration; // Kalibrasyon ekranını göster/gizle
    }

    ImGui::End();

    // ImGui frame'ini bitir ve render et
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Eğer showSquare true ise pembe kare çiz
    if (showSquare) {
        // OpenGL çizim ayarlarını yap
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); // Ortho proje ayarları
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Pembe kareyi çiz
        glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 1.0f); // Pembe renk
        glVertex2f(-0.05f, -0.05f);
        glVertex2f( 0.05f, -0.05f);
        glVertex2f( 0.05f,  0.05f);
        glVertex2f(-0.05f,  0.05f);
        glEnd();
    }

    // Ekranı güncelle
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void CameraInterface::DrawCrosshair() {
    // Ortada bir odak işareti çiz
    glBegin(GL_LINES);
    glVertex2f(-0.1f, 0.0f);
    glVertex2f(0.1f, 0.0f);
    glVertex2f(0.0f, -0.1f);
    glVertex2f(0.0f, 0.1f);
    glEnd();
}

void CameraInterface::DrawSettingsMenu() {
    // Ayarlar menüsünü çiz
}

void CameraInterface::DrawCalibrationScreen() {
    // OpenGL çizim ayarlarını yap
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0); // Ortho proje ayarları
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Kalibrasyon ekranının üst kısmında bir dikdörtgen çerçeve
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.1f, 0.8f);
    glVertex2f(0.9f, 0.8f);
    glVertex2f(0.9f, 0.9f);
    glVertex2f(0.1f, 0.9f);
    glEnd();

    // Hareketli çizgi doğrusu
    static float offset = 0.0f;
    offset += 0.01f; // Çizginin hareket hızını kontrol edin
    if (offset > 1.0f) offset -= 1.0f;

    glBegin(GL_LINES);
    glVertex2f(0.1f + offset, 0.85f);
    glVertex2f(0.9f + offset, 0.85f);
    glEnd();

    // Zoom yapılmış bölüm
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.1f, 0.1f);
    glVertex2f(0.3f, 0.1f);
    glVertex2f(0.3f, 0.3f);
    glVertex2f(0.1f, 0.3f);
    glEnd();

    // Dikey çizgi doğrusu sol tarafta
    glBegin(GL_LINES);
    glVertex2f(0.2f, 0.2f);
    glVertex2f(0.2f, 0.8f);
    glEnd();

    // Ekran üzerinde rastgele sayılar (örnek)
    // (Daha karmaşık bir sayı yerleştirme işlemi gerektirir)
}

// Ana fonksiyon
int main(int argc, char** argv) {
    try {
        // CameraInterface sınıfını başlat
        CameraInterface cameraInterface;

        // Başlatma
        cameraInterface.Initialize();

        // Ana döngü
        while (!glfwWindowShouldClose(cameraInterface.getWindow())) {
            // UI render et
            cameraInterface.RenderUI();
        }

        // Kapatma
        cameraInterface.Shutdown();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
