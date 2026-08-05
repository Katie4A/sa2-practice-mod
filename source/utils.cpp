#include "utils.h"

namespace Utils {

    bool inHunting() {
        CharObj2Base* player = MainCharObj2[0];
        if (player != nullptr) {
            int charID = player->CharID;
            if (charID == Characters_Knuckles || charID == Characters_Rouge || charID == Characters_Tikal || charID == Characters_Chaos) {
                return true;
            }
            return false;
        }
    }
    
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
}