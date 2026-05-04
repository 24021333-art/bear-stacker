#include "../include/game.h"
#include "../include/constants.h"
#include <cmath>
#include <algorithm>

using namespace std;

// ===== Perfect landing =====
bool Game::isPerfectLanding() {
    // Kiểm tra stack có rỗng không
    if (stack.empty()) return false;

    Block& last = stack.back();
    // lấy block cuối cùng trong stack

    return std::fabs(current.x - last.x) < PERFECT_THRESHOLD;
    // nếu độ lệch vị trí nhỏ hơn ngưỡng cho phép
    // thì được tính là perfect landing
}

// ===== Tính overlap =====
float Game::calculateOverlap() {
    if (stack.empty()) return 0.0f;
    // nếu chưa có block nào thì overlap = 0

    Block& last = stack.back();
    // lấy block trên cùng của stack

    float left = max(current.x, last.x);
    // mép trái phần giao nhau

    float right = min(current.x + current.w, last.x + last.w);
    // mép phải phần giao nhau

    return max(0.0f, right - left);
    // trả về chiều rộng phần overlap
    // nếu âm thì trả 0
}

// ===== Game over =====
void Game::processGameOver() {
    if (score > highScore) {
        // nếu phá kỷ lục điểm cao

        highScore = score;
        // cập nhật điểm cao mới

        if (newHighScoreSound) {
            MIX_PlayAudio(mixer, newHighScoreSound);
            // phát âm thanh high score
        }
    }

    if (bgTrack) {
        MIX_StopTrack(bgTrack, 0);
        // dừng nhạc nền
    }

    if (gameOverSound) {
        MIX_PlayAudio(mixer, gameOverSound);
        // phát âm thanh thua
    }

    state = GAME_OVER;
    // chuyển trạng thái game sang game over
}

// ===== Đặt block thành công =====
void Game::processSuccessfulLanding() {
    if (stack.empty()) return;

    Block& last = stack.back();
    // lấy block cuối stack

    current.y = last.y - GAP_DISTANCE;
    // đặt block mới nằm trên block cũ

    current.isFalling = false;
    // block đã đứng yên sau khi chạm

    stack.push_back(current);
    // thêm block vào stack chính

    score++;
    // tăng điểm

    float viewThreshold = 600.0f;
    // ngưỡng camera bắt đầu cuộn

    if (current.y < viewThreshold) {
        targetCameraOffset = current.y - viewThreshold;
        // camera sẽ cuộn lên theo stack
    }

    createNewBlock();
    // tạo block mới cho lượt tiếp theo
}