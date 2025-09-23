#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

#include <atomic>
#include <mutex>

#include "Azer/Core/Log.h"

#include "Azer/DebugTools/Instrumentor.h"
#include "Azer/Core/UUID.hpp"

#include "Azer/FileSystem/FileFormat.h"

#include "Azer/FileSystem/Source/Image.h"
#include "Azer/Mesh/Mesh.h"
#include "Azer/Material/Material.h"

#ifdef AZ_PLATFORM_WINDOWS
	#include <Windows.h>
#endif // AZ_PLATFORM_WINDOWS


