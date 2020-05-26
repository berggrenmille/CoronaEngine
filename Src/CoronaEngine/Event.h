#pragma once
#include <memory>
#include <dexode/EventBus.hpp>

namespace Corona::Event
{

	static const std::shared_ptr<dexode::EventBus> MainBus = std::make_shared<dexode::EventBus>();
}
