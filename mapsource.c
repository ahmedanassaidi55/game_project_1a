#ifndef MINIMAP_H
#define MINIMAP_H

// 2D vector
typedef struct {
    float x;
    float y;
} Vector2;

// 3D vector
typedef struct {
    float x;
    float y;
    float z;
} Vector3;

// MiniMap system
typedef struct {
    Vector2 worldSize;   // World dimensions (X, Z)
    Vector2 worldOffset; // Offset if world not starting at (0,0)
    Vector2 mapSize;     // Mini-map size

    int clampInside;     // 1 = clamp, 0 = no
} MiniMap;

// Function prototypes
void initMiniMap(MiniMap* map, Vector2 worldSize, Vector2 mapSize);
void setWorldOffset(MiniMap* map, Vector2 offset);
Vector2 worldToMiniMap(MiniMap* map, Vector3 worldPos);
float clamp(float value, float min, float max);

#endif
