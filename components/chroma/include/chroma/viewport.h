#ifndef CHROMA_VIEWPORT_H
#define CHROMA_VIEWPORT_H

#include "details/compiler.h"
#include <math/scalar.h>
#include <math/vec2.h>
#include <stddef.h>
#include <stdint.h>

namespace chroma {

	// Viewport describes a view port in pixel coordinates
	class PUBLIC_API Viewport {
	public:
		int32_t left = 0;
		int32_t bottom = 0;
		uint32_t width = 0;
		uint32_t height = 0;

	public:
		Viewport() noexcept = default;
		Viewport(const Viewport& viewport) noexcept = default;
		Viewport(Viewport&& viewport) noexcept = default;
		Viewport& operator=(const Viewport& viewport) noexcept = default;
		Viewport& operator=(Viewport&& viewport) noexcept = default;

		// Create a Viewport from its left-bottom coordinates and size in pixels
		Viewport(int32_t left, int32_t bottom, uint32_t width, uint32_t height) noexcept
				: left(left), bottom(bottom), width(width), height(height) { }
		bool empty() const noexcept { return !width || !height; }
		Viewport scale(math::float2 s) const noexcept;

	private:
		friend bool operator==(Viewport const& rhs, Viewport const& lhs) noexcept {
			return (&rhs == &lhs) ||
				   (rhs.left == lhs.left && rhs.bottom == lhs.bottom &&
					rhs.width == lhs.width && rhs.height == lhs.height);
		}

		friend bool operator!=(Viewport const& rhs, Viewport const& lhs) noexcept {
			return !(rhs == lhs);
		}
	};

} 

#endif 
