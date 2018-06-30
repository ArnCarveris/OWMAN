#pragma once
class ResourceImage
{
public:
    bool load(const char* path);
    bool free();

	const unsigned char* getData()const;
	int getWidth()const;
	int getHeight()const;
private:
    unsigned char* data = nullptr;
    int width = 0;
    int height = 0;
};
