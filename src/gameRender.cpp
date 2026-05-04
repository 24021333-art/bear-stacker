#include "../include/game.h"
#include "../include/constants.h"
#include <algorithm>
#include <string>
#include <cmath>

using namespace std;

// ===== COLOR =====
Color Game::interpolateColor(Color c1, Color c2, float t) {
    t = max(0.0f, min(1.0f, t));
    return { (Uint8)(c1.r + t * (c2.r - c1.r)), 
             (Uint8)(c1.g + t * (c2.g - c1.g)), 
             (Uint8)(c1.b + t * (c2.b - c1.b)), 
             255 };
}

// ===== DRAW CIRCLE =====
void Game::fillCircle(SDL_Renderer* renderer, float cx, float cy, float r) {
    for (float w = -r; w <= r; w++) {
        for (float h = -r; h <= r; h++) {
            if (w * w + h * h <= r * r) {
                SDL_RenderPoint(renderer, cx + w, cy + h);
            }
        }
    }
}

// ===== BACKGROUND =====
void Game::drawBackground(SDL_Renderer* renderer, SDL_Texture* bgTex, float cameraOffset) {
    float tSky = 0.0f;
    // Giữ nguyên logic tính tSky theo score của bạn
    if (score < 25) tSky = 0.0f;
    else if (score < 40) tSky = (score - 25) / 15.0f;
    else if (score < 60) tSky = 1.0f;
    else if (score < 75) tSky = 1.0f - (score - 60) / 15.0f;
    else tSky = 0.0f;

    Color currentSkyColor = interpolateColor(nightSky, daySky, tSky);
    SDL_SetRenderDrawColor(renderer, currentSkyColor.r, currentSkyColor.g, currentSkyColor.b, 255);
    SDL_RenderClear(renderer);

    // FIX LỖI BACKGROUND: Chỉ vẽ 1 tấm trôi theo cameraOffset
    // Nếu vẽ 2 tấm nối đuôi mà texture không chuẩn sẽ bị hiện đè/lặp
    if (bgTex) {
        float bgOffset = cameraOffset * 0.2f; 
        // Vẽ tấm background trôi theo parallax
        SDL_FRect bgRect = {0, -bgOffset, (float)LOGICAL_WIDTH, (float)LOGICAL_HEIGHT};
        SDL_RenderTexture(renderer, bgTex, NULL, &bgRect);
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // FIX LỖI SAO: Dùng fmod để sao xuất hiện liên tục không bị mất khi lên cao
    if (tSky < 1.0f) {
        for (auto& s : stars) {
            s.brightness += s.pulseSpeed;
            if (s.brightness > 1.0f || s.brightness < 0.2f) s.pulseSpeed *= -1;

            float drawY = fmod(s.y - cameraOffset * 0.3f, (float)LOGICAL_HEIGHT);
            if (drawY < 0) drawY += LOGICAL_HEIGHT;

            Uint8 alpha = (Uint8)(s.brightness * 255 * (1.0f - tSky));
            if (alpha > 0) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
                SDL_RenderPoint(renderer, s.x, drawY);
            }
        }
    }

    // MOON: Cập nhật vị trí mặt trăng theo bạn của bạn (Nhô cao lên 200)
    if ((score >= 10 && score <= 25) || score >= 60) {
        float tMoonRise = min(1.0f, (score >= 60) ? 1.0f : (score - 10) / 5.0f);
        Uint8 mAlpha = (Uint8)(230 * tMoonRise * (1.0f - tSky));

        if (mAlpha > 0) {
            SDL_SetRenderDrawColor(renderer, 240, 240, 220, mAlpha);
            // Vị trí y = 200 như code bạn của bạn
            fillCircle(renderer, (float)LOGICAL_WIDTH - 250, 200, 60);
        }
    }

    // SUN: Mặt trời cố định ở y = 200 như code bạn của bạn
    if (score >= 30 && score <= 65) {
        float tSunRise = min(1.0f, (score - 25) / 10.0f);
        Uint8 sAlpha = (Uint8)(255 * tSunRise);
        SDL_SetRenderDrawColor(renderer, 255, 240, 180, sAlpha);
        fillCircle(renderer, 250, 200, 80);
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

// ===== TEXT & RENDER (Giữ nguyên cấu trúc của bạn) =====
void Game::drawText(SDL_Renderer* renderer, TTF_Font* font, string text, int x, int y, SDL_Color color, bool center) {
    if (!font) return;
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), text.length(), color);
    if (!surface) return;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    float w = (float)surface->w;
    float h = (float)surface->h;
    if (center) x = (int)((LOGICAL_WIDTH - w) / 2.0f);
    SDL_FRect dst = {(float)x, (float)y, w, h};
    SDL_RenderTexture(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

void Game::render(SDL_Renderer* renderer, SDL_Texture* bgTex, TTF_Font* big, TTF_Font* mid, TTF_Font* small) {
    drawBackground(renderer, bgTex, cameraOffset);
    if (state == MENU) {
        drawText(renderer, big, "BEAR STACKER", 0, 350, {255,215,0,255}, true);
        drawText(renderer, small, "PRESS SPACE TO START", 0, 550, {255,255,255,255}, true);
    } else {
        for (auto& b : stack) {
            b.renderBear(renderer, textureCache[bearTypes[b.bearType]], cameraOffset);
        }
        if (state == PLAYING) {
            current.renderBear(renderer, textureCache[bearTypes[current.bearType]], cameraOffset);
            drawText(renderer, mid, "Score: " + to_string(score), 60, 60, {255,255,255,255});
        } else {
            drawText(renderer, big, "GAME OVER", 0, 300, {255,50,50,255}, true);
            drawText(renderer, mid, "SCORE: " + to_string(score), 0, 450, {255,215,0,255}, true);
            drawText(renderer, small, "BEST: " + to_string(highScore), 0, 550, {255,255,255,255}, true);
        }
    }
    SDL_RenderPresent(renderer);
}