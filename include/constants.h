#pragma once
// Tránh file header bị include nhiều lần

#include <vector>
#include <string>
// Dùng vector để chứa danh sách ảnh gấu
// string để lưu đường dẫn file ảnh

using namespace std;

// ===== GAME SIZE =====
const float LOGICAL_WIDTH = 1920.0f;
// chiều rộng logic của game
// dùng để scale game đồng nhất trên mọi màn hình

const float LOGICAL_HEIGHT = 1080.0f;
// chiều cao logic của game

const float BEAR_HEIGHT_LOGIC = 45.0f;
// chiều cao logic của 1 block gấu

const float GAP_DISTANCE = 160.0f;
// khoảng cách dọc giữa 2 con gấu khi stack lên

const float PERFECT_THRESHOLD = 15.0f;
// ngưỡng cho phép để tính perfect landing
// block lệch ít hơn 15 pixel sẽ được tính là chuẩn

// ===== BEAR TEXTURES =====
inline vector<string> bearTypes = {
    "bear/Binhthuong.png",
    "bear/Binhthuong1.png",
    "bear/Binhthuong2.png",
    "bear/Chiukho.png",
    "bear/Metmoi.png",
    "bear/Sayhi.png",
    "bear/Uongnuoc.png",
    "bear/NghichDT.png"
};
// Danh sách tất cả đường dẫn ảnh gấu
// game sẽ random một ảnh từ danh sách này