#include "hunting.h"
#include "utils.h"
#include <vector>
#include <random>

//most code taken from the lab mod's minimum set cover functions
static bool useSetCoverSets = 0;
static int lastLevel = -1;
static std::random_device rd;
static std::mt19937 gen;
FunctionHook<void, SearchEmeraldsGameManager*> hGenerateSet((intptr_t)0x7380A0);

std::vector<int> setIDs, setIDsCopy;
//thank you to Tenzit for figuring out most of these
std::vector<int> min_sets_wc = { 174,785,258,783,959,774,563,395,413,70,87,183,842,809,722,341,798,1003,827,473,992,334,255,972,485,399,477,970,703,372,813,691,659,414,442,262,111,365,979 };
std::vector<int> min_sets_ph = { 0,2,3,4,5,369,10,11,12,14,211,34,56,241,60,69,74,85,118,153,176,202,343,294,348,603,610,929,633,273,410,716,524,383,803,966,154,276,708 };
std::vector<int> min_sets_am = { 248,1,2,3,5,6,7,12,17,902,22,23,222,40,41,47,51,52,78,106,115,131,154,179,213,492,563,408,589,428,168,439,546,254,1000,740,189 };
std::vector<int> min_sets_dc = { 0,1,5,6,8,11,15,16,19,21,22,23,26,29,37,39,67,68,81,83,106,132,140,151,159,167,358,405,443,209,536,147,813,523,580,748,326,526,181,793,584,969,12,164 };
std::vector<int> min_sets_mh = { 0,1,2,3,4,5,7,10,17,21,219,29,39,42,65,67,77,91,146,156,181,276,393,450,917,550,175,780,484,281,953,95,989,457,629,481,571,882,766,412 };
std::vector<int> min_sets_dl = { 0,1,2,3,4,7,10,13,14,17,19,23,29,47,55,67,79,83,147,173,178,236,251,270,396,456,465,523,542,544,554,570,590,743,752,849,860,861,881,912,284,891 };
std::vector<int> min_sets_eq = { 0,1,2,3,5,6,7,15,17,23,26,30,32,38,52,59,72,79,99,137,206,234,301,307,395,412,846,401,405,922,240,663,204,804,787,565,432,765,424,398,678,571,732 };
std::vector	<int> min_sets_sh = { 0,1,2,4,5,10,15,16,18,20,22,24,25,28,32,35,41,43,50,53,64,87,95,126,200,225,321,555,1002,553,660,466,247,632,233,987,678,951,1004,757,157,819,557 };
std::vector<int> min_sets_ms = { 0,1,2,3,87,739,6,7,14,16,293,29,70,119,122,128,137,149,278,560,327,951,714,152,461,227,687,725,253,317,679,712,683,74,1019,112 };



void HuntingSettings::init() {
	hGenerateSet.Hook(generateSet_impl);
	//initialize rng
	gen = std::mt19937(rd());
}

void HuntingSettings::RenderTab() {
	if (ImGui::CollapsingHeader("Hunting Settings")) {
		ImGui::Checkbox("Cycle Through All Pieces", &useSetCoverSets);
		ImGui::SameLine();
		Utils::HelpMarker("If checked, will cycle randomly through a list of sets that give you every piece possible in a stage.");
	}
}

int chooseRandomSet() {
	int set_num;
	if (setIDs.size() == setIDsCopy.size()) {
		std::shuffle(setIDs.begin(), setIDs.end(), gen);
	}
	set_num = setIDs.front();
	setIDs.erase(std::remove(setIDs.begin(), setIDs.end(), set_num), setIDs.end());
	if (setIDs.size() == 0) {
		PrintDebug("List Exhausted, Reloading");
		setIDs = setIDsCopy;
	}
	return set_num;
}

void generateSet_impl(SearchEmeraldsGameManager* emeraldManager) {
	//M1 Only
	if (useSetCoverSets && MissionNum == 0) {
		if (lastLevel != CurrentLevel) {
			switch (CurrentLevel) {
			case LevelIDs_WildCanyon:
				setIDs = min_sets_wc;
				break;
			case LevelIDs_PumpkinHill:
				setIDs = min_sets_ph;
				break;
			case LevelIDs_AquaticMine:
				setIDs = min_sets_am;
				break;
			case LevelIDs_DeathChamber:
				setIDs = min_sets_dc;
				break;
			case LevelIDs_MeteorHerd:
				setIDs = min_sets_mh;
				break;
			case LevelIDs_DryLagoon:
				setIDs = min_sets_dl;
				break;
			case LevelIDs_EggQuarters:
				setIDs = min_sets_eq;
				break;
			case LevelIDs_SecurityHall:
				setIDs = min_sets_sh;
				break;
			case LevelIDs_MadSpace:
				setIDs = min_sets_ms;
				break;
			default:
				PrintDebug("NOT A HUNTING STAGE");
				break;
			}
			lastLevel = CurrentLevel;
			setIDsCopy = setIDs;
		}
		int nextSet = chooseRandomSet();
		//make FrameCount % 1024 = 0
		FrameCount = (int)(FrameCount / 1024) * 1024;
		FrameCount += nextSet;
		PrintDebug("Next set: %d", nextSet);
	}
	hGenerateSet.Original(emeraldManager);
}