#pragma once
#include "Layer.h"

struct NEATLayer : public Layer
{
public:
	NEATLayer(std::shared_ptr<Camera> camera) { m_camera = camera; }

	void Init() override;
	void Update(float deltaTime, bool& running) override;
	void Render(ShaderLibrary& shaderLibrary, Renderer& renderer, SDL_Window* window) override;
private:
	SDL_Event m_windowEvent;

	std::shared_ptr<Camera> m_camera;

	std::shared_ptr<GameObject> m_grassPlane;
	std::shared_ptr<GameObject> m_SpeciesMember;

	const float WIDTH = 800;
	const float HEIGHT = 800;
};