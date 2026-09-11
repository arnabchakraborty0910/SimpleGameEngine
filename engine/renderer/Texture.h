#pragma once
class Texture
{
public:
	Texture(const char* path);
	void bind(unsigned int unit) const;
private:
	unsigned int ID;

};

