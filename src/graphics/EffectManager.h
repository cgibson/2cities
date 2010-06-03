#ifndef _EFFECTMANAGER_H_
#define _EFFECTMANAGER_H_

#include <vector>

#include "../system/enum.h"
#include "shader.h"
#include "../helper/texloader.h"
#include "Effect.h"

class EffectManager {
public:
	EffectManager();
	~EffectManager();

	void init();
	void update(int ms);
	void draw();

	void addEffect(Effect *effect);

	DefaultShader *shader(EffectShader shader) { return _shaders[shader]; }
	GLuint texture(EffectTexture texture) { return _textures[texture]; }

private:
	std::vector<DefaultShader *> _shaders;
	std::vector<GLuint> _textures;
	std::vector<Effect *> _effects;
};

#endif
