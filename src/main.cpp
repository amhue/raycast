#include <array>
#include <cmath>
#include <iostream>
#include <math.h>
#include <raylib.h>
#include <sys/types.h>

constexpr int win_width = 600;
constexpr int win_height = 480;
constexpr int unit_sz = 40;

using Map = std::array<std::array<u_int8_t, win_width / unit_sz>,
    win_height / unit_sz>;

void draw_map_2d(const Map& map)
{
    for (int i = 0; i < map.size(); ++i)
        for (int j = 0; j < map[i].size(); ++j) {
            Color color { map[i][j] == 1 ? BLACK : WHITE };
            DrawRectangle(j * unit_sz, i * unit_sz, unit_sz, unit_sz, color);
            DrawRectangleLines(
                j * unit_sz, i * unit_sz, unit_sz, unit_sz, Color { GRAY });
        }
}

void player_control(
    Vector2& player, int win_width, int win_height, double& rotation)
{
    if (IsKeyDown(KEY_W)) {
        player.y += -cos(-rotation * PI / 180) * 5;
        player.x += sin(rotation * PI / 180) * 5;
    }
    if (IsKeyDown(KEY_S)) {
        player.y -= -cos(-rotation * PI / 180) * 5;
        player.x -= sin(rotation * PI / 180) * 5;
    }
    if (IsKeyDown(KEY_D))
        rotation += 2;
    if (IsKeyDown(KEY_A))
        rotation -= 2;

    if (player.x < 0)
        player.x = 0;
    if (player.y < 0)
        player.y = 0;
    if (player.x > win_width)
        player.x = win_width;
    if (player.y > win_height)
        player.y = win_height;

    rotation = static_cast<int>(rotation) % 360;

    DrawPoly(player, 4, 5, rotation, DARKBLUE);
    DrawLine(player.x, player.y, player.x + sin(rotation * PI / 180) * 60,
        player.y + -cos(-rotation * PI / 180) * 60, RED);
    std::cout << player.x << ' ' << player.y << ' ' << rotation << '\n';
}

void raycast(Vector2& player, Vector2& dir_vector, Vector2& plane_vector) { }

int main(int argc, const char* argv[])
{

    constexpr Map map { {
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    } };

    InitWindow(win_width, win_height, "Raycasting");
    Vector2 player { 60, 100 };
    Vector2 dir_vector {};
    Vector2 plane_vector {};

    double rotation = 0;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw_map_2d(map);
        player_control(player, win_width, win_height, rotation);
        raycast(player, dir_vector, plane_vector);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
