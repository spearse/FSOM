#pragma once
#include "catch.hpp"


template<class FloatType>
inline bool EQ_TOL(FloatType a, FloatType b, FloatType tol = FloatType(0.0001))
{
	return fabs(a - b) < tol;
}