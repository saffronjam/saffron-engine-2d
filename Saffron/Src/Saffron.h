//
// Note:	this file is to be included in client applications ONLY
//			NEVER include this file anywhere in the engine codebase
//
#pragma once

#include "Saffron/Base.h"

#include "Saffron/Core/Application.h"

#include "Saffron/Graphics/ControllableRenderTexture.h"
#include "Saffron/Graphics/RenderTargetManager.h"
#include "Saffron/Graphics/Scene.h"

#include "Saffron/Gui/DockSpace.h"
#include "Saffron/Gui/Terminal.h"
#include "Saffron/Gui/ViewportPane.h"

#include "Saffron/Input/Keyboard.h"
#include "Saffron/Input/Mouse.h"

#ifdef SAFFRON_ENTRY_POINT
#include <Saffron/EntryPoint.h>
#endif