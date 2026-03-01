#pragma once
#include "pch.h"
#include <imgui.h>

// upgrades are a bitfield within CharObj2
// the demo in imgui works with just booleans, is it possible for me to cast that bit as a boolean??
// how do i write to that bit?

// MORE NOTES:
// starting at address 0x01DEB300, there are 29 booleans (a byte each) for a specific upgrade.
// this is different from the MainCharObj2's upgrade bitfield (4-byte int) 
// emerua's "realtime" toggle updates the charobj2's bitfield with the upgrade bytes, while also updating the upgrade booleans.
// informative stuff!
//
//if (checkBox2.CheckState == CheckState.Checked && rwRAM.getCurrentStageNumber() != 0)
//{
//	rwRAM.setUpgradeRealtime(num, b);
//}

// for locking upgrades: this is what emerua does, id probably just copy luna's upgrade restart code though
//if (checkBox1.CheckState == CheckState.Checked) - checkbox 1 = "Lock"
//{
//	if (rwRAM.getCurrentStageNumber() == 0)
//	{
//		for (int i = 0; i < UpgradesArray.Length; i++)
//		{
//			UpgradesArray[i] = Convert.ToByte(Nodes[i].Checked);
//		}
//		rwRAM.setUpgradeRange(UpgradesArray);

// upgrade booleans stored on file (there are 29 bytes to update)
// thank you Emerua for making UpgradeRemover & figuring this out
DataArray(bool, UpgradesOnFile, 0x1DEB300, 29);


class UpgradeRemover {
	public:
		UpgradeRemover() {};
		void UpdateRealTime(CharObj2Base* player);
		void RenderTab();
		void SonicTab();
		void TailsTab();
		void KnucklesTab();
		void ShadowTab();
		void EggmanTab();
		void RougeTab();
};