#pragma once

#include <SDL3/SDL.h>

struct Block {

    float x, y, w, h;
    // x = vị trí ngang
    // y = vị trí dọc
    // w = chiều rộng block
    // h = chiều cao block

    float speed;
    // tốc độ di chuyển ngang của block

    bool movingRight;
    // true = đang đi sang phải
    // false = đang đi sang trái

    bool isFalling = false;
    // kiểm tra block có đang rơi xuống không
    // mặc định false = chưa rơi

    int bearType = 0;
    // loại hình gấu đang dùng
    // số này dùng để lấy ảnh trong bearTypes

    void update();
    // hàm cập nhật vị trí block mỗi frame
    // xử lý block chạy trái phải hoặc rơi xuống

    void renderBear(SDL_Renderer* renderer, SDL_Texture* texture, float offset);
    // hàm vẽ block gấu lên màn hình
    // renderer = công cụ vẽ
    // texture = ảnh con gấu
    // offset = camera offset để màn hình cuộn lên
};