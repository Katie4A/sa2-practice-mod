#include "utils.h"

namespace Utils {
    void HelpMarker(const char* desc)
    {
        ImGui::TextDisabled("(?)");
        if (ImGui::BeginItemTooltip())
        {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(desc);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }
    double getIGT() {
        double igt = TimerMinutes * 60.0 + TimerSeconds;
        double centis = ceil(TimerFrames / 60.0 * 100);
        igt += centis / 100.0;
        return igt;
    }
}