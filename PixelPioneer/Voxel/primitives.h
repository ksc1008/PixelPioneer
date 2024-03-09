#pragma once


class PrimitiveCube {
	static PrimitiveCube instance;
    bool initiated = false;

public:
    short* cube;
    short* uvs;
    short* buffer;
	PrimitiveCube() {
        cube = nullptr;
        uvs = nullptr;
        buffer = nullptr;
    }

    void inititateCube(int size);

	static PrimitiveCube* getInstance() { return &instance; }
};