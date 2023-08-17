#pragma once
struct Vec3 {
    float x, y, z;
    Vec3 operator+(Vec3 d) {
        Vec3 t;
        t.x = x + d.x;
        t.y = y + d.y;
        t.z = z + d.z;
        return t;
    }
    Vec3 operator-(Vec3 d) {
        Vec3 t;
        t.x = x - d.x;
        t.y = y - d.y;
        t.z = z - d.z;
        return t;
    }
    Vec3 operator*(float d) {
        Vec3 t;
        t.x = x * d;
        t.y = y * d;
        t.z = z * d;
        return t;
    }
    void normal() {
        if (y < -180.0f) y += 360;
        if (y > 180.0f)  y -= 360;
        if (x > 89.0f)  x = 89;
        if (x < -89.0f) x = -89;

    }
};