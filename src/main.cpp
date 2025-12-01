#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <raylib.h>

constexpr int WIN_WIDTH = 600;
constexpr int WIN_HEIGHT = 480;
constexpr int UNIT_SZ = 40;
constexpr int MAP_UNIT_SZ = 10;
constexpr float FOV = 90;
constexpr float MAP_SCALE = static_cast<float>(MAP_UNIT_SZ) / UNIT_SZ;

using std::cos;
using std::cout;
using std::floor;
using std::max;
using std::min;
using std::pow;
using std::sin;
using std::sqrt;
using Map = std::array<std::array<u_int8_t, WIN_WIDTH / UNIT_SZ>,
    WIN_HEIGHT / UNIT_SZ>;

float raycast(const Vector2& player, float rotation, const Map& map)
{
    Vector2 d { cos((rotation - 90) * PI / 180),
        sin((rotation - 90) * PI / 180) };
    Vector2 p { player };

    while (p.x >= 0 && p.x < WIN_WIDTH && p.y >= 0 && p.y < WIN_HEIGHT
        && map[floor(p.y / UNIT_SZ)][floor(p.x / UNIT_SZ)] == 0) {
        p.x += d.x * 0.1;
        p.y += d.y * 0.1;
    }

    return sqrt(pow(p.x - player.x, 2) + pow(p.y - player.y, 2));
}

void draw_map_2d(const Map& map, float dist, Vector2& player, float rotation)
{
    Color black { BLACK };
    black.a = 0xB4;
    Color white { WHITE };
    white.a = 0xB4;

    for (unsigned i = 0; i < map.size(); ++i)
        for (unsigned j = 0; j < map[i].size(); ++j) {
            Color color { map[i][j] == 1 ? black : white };
            DrawRectangle(j * MAP_UNIT_SZ, i * MAP_UNIT_SZ, MAP_UNIT_SZ,
                MAP_UNIT_SZ, color);
            DrawRectangleLines(j * MAP_UNIT_SZ, i * MAP_UNIT_SZ, MAP_UNIT_SZ,
                MAP_UNIT_SZ, Color { GRAY });
        }
    DrawPoly(Vector2 { player.x * MAP_SCALE, player.y * MAP_SCALE }, 4, 2,
        rotation, DARKBLUE);
    DrawLine(player.x * MAP_SCALE, player.y * MAP_SCALE,
        (player.x + sin(rotation * PI / 180) * dist) * MAP_SCALE,
        (player.y + -cos(-rotation * PI / 180) * dist) * MAP_SCALE, RED);
}

void player_control(Vector2& player, Vector2& prev_player, float& rotation,
    int dist, int prev_dist, const Map& map)
{
    if (!prev_dist && !dist) {
        player = prev_player;
    }
    prev_player = player;

    if (IsKeyDown(KEY_W)) {
        player.y += -cos(-rotation * PI / 180) * 2;
        player.x += sin(rotation * PI / 180) * 2;
    }
    if (IsKeyDown(KEY_S)) {
        player.y -= -cos(-rotation * PI / 180) * 2;
        player.x -= sin(rotation * PI / 180) * 2;
    }
    if (IsKeyDown(KEY_D)) {
        rotation += 2;
    }
    if (IsKeyDown(KEY_A)) {
        rotation -= 2;
    }

    if (player.x < 0)
        player.x = 0;
    if (player.y < 0)
        player.y = 0;
    if (player.x > WIN_WIDTH)
        player.x = WIN_WIDTH;
    if (player.y > WIN_HEIGHT)
        player.y = WIN_HEIGHT;

    if (!raycast(player, rotation - 180, map)) {
        player = prev_player;
    }

    rotation = static_cast<int>(rotation) % 360;
}

void draw_walls(Vector2& player, float rotation, const Map& map)
{
    float init_angle { rotation - (FOV / 2) };
    int dist {};
    Color red { RED };
    Color floor_top { 0x4C, 0XA3, 0X4C, 0XA0 };
    Color floor_bot { 0x4C, 0XA3, 0X4C, 0XFF };

    for (float i = 0; i < FOV; i += 0.5) {
        dist = raycast(player, init_angle + i, map);
        float offset_rad = (i - FOV / 2) * PI / 180;
        cout << offset_rad << '\n';
        float w_wall = WIN_WIDTH / FOV;
        float h_wall
            = min(20 * WIN_HEIGHT / static_cast<float>(dist * cos(offset_rad)),
                static_cast<float>(WIN_HEIGHT));

        Rectangle wall { i * w_wall, (WIN_HEIGHT - h_wall) / 2, w_wall,
            h_wall };
        Rectangle floor { i * w_wall, (WIN_HEIGHT - h_wall) / 2 + h_wall,
            w_wall, (WIN_HEIGHT - h_wall) / 2 };
        Rectangle sky { i * w_wall, 0, w_wall, (WIN_HEIGHT - h_wall) / 2 };

        red.a = max(0xFF - dist, 0x90);
        DrawRectangleRec(sky, Color { 0x87, 0xCE, 0xFA, 0xFF });
        DrawRectangleRec(wall, red);
        DrawRectangleGradientEx(
            floor, floor_top, floor_bot, floor_bot, floor_top);
    }
    cout << '\n';
}

/* void draw_walls(Vector2& player, float rotation, const Map& map) */
/* { */
/*     /\* float init_angle = rotation - (FOV / 2); *\/ */
/*     float a { 90 - (2 * rotation) }; */
/*     Vector2 init_player { player.x + (FOV / 2) * sin(a * PI / 180), */
/*         player.y - (FOV / 2) * cos(a * PI / 180) }; */
/*     int dist {}; */
/*     Vector2 p {}; */
/*     for (int i = 0; i < FOV; ++i) { */
/*         p.x = init_player.x - i * sin(a * PI / 180); */
/*         p.y = init_player.y + i * cos(a * PI / 180); */
/*         dist = raycast(p, rotation, map); */
/*         float w = WIN_WIDTH / FOV; */
/*         float h = min(20 * WIN_HEIGHT / static_cast<float>(dist), */
/*             static_cast<float>(WIN_HEIGHT)); */
/*         Rectangle wall { i * w, (WIN_HEIGHT - h) / 2, w, h }; */
/*         DrawRectangleRec(wall, Color { RED }); */
/*     } */
/* } */

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
        { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    } };

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "Raycasting");
    Vector2 player { 60, 100 };
    Vector2 prev_player {};
    float rotation { 0 };
    float dist { 0 };
    float prev_dist { 1e30 };

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        dist = raycast(player, rotation, map);
        draw_walls(player, rotation, map);
        draw_map_2d(map, dist, player, rotation);
        player_control(player, prev_player, rotation, dist, prev_dist, map);
        prev_dist = dist;
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
