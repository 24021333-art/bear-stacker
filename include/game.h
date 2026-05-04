#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <vector>
#include <map>
#include <string>

#include "block.h"
#include "constants.h"

using namespace std;

// ===== GAME STATE =====
enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};
// Enum dùng để quản lý trạng thái game
// MENU = menu ban đầu
// PLAYING = đang chơi
// GAME_OVER = thua

// ===== STAR =====
struct Star {
    float x, y;
    float brightness;
    float pulseSpeed;
};
// Struct lưu dữ liệu 1 ngôi sao nền trời

// ===== COLOR =====
struct Color {
    Uint8 r, g, b, a;
};
// Struct lưu màu RGBA cho sky transition

class Game {
public:
    // ===== STATE =====
    GameState state = MENU;
    // trạng thái hiện tại của game

    // ===== DATA =====
    vector<Block> stack;
    // danh sách các gấu đã đặt thành công

    Block current;
    // block hiện tại đang di chuyển

    vector<Star> stars;
    // danh sách sao để render background

    map<string, SDL_Texture*> textureCache;
    // cache texture theo đường dẫn ảnh
    // tránh load ảnh nhiều lần

    int score = 0;
    int highScore = 0;
    // điểm hiện tại và điểm cao nhất

    float cameraOffset = 0;
    float targetCameraOffset = 0;
    // dùng để camera cuộn theo stack gấu

    // ===== AUDIO =====
    MIX_Mixer* mixer = nullptr;
    // thiết bị mixer chính

    MIX_Audio* bgMusic = nullptr;
    MIX_Audio* fallSound = nullptr;
    MIX_Audio* gameStartSound = nullptr;
    MIX_Audio* gameOverSound = nullptr;
    MIX_Audio* newHighScoreSound = nullptr;
    // các file âm thanh

    MIX_Track* bgTrack = nullptr;
    // track nhạc nền lặp vô hạn

    // ===== SKY COLOR =====
    Color nightSky = {10, 20, 40, 255};
    Color daySky = {135, 206, 235, 255};
    // màu bầu trời đêm và ngày

    // ===== INIT =====
    void initEnvironment(SDL_Renderer* renderer, MIX_Mixer* m);
    // load texture, âm thanh, sao nền

    void init(SDL_Renderer* renderer, MIX_Mixer* m);
    // reset game khi bắt đầu ván mới

    // ===== GAMEPLAY =====
    void createNewBlock();
    // tạo block mới sau mỗi lần đặt thành công

    void handleLanding();
    // xử lý khi block rơi chạm stack

    void update();
    // cập nhật game mỗi frame

    bool isPerfectLanding();
    // kiểm tra người chơi đặt có chuẩn giữa không

    float calculateOverlap();
    // tính phần giao nhau giữa 2 block

    void processGameOver();
    // xử lý thua game

    void processSuccessfulLanding();
    // xử lý đặt block thành công

    // ===== RENDER =====
    Color interpolateColor(Color c1, Color c2, float t);
    // nội suy màu giữa ngày và đêm

    void fillCircle(SDL_Renderer* renderer, float cx, float cy, float r);
    // vẽ mặt trời / mặt trăng

    void drawBackground(SDL_Renderer* renderer,
                        SDL_Texture* bgTex,
                        float cameraOffset);
    // vẽ background + sky + stars + moon + sun

    void drawText(SDL_Renderer* renderer,
                  TTF_Font* font,
                  string text,
                  int x,
                  int y,
                  SDL_Color color,
                  bool center = false);
    // vẽ text lên màn hình

    void render(SDL_Renderer* renderer,
                SDL_Texture* bgTex,
                TTF_Font* big,
                TTF_Font* mid,
                TTF_Font* small);
    // render toàn bộ game mỗi frame
};