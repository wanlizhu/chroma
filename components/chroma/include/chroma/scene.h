#ifndef CHROMA_SCENE_H
#define CHROMA_SCENE_H

#include "details/compiler.h"
#include "details/chroma_api.h"
#include "details/entity.h"

namespace chroma {
    
	class IndirectLight;
	class Skybox;

	// A Scene is a flat container of Renderable and Light instances.
	// A Scene doesn't provide a hierarchy of Renderable objects.
	// A Renderable *must* be added to a Scene in order to be rendered, and the Scene must be provided to a View.
	/*
	* A Scene is created using Engine.createScene() and destroyed using
	* Engine.destroy(const Scene*).
	*
	* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	* #include <chroma/Scene.h>
	* #include <chroma/Engine.h>
	* using namespace chroma;
	*
	* Engine* engine = Engine::create();
	*
	* Scene* scene = engine->createScene();
	* engine->destroy(&scene);
	* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	*/
	class PUBLIC_API Scene : public ChromaAPI {
		// The Skybox is drawn last and covers all pixels not touched by geometry.
		void setSkybox(Skybox const* skybox) noexcept;
		// Set the IndirectLight to use when rendering the Scene.
		void setIndirectLight(IndirectLight const* ibl) noexcept;
		// The entity is ignored if it doesn't have a Renderable or Light component.
		void addEntity(Entity entity);
		// Removes the Renderable from the Scene.
		void remove(Entity entity);
		size_t getRenderableCount() const noexcept;
		size_t getLightCount() const noexcept;
	};
}	

#endif