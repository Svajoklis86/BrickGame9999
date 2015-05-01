#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <map>
#include <iostream>

using namespace std;

// a class to store and manage resource loading
// an easier way to pass down resources

struct coord
{
	int x;
	int y;
};

class ResourceStore
{
public:
	ResourceStore(SDL_Rect windowSize);
	~ResourceStore();

	void setWindow(SDL_Window *w);
	void setRenderer(SDL_Renderer *r);

	SDL_Rect windowSize;

	SDL_Window *getWindow();
	SDL_Renderer *getRenderer();
	SDL_Texture *img(string name);

	map<string, coord> location;
	map<string, SDL_Rect> item;

private:
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;

	map<string, SDL_Texture *> imageMap;

	SDL_Texture *loadTexture(string path);
};