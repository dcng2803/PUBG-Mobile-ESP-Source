#pragma once

#include "Includes.h"

namespace Offset
{



	const DWORD GWorld = 0x7F2FD1C;
	const DWORD GName = 0x7C1ADB8;

	const DWORD PersistentLevel = 0x20;
	const DWORD NetDriver = 0x24;
	const DWORD ServerConnection = 0x60;
	const DWORD PlayerController = 0x20;
	const DWORD AcknowledgedPawn = 0x350;
	const DWORD EntityList = 0x70;
	const DWORD EntityCount = 0x74;

	const DWORD 	gameInstance = 0x24;
	const DWORD 	playerCarry = 0x20;


	const DWORD RootComponent = 0x164;
	const DWORD CurrentStates = 0xb08;

	// AActor
	const DWORD TeamId = 0x6a4;
	const DWORD PoseState = 0xfd8;
	const DWORD IsBot = 0x71c;
	const DWORD Name = 0x670;
	const DWORD Health = 0x9b8;
	const DWORD HealthMax = 0x9bc;

	const DWORD CharacterMovement = 0x344;
	const DWORD LastUpdateVelocity = 0x250;
	const DWORD Velocity = 0xc8;

	const DWORD RelativeLocation = 0x118;
	const DWORD RelativeRotation = 0x124;
	const DWORD Position = 0x150;

	// Bone
	const DWORD Mesh = 0x340;
	const DWORD BodyAddv = 0x140;
	const DWORD MinLOD = 0x5e8;			// int MinLOD;//[Offset: 0x5b0,  / boneOffset

	// Camera
	const DWORD PlayerCameraManager = 0x360;
	const DWORD CameraCache = 0x370;
	const DWORD POV = 0x10;
	const DWORD Location = 0x0;
	const DWORD Rotation = 0x18;
	const DWORD FOV = 0x24;

	// Instant hit
	const DWORD CurrentReloadWeapon = 0x1ad8;
	const DWORD ShootWeaponEntity = 0xc30;
	const DWORD BulletFireSpeed = 0x408;

	// NoRecoil
	const DWORD AccessoriesVRecoilFactor = 0x858;
	const DWORD AccessoriesHRecoilFactor = 0x85c;
	const DWORD AccessoriesRecoveryFactor = 0x860;
	const DWORD RecoilKickADS = 0x960;
	const DWORD AnimationKick = 0x978;

	////vehicle
	const  DWORD VehicleCommon = 0x73c;
	const  DWORD HP = 0x1e4;
	const  DWORD HPMax = 0x1e0;
	const  DWORD FuelMax = 0x1f8;
	const  DWORD Fuel = 0x1fc;
}
