#pragma once
#include <cmath>
#include <vector>
#include <algorithm>
#include <numeric>

class PerlinNoise {
private:
    std::vector<int> p; // Permutation vector

    // The fade function smooths the coordinate transitions
    double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }

    // Linear interpolation function
    double lerp(double t, double a, double b) { return a + t * (b - a); }

    // Gradient function contributes to the pseudo-randomness
    double grad(int hash, double x, double y, double z);

public:
    float*** GenerateMap3D(int width, int height, int depth, float scale,
        int octaves, float persistance, float lacunarity, float xOrg, float yOrg, float zOrg,
        int minHeight, float scaleHeight);
    float** GenerateMap(int width, int height, float scale, float octaves, float persistance, float lacunarity, float xOrg, float yOrg);
    PerlinNoise();
    PerlinNoise(unsigned int seed);

    double noise(double x, double y, double z);
};