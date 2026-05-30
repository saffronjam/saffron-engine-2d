//
// Note:	this file is to be included in client applications ONLY
//			NEVER include this file anywhere in the engine codebase
//
#pragma once

#include "saffron/base.h"

#include "saffron/core/app.h"
#include "saffron/core/global.h"
#include "saffron/core/thread_pool.h"
#include "saffron/core/voronoi.h"

#include "saffron/graphics/controllable_render_texture.h"
#include "saffron/graphics/render_target_manager.h"
#include "saffron/graphics/scene.h"

#include "saffron/gui/dock_space.h"
#include "saffron/gui/terminal.h"
#include "saffron/gui/viewport_pane.h"

#include "saffron/input/keyboard.h"
#include "saffron/input/mouse.h"

#include "saffron/libraries/gen_utils.h"
#include "saffron/libraries/vec_utils.h"

#include "saffron/math/saffron_math.h"
#include "saffron/math/random.h"

#include "saffron/resource/font_store.h"
#include "saffron/resource/image_store.h"
#include "saffron/resource/music_store.h"
#include "saffron/resource/sound_buffer_store.h"
#include "saffron/resource/texture_store.h"

#ifdef SAFFRON_ENTRY_POINT
#include <saffron/entry_point.h>
#endif