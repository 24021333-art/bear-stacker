#include "../include/block.h"
#include "../include/constants.h"
// include struct Block và các hằng số game

void Block::update() {
    // Hàm cập nhật vị trí block mỗi frame

    if (!isFalling) {
        // Nếu block chưa rơi thì di chuyển ngang

        x += movingRight ? speed : -speed;
        // Nếu movingRight = true -> cộng speed
        // false -> trừ speed
        // giúp block chạy qua lại trái phải

        if (x <= 0) {
            // chạm mép trái màn hình

            x = 0;
            // giữ block không đi ra ngoài

            movingRight = true;
            // đổi hướng sang phải
        } else if (x + w >= LOGICAL_WIDTH) {
            // chạm mép phải màn hình

            x = LOGICAL_WIDTH - w;
            // giữ block nằm trong màn hình

            movingRight = false;
            // đổi hướng sang trái
        }
    } else {
        // Nếu đang rơi thì chỉ cập nhật theo trục y
        y += 22.0f;
        // tăng y để block rơi xuống
    }
}

void Block::renderBear(SDL_Renderer* renderer,
                       SDL_Texture* texture,
                       float offset) {
    // Hàm vẽ block gấu lên màn hình

    if (!texture) return;
    // nếu ảnh lỗi thì không vẽ

    float texW, texH;
    SDL_GetTextureSize(texture, &texW, &texH);
    // lấy kích thước ảnh gốc

    float scale = 1.45f;
    // scale ảnh to hơn block logic cho dễ nhìn

    float drawW = w * scale;
    // chiều rộng ảnh sau scale

    float ratio = texH / texW;
    // giữ tỉ lệ gốc của ảnh

    float drawH = drawW * ratio;
    // tính chiều cao ảnh theo tỉ lệ

    float drawX = x - (drawW - w) / 2.0f;
    // căn giữa ảnh theo block logic

    float drawY = y - drawH + h - offset - 20.0f;
    // tính vị trí y thực tế
    // trừ offset để camera cuộn theo stack

    SDL_FRect dst = { drawX, drawY, drawW, drawH };
    // tạo hình chữ nhật đích để render

    SDL_RenderTexture(renderer, texture, NULL, &dst);
    // vẽ texture gấu lên màn hình
}