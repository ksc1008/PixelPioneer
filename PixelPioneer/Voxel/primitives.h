#pragma once


class PrimitiveCube {
	static PrimitiveCube instance;
    bool initiated = false;

public:
    int* cube;
    int* uvs;
    int* buffer;
	PrimitiveCube() {
        cube = nullptr;
        uvs = nullptr;
        buffer = nullptr;
    }

    void inititateCube(int size);

	static PrimitiveCube* getInstance() { return &instance; }
};