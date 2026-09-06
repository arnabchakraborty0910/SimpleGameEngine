#pragma once


class Clock
{
public:
	void tick();
	float getDT() const;
	float getTime() const;
private:
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float currentFrame = 0.0f;

};

