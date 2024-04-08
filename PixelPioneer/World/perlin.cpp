#include "perlin.h"
#include "../debug.h"
#include <cmath>
#include <ctime>

PerlinNoise::PerlinNoise() {
    p.resize(256);
    std::iota(p.begin(), p.end(), 0);
    std::srand(std::time(0));
    std::random_shuffle(p.begin(), p.end());
    p.insert(p.end(), p.begin(), p.end());
}

PerlinNoise::PerlinNoise(unsigned int seed) {
    p.resize(256);
    std::iota(p.begin(), p.end(), 0);
    std::srand(seed);
    std::random_shuffle(p.begin(), p.end());
    p.insert(p.end(), p.begin(), p.end());
}

double PerlinNoise::noise(double x, double y, double z) {
    int X = (int)floor(x) & 255,
        Y = (int)floor(y) & 255,
        Z = (int)floor(z) & 255;
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);
    double u = fade(x), v = fade(y), w = fade(z);
    int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z,
        B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

    return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
        grad(p[BA], x - 1, y, z)),
        lerp(u, grad(p[AB], x, y - 1, z),
            grad(p[BB], x - 1, y - 1, z))),
        lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
            grad(p[BA + 1], x - 1, y, z - 1)),
            lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
                grad(p[BB + 1], x - 1, y - 1, z - 1))));
}


double PerlinNoise::grad(int hash, double x, double y, double z) {
    int h = hash & 15;
    double u = h < 8 ? x : y,
        v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float** PerlinNoise::GenerateMap(int width, int height, float scale, float octaves, float persistance, float lacunarity, float xOrg, float yOrg)
{
    float** noiseMap = new float*[height];
    for (int i = 0; i < height; i++)
        noiseMap[i] = new float[width];
    scale = std::max(0.0001f, scale);
    float maxNoiseHeight = FLT_MIN; //�ִ� ���� ��� ���� ����
    float minNoiseHeight = FLT_MAX; //�ּ� ���� ��� ���� ����
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            float amplitude = 1; //����. �������� ���� ���õ� ��.
            float frequency = 1; //���ļ�. �������� ���ݰ� ���õ� ��. ���ļ��� Ŀ������ ����� ��������
            float noiseHeight = 0;

            for (int i = 0; i < octaves; i++) //��Ÿ�갡 �����Ҽ��� ���� ���ļ��� ���� ������ ����� ��ø��.
            {
                float xCoord = xOrg + x / scale * frequency;
                float yCoord = yOrg + y / scale * frequency;
                float perlinValue = noise(xCoord,yCoord,0.5) * 2 - 1; //0~1 ������ ���� ��ȯ�ϴ� �Լ�. 2�� ���ϰ� 1�� ���� -1~1 ������ ������ ��ȯ
                noiseHeight += perlinValue * amplitude;
                amplitude *= persistance;
                frequency *= lacunarity;
            }
            if (noiseHeight > maxNoiseHeight) maxNoiseHeight = noiseHeight;
            else if (noiseHeight < minNoiseHeight) minNoiseHeight = noiseHeight;
            noiseMap[y][x] = noiseHeight;
        }
    }
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            noiseMap[y][x] = (noiseMap[y][x] - minNoiseHeight) / (maxNoiseHeight - minNoiseHeight); //lerp�� ���Լ��� �ּڰ��� �ִ��� ���հ��� 3��° ���ڷ� ������ 0~1������ ���� ��ȯ
        }
    }
    return noiseMap;
}
float*** PerlinNoise::GenerateMap3D(int width, int height, int depth, float scale, int octaves, 
    float persistance, float lacunarity, float xOrg, float yOrg, float zOrg,
    int minHeight, float scaleHeight)
{
    float*** noiseMap = new float** [height];
    for (int i = 0; i < height; i++) {
        noiseMap[i] = new float* [depth];
        for (int j = 0; j < depth; j++)
            noiseMap[i][j] = new float [width];
    }
    scale = std::max(0.0001f, scale);
    float maxNoiseHeight = FLT_MIN; //�ִ� ���� ��� ���� ����
    float minNoiseHeight = FLT_MAX; //�ּ� ���� ��� ���� ����
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int z = 0; z < depth; z++) {
                float amplitude = 1; //����. �������� ���� ���õ� ��.
                float frequency = 1; //���ļ�. �������� ���ݰ� ���õ� ��. ���ļ��� Ŀ������ ����� ��������
                float noiseHeight = 0;

                for (int i = 0; i < octaves; i++) //��Ÿ�갡 �����Ҽ��� ���� ���ļ��� ���� ������ ����� ��ø��.
                {
                    float xCoord = xOrg + ((float)x / width / scale) * frequency;
                    //float yCoord = yOrg + y / scale * frequency;
                    float yCoord = yOrg + ((float)y / height / scale/100) * frequency;
                    float zCoord = zOrg + ((float)z / depth / scale) * frequency;
                    float perlinValue = noise(xCoord, yCoord, zCoord) / 2; //0~1 ������ ���� ��ȯ�ϴ� �Լ�. 2�� ���ϰ� 1�� ���� -1~1 ������ ������ ��ȯ
                    noiseHeight += perlinValue * amplitude;
                    amplitude *= persistance;
                    frequency *= lacunarity;
                }
                if (noiseHeight > maxNoiseHeight) maxNoiseHeight = noiseHeight;
                else if (noiseHeight < minNoiseHeight) minNoiseHeight = noiseHeight;
                noiseMap[y][z][x] = noiseHeight;
            }
        }
    }

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            for (int z = 0; z < depth; z++) {
                noiseMap[y][z][x] = (noiseMap[y][z][x] - minNoiseHeight) / (maxNoiseHeight - minNoiseHeight) - 0.5; //lerp�� ���Լ��� �ּڰ��� �ִ��� ���հ��� 3��° ���ڷ� ������ 0~1������ ���� ��ȯ
                noiseMap[y][z][x] = noiseMap[y][z][x] * scaleHeight + minHeight - y;
                noiseMap[y][z][x] += scaleHeight * 0.5 + (height - 1) - minHeight;
                noiseMap[y][z][x] /= (scaleHeight + height);
            }
        }
    }

        
    
    return noiseMap;
}