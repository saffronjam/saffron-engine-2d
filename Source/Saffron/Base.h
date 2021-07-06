#pragma once


// namespace std
// {
// template <>
// struct hash<Se::Pair<int, int>>
// {
// 	auto operator()(const Se::Pair<int, int>& uuid) const -> std::size_t
// 	{
// 		return 0u;
// 	}
// };
// }

#include <memory>

#include "Saffron/Config.h"

#include "Saffron/Core/EventSubscriberList.h"
#include "Saffron/Core/Macros.h"
#include "Saffron/Core/Run.h"
#include "Saffron/Core/SingleTon.h"
#include "Saffron/Core/TypeDefs.h"
#include "Saffron/Core/UUID.h"

#include "Saffron/Debug/Debug.h"
#include "Saffron/Log/Log.h"
