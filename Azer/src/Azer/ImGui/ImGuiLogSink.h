#pragma once
#include <spdlog/sinks/base_sink.h>
#include <mutex>
#include <vector>
#include <string>

namespace Azer {

    struct LogItem {
        spdlog::level::level_enum level;
        std::string text;
    };

    class ImGuiLogSink : public spdlog::sinks::base_sink<std::mutex>
    {
    public:
        std::vector<LogItem> items;
        bool auto_scroll = true;

    protected:
        void sink_it_(const spdlog::details::log_msg& msg) override
        {
            spdlog::memory_buf_t formatted;
            base_sink<std::mutex>::formatter_->format(msg, formatted);
            items.push_back({ msg.level,fmt::to_string(formatted) });
        }

        void flush_() override {}
    };
    
}

