#include "Memory.h"
#include "Aimbot.h"
#include<thread>

BYTE MagicBulletShell[60] =
{
	0xC7, 0x45, 0x78, 0x4B, 0x48, 0x41, 0x4E,
	0x83, 0x3D, 0x00, 0x0A, 0x52, 0x55, 0x45,
	0x74, 0x0E,
	0x60,
	0x8B, 0x5D, 0x50,
	0x8B, 0x53, 0x18,
	0x8A, 0x02,
	0x3C, 0x20,
	0x74, 0x02,
	0x61,
	0xC3,
	0x8A, 0x42, 0x01,
	0x3C, 0x4C,
	0x75, 0x14,
 0xC7, 0x83, 0x20, 0x02, 0x00, 0x00, 0x50, 0x45, 0x4F, 0x00,
	 0xC7, 0x83, 0x24, 0x02, 0x00, 0x00, 0x50, 0x4C, 0x45, 0x00,
	 0xEB, 0xE1
};

// BYTE MagicBulletShell[60] = {
// 0xC7, 0x45, 0x78, 0x00, 0x00, 0x00, 0x00,
// 0x83, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x01,
// 0x74, 0x0E,
// 0x60, 0x8B,
// 0x5D, 0x00, 0x8B,
// 0x53, 0x18, 0x8A,0x02, 0x3C, 0x48, 0x74,
// 0x02, 0x61, 0xC3, 0x8A, 0x42, 0x01, 0x3C,
// 0x2B, 0x75, 0x14,
// 0xC7, 0x83, 0x30,
// 0x02, 0x00, 0x00, 0x00,
// 0x00, 0x00, 0x00, 0xC7,
// 0x83, 0x34, 0x02,0x00, 0x00, 0x00,
// 0x00, 0x00, 0x00, 0xEB, 0xE1
// };

// 0x66 , 0x0F , 0x7E, 0xBE , 0x38, 0x02 , 0x00 , 0x00 , 0xC7 , 0x45 , 0x68 , 0x48 
//66 0F 7E ? ? 38 02 00 00 C7 45 78 48
BYTE MagicBulletSearch1[] = { 0x66, 0x0F, 0x7E, '?', 0x28, 0x02, 0x00, 0x00, 0xC7, 0x45, 0x78, 0x70 }; // Need update
BYTE MagicBulletSearch2[] = { 0x66, 0x0F, 0x7E, '?', 0x28, 0x02, 0x00, 0x00, 0xC7, 0x45, 0x78, 0x70 }; // Need update

DWORD MagicBulletAddress = 0;
vector<DWORD_PTR> MagicBulletList = {};
INT MagicBulletHook = 0;
BYTE RealByteCode[7];
DWORD FlyCarAddress = 0;
VOID GetMagicAddress()
{
	std::vector<DWORD_PTR> FoundedBase;

	if (Emulator::IsGameLoop4 == true)
	{
		Utility::MemSearch(MagicBulletSearch2, sizeof(MagicBulletSearch2), 0x18000000, 0x20000000, 0, 0, FoundedBase);
	}
	else if (Emulator::IsGameLoop7 == true)
	{
		Utility::MemSearch(MagicBulletSearch1, sizeof(MagicBulletSearch1), 0x0D000000, 0x10000000, 0, 0, FoundedBase);
	}
	else if (Emulator::IsSmartGaGa4 == true)
	{
		Utility::MemSearch(MagicBulletSearch2, sizeof(MagicBulletSearch2), 0x18000000, 0x20000000, 0, 0, FoundedBase);
	}
	else if (Emulator::IsSmartGaGa7 == true)
	{
		Utility::MemSearch(MagicBulletSearch1, sizeof(MagicBulletSearch1), 0x0D000000, 0x10000000, 0, 0, FoundedBase);
	}

	for (int i = 0; i < FoundedBase.size(); i++)
	{
		if (Utility::ReadMemoryEx<BYTE>(FoundedBase[i] + 0xC) == 0xA4) // Need update 
		{
			MagicBulletList.push_back(FoundedBase[i]);
			MagicBulletAddress = FoundedBase[i];
		}
	}

	FoundedBase.clear();
}

VOID InitializeMagic()
{
	INT Addv = MagicBulletAddress;

	MagicBulletHook = (INT)VirtualAllocEx(Game::hProcess, 0, 500, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	WriteProcessMemory(Game::hProcess, (LPVOID)MagicBulletHook, &MagicBulletShell, sizeof(MagicBulletShell), 0);

	if (Emulator::IsGameLoop4 == true)
	{
		Utility::WriteMemoryEx<BYTE>(MagicBulletHook + 0x13, 0x40);
	}
	else if (Emulator::IsGameLoop7 == true)
	{
		Utility::WriteMemoryEx<BYTE>(MagicBulletHook + 0x13, 0x50);
	}
	if (Emulator::IsSmartGaGa4 == true)
	{
		Utility::WriteMemoryEx<BYTE>(MagicBulletHook + 0x13, 0x40);
	}
	else if (Emulator::IsSmartGaGa7 == true)
	{
		Utility::WriteMemoryEx<BYTE>(MagicBulletHook + 0x13, 0x50);
	}

	Utility::WriteMemoryEx<INT>(MagicBulletHook + 9, MagicBulletHook + 0x5E);
	ReadProcessMemory(Game::hProcess, (LPCVOID)(Addv + 0x8), &RealByteCode, sizeof(RealByteCode), 0);
	WriteProcessMemory(Game::hProcess, (LPVOID)(MagicBulletHook), &RealByteCode, sizeof(RealByteCode), 0);
}

VOID HookCall(INT Address, INT HookAdress)
{
	BYTE Shell[7] = { 0xE8, 0x00, 0x00, 0x00, 0x00, 0x90, 0x90 };
	*(INT*)(Shell + 1) = (INT)(HookAdress - Address - 5);
	WriteProcessMemory(Game::hProcess, (LPVOID)Address, &Shell, sizeof(Shell), 0);
}

bool IsEqual(BYTE Array1[], BYTE Array2[])
{
	for (int i = 0; i < 6; i++)
	{
		if (Array1[i] != Array2[i])
		{
			return false;
		}
	}

	return true;
}

bool Hooked(int Address)
{
	unsigned char Tmp[7];
	ReadProcessMemory(Game::hProcess, (LPCVOID)(Address + 0x8), &Tmp, sizeof(Tmp), 0);

	if ((Tmp[0] == 0xE8 && Tmp[5] == 0x90 && Tmp[6] == 0x90))
	{
		return true;
	}

	return false;
}

VOID HookMagic()
{
	for (DWORD_PTR& Addv : MagicBulletList)
	{
		BYTE Tmp[7];
		ReadProcessMemory(Game::hProcess, (LPCVOID)((int)Addv + 0x8), &Tmp, sizeof(Tmp), 0);

		if (IsEqual(Tmp, RealByteCode))
		{
			HookCall((INT)(Addv + 0x8), (INT)MagicBulletHook);
		}
	}
}

VOID Memory::RestoreHook()
{
	for (DWORD_PTR& Addv : MagicBulletList)
	{
		if (Hooked(Addv))
		{
			WriteProcessMemory(Game::hProcess, (LPVOID)(Addv + 0x8), &RealByteCode, sizeof(RealByteCode), 0);
		}
	}
}

VECTOR3 Subtract(VECTOR3 Src, VECTOR3 Dst)
{
	VECTOR3 Result;
	Result.X = Src.X - Dst.X;
	Result.Y = Src.Y - Dst.Y;
	Result.Z = Src.Z - Dst.Z;

	return Result;
}

float Magnitude(VECTOR3 Vec)
{
	return sqrtf(Vec.X * Vec.X + Vec.Y * Vec.Y + Vec.Z * Vec.Z);
}

float GetDistance(VECTOR3 Src, VECTOR3 Dst)
{
	VECTOR3 Result = Subtract(Src, Dst);
	return Magnitude(Result);
}

VECTOR3 NormalizeVec(VECTOR3 Vector)
{
	float Length = Magnitude(Vector);

	Vector.X /= Length;
	Vector.Y /= Length;
	Vector.Z /= Length;

	return Vector;
}
float Distance(VECTOR3 myPos, VECTOR3 enPos)
{
	return sqrt((myPos.X - enPos.X) * (myPos.X - enPos.X) + (myPos.Y - enPos.Y) * (myPos.Y - enPos.Y) + (myPos.Z - enPos.Z) * (myPos.Z - enPos.Z));
}
float BulletDrop(float TravelTime)
{
	return (TravelTime * TravelTime * 980 / 2);
}
bool Fov(float OverlayScreenWidth, float OverlayScreenHeight, VECTOR2 PlayerW2SBone, float FovRadius)
{
	VECTOR2 Cenpoint;
	Cenpoint.X = PlayerW2SBone.X - (OverlayScreenWidth / 2);
	Cenpoint.Y = PlayerW2SBone.Y - (OverlayScreenHeight / 2);

	if (Cenpoint.X * Cenpoint.X + Cenpoint.Y * Cenpoint.Y <= FovRadius * FovRadius)
	{
		return true;
	}


	return false;
}


VECTOR2 GetMagicCoordinates()
{
	VECTOR2 Coordinate;
	Coordinate.X = 0;
	Coordinate.Y = 0;
	VECTOR3 HeadPos;
	HeadPos.X = 0;
	HeadPos.Y = 0;
	HeadPos.Z = 0;
	VECTOR2 Screen;
	Screen.X = 0;
	Screen.Y = 0;
	VECTOR3 Velocity;
	Velocity.X = 0;
	Velocity.Y = 0;
	Velocity.Z = 0;
	FLOAT TargetDistance = 9999.0f;
	FLOAT CrossDist = 0;
	INT Distance4 = 0;
	VECTOR3 posi;
	

	for (AActor& AActor : Data::AActorList)
	{
		
			if (Algorithm::WorldToScreenBone(Esp::ViewMatrix, AActor.ChestPos, Screen, &Distance4))
				{
					
					CrossDist = sqrt(pow(Screen.X - DX11Window::Width / 2, 2) + pow(Screen.Y - DX11Window::Height / 2, 2));

					if (CrossDist < TargetDistance)
					{
						if (Setting::curraim == 0)
						{
							HeadPos = AActor.HeadPos;
							HeadPos.Z += Setting::magicv;
						}
						if (Setting::curraim == 1)
						{
							HeadPos = AActor.ChestPos;
						}
						if (Setting::curraim == 2)
						{
							HeadPos = AActor.PelvisPos;
						}

						TargetDistance = CrossDist;
						Velocity = AActor.Velocity;
					}

			  }
	
	}
	DWORD MeshAddv = Utility::ReadMemoryEx<DWORD>(Data::LocalPlayer + Offset::Mesh);
	DWORD BodyAddv = MeshAddv + Offset::BodyAddv;
	DWORD BoneAddv = Utility::ReadMemoryEx<DWORD>(MeshAddv + Offset::MinLOD) + 48;
	VECTOR3 MyPosition = Algorithm::GetBoneWorldPosition(BodyAddv, BoneAddv + 5 * 48);

	float AddTime = 30 / 10.0f + 1.0f;
	float Gravity = 5.72f;
	float distance = GetDistance(MyPosition, HeadPos) / 100.0f;
	float zAssist;
	if (distance < 5000.f)
		zAssist = 1.8f;
	else if (distance < 10000.f)
		zAssist = 1.72f;
	else if (distance < 15000.f)
		zAssist = 1.23f;
	else if (distance < 20000.f)
		zAssist = 1.24f;
	else if (distance < 25000.f)
		zAssist = 1.25f;
	else if (distance < 30000.f)
		zAssist = 1.26f;
	else if (distance < 35000.f)
		zAssist = 1.27f;
	else if (distance < 40000.f)
		zAssist = 1.28f;
	else if (distance < 45000.f)
		zAssist = 1.29f;
	else if (distance < 50000.f)
		zAssist = 1.30f;

	DWORD CurrentReloadWeapon = Utility::ReadMemoryEx<DWORD>(Data::LocalPlayer + Offset::CurrentReloadWeapon);
	DWORD ShootWeaponEntity = Utility::ReadMemoryEx<DWORD>(CurrentReloadWeapon + Offset::ShootWeaponEntity);
	float BulletSpeed = Utility::ReadMemoryEx<FLOAT>(ShootWeaponEntity + Offset::BulletFireSpeed);
	float BulletTravelTime = distance / Offset::BulletFireSpeed;
	DWORD SceneComponent = Utility::ReadMemoryEx<INT>(Offset::RootComponent + 0x14C);
	DWORD VehicleCommon = Utility::ReadMemoryEx<INT>(Offset::VehicleCommon + 0x6a0);


	HeadPos.X += Velocity.X * BulletTravelTime;
	HeadPos.Y += Velocity.Y * BulletTravelTime;
	HeadPos.Z += Velocity.Z * BulletTravelTime * zAssist + 50.5f * 5.72f * BulletTravelTime * BulletTravelTime;


	FLOAT xDif = HeadPos.X - MyPosition.X;
	FLOAT yDif = HeadPos.Y - MyPosition.Y;
	FLOAT zDif = HeadPos.Z - MyPosition.Z;

	float Hyp = sqrt(xDif * xDif + yDif * yDif);
	Coordinate.X = atan2(zDif, Hyp) * 180.0f / 3.1415926535897f;
	Coordinate.Y = atan2(yDif, xDif) * 180.0f / 3.1415926535897f;

	return Coordinate;



}
int Memory::AimFindBestTarget() {
	float targetDistance = 9999.0;
	VECTOR2 retval;
	int dist, headdist;
	int ret;
	for (int i = 0; i < Data::AActorList.size(); i++)
	{
		DWORD tmpAddv = Utility::ReadMemoryEx<INT>(Data::AActorList[i].Address  + Offset::Mesh);
		DWORD bodyAddv = tmpAddv + Offset::BodyAddv;
		DWORD boneAddv = Utility::ReadMemoryEx<INT>(tmpAddv + Offset::MinLOD) + 48;

		VECTOR2 chest;
		VECTOR2 head;
		VECTOR3 chestPos = Algorithm::GetBoneWorldPosition(bodyAddv, boneAddv + 4 * 48);
		

		if (Algorithm::WorldToScreenBone(Esp::ViewMatrix, chestPos, chest, &dist))
		{
			if (Setting::fovcircle)
			{
				if (Aimbot::InsideFov(DX11Window::Width, DX11Window::Height, chest, Setting::fovcircleredus/*FovRadius*/))
				{
					float cross_dist = sqrt(pow(chest.X - DX11Window::Width / 2, 2) + pow(chest.Y - DX11Window::Height / 2, 2));
					if (cross_dist < targetDistance)
					{
						ret = i;
						targetDistance = cross_dist;

					}
				}
			}
			else
			{
				
					float cross_dist = sqrt(pow(chest.X - DX11Window::Width / 2, 2) + pow(chest.Y - DX11Window::Height / 2, 2));

					if (cross_dist < targetDistance)
					{
						ret = i;
						targetDistance = cross_dist;

					}
				
			}
		}
	}
	return ret;

}

bool  Memory::IsMagicInitialized = false;
VOID Memory::MemoryThread()
{
	
	VECTOR2 MagicCoordinate;
	VECTOR3 AimRotation;

	while (true)
	{
		Sleep(10);
		if (Data::NetDriver > 0)
		{
			if (Setting::MagicBullet || Setting::engine == 1)
			{
				if (IsMagicInitialized == false)
				{
					GetMagicAddress();
					InitializeMagic();
					HookMagic();
					IsMagicInitialized = true;
				}

				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
				{
					MagicCoordinate = GetMagicCoordinates();

					Utility::WriteMemoryEx<FLOAT>(MagicBulletHook + 44, MagicCoordinate.X);
					Utility::WriteMemoryEx<FLOAT>(MagicBulletHook + 54, MagicCoordinate.Y);
				}
				if (GetAsyncKeyState(0x79) & 1)
				{
					if (IsMagicInitialized == true)
					{
						if (Data::NetDriver > 0)
						{
							IsMagicInitialized = false;
							GetMagicAddress();							
							HookMagic();
							IsMagicInitialized = true;
						}
					}
				}
				
			}
			else if (Setting::MagicBullet == false && IsMagicInitialized == true)
			{
				RestoreHook();
				MagicBulletList.clear();
				VirtualFreeEx(Game::hProcess, (LPVOID)MagicBulletHook, 0, MEM_RELEASE);
				MagicBulletAddress = 0;
				MagicBulletHook = 0;
				ZeroMemory(RealByteCode, sizeof(RealByteCode));
				IsMagicInitialized = false;
			}
			
		}
	}
}

void Memory::magnatbullot()
{

	/*if (Setting::CameraCatch)
	{
		DWORD LocalController = Utility::ReadMemoryEx<DWORD>(Data::LocalPlayer + 9692);
		if (LocalController)
		{

			DWORD PlayerCameraManager = Utility::ReadMemoryEx<DWORD>(LocalController + 0x340);
			if (PlayerCameraManager)
			{
				CameraCacheEntry CameraCache = Utility::ReadMemoryEx<CameraCacheEntry>(PlayerCameraManager + 0x350);

				VECTOR3 HeadPosition = ClosestToCrosshair();
				VECTOR3 currentViewAngle = CameraCache.POV.Location;

				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
				{
					FRotator aimRotation = ToRotator(currentViewAngle, HeadPosition);
					CameraCache.POV.Rotation = aimRotation;
					Utility::WriteMemoryEx<CameraCacheEntry>(PlayerCameraManager + 0x350, CameraCache);

				}
			}
		}

	}*/


}
void Memory::Instant() {
	while (true)
	{
		Sleep(200);
		if (Setting::instanthit)
		{
			DWORD gameInstance = Utility::ReadMemoryEx<INT>(Data::UWorld + 0x24);
			DWORD playerController = Utility::ReadMemoryEx<INT>(gameInstance + 0x60);
			DWORD playerCarry = Utility::ReadMemoryEx<INT>(playerController + 0x20);
			DWORD uMyObject = Utility::ReadMemoryEx<INT>(playerCarry + 0x330);
			int weaponOffSet = Utility::ReadMemoryEx<INT>(uMyObject + 5952);
			if (weaponOffSet != 0)
			{
				int shootOffset = Utility::ReadMemoryEx<INT>(weaponOffSet + 3292);

				if (shootOffset != 0)
				{
					float InstantHit = Utility::ReadMemoryEx<FLOAT>(shootOffset + 980);
					if (InstantHit != Setting::instantv)
					{
						Utility::WriteMemoryEx<float>(shootOffset + 980, Setting::instantv);
					}
				}
			}
		}
	}
}

void Memory::FastCar() {
	while (true)
	{
		Sleep(400);
		if (Setting::speedcar)
		{
			if (GetAsyncKeyState(VK_LSHIFT)) {
				DWORD gameInstance = Utility::ReadMemoryEx<INT>(Data::UWorld + 0x24);
				DWORD playerController = Utility::ReadMemoryEx<INT>(gameInstance + 0x60);
				DWORD playerCarry = Utility::ReadMemoryEx<INT>(playerController + 0x20);
				DWORD uMyObject = Utility::ReadMemoryEx<INT>(playerCarry + 0x330);
				DWORD num12 = Utility::ReadMemoryEx<INT>(Utility::ReadMemoryEx<INT>(Utility::ReadMemoryEx<INT>(Utility::ReadMemoryEx<INT>(Utility::ReadMemoryEx<INT>(uMyObject + 332L) + 0x6D0) + 1776) + 416) + 88);
				Utility::WriteMemoryEx<float>(num12, Setting::carsv);
				Utility::WriteMemoryEx<float>(num12 + 4, Setting::carsv);
				Utility::WriteMemoryEx<float>(num12 + 8, Setting::carsv);
				Utility::WriteMemoryEx<float>(num12 + 12, Setting::carsv);
				Utility::WriteMemoryEx<float>(num12 + 16, Setting::carsv);
				Utility::WriteMemoryEx<float>(num12 + 20, Setting::carsv);
				Utility::WriteMemoryEx<float>(num12 + 24, Setting::carsv);
				Utility::WriteMemoryEx<float>(num12 + 28, Setting::carsv);

			}
		}
	}
}

void Memory::FastRunn() {
	while (true)
	{
		Sleep(200);
		if (Setting::fastrun)
		{
			if (GetAsyncKeyState(Setting::fastrun) & 1) {
				float write = Setting::fastrv;

				DWORD gameInstance = Utility::ReadMemoryEx<INT>(Data::UWorld + 0x24);
				DWORD playerController = Utility::ReadMemoryEx<INT>(gameInstance + 0x60);
				DWORD playerCarry = Utility::ReadMemoryEx<INT>(playerController + 0x20);
				DWORD uMyObject = Utility::ReadMemoryEx<INT>(playerCarry + 0x330);
				Utility::WriteMemoryEx<float>(uMyObject + 0x1e24, write);
			}
			else if (GetAsyncKeyState(Setting::fastrun) == 0)
			{
				float writee = 1.0f;
				DWORD gameInstance = Utility::ReadMemoryEx<INT>(Data::UWorld + 0x24);
				DWORD playerController = Utility::ReadMemoryEx<INT>(gameInstance + 0x60);
				DWORD playerCarry = Utility::ReadMemoryEx<INT>(playerController + 0x20);
				DWORD uMyObject = Utility::ReadMemoryEx<INT>(playerCarry + 0x330);
				Utility::WriteMemoryEx<float>(uMyObject + 0x1e24, writee);
			}
		}

	}
}
 
void Memory::norecoil() {
	float writee = 0.0f;
	while (true)
	{
		Sleep(300);
		DWORD gameInstance = Utility::ReadMemoryEx<INT>(Data::UWorld + 0x24);
		DWORD playerController = Utility::ReadMemoryEx<INT>(gameInstance + 0x60);
		DWORD playerCarry = Utility::ReadMemoryEx<INT>(playerController + 0x20);
		DWORD uMyObject = Utility::ReadMemoryEx<INT>(playerCarry + 0x330);
		if (Setting::recoil)
		{
			DWORD weaponOffSet = Utility::ReadMemoryEx<INT>(uMyObject + 5952);
			if (weaponOffSet != 0)
			{
				DWORD shootOffset = Utility::ReadMemoryEx<INT>(weaponOffSet + 3292);

				if (shootOffset != 0)
				{
					Utility::WriteMemoryEx<float>(shootOffset + 1956, writee);
					Utility::WriteMemoryEx<float>(shootOffset + 1960, writee);
					Utility::WriteMemoryEx<float>(shootOffset + 1964, writee);

				}
			}
		}
	}
}



bool IsFlyCarInitialized = false;
BYTE FlyCarSearch[] = { 0x00, 0x00, 0x75, 0xC4, 0x00, 0x00, 0x7A, 0x45, 0x9A, 0x99 };

VOID SetFlyCarAddress()
{
	std::vector<DWORD_PTR> FoundedBase;
	Utility::FindPattern(FlyCarSearch, sizeof(FlyCarSearch), 0x10000000, 0xA0000000, 0, 0, FoundedBase);
	FlyCarAddress = FoundedBase[0];
}


void Memory::flyCar()
{
	while (true)
	{
		Sleep(200);
		if (Setting::carfly)
		{
			if (IsFlyCarInitialized == false)
			{
				SetFlyCarAddress();

				IsFlyCarInitialized = true;
			}

			if (GetAsyncKeyState(VK_SPACE) & 1)
			{
				Utility::WriteMemoryEx<FLOAT>(FlyCarAddress, Setting::flycarv);
			}
			else if (GetAsyncKeyState(VK_SPACE) == 0)
			{
				if (Utility::ReadMemoryEx<float>(FlyCarAddress) != Setting::flycarv)
				{
					Utility::WriteMemoryEx<FLOAT>(FlyCarAddress, Setting::flycarv);
				}
			}
		}
	}
}

void Memory::MagicX()
{
	bool IsChanged = false;
	while (true)
	{
		Sleep(10);
		if (Setting::MagicX && IsChanged == false)
		{
			BYTE pattern[] = { 0x00, 0x00, 0x20, 0x41, 0x00, 0x00, 0x38, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f };
			BYTE patter2[] = { 0x00, 0x08, 0xF3, 0x45, 0x00, 0x00, 0x38, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f };
			DWORD bytes;
			std::vector<DWORD_PTR> foundedBases2;
			DWORD oldprotect, temps;
			foundedBases2.clear();
			Utility::MemSearch(pattern, sizeof(pattern), 0x10000000, 0x90000000, false, 0, foundedBases2);
			for (int i = 0; i < foundedBases2.size(); i++)
			{
				VirtualProtectEx(Game::hProcess, LPVOID(foundedBases2[i]), 4, PAGE_EXECUTE_READWRITE, &oldprotect);
				WriteProcessMemory(Game::hProcess, (PVOID)foundedBases2[i], patter2, sizeof(patter2), nullptr);
				VirtualProtectEx(Game::hProcess, LPVOID(foundedBases2[i]), 4, oldprotect, &temps);

			}
			IsChanged = true;
		}
		else if (!Setting::MagicX && IsChanged == true)
		{
			BYTE pattern[] = { 0x00, 0x00, 0x20, 0x41, 0x00, 0x00, 0x38, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f };
			BYTE patter2[] = { 0x00, 0x08, 0xF3, 0x45, 0x00, 0x00, 0x38, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f };
			DWORD bytes;
			std::vector<DWORD_PTR> foundedBases;
			DWORD oldprotect, temps;
			foundedBases.clear();
			Utility::MemSearch(patter2, sizeof(patter2), 0x10000000, 0x90000000, false, 0, foundedBases);
			for (int i = 0; i < foundedBases.size(); i++)
			{
				VirtualProtectEx(Game::hProcess, LPVOID(foundedBases[i]), 4, PAGE_EXECUTE_READWRITE, &oldprotect);
				WriteProcessMemory(Game::hProcess, (PVOID)foundedBases[i], pattern, sizeof(pattern), nullptr);
				VirtualProtectEx(Game::hProcess, LPVOID(foundedBases[i]), 4, oldprotect, &temps);

			}
			IsChanged = false;
		}
		if (Setting::MagicX)
			std::this_thread::sleep_for(std::chrono::milliseconds(70));
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}
}

void Memory::MagicBct()
{

	bool IsChanged1 = false;
	while (true)
	{
		Sleep(10);
		if (Setting::magicb && IsChanged1 == false)
		{
					BYTE Scan[] = { 0xB8, 0x41, 0x00, 0x00, 0xC8, 0x41, 0x00, 0x00, 0xF4, 0x41 };
					BYTE Replace[] = { 0xB8, 0x43, 0x00, 0x00, 0xAF, 0x96, 0x00, 0x00, 0xAF, 0x43 };
					BYTE Scan2[] = { 0x01, 0x00, 0x00, 0x00, 0x00, '?', '?', '?', '?', '?', '?', 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '?', 0x3F, '?', 0x00, 0x00, 0x00, 0x01 };
                    BYTE Replace2[] = { 0x01, 0x00, 0x00, 0x00, 0x00, '?', '?', '?', 0xFA, 0x96, 0x28, 0x49, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '?', 0x3F, '?', 0x00, 0x00, 0x00, 0x01 };
					BYTE Scan3[] = { 0xB8, 0x41, 0x00, 0x00, 0xC8, 0x41, 0x00, 0x00, 0xF4, 0x41 };
					BYTE Replace3[] = { 0xB8, 0x41, 0x00, 0x00, 0xAF, 0x43, 0x00, 0x00, 0xAF, 0x43 };
					BYTE Scan4[] = { 0xB8, 0x41, 0x00, 0x00, 0x2F, 0x44, 0x00, 0x00, 0x2F, 0x44 };
					BYTE Replace4[] = { 0xB8, 0x41, 0x00, 0x00, 0xAF, 0x43, 0x00, 0x00, 0xAF, 0x43 };

					Utility::AOBREP(Scan, Replace, sizeof(Scan), sizeof(Replace), 4000);
					Utility::AOBREP(Scan2, Replace2, sizeof(Scan2), sizeof(Replace2), 4000);
					Utility::AOBREP(Scan3, Replace3, sizeof(Scan3), sizeof(Replace3), 4000);
					Utility::AOBREP(Scan4, Replace4, sizeof(Scan4), sizeof(Replace4), 4000);
					IsChanged1 = true;
		}
		else if (!Setting::magicb && IsChanged1 == true)
		{
			BYTE Scan[] = { 0xB8, 0x41, 0x00, 0x00, 0xC8, 0x41, 0x00, 0x00, 0xF4, 0x41 };
			BYTE Replace[] = { 0xB8, 0x43, 0x00, 0x00, 0xAF, 0x96, 0x00, 0x00, 0xAF, 0x43 };
			BYTE Scan2[] = { 0x01, 0x00, 0x00, 0x00, 0x00, '?', '?', '?', '?', '?', '?', 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '?', 0x3F, '?', 0x00, 0x00, 0x00, 0x01 };
			BYTE Replace2[] = { 0x01, 0x00, 0x00, 0x00, 0x00, '?', '?', '?', 0xFA, 0x96, 0x28, 0x49, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '?', 0x3F, '?', 0x00, 0x00, 0x00, 0x01 };
			BYTE Scan3[] = { 0xB8, 0x41, 0x00, 0x00, 0xC8, 0x41, 0x00, 0x00, 0xF4, 0x41 };
			BYTE Replace3[] = { 0xB8, 0x41, 0x00, 0x00, 0xAF, 0x43, 0x00, 0x00, 0xAF, 0x43 };
			BYTE Scan4[] = { 0xB8, 0x41, 0x00, 0x00, 0x2F, 0x44, 0x00, 0x00, 0x2F, 0x44 };
			BYTE Replace4[] = { 0xB8, 0x41, 0x00, 0x00, 0xAF, 0x43, 0x00, 0x00, 0xAF, 0x43 };

			Utility::AOBREP(Replace, Scan, sizeof(Replace), sizeof(Scan), 4000);
			Utility::AOBREP(Replace2, Scan2, sizeof(Replace2), sizeof(Scan2), 4000);
			Utility::AOBREP(Replace3, Scan3, sizeof(Replace3), sizeof(Scan3), 4000);
			Utility::AOBREP(Replace4, Scan4, sizeof(Replace4), sizeof(Scan4), 4000);
					IsChanged1 = false;
		}
	}
}

VOID Memory::flyman()
{
		if (Setting::fly) {
			std::thread([=]()
				{

					BYTE Scan[] = { 0x00 ,0x00 ,0x75 ,0xC4 ,0x00,0x00 ,0x7A ,0x45 ,0x9A ,0x99 ,0x99 ,0x3E ,0x00 ,0x00 ,0x04 ,0x00 ,0x04 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0xA0 ,0x40 ,0x00 };
					BYTE Scan2[] = { 0x10 ,0x27 ,0x00 ,0x00 ,0x10 ,0x27 ,0x00 ,0x00 ,0x20 ,0x4E ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 };
					BYTE Replace[] = { 0x00 ,0x00 ,0x70,0xC1 ,0x00,0x00,0x7A ,0x45 ,0x9A ,0x99 ,0x99 ,0x3E ,0x00 ,0x00 ,0x04 ,0x00 ,0x04,0x00 ,0x00 ,0x00 ,0x00,0x00 ,0xA0 ,0x40 ,0x00 };
					BYTE Replace2[] = { 0xF4 ,0x01 ,0x00 ,0x00 ,0x10 ,0x27 ,0x00 ,0x00 ,0x20 ,0x4E ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,0x00 ,0x00 ,0x00 ,0x00,0x00 ,0x00 ,0x00 ,0x00 };

					Utility::AOBREP(Scan, Replace, sizeof(Scan), sizeof(Replace), 4000);
					Utility::AOBREP(Scan2, Replace2, sizeof(Scan2), sizeof(Replace2), 4000);
				}).detach();

		}
		else if (!Setting::fly)
		{
			std::thread([=]()
				{
					BYTE Scan[] = { 0x00 ,0x00 ,0x70,0xC1 ,0x00,0x00,0x7A ,0x45 ,0x9A ,0x99 ,0x99 ,0x3E ,0x00 ,0x00 ,0x04 ,0x00 ,0x04,0x00 ,0x00 ,0x00 ,0x00,0x00 ,0xA0 ,0x40 ,0x00 };
					BYTE Scan2[] = { 0xF4 ,0x01 ,0x00 ,0x00 ,0x10 ,0x27 ,0x00 ,0x00 ,0x20 ,0x4E ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,0x00 ,0x00 ,0x00 ,0x00,0x00 ,0x00 ,0x00 ,0x00 };
					BYTE Replace[] = { 0x00 ,0x00 ,0x75 ,0xC4 ,0x00,0x00 ,0x7A ,0x45 ,0x9A ,0x99 ,0x99 ,0x3E ,0x00 ,0x00 ,0x04 ,0x00 ,0x04 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0xA0 ,0x40 ,0x00 };
					BYTE Replace2[] = { 0x10 ,0x27 ,0x00 ,0x00 ,0x10 ,0x27 ,0x00 ,0x00 ,0x20 ,0x4E ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 };

					Utility::AOBREP(Scan, Replace, sizeof(Scan), sizeof(Replace), 0);
					Utility::AOBREP(Scan2, Replace2, sizeof(Scan2), sizeof(Replace2), 0);
				}).detach();
		}
}