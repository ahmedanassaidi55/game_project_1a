#include "minimap.h"

// Initialize minimap
void initMiniMap(MiniMap* map, Vector2 worldSize, Vector2 mapSize)
{
    map->worldSize = worldSize;
    map->mapSize = mapSize;
    map->worldOffset.x = 0;
    map->worldOffset.y = 0;
    map->clampInside = 1;
}

// Set offset
void setWorldOffset(MiniMap* map, Vector2 offset)
{
    map->worldOffset = offset;
}

// Clamp helper
float clamp(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Convert world position → minimap position
Vector2 worldToMiniMap(MiniMap* map, Vector3 worldPos)
{
    Vector2 result;

    // Normalize (0 → 1)
    float nx = (worldPos.x - map->worldOffset.x) / map->worldSize.x;
    float ny = (worldPos.z - map->worldOffset.y) / map->worldSize.y;

    // Convert to map coordinates (centered)
    result.x = (nx - 0.5f) * map->mapSize.x;
    result.y = (ny - 0.5f) * map->mapSize.y;

    // Clamp inside map
    if (map->clampInside)
    {
        float halfW = map->mapSize.x / 2;
        float halfH = map->mapSize.y / 2;

        result.x = clamp(result.x, -halfW, halfW);
        result.y = clamp(result.y, -halfH, halfH);
    }

    return result;
}
