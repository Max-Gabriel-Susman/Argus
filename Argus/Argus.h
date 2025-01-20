#pragma once

#include <string>

// Simple function prototypes for GStreamer pipeline control
void init_gstreamer();
void start_rtmp_forwarding(const std::string& pipelineStr);
void stop_pipeline();
