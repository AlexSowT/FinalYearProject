#pragma once
#include "Layer.h"
#include "SpeciesMember.h"
#include "NeatController.h"
#include "../Collidable.h"

//#include <tinyneat.hpp> -- Will take work to fix...
#include <vector>
#include <neatmain.h>

struct NEATLayer : public Layer
{
public:
	NEATLayer(std::shared_ptr<Camera> camera) { m_camera = camera; }

	void Init() override;
	void Update(float deltaTime, bool& running) override;
	void Render(ShaderLibrary& shaderLibrary, Renderer& renderer, SDL_Window* window) override;
private:
	SDL_Event m_windowEvent;
	NeatController m_neatController;
	
	std::shared_ptr<Camera> m_camera;

	std::shared_ptr<GameObject> m_grassPlane;
	//std::vector<std::shared_ptr<SpeciesMember>> m_SpeciesMembers;

	const float WIDTH = 800;
	const float HEIGHT = 800;
};