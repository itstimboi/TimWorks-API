#pragma once

namespace TAPI
{
    struct Vector2
    {
        float x = 0.0f;
        float y = 0.0f;

        Vector2() = default;

        Vector2(float x, float y)
            : x(x), y(y)
        {}
    };

    struct Vector2Double
    {
        double x = 0.0;
        double y = 0.0;

        Vector2Double() = default;

        Vector2Double(float x, float y)
            : x(x), y(y)
        {}
    };

    struct Vector2Int
    {
        int x = 0;
        int y = 0;

        Vector2Int() = default;

        Vector2Int(int x, int y)
            : x(x), y(y)
        {}
    };


    struct Vector3
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        Vector3() = default;

        Vector3(float x, float y, float z)
            : x(x), y(y), z(z)
        {}
    };


    struct Vector4
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;

        Vector4() = default;

        Vector4(float x, float y, float z, float w)
            : x(x), y(y), z(z), w(w)
        {}
    };
}