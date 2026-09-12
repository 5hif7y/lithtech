#pragma once
#include "Platform/platform.h"
#include <type_traits>
#include <iltphysics.h>
class HostPhysics : public ILTPhysics {
public:
LTRESULT IsWorldObject(HOBJECT hObj) override { return LT_OK; }
LTRESULT GetStairHeight(float &fHeight) override { return LT_OK; }
LTRESULT SetStairHeight(float fHeight) override { return LT_OK; }
LTRESULT GetMass(HOBJECT hObj, float *m) override { return LT_OK; }
LTRESULT SetMass(HOBJECT hObj, float m) override { return LT_OK; }
LTRESULT GetFrictionCoefficient(HOBJECT hObj, float* u) override { return LT_OK; }
LTRESULT SetFrictionCoefficient(HOBJECT hObj, float u) override { return LT_OK; }
LTRESULT GetObjectDims(HOBJECT hObj, LTVector *d) override { return LT_OK; }
LTRESULT SetObjectDims(HOBJECT hObj, LTVector *d, uint32 flag) override { return LT_OK; }
LTRESULT GetVelocity(HOBJECT hObj, LTVector *v) override { return LT_OK; }
LTRESULT SetVelocity(HOBJECT hObj, const LTVector *v) override { return LT_OK; }
LTRESULT GetForceIgnoreLimit(HOBJECT hObj, float &f) override { return LT_OK; }
LTRESULT SetForceIgnoreLimit(HOBJECT hObj, float f) override { return LT_OK; }
LTRESULT GetAcceleration(HOBJECT hObj, LTVector *a) override { return LT_OK; }
LTRESULT SetAcceleration(HOBJECT hObj, const LTVector *a) override { return LT_OK; }
LTRESULT MoveObject(HOBJECT hObj, const LTVector *p, uint32 flag) override { return LT_OK; }
LTRESULT GetStandingOn(HOBJECT hObj, CollisionInfo *info) override { return LT_OK; }
LTRESULT GetGlobalForce(LTVector &a) override { return LT_OK; }
LTRESULT SetGlobalForce(const LTVector &a) override { return LT_OK; }
const char* _InterfaceImplementation() override { return ""; }
};
