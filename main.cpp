#if __has_include(<raylib.h>)
#include "Dependencies.h"
#else
// Minimal compatibility layer for environments without the Raylib headers.
struct Vector2 {
    float x = 0.f;
    float y = 0.f;
};

struct Rectangle {
    float x = 0.f;
    float y = 0.f;
    float width = 0.f;
    float height = 0.f;
};

struct Color {
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 255;
};

struct Image {
    int width = 0;
    int height = 0;
    void* data = nullptr;
};

struct Texture2D {
    int id = 0;
};

constexpr Color BLACK{0, 0, 0, 255};
constexpr Color WHITE{255, 255, 255, 255};

constexpr int KEY_LEFT_SHIFT = 340;
constexpr int KEY_LEFT_CONTROL = 341;
constexpr int KEY_SPACE = 32;
constexpr int KEY_W = 87;
constexpr int KEY_S = 83;
constexpr int KEY_D = 68;
constexpr int KEY_A = 65;

inline bool IsKeyDown(int) { return false; }
inline Vector2 GetMouseDelta() { return {}; }
inline float GetFrameTime() { return 1.f / 60.f; }
inline int GetScreenWidth() { return 1280; }
inline int GetScreenHeight() { return 960; }

inline void InitWindow(int, int, const char*) {}
inline void SetTargetFPS(int) {}
inline void DisableCursor() {}
inline bool WindowShouldClose() { return false; }
inline void BeginDrawing() {}
inline void ClearBackground(Color) {}
inline void DrawTexturePro(Texture2D, Rectangle, Rectangle, Vector2, float, Color) {}
inline void DrawFPS(int, int) {}
inline void DrawText(const char*, int, int, int, Color) {}
inline void EndDrawing() {}
inline void UnloadTexture(Texture2D) {}
inline void CloseWindow() {}
inline Image GenImageColor(int width, int height, Color) { return {width, height, nullptr}; }
inline Texture2D LoadTextureFromImage(Image) { return {}; }
inline void UnloadImage(Image) {}
inline void UpdateTexture(Texture2D, const void*) {}
#endif

#include <vector>
#include <tuple>
#include <algorithm>
#include <cmath>




// ==========================================
// RENDERING
// ==========================================

constexpr int WIDTH = 640;
constexpr int HEIGHT = 480;
constexpr float FOV = 1.05f;

Color to_color(const vec3& color) {
    auto convert = [](float value) {
        value = std::max(0.f, value);
        value = value / (1.f + value);
        value = std::pow(value, 1.f / 2.2f);
        return static_cast<unsigned char>(std::clamp(value * 255.f, 0.f, 255.f));
    };

    return {
        convert(color.x),
        convert(color.y),
        convert(color.z),
        255
    };
}

void render(
    std::vector<Color>& framebuffer,
    const RTCamera3D& camera,
    int max_depth
) {
    vec3 forward = camera.forward();
    vec3 right = camera.right();
    vec3 up = camera.up();

    float focal_length = HEIGHT / (2.f * std::tan(FOV / 2.f));
    int total_pixels = WIDTH * HEIGHT;

    #pragma omp parallel for schedule(dynamic, 64)
    for (int i = 0; i < total_pixels; i++) {
        int x = i % WIDTH;
        int y = i / WIDTH;

        float screen_x = (x + 0.5f - WIDTH / 2.f);
        float screen_y = -(y + 0.5f - HEIGHT / 2.f);

        vec3 direction = (
            forward * focal_length +
            right * screen_x +
            up * screen_y
        ).normalized();

        vec3 color = cast_ray(camera.position, direction, max_depth);
        framebuffer[i] = to_color(color);
    }
}

// ==========================================
// MAIN
// ==========================================

int main() {
    InitWindow(1280, 960, "Ray Tracing + Static Cache + Adaptive Recursion + OpenMP");
    SetTargetFPS(60);
    DisableCursor();

    RTCamera3D camera;
    std::vector<Color> framebuffer(WIDTH * HEIGHT);

    Image image = GenImageColor(WIDTH, HEIGHT, BLACK);
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);

    bool force_redraw = true; // При старте рендерим первый кадр
    bool needs_high_quality_render = true; // После движения требуется качественный пересчет

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        
        // Проверяем, двигается ли камера в данный момент
        bool camera_moved = camera.update(dt);

        if (camera_moved) {
            render(framebuffer, camera, 2); // Быстрый рендер (глубина 2) при движении
            UpdateTexture(texture, framebuffer.data());
            needs_high_quality_render = true;
        } else if (needs_high_quality_render) {
            render(framebuffer, camera, 4); // Финальный качественный рендер (глубина 4) в статике
            UpdateTexture(texture, framebuffer.data());
            needs_high_quality_render = false; // Замораживаем пересчет
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawTexturePro(
            texture,
            {0, 0, (float)WIDTH, (float)HEIGHT},
            {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
            {0, 0},
            0,
            WHITE
        );

        DrawFPS(10, 10);
        DrawText(
            "WASD: Move | Mouse: Look | SPACE/CTRL: Up/Down | (Static Cache Active)",
            10, 35, 18, WHITE
        );

        EndDrawing();
    }

    UnloadTexture(texture);
    CloseWindow();

    return 0;
}