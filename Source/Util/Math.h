#pragma once

#define MATH_CREATE_TYPEDEFS
#include "Math\Math.h"

#ifdef MATH_CREATE_TYPEDEFS
typedef math::vec2   vec2;
typedef math::dvec2 dvec2;
typedef math::ivec2 ivec2;
typedef math::uvec2 uvec2;
typedef math::bvec2 bvec2;

typedef math::vec3   vec3;
typedef math::dvec3 dvec3;
typedef math::ivec3 ivec3;
typedef math::uvec3 uvec3;
typedef math::bvec3 bvec3;

typedef math::vec4   vec4;
typedef math::dvec4 dvec4;
typedef math::ivec4 ivec4;
typedef math::uvec4 uvec4;
typedef math::bvec4 bvec4;
#endif