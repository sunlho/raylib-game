#ifndef TILEMAP_CAMERA_H
#define TILEMAP_CAMERA_H

#include "raylib.h"

void TilemapCreateCamera();
void TilemapBeginWorldMode(void);
void TilemapEndWorldMode(void);
void TilemapDrawWorld(void);
void TilemapUpdateWorldCamera(Camera2D *camera, float x, float y);
Camera2D *TilemapGetScreenCamera(void);

#endif // TILEMAP_CAMERA_H

#ifdef TILEMAP_CAMERA_IMPLEMENTATION
#ifndef TILEMAP_CAMERA_IMPLEMENTATION_ONCE
#define TILEMAP_CAMERA_IMPLEMENTATION_ONCE

static Camera2D tilemap_screen_camera = {0};
static float tilemap_virtual_ratio = 1.0f;
static RenderTexture2D tilemap_world_render_texture = {0};

Camera2D *TilemapGetScreenCamera(void) {
    return &tilemap_screen_camera;
}

void TilemapBeginWorldMode(void) {
    BeginTextureMode(tilemap_world_render_texture);
    BeginMode2D(tilemap_screen_camera);
    ClearBackground(RAYWHITE);
}

void TilemapEndWorldMode(void) {
    EndMode2D();
    EndTextureMode();
}

void TilemapDrawWorld() {

    Rectangle source = {
        0, 0, (float)tilemap_world_render_texture.texture.width, (float)-tilemap_world_render_texture.texture.height};
    Rectangle dest = {
        -tilemap_virtual_ratio, -tilemap_virtual_ratio, GetScreenWidth() + (tilemap_virtual_ratio * 2), GetScreenHeight() + (tilemap_virtual_ratio * 2)};

    DrawTexturePro(
        tilemap_world_render_texture.texture,
        source,
        dest,
        (Vector2){0.0f, 0.0f},
        0.0f,
        WHITE);
}

void TilemapUpdateWorldCamera(Camera2D *camera, float x, float y) {

    // tilemap_screen_camera.target = (Vector2){x, y};

    // camera->target.x = (int)tilemap_screen_camera.target.x;
    // tilemap_screen_camera.target.x -= camera->target.x;
    // tilemap_screen_camera.target.x *= tilemap_virtual_ratio;

    // camera->target.y = (int)tilemap_screen_camera.target.y;
    // tilemap_screen_camera.target.y -= camera->target.y;
    // tilemap_screen_camera.target.y *= tilemap_virtual_ratio;

    camera->offset = (Vector2){x, y};

    tilemap_screen_camera.target.x = (int)x;
    tilemap_screen_camera.target.y = (int)y;

    tilemap_screen_camera.offset.x = (x - tilemap_screen_camera.target.x) * tilemap_virtual_ratio;
    tilemap_screen_camera.offset.y = (y - tilemap_screen_camera.target.y) * tilemap_virtual_ratio;
}

void TilemapCreateCamera() {
    tilemap_screen_camera.offset = (Vector2){0, 0};
    tilemap_screen_camera.target = (Vector2){0, 0};
    tilemap_screen_camera.rotation = 0.0f;
    tilemap_screen_camera.zoom = 1.0f;

    tilemap_world_render_texture = LoadRenderTexture((GetScreenWidth() / tilemap_virtual_ratio) + 2, (GetScreenHeight() / tilemap_virtual_ratio) + 2);
}

#endif // TILEMAP_CAMERA_IMPLEMENTATION_ONCE
#endif // TILEMAP_CAMERA_IMPLEMENTATION
