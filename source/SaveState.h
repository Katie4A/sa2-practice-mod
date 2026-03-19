#pragma once
#include "pch.h"
#include <imgui.h>
#include <vector>

// !!! VARIOUS NOTES ABOUT ONVARS BELOW !!!

// For pointers, OnVar's has a type that accesses the address if its a pointer, then after reading that address it saves the relevant data.
// for savestate purposes, i dont really need to know about the data structures, i just need to save raw bytes

// CharacterPhys [changes per character]
	// speed = 0x3a0 bytes
	// hunting = 0x420 bytes (LOL!!!!!!!)
	// mech = 0x454 bytes
// LevelCollision [struct that holds info about collision with the level, size = u8;0x84]

//!! Character !!
// Character Task Struct [pointers to actionstruct, globalmetricsstruct, physicsstruct]
	// ActionStruct [0x30 bytes + pointer to collisiondata, read from address + 0x2c (actually i guess address + 0x34?)]
		// CollisionData [size = u8;0xa8, with pointer to collisionelement array, address + 0x38]
			// CollisionElement [size = u8;0x30, although the savestate reads from address + 0xc (maybe address + 0x40?)]	
	// PhysicsStruct [data = characterphys, plus pointer to level collision]	
	// GlobalMetricsStruct [0x40 bytes]
	
// Camera [array of 4 starting at 0x01dcff40, size = 0x24d8 + 0x40 set at 0x01dcff00]
	// note that onvar's only uses 1 of the elements of CameraData
// PastPositionTable [set of 0x100 3vecs, 0xc00 bytes total]

// !! UNITS !!
// CharacterUnit -> Character (address 0x01dea6e0) [JUST POINTERS TO DATA]
	// 
// CameraUnit
	// Camera (address = 0x01dcff00)
	// 2x Pointer to PastPositionTable for past positions & rotations (addresses 0x01a5a234 + 0x01a5a238)
	// past_positions_idx - byte (address 0x01945910)
	// camera_cons_past_positions: size of PastPositionTable (address 0x019f1740)
	// camera_cons_past_positions_idx: byte (address 0x019f173c)
// TimeUnit = 3 bytes (address 0x0174AFDB)
// ScoreUnit = 4 bytes (address 0x0174B050)
// RingUnit = 2 bytes (address 0x0174B028)
// LivesUnit = 2 bytes (address 0x0174B024)
// GravityUnit = 0xC bytes (address 0x01DE94A0)

// LevelCollisionUnit = crashes the game ([0x3000], [0x2]) (addresses 0x01a5a2dc and 0x01de9484)


// all names match the names in SA2Variables.h
struct Camera {
	int CameraTargetMode;
	BOOL boolCameraCollision;
	BOOL boolCameraRotate;
	NJS_POINT3 CameraPos;
	Angle3 CameraAng;
	NJS_POINT3 CameraPos;
	Angle3 CameraAng;
	NJS_POINT3 CameraDir;
	NJS_POINT3 CameraTgt;
	float CameraSpeed;
	CameraInfo CameraData1;
};


// pastpositiontable
// 0xc00 bytes
// a set of 0x100 3vecs (3 floats)
// onvar saves it blindly
struct CameraUnit {
	Camera camera;
	char pastPositions[0xc00];
	char pastRotations[0xc00];
	char pastPositionsIndex;
	char camera_cons_past_positions[0xc00];
	char camera_cons_past_positionsIndex;
};

// objectmaster maincharacter
	// -> data2ptr data2
		// -> charobj2base character
			// of which a character specific charobj2 is built on top of
// https://github.com/Sora-yx/SA2-Debug-Mode/blob/377292b0c6ec198ba7727f55f2c7112b89881222/SA2-Debug-Mode/save-state.cpp#L36


		
struct CharacterUnit {
	SonicCharObj2 sonicco2;
};


/// <summary>
/// SaveStateUnits used by OnVar's Tool are:
///		CharacterUnit
///		CameraUnit
///		TimeUnit
///		GravityUnit
///		ScoreUnit
///		RingsUnit
///		LivesUnit
/// The save is invalidated if the player exits the level.
/// </summary>
struct SaveUnit {
	bool SaveValid = false;
	short CurrentLevel;
	char charID;
	
	CharacterUnit character;
	CameraUnit camera;
	char TimerMinutes;
	char TimerSeconds;
	char TimerFrames;
	NJS_VECTOR gravity;
	int ScoreP1;
	short Life_Count;
	short RingCount;
};



class SaveStates {
public:
	SaveStates();
	void OnInput();
	void OnFrame();
	void SaveState(int saveSlot);
	void LoadState(int saveSlot);
private:
	std::vector<SaveUnit> saves;
	int saveSlot = 0;
	int prevButtons = 0;
};