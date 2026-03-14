/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "Event.h"

namespace UI {

std::unordered_map<std::type_index, std::vector<std::function<bool(EventVariant const&)>>> EventDispatcher::m_listeners;

}