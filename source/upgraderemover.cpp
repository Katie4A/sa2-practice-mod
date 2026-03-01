#include "upgraderemover.h"

// question to ask myself in the future:
// do players necessarily want real-time updating forced?
void UpgradeRemover::UpdateRealTime(CharObj2Base* player) {
	// pack upgrades on file into a bitfield
	// this *should* be a part of the main character object???
	//for (int i = 0; i < 29; i++) {
	//	player->Upgrades = &UpgradesOnFile[i];
	//}

}

void UpgradeRemover::RenderTab() {
	if (ImGui::CollapsingHeader("Upgrades")) {
		SonicTab();
		TailsTab();
		KnucklesTab();
		ShadowTab();
		EggmanTab();
		RougeTab();
	}
}


// whether or not this is a good way to organize this will remain a mystery
void UpgradeRemover::SonicTab() {
	if (ImGui::TreeNode("Sonic")) {
		if (ImGui::BeginTable("Sonic", 2)) {
			ImGui::TableNextColumn(); ImGui::Checkbox("Light Shoes", &UpgradesOnFile[0]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Ancient Light", &UpgradesOnFile[1]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Magic Gloves", &UpgradesOnFile[2]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Flame Ring", &UpgradesOnFile[3]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Bounce Bracelet", &UpgradesOnFile[4]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Mystic Melody", &UpgradesOnFile[5]);
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}

void UpgradeRemover::TailsTab() {
	if (ImGui::TreeNode("Tails")) {
		if (ImGui::BeginTable("Tails", 2)) {
			ImGui::TableNextColumn(); ImGui::Checkbox("Booster", &UpgradesOnFile[6]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Bazooka", &UpgradesOnFile[7]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Laser Blaster", &UpgradesOnFile[8]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Mystic Melody", &UpgradesOnFile[9]);
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}

void UpgradeRemover::KnucklesTab() {
	if (ImGui::TreeNode("Knuckles")) {
		if (ImGui::BeginTable("Knuckles", 2)) {
			ImGui::TableNextColumn(); ImGui::Checkbox("Shovel Claw", &UpgradesOnFile[10]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Sunglasses", &UpgradesOnFile[11]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Hammer Gloves", &UpgradesOnFile[12]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Air Necklace", &UpgradesOnFile[13]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Mystic Melody", &UpgradesOnFile[14]);
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}

void UpgradeRemover::ShadowTab() {
	if (ImGui::TreeNode("Shadow")) {
		if (ImGui::BeginTable("Shadow", 2)) {
			// shadow skips a byte??? thanks man
			ImGui::TableNextColumn(); ImGui::Checkbox("Air Shoes", &UpgradesOnFile[16]); 
			ImGui::TableNextColumn(); ImGui::Checkbox("Ancient Light", &UpgradesOnFile[17]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Flame Ring", &UpgradesOnFile[18]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Mystic Melody", &UpgradesOnFile[19]);
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}

void UpgradeRemover::EggmanTab() {
	if (ImGui::TreeNode("Eggman")) {
		if (ImGui::BeginTable("Eggman", 2)) {
			ImGui::TableNextColumn(); ImGui::Checkbox("Jet Engine", &UpgradesOnFile[20]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Large Cannon", &UpgradesOnFile[21]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Laser Blaster", &UpgradesOnFile[22]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Protective Armor", &UpgradesOnFile[23]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Mystic Melody", &UpgradesOnFile[24]);
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}

void UpgradeRemover::RougeTab() {
	if (ImGui::TreeNode("Rouge")) {
		if (ImGui::BeginTable("Rouge", 2)) {
			ImGui::TableNextColumn(); ImGui::Checkbox("Pick Nails", &UpgradesOnFile[25]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Treasure Scope", &UpgradesOnFile[26]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Iron Boots", &UpgradesOnFile[27]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Mystic Melody", &UpgradesOnFile[28]);
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}