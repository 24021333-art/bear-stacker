#include "../include/game.h"
#include "../include/constants.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// ================= INIT ENVIRONMENT =================
void Game::initEnvironment(SDL_Renderer* renderer, MIX_Mixer* m) {
    mixer = m;
    // lưu mixer để phát âm thanh toàn game

    stars.clear();
    // reset danh sách sao cũ

    for (int i = 0; i < 100; i++) {
        stars.push_back({
            (float)(rand() % (int)LOGICAL_WIDTH),
            // random vị trí x

            (float)(rand() % ((int)LOGICAL_HEIGHT - 300)),
            // random vị trí y

            (float)(rand() % 100) / 100.0f,
            // độ sáng ban đầu

            0.02f + (rand() % 5) / 100.0f
            // tốc độ nhấp nháy
        });
    }

    // load texture gấu
    for (const auto& file : bearTypes) {
        if (!textureCache.count(file)) {
            // nếu texture chưa có trong cache thì mới load

            SDL_Texture* tex = IMG_LoadTexture(renderer, file.c_str());

            if (tex) textureCache[file] = tex;
            // lưu texture vào cache
        }
    }

    // load âm thanh
    if (!bgMusic) bgMusic = MIX_LoadAudio(mixer, "Sound/Nhacnen.wav", false);
    if (!fallSound) fallSound = MIX_LoadAudio(mixer, "Sound/falling.wav", false);
    if (!gameStartSound) gameStartSound = MIX_LoadAudio(mixer, "Sound/gameStart.wav", false);
    if (!gameOverSound) gameOverSound = MIX_LoadAudio(mixer, "Sound/gameOver.wav", false);
    if (!newHighScoreSound) newHighScoreSound = MIX_LoadAudio(mixer, "Sound/tada.wav", false);

    if (mixer && !bgTrack) bgTrack = MIX_CreateTrack(mixer);
    // tạo track nhạc nền
}

// ================= INIT GAME =================
void Game::init(SDL_Renderer* renderer, MIX_Mixer* m) {
    stack.clear();
    // xóa toàn bộ stack cũ

    score = 0;
    // reset điểm

    cameraOffset = 0;
    targetCameraOffset = 0;
    // reset camera

    initEnvironment(renderer, m);
    // load lại texture, sao, audio

    if (bgTrack && bgMusic) {
        MIX_SetTrackAudio(bgTrack, bgMusic);
        // gắn audio vào track

        SDL_PropertiesID props = SDL_CreateProperties();

        SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
        // loop vô hạn

        MIX_PlayTrack(bgTrack, props);
        // phát nhạc nền

        SDL_DestroyProperties(props);
    }

    // tạo block nền đầu tiên
    stack.push_back({
        (LOGICAL_WIDTH - 450) / 2.0f,
        LOGICAL_HEIGHT - 150.0f,
        450.0f,
        BEAR_HEIGHT_LOGIC,
        0.0f,
        true,
        false,
        0
    });

    createNewBlock();
    // tạo block đầu tiên cho người chơi
}

// ================= CREATE BLOCK =================
void Game::createNewBlock() {
    float lastW = stack.back().w;
    // lấy width block trước

    float newY = stack.back().y - 400.0f;
    // block mới xuất hiện cao hơn block cũ

    int nextBearType = rand() % (int)bearTypes.size();
    // random loại gấu

    float moveSpeed = min(25.0f, 7.0f + score * 3.0f);
    // điểm càng cao tốc độ càng tăng

    current = {
        0,
        newY,
        lastW,
        BEAR_HEIGHT_LOGIC,
        moveSpeed,
        rand()%2==0,
        false,
        nextBearType
    };
}

// ================= HANDLE LANDING =================
void Game::handleLanding() {
    if (fallSound) MIX_PlayAudio(mixer, fallSound);
    // phát sound khi block chạm

    float overlap = calculateOverlap();
    // tính phần giao nhau

    if (overlap <= 0) {
        // không chồng lên nhau -> thua
        processGameOver();
        return;
    }

    processSuccessfulLanding();
    // đặt thành công
}

// ================= UPDATE =================
void Game::update() {
    if (state == PLAYING) {
        current.update();
        // update block hiện tại mỗi frame

        if (current.isFalling &&
            current.y + BEAR_HEIGHT_LOGIC >= stack.back().y) {
            // block đã rơi tới stack
            handleLanding();
        }
    }

    cameraOffset += (targetCameraOffset - cameraOffset) * 0.1f;
    // camera di chuyển mượt lên theo easing
}