#pragma once


class TemporaryStorage {
	static TemporaryStorage instance;
    bool initiated = false;
    bool facetypeInitiated = false;

public:
    unsigned int* cube;
    unsigned int* uvs;
    unsigned int* buffer;
    int**** faceTypes;
    unsigned char**** ao;
	TemporaryStorage() {
        cube = nullptr;
        uvs = nullptr;
        buffer = nullptr;
        faceTypes = nullptr;
        ao = nullptr;
    }

    void inititateCube(int size);

    void initiateFaceTypes();

	static TemporaryStorage* getInstance() { return &instance; }
};