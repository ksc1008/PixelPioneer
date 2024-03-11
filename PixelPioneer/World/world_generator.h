#pragma once

#include "../Voxel/chunk.h"
#include "perlin.h"

class WorldGenerator {
public:
    static int** createLandscape(const int n, const int max_height) {
        PerlinNoise perlin;
        int** output = new int* [n];
        for (int i = 0; i < n; i++) {
            output[i] = new int[n];
            for (int j = 0; j < n; j++) {
                // Scale and adjust to get interesting landscapes
                double x = (double)i / ((double)n) * 10;
                double y = (double)j / ((double)n) * 10;
                double z = 0.5; // Constant, as we're generating a 2D height map
                double noiseValue = perlin.noise(x, y, z);
                // Normalize to 0-1
                int height = floor(((noiseValue + 1.0) / 2.0) * max_height + 1);
                output[i][j] = height;
            }
        }
        return output;
    }
};
