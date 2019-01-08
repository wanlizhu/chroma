#ifndef CHROMA_SKYBOX_H
#define CHROMA_SKYBOX_H

#include "details/chroma_api.h"
#include "details/compiler.h"
#include <stdint.h>

namespace chroma {

	namespace details {
		class CHMSkybox;
	} 

	class Engine;
	class Texture;

	/**
	 * Creation and destruction
	 * ========================
	 *
	 * A Skybox object is created using the Skybox::Builder and destroyed by calling
	 * Engine::destroy(const Skybox*).
	 *
	 * ~~~~~~~~~~~{.cpp}
	 *  chroma::Engine* engine = chroma::Engine::create();
	 *
	 *  chroma::IndirectLight* skybox = chroma::Skybox::Builder()
	 *              .environment(cubemap)
	 *              .build(*engine);
	 *
	 *  engine->destroy(skybox);
	 * ~~~~~~~~~~~
	 */
	class PUBLIC_API Skybox : public ChromaAPI {
		struct BuilderDetails;
	public:
		//! Use Builder to construct an Skybox object instance
		class Builder : public BuilderBase<BuilderDetails> {
			friend struct BuilderDetails;
		public:
			Builder() noexcept;
			Builder(Builder const& rhs) noexcept;
			Builder(Builder&& rhs) noexcept;
			~Builder() noexcept;
			Builder& operator=(Builder const& rhs) noexcept;
			Builder& operator=(Builder&& rhs) noexcept;

			Builder& environment(Texture* cubemap) noexcept;
			Builder& showSun(bool show) noexcept;
			Skybox* build(Engine& engine);

		private:
			friend class details::CHMSkybox;
		};

		void setLayerMask(uint8_t select, uint8_t values) noexcept;
		uint8_t getLayerMask() const noexcept;
	};

} 

#endif 
