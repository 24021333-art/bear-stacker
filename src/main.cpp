#include "../include/game.h"
#include "../include/constants.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <cstdlib>
#include <ctime>

int main() {
    srand((unsigned)time(NULL));
    // khởi tạo random seed để random gấu và sao khác nhau mỗi lần chạy

    // ===== INIT SDL =====
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    // khởi tạo hệ thống video + audio

    TTF_Init();
    // khởi tạo SDL_ttf để dùng font chữ

    MIX_Init();
    // khởi tạo SDL_mixer cho âm thanh

    // ===== CREATE MIXER =====
    MIX_Mixer* mixer =
        MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    // tạo thiết bị mixer phát âm thanh mặc định

    // ===== WINDOW =====
    SDL_Window* window =
        SDL_CreateWindow("Bear Stacker Pro",
                         0,
                         0,
                         SDL_WINDOW_FULLSCREEN);
    // tạo cửa sổ game fullscreen

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, NULL);
    // renderer dùng để vẽ mọi thứ lên màn hình

    SDL_SetRenderLogicalPresentation(
        renderer,
        (int)LOGICAL_WIDTH,
        (int)LOGICAL_HEIGHT,
        SDL_LOGICAL_PRESENTATION_LETTERBOX
    );
    // thiết lập độ phân giải logic cố định
    // giúp game scale đẹp trên mọi màn hình

    // ===== FONT =====
    TTF_Font* big = TTF_OpenFont("font/TIMES.TTF", 120);
    TTF_Font* mid = TTF_OpenFont("font/TIMES.TTF", 80);
    TTF_Font* small = TTF_OpenFont("font/TIMES.TTF", 45);
    // load 3 font size cho title, score, text nhỏ

    // ===== BACKGROUND =====
    SDL_Texture* backgroundTexture =
        IMG_LoadTexture(renderer, "background2.png");
    // load texture background

    // ===== GAME =====
    Game game;
    // tạo object game chính

    game.init(renderer, mixer);
    // khởi tạo dữ liệu ban đầu cho game

    bool quit = false;
    SDL_Event e;
    // biến điều khiển vòng lặp và event

    // ===== GAME LOOP =====
    while (!quit) {
        // loop chạy liên tục cho tới khi thoát game

        while (SDL_PollEvent(&e)) {
            // lấy toàn bộ input event từ bàn phím / cửa sổ

            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
                // bấm nút X để thoát
            }

            if (e.type == SDL_EVENT_KEY_DOWN) {
                // xử lý khi nhấn phím

                if (e.key.key == SDLK_SPACE) {
                    // nhấn SPACE

                    if (game.state == PLAYING) {
                        // nếu đang chơi

                        if (!game.current.isFalling) {
                            game.current.isFalling = true;
                            // block bắt đầu rơi
                        }
                    }
                    else {
                        // nếu đang menu hoặc game over

                        if (game.gameStartSound) {
                            MIX_PlayAudio(mixer, game.gameStartSound);
                            // phát âm thanh bắt đầu
                        }

                        game.init(renderer, mixer);
                        // reset game mới

                        game.state = PLAYING;
                        // chuyển sang trạng thái chơi
                    }
                }

                if (e.key.key == SDLK_ESCAPE) {
                    quit = true;
                    // nhấn ESC để thoát
                }
            }
        }

        game.update();
        // update logic game mỗi frame

        game.render(renderer,
                    backgroundTexture,
                    big,
                    mid,
                    small);
        // render frame mới

        SDL_Delay(10);
        // delay nhỏ để game chạy ổn định
    }

    return 0;
}