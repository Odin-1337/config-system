#pragma once

namespace settings {
	namespace crosshair {
        bool draw_crosshair = false;
        bool crosshair_outline = true;
        float crosshair_size = 20.0f;
        float crosshair_outline_thickness = 2.5f;
        float crosshair_thickness = 1.5f;
        float crosshair_gap = 4.0f;

        bool draw_dot = true;
        bool dot_outline = true;
        float dot_radius = 1.5f;
        float dot_outline_thickness = 2.f;

        ImVec4 crosshair_color = ImVec4( 1.0f, 1.0f, 1.0f, 1.0f );
        ImVec4 dot_color = ImVec4( 1.0f, 1.0f, 1.0f, 1.0f );
	}
}