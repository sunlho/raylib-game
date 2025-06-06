#ifndef TILEMAP_COLLISION_H
#define TILEMAP_COLLISION_H

#include "box2d/box2d.h"
#include "raylib.h"
#include "tmx.h"

void TilemapPolygonCollision(tmx_object *obj, double pos_x, double pos_y);
void TilemapSquareCollision(tmx_object *obj, double pos_x, double pos_y);
void TilemapEllipseCollision(tmx_object *obj, double pos_x, double pos_y);

#endif // TILEMAP_COLLISION_H

#ifdef TILEMAP_COLLISION_IMPLEMENTATION
#ifndef TILEMAP_COLLISION_IMPLEMENTATION_ONCE
#define TILEMAP_COLLISION_IMPLEMENTATION_ONCE

void TilemapPolygonCollision(tmx_object *obj, double pos_x, double pos_y) {
    tmx_shape *shape = obj->content.shape;
    int len = shape->points_len;

    b2Vec2 *points = MemAlloc(sizeof(b2Vec2) * len);
    for (int i = 0; i < len; i++) {
        points[i] = (b2Vec2){shape->points[i][0] + pos_x, shape->points[i][1] + pos_y};
    }

    b2BodyDef body_def = b2DefaultBodyDef();
    b2BodyId body_id = b2CreateBody(tilemap_physics_world, &body_def);
    b2ChainDef chain_def = b2DefaultChainDef();
    chain_def.points = points;
    chain_def.count = obj->content.shape->points_len;
    chain_def.isLoop = true;
    b2ChainId chain_id = b2CreateChain(body_id, &chain_def);
}

void TilemapSquareCollision(tmx_object *obj, double pos_x, double pos_y) {
    Rectangle rect = {pos_x, pos_y, obj->width, obj->height};

    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.position = (b2Vec2){rect.x + rect.width / 2, rect.y + rect.height / 2};
    b2BodyId body_id = b2CreateBody(tilemap_physics_world, &body_def);
    b2ShapeDef shape_def = b2DefaultShapeDef();
    b2Polygon box = b2MakeBox(rect.width / 2, rect.height / 2);
    b2ShapeId shape_id = b2CreatePolygonShape(body_id, &shape_def, &box);
}

void TilemapEllipseCollision(tmx_object *obj, double pos_x, double pos_y) {
    Rectangle rect = {pos_x, pos_y, obj->width, obj->height};

    int segments = 32;
    b2Vec2 *points = MemAlloc(sizeof(b2Vec2) * segments);
    for (int i = 0; i < segments; i++) {
        float angle = (float)i / segments * 2.0f * PI;
        points[i] = (b2Vec2){
            rect.x + rect.width / 2 + cosf(angle) * (rect.width / 2),
            rect.y + rect.height / 2 + sinf(angle) * (rect.height / 2),
        };
    }

    b2BodyDef body_def = b2DefaultBodyDef();
    b2BodyId body_id = b2CreateBody(tilemap_physics_world, &body_def);
    b2ChainDef chain_def = b2DefaultChainDef();
    chain_def.points = points;
    chain_def.count = segments;
    chain_def.isLoop = true;
    b2ChainId chain_id = b2CreateChain(body_id, &chain_def);
}

#endif // TILEMAP_COLLISION_IMPLEMENTATION_ONCE
#endif // TILEMAP_COLLISION_IMPLEMENTATION
