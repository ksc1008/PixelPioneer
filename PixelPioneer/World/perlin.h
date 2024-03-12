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
    PerlinNoise();
    PerlinNoise(unsigned int seed);

    double noise(double x, double y, double z);
};