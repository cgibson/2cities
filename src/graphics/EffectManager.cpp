#include "EffectManager.h"

EffectManager::EffectManager()
{
	// nothing yet
}

EffectManager::~EffectManager()
{
	_shaders.clear();
	_textures.clear();
	_effects.clear();
}

void EffectManager::init()
{
	DefaultShader *shader;
	GLuint texture;

	// LOAD SHADERS IN ORDER OF THE ENUM!
	shader = new DefaultShader();
	shader->load("resources/shaders/shockwave");
	_shaders.push_back(shader);
	shader = new DefaultShader();
	shader->load("resources/shaders/blackhole");
	_shaders.push_back(shader);


	// LOAD TEXTURES IN THE ORDER OF THE ENUM!
	texture = texldr::loadBMP("resources/textures/shockwave.bmp", NULL, NULL);
	_textures.push_back(texture);
}

void EffectManager::update(int ms)
{
	// loop through all effects and update them
	for (int i = 0; i < (int)_effects.size(); i++)
	{
		if (!_effects[i]->alive()) continue;
		_effects[i]->update(ms);
	}

	// remove one dead effect every update (multiple deaths will get handled
	// smoothly over the course of several frames)
	for (int i = 0; i < (int)_effects.size(); i++)
	{
		if (!_effects[i]->alive())
		{
			_effects.erase(_effects.begin() + i);
			break;
		}
	}
}

void EffectManager::draw()
{
	// draw all effects
	for (int i = 0; i < (int)_effects.size(); i++)
	{
		if (!_effects[i]->alive()) continue;
		_effects[i]->draw();
	}
}

void EffectManager::addEffect(Effect *effect)
{
	_effects.push_back(effect);
}

void EffectManager::reset()
{
	_effects.clear();
}
