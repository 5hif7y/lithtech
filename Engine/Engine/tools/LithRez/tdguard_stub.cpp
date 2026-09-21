// Stub open-source de TdGuard::Aegis.
//
// El .lib original (Engine/Engine/sdk/lib/TdGuard.lib, DRM de Touchdown)
// no esta en el repo, y todos los tools legacy lo linkean (ver .vcproj).
// Para el build CMake, Init()/DoWork() siempre OK: LithRez no necesita
// verificacion de licencia para empaquetar/desempaquetar .rez.
#include "tdguard.h"

namespace TdGuard
{

Aegis::Aegis() = default;
Aegis::~Aegis() = default;

Aegis& Aegis::GetSingleton()
{
	static Aegis sInstance;
	return sInstance;
}

bool Aegis::Init()
{
	return true;
}

bool Aegis::DoWork()
{
	return true;
}

} // namespace TdGuard
