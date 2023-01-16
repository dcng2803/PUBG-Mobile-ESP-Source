#include "Data.h"
#include<thread>
Espp* g_pEsp = new Espp();
DWORD Data::ViewMatrixBase;
DWORD Data::UWorld = 0;
BYTE ViewMatrixSearch[] =
{
	0x02, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x01
};

DWORD Data::GetViewMatrixBase(DWORD StartAddress, DWORD EndAddress)
{
	DWORD ViewMatrixBase = 0;
	std::vector<DWORD_PTR> FoundedBase;
	Utility::MemSearch(ViewMatrixSearch, sizeof(ViewMatrixSearch), StartAddress, EndAddress, 0, 0, FoundedBase);

	for (int i = 0; i < FoundedBase.size(); i++)
	{
		DWORD Cand = FoundedBase[i] - 0x20;
		DWORD Eng = Utility::ReadMemoryEx<DWORD>(Cand) + 0x20;
		DWORD Tmp = Utility::ReadMemoryEx<DWORD>(Eng) + 0x200;

		float v1, v2, v3, v4;
		v1 = Utility::ReadMemoryEx<float>(Tmp + 0x8);
		v2 = Utility::ReadMemoryEx<float>(Tmp + 0x18);
		v3 = Utility::ReadMemoryEx<float>(Tmp + 0x28);
		v4 = Utility::ReadMemoryEx<float>(Tmp + 0x38);

		if (v1 == 0 && v2 == 0 && v3 == 0 && v4 >= 3)
		{
			ViewMatrixBase = Cand;
			FoundedBase.clear();
			break;
		}
	}

	FoundedBase.clear();
	return ViewMatrixBase;
}


string GetString(DWORD BaseAddress)
{
	char* String = new char[34];
	for (int i = 0; i < 34; i++)
	{
		String[i] = Utility::ReadMemoryEx<char>(BaseAddress + i);
	}

	string Result = String;

	delete[] String;
	return Result;
}

string GetEntityType(LONG GNames, LONG Id)
{
	string Result = "";

	DWORD GName = Utility::ReadMemoryEx<DWORD>(GNames);

	if (Id > 0 && Id < 2000000)
	{
		DWORD Page = Id / 16384;
		DWORD Index = Id % 16384;
		DWORD SecPartAddv = Utility::ReadMemoryEx<DWORD>(GName + Page * 4);

		if (SecPartAddv > 0)
		{
			LONG NameAddv = Utility::ReadMemoryEx<DWORD>(SecPartAddv + Index * 4);

			if (NameAddv > 0)
			{
				Result = GetString(NameAddv + 0x8);
			}
		}
	}

	return Result;
}

struct ObjectName
{
	char Data[64];
};

string GetEntityType2(LONG GNames, LONG Id)
{
	DWORD GName = Utility::ReadMemoryEx<DWORD>(GNames);
	DWORD NamePtr = Utility::ReadMemoryEx<DWORD>(GName + int(Id / 16384) * 0x4);
	DWORD Name = Utility::ReadMemoryEx<DWORD>(NamePtr + int(Id % 16384) * 0x4);
	ObjectName pBuffer = Utility::ReadMemoryEx<ObjectName>(Name + 0x8);

	return string(pBuffer.Data);
}

string GetPlayerName(DWORD BaseAddress)
{
	char* String = new char[34];
	for (int i = 0; i < 34; i++)
	{
		String[i] = Utility::ReadMemoryEx<char>(BaseAddress + i * 2);
	}

	string Name = String;

	delete[] String;
	return Name;
}

BOOL IsPlayer(string EntityType)
{
	if (EntityType.find("BP_PlayerPawn") != string::npos)
	{
		return TRUE;
	}

	return FALSE;
}

std::string GetGrenadeType(std::string classname)
{
	if (classname.find("BP_Grenade_Smoke_C") != std::string::npos)
		return "Smoke";
	if (classname.find("BP_Grenade_Burn_C") != std::string::npos)
		return "Burn";
	/* if (classname.find("BP_Grenade_tun_C") != std::string::npos)
		 return "[ALERT] !Flash Granade";*/
	if (classname.find("BP_Grenade_Shoulei_C") != std::string::npos)
		return "Frag";
	if (classname.find("BP_Rifle_M416_Wrapper_C") != std::string::npos)
		return "M416.";
	return "tatti";
}
//std::string Dead(DWORD code, std::string classname)
//{
//	if (code == 103003 || classname.find("BP_Sniper_AWM_Wrapper_C") != std::string::npos)
//		return "AWM";
//	if (code == 103010 || classname.find("BP_Sniper_QBU_Wrapper_C") != std::string::npos)
//		return "QBU";
//	if (code == 103009 || classname.find("BP_Sniper_SLR_Wrapper_C") != std::string::npos)
//		return "SLR";
//	if (code == 103004 || classname.find("BP_Sniper_SKS_Wrapper_C") != std::string::npos)
//		return "SKS";
//	if (code == 103006 || classname.find("BP_Sniper_Mini14_Wrapper_C") != std::string::npos)
//		return "Mini14";
//	if (code == 103002 || classname.find("BP_Sniper_M24_Wrapper_C") != std::string::npos)
//		return "M24";
//	if (code == 103001 || classname.find("BP_Sniper_Kar98k_Wrapper_C") != std::string::npos)
//		return "Kar98k";
//	if (code == 103005 || classname.find("BP_Sniper_VSS_Wrapper_C") != std::string::npos)
//		return "VSS";
//	if (code == 103008 || classname.find("BP_Sniper_Win94_Wrapper_C") != std::string::npos)
//		return "Win94";
//
//		if (code == 101008 || classname.find("BP_Rifle_M762_Wrapper_C") != std::string::npos)
//			
//		     return "M762";
//	
//	
//		
//	//
//	
//	
//		if (code == 101003 || classname.find("BP_Rifle_SCAR_Wrapper_C") != std::string::npos)
//		
//			return "SCAR-L";
//	
//	if (code == 101004 || classname.find("BP_Rifle_M416_Wrapper_C") != std::string::npos)
//	;
//		return "M416";
//
//	if (code == 101002 || classname.find("BP_Rifle_M16A4_Wrapper_C") != std::string::npos)
//		return "M16A-4";
//	if (code == 101009 || classname.find("BP_Rifle_Mk47_Wrapper_C") != std::string::npos)
//		return "Mk47 Mutant";
//	if (code == 101010 || classname.find("BP_Rifle_G36_Wrapper_C") != std::string::npos)
//		return "G36C";
//	if (code == 101007 || classname.find("BP_Rifle_QBZ_Wrapper_C") != std::string::npos)
//		return "QBZ";
//	if (code == 101001 || classname.find("BP_Rifle_AKM_Wrapper_C") != std::string::npos)
//
//		return "AKM";
//	if (code == 101005 || classname.find("BP_Rifle_Groza_Wrapper_C") != std::string::npos)
//		return "Groza";
//	if (code == 101006 || classname.find("BP_Rifle_AUG_Wrapper_C") != std::string::npos)
//		return "AUG_A3";
//	if (code == 104003 || classname.find("BP_ShotGun_S12K_Wrapper_C") != std::string::npos)
//		return "S12K";
//	if (code == 104004 || classname.find("BP_ShotGun_DP12_Wrapper_C") != std::string::npos)
//		return "DBS";
//	if (code == 104001 || classname.find("BP_ShotGun_S686_Wrapper_C") != std::string::npos)
//		return "S686";
//	if (code == 104002 || classname.find("BP_ShotGun_S1897_Wrapper_C") != std::string::npos)
//		return "S1897";
//	if (code == 106006 || classname.find("BP_ShotGun_SawedOff_Wrapper_C") != std::string::npos)
//		return "SawedOff";
//	if (code == 102005 || classname.find("BP_MachineGun_PP19_Wrapper_C") != std::string::npos)
//		return "PP19  Bizon";
//	if (code == 102004 || classname.find("BP_MachineGun_TommyGun_Wrapper_C") != std::string::npos)
//		return "Tommy  Gun";
//	if (code == 102007 || classname.find("BP_MachineGun_MP5K_Wrapper_C") != std::string::npos)
//		return "MP5K";
//	if (code == 102002 || classname.find("BP_MachineGun_UMP9_Wrapper_C") != std::string::npos)
//		return "UMP9";
//	if (code == 102003 || classname.find("BP_MachineGun_Vector_Wrapper_C") != std::string::npos)
//		return "Vector";
//	if (code == 102001 || classname.find("BP_MachineGun_Uzi_Wrapper_C") != std::string::npos)
//		return "Uzi";
//	
//	if (code == 108004 || classname.find("BP_WEP_Pan_Pickup_C") != std::string::npos)
//		return "Pan";
//	if (code == 103007 || classname.find("BP_WEP_Mk14_Pickup_C") != std::string::npos)
//		return "Mk14";
//	if (code == 302001 || classname.find("BP_Ammo_762mm_Pickup_C") != std::string::npos)
//		return "7.62";
//	if (code == 305001 || classname.find("BP_Ammo_45ACP_Pickup_C") != std::string::npos)
//		return "45ACP";
//	if (code == 303001 || classname.find("BP_Ammo_556mm_Pickup_C") != std::string::npos)
//		return "5.56";
//	if (code == 301001 || classname.find("BP_Ammo_9mm_Pickup_C") != std::string::npos)
//		return "9mm";
//	if (code == 306001 || classname.find("BP_Ammo_300Magnum_Pickup_C") != std::string::npos)
//		return "300  Magnum";
//	if (code == 304001 || classname.find("BP_Ammo_12Guage_Pickup_C") != std::string::npos)
//		return "12Guage";
//	if (code == 307001 || classname.find("BP_Ammo_Bolt_Pickup_C") != std::string::npos)
//		return "Arrows";
//	
//	
//
//	if (code == 204013 || classname.find("BP_DJ_Large_EQ_Pickup_C") != std::string::npos)
//
//	if (code == 203005 || classname.find("BP_MZJ_8X_Pickup_C") != std::string::npos)
//		return "8x";
//	if (code == 203003 || classname.find("BP_MZJ_2X_Pickup_C") != std::string::npos)
//		return "2x";
//	if (code == 203001 || classname.find("BP_MZJ_HD_Pickup_C") != std::string::npos)
//		return "Lazer";
//	if (code == 203014 || classname.find("BP_MZJ_3X_Pickup_C") != std::string::npos)
//		return "3X";
//	if (code == 203002 || classname.find("BP_MZJ_QX_Pickup_C") != std::string::npos)
//		return "Holo";
//	if (code == 203015 || classname.find("BP_MZJ_6X_Pickup_C") != std::string::npos)
//		return "6x";
//	if (code == 203004 || classname.find("BP_MZJ_4X_Pickup_C") != std::string::npos)
//		return "4x";
//	if (code == 105002 || classname.find("BP_Other_DP28_Wrapper_C") != std::string::npos)
//		return "DP28";
//	if (code == 107001 || classname.find("BP_Other_CrossBow_Wrapper_C") != std::string::npos)
//		return "CrossBow";
//	if (code == 105001 || classname.find("BP_Other_M249_Wrapper_C") != std::string::npos)
//		return "M249";
//	if (code == 501006 || classname.find("PickUp_BP_Bag_Lv3_C") != std::string::npos)
//		return "Bag Lv.3";
//
//	
//	if (code == 502003 || classname.find("PickUp_BP_Helmet_Lv3_B_C") != std::string::npos)
//		return "Helmet Lv.3";
//
//
//	if (code == 0 && classname.find("BP_Grenade_Shoulei_C") != std::string::npos)
//		return "Bomb!";
//	if (code == 602002 || classname.find("BP_Grenade_Smoke_Weapon_Wrapper_C") != std::string::npos)
//		return "Smoke";
//	if (code == 602003 || classname.find("BP_Grenade_Burn_Weapon_Wrapper_C") != std::string::npos)
//		return "Molotof";
//	if (code == 0 && classname.find("BP_Grenade_Burn_C") != std::string::npos)
//		return "Burn!";
//	if (code == 602005 || classname.find("BP_Grenade_Apple_Weapon_Wrapper_C") != std::string::npos)
//		return "Apple";
//	if (code == 601003 || classname.find("Pills_Pickup_C") != std::string::npos)
//		return "Painkiller";
//	if (code == 601002 || classname.find("Injection_Pickup_C") != std::string::npos)
//		return "Adrenaline ";
//	if (code == 601001 || classname.find("Drink_Pickup_C") != std::string::npos)
//		return "Energy Drink";
//	if (code == 601005 || classname.find("Firstaid_Pickup_C") != std::string::npos)
//		return "FirstaidKit";
//
//	if (code == 308001 || classname.find("BP_Ammo_Flare_Pickup_C") != std::string::npos)
//		return "Flaregun";
//	if (code == 501003 || classname.find("PickUp_BP_Bag_Lv3_Inbox_C") != std::string::npos)
//		return "Bag Lv.3";
//	
//	return "tatti";
//}
string GetItemType(string Name)
{///name.find("BP_Rifle_AKM_Wrapper_C") != std::string::npos
	if (Setting::Esp_Item_AKM) if (Name.find("BP_Rifle_AKM_Wrapper_C") != std::string::npos) return "AKM";
	if (Setting::Esp_Item_M249) if (Name.find("BP_Other_M249_Wrapper_C") != std::string::npos)return "M249";
		
	if (Setting::Esp_Item_Mk12) if (Name.find("BP_Other_MK12_Wrapper_C") != std::string::npos)return "MK12";
	if (Setting::plan)  if (Name.find("BP_AirDropPlane_C") != std::string::npos) return "Plan";
	if (Setting::Esp_Item_SCARL) if (Name.find("BP_Rifle_SCAR_Wrapper_C") != string::npos) return "SCAR-L";
	if (Setting::Esp_Item_M416) if (Name.find("BP_Rifle_M416_Wrapper_C") != std::string::npos) return "M416";
	if (Setting::Esp_Item_M24) if (Name.find("BP_Sniper_M24_Wrapper_C") != string::npos) return "M24";
	if (Setting::Esp_Item_Kar98k) if (Name.find("BP_Sniper_Kar98k_Wrapper_C") != string::npos) return "Kar98k";
	if (Setting::Esp_Item_Awm) if (Name.find("BP_Sniper_AWM_Wrapper_C") != string::npos) return "AWM";
	if (Setting::Esp_Item_FlareGun) if (Name.find("BP_Pistol_Flaregun_Wrapper_C") != string::npos) return "Flare Gun";
	if (Setting::Esp_Item_Flare) if (Name.find("BP_Ammo_Flare_Pickup_C") != string::npos) return "Flare";
	//if (Setting::Esp_Item_AirDrop) if (Name.find("AirDropListWrapperActor") != string::npos) return "Air Drop";
	
	if (Setting::Esp_Item_M762) if (Name.find("BP_Rifle_M762_Wrapper_C") != string::npos) return "M762";
	if (Setting::Esp_Item_Mk47Mutant) if (Name.find("BP_Rifle_Mk47_Wrapper_C") != string::npos) return "Mk47 Mutant";
	if (Setting::Esp_Item_DP28) if (Name.find("BP_Other_DP28_Wrapper_C") != string::npos) return "DP28";
	if (Setting::Esp_Item_SCARL) if (Name.find("BP_Rifle_SCAR_Wrapper_C") != string::npos) return "SCAR-L";
	if (Setting::Esp_Item_M416) if (Name.find("BP_Rifle_M416_Wrapper_C") != string::npos) return "M416";
	if (Setting::Esp_Item_M16A4) if (Name.find("BP_Rifle_M16A4_Wrapper_C") != string::npos) return "M16A4";
	if (Setting::Esp_Item_G36C) if (Name.find("BP_Rifle_G36_Wrapper_C") != string::npos) return "G36C";
	if (Setting::Esp_Item_QBZ) if (Name.find("BP_Rifle_QBZ_Wrapper_C") != string::npos) return "QBZ";

	if (Setting::Esp_Item_QBU) if (Name.find("BP_Sniper_QBU_Wrapper_C") != string::npos) return "QBU";
	if (Setting::Esp_Item_SLR) if (Name.find("BP_Sniper_SLR_Wrapper_C") != string::npos) return "SLR";
	if (Setting::Esp_Item_SKS) if (Name.find("BP_Sniper_SKS_Wrapper_C") != string::npos) return "SKS";
	if (Setting::Esp_Item_Mini14) if (Name.find("BP_Sniper_Mini14_Wrapper_C") != string::npos) return "Mini-14";


	if (Setting::Esp_Item_PP19) if (Name.find("BP_MachineGun_PP19_Wrapper_C") != string::npos) return "PP19";
	if (Setting::Esp_Item_TommyGun) if (Name.find("BP_MachineGun_TommyGun_Wrapper_C") != string::npos) return "Tommy Gun";
	if (Setting::Esp_Item_MP5K) if (Name.find("BP_MachineGun_MP5K_Wrapper_C") != string::npos) return "MP5K";
	if (Setting::Esp_Item_UMP9) if (Name.find("BP_MachineGun_UMP9_Wrapper_C") != string::npos) return "UMP9";
	if (Setting::Esp_Item_Vector) if (Name.find("BP_MachineGun_Vector_Wrapper_C") != string::npos) return "Vector";
	if (Setting::Esp_Item_Uzi) if (Name.find("BP_MachineGun_Uzi_Wrapper_C") != string::npos) return "Uzi";
	if (Setting::Esp_Item_Awmimo) if (Name.find("BP_Ammo_300Magnum_Pickup_C") != std::string::npos) return "300Magnum";
	if (Setting::Esp_Item_762mm) if (Name.find("BP_Ammo_762mm_Pickup_C") != string::npos) return "7.62mm";
	if (Setting::Esp_Item_556mm) if (Name.find("BP_Ammo_556mm_Pickup_C") != string::npos) return "5.56mm";
	if (Setting::Esp_Item_9mm) if (Name.find("BP_Ammo_9mm_Pickup_C") != string::npos) return "9mm";
	if (Setting::Esp_Item_45ACP) if (Name.find("BP_Ammo_45ACP_Pickup_C") != string::npos) return "45ACP";

	if (Setting::Esp_Item_Holo) if (Name.find("BP_MZJ_QX_Pickup_C") != string::npos) return "Holo";
	

	//if (Setting::Esp_Item_AirDrop) if (Name.find("BP_AirDropBox_C") != string::npos) return "Air Drop";
	if (Setting::Esp_Item_MedKit) if (Name.find("FirstAidbox_Pickup_C") != string::npos) return "Med Kit";
	if (Setting::Esp_Item_FirstAidKit) if (Name.find("Firstaid_Pickup_C") != string::npos) return "First Aid Kit";
	if (Setting::Esp_Item_Painkiller) if (Name.find("Pills_Pickup_C") != string::npos) return "Painkiller";
	if (Setting::Esp_Item_EnergyDrink) if (Name.find("Drink_Pickup_C") != string::npos) return "Energy Drink";
	if (Setting::Esp_Item_AdrenalineSyringe) if (Name.find("Injection_Pickup_C") != string::npos) return "Adrenaline Syringe";

	if (Setting::Esp_Item_Frag) if (Name.find("BP_Grenade_Shoulei_Weapon_Wrapper_C") != string::npos) return "frag";
	if (Setting::Esp_Item_Frag) if (Name.find("BP_Grenade_Shoulei_C") != string::npos) return "Frag";
	if (Setting::Esp_Item_Smoke) if (Name.find("BP_Grenade_Smoke_Weapon_Wrapper_C") != string::npos) return "Smoke";
	if (Setting::Esp_Item_Molotof) 	if (Name.find("BP_Grenade_Burn_C") != std::string::npos)return "Burn";
		
	if (Setting::Esp_Item_Molotof) if (Name.find("BP_Grenade_Burn_Weapon_Wrapper_C") != string::npos) return "Molotof";

	if (Setting::Esp_Item_Helmet1) if (Name.find("PickUp_BP_Helmet_Lv1") != string::npos) return "Helmet L.1";
	if (Setting::Esp_Item_Helmet2) if (Name.find("PickUp_BP_Helmet_Lv2") != string::npos) return "Helmet L.2";
	if (Setting::Esp_Item_Helmet3) if (Name.find("PickUp_BP_Helmet_Lv3") != string::npos) return "Helmet L.3";

	if (Setting::Esp_Item_Armor1) if (Name.find("PickUp_BP_Armor_Lv1") != string::npos) return "Armor L.1";
	if (Setting::Esp_Item_Armor2) if (Name.find("PickUp_BP_Armor_Lv2") != string::npos) return "Armor L.2";
	if (Setting::Esp_Item_Armor3) if (Name.find("PickUp_BP_Armor_Lv3") != string::npos) return "Armor L.3";
	if (Setting::Esp_Item_x2) if (Name.find("BP_MZJ_2X_Pickup_C") != string::npos) return "2x";
	if (Setting::Esp_Item_x3) if (Name.find("BP_MZJ_3X_Pickup_C") != string::npos) return "3x";
	if (Setting::Esp_Item_x4) if (Name.find("BP_MZJ_4X_Pickup_C") != string::npos) return "4x";
	if (Setting::Esp_Item_x6) if (Name.find("BP_MZJ_6X_Pickup_C") != string::npos) return "6x";
	if (Setting::Esp_Item_x8) if (Name.find("BP_MZJ_8X_Pickup_C") != string::npos) return "8x";
	if (Setting::Esp_Item_Bag1) if (Name.find("PickUp_BP_Bag_Lv1") != string::npos) return "Bag L.1";
	if (Setting::Esp_Item_Bag2) if (Name.find("PickUp_BP_Bag_Lv2") != string::npos) return "Bag L.2";
	if (Setting::Esp_Item_Bag3) if (Name.find("PickUp_BP_Bag_Lv3") != string::npos) return "Bag L.3";
//	if (Setting::deadbox) if (Name.find("PlayerDeadInventoryBox") != std::string::npos || Name.find("PickUpListWrapperActor") != std::string::npos) return "Dead-Box";
	
		
	return "Unknown";
}
bool IsBox(std::string classname)
{
	/*if ( classname.find("BP_AirDropBox_C") != std::string::npos)
		return true;*/
	if (classname.find("PickUpListWrapperActor") != std::string::npos || classname.find("AirDropListWrapperActor") != std::string::npos)
		return true;

	return false;
}
string GetVehicleType(string Name)
{
	if (Name.find("BRDM") != std::string::npos) return "BRDM";
	if (Name.find("Scooter") != std::string::npos) return "Scooter";
	if (Name.find("Motorcycle") != std::string::npos) return "Motorcycle";
	if (Name.find("Snowmobile") != std::string::npos) return "Snowmobile";
	if (Name.find("Tuk") != std::string::npos) return "Tuk";
	if (Name.find("Buggy") != std::string::npos) return "Buggy";
	if (Name.find("open") != std::string::npos) return "Sports";
	if (Name.find("close") != std::string::npos) return "Sportsa";
	if (Name.find("Dacia") != std::string::npos) return "Dacia";
	if (Name.find("Rony") != std::string::npos) return "Rony";
	if (Name.find("UAZ") != std::string::npos) return "UAZ";
	if (Name.find("MiniBus") != std::string::npos) return "Minibus";
	if (Name.find("PG117") != std::string::npos) return "PG117";
	if (Name.find("AquaRail") != std::string::npos) return "Aquarail";
	

	return "Unknown";
}
std::string GetBoxItems(int code)
{
	if (code == 101008)
	{
		Setting::colorcode = weapon::m762;
		return "M762";

	}
	else if (code == 306001)
	{
		Setting::colorcode = weapon::AWMimmo;
		return "AWM Ammo";
	}

	else if (code == 101003)
	{
		Setting::colorcode = weapon::scarl;
		return "SCAR-L";
	}
	else if (code == 302001)
	{

		Setting::colorcode = Colors::ammo7;
		return "7.62";
	}
	else if (code == 303001)
	{

		Setting::colorcode = Colors::ammo5;
		return "5.56";
	}
	else if (code == 602004)
	{
		Setting::colorcode = Colors::Grandes;

		return "Grenade";
	}
	else if (code == 601006)
	{
		Setting::colorcode = Colors::madkit;

		return "Medkit";
	}
	else if (code == 101004)
	{
		Setting::colorcode = weapon::m16;
		return "M416";
	}
	
	else if (code == 101010)
	{
		Setting::colorcode = Colors::g36;
		return "G36";
	}
		
	else if (code == 101006)
	{
		Setting::colorcode = Colors::aug;
		return "AUG";
	}
		
	else if (code == 101101)
	{
		Setting::colorcode = Colors::assm;
		return "ASM"; //time
	}
		
	else if (code == 101001)
	{
		Setting::colorcode = weapon::AKM;
		return "AKM";
	}
		
	else if (code == 101005)
	{
		Setting::colorcode = weapon::GROZZA;
		return "Groza";
	}
		
	else if (code == 103003)
	{
		Setting::colorcode = weapon::AWM;
		return "AWM";
	}
		
	else if (code == 103002)
	{
		Setting::colorcode = weapon::M24;
		return "M24";
	}
		
	else if (code == 103001)
	{
		Setting::colorcode = weapon::kar98;
		return "Kar98k";
	}
		
	else if (code == 103011)
	{
		Setting::colorcode = Colors::mosin;
		return "Mosin";
	}
		
	else if (code == 502002)
	{
		Setting::colorcode = Colors::helmat;
		return "Helmet Lv.2";
	}
		
	else if (code == 502003)
	{
		Setting::colorcode = Colors::helmat;
		return "Helmet Lv.3";
	}
	
	else if (code == 503002)
	{
		Setting::colorcode = Colors::armer;
		return "Armor Lv.2";
	}
		
	else if (code == 503003)
	{
		Setting::colorcode = Colors::armer;
		return "Armor Lv.3";
	}
		
	else if (code == 501006)
	{
		Setting::colorcode = Colors::bag;
		return "Bag Lv.3";
	}
		
	else if (code == 501005)
	{
	Setting::colorcode = Colors::bag;
		return "Bag Lv.2";
	}
		
	else if (code == 203014)
	{
	Setting::colorcode = Colors::scope3x;
		return "Scope 3x";

	}
		
	else if (code == 203004)
	{
	Setting::colorcode = Colors::scope4x;
		return "Scope 4x";
	}
		
	else if (code == 203015)
	{
	Setting::colorcode = Colors::scope6x;
	return "Scope 6x";
	}
		
	else if (code == 203005)
	{
	Setting::colorcode = weapon::scope8x;
	return "Scope 8x";
	}
		
	else if (code == 106007)
	{
	Setting::colorcode = weapon::Flare;
	return "Flaregun";
	}
		
	else if (code == 105001)
	{
	Setting::colorcode = Colors::M249;
	return "M249";
	}
		
	else if (code == 105002)
	{
	Setting::colorcode = Colors::dp;
	return "DP28";
	}
		
	else if (code == 105010)
	{
	Setting::colorcode = Colors::mg3;
	return "MG3";
	}
	
	return "tatti";
}
DWORD Data::NetDriver;
DWORD Data::LocalPlayer;
DWORD Data::PlayerController;
DWORD Data::EntityAddress = 0;
vector<AActor> Data::AActorList = {};
vector<BombAlert> Data::BombAlertList = {};
vector<Item> Data::ItemList = {};
vector<impItem> Data::impItemList = {};
vector<Vehicle> Data::VehicleList = {};
//std::vector<Airdrop>  Data::LootData, Data::LootDatadatar = {};

VOID Data::QueryThread()
{
	int lib_UE4_so = Utility::ReadMemoryEx<int>(0xE0C36E8);
	DWORD GWorld = Utility::ReadMemoryEx<DWORD>(Data::ViewMatrixBase + 114572) + 0x3C;
	DWORD GName = Utility::ReadMemoryEx<DWORD>(Data::ViewMatrixBase - 3117016) + 0x88;

	//DWORD UWorld = 0;
	DWORD PersistentLevel = 0;	
	DWORD ServerConnection = 0; 
	DWORD EntityList = 0;
	INT EntityCount = 0;

	
	DWORD EntityStruct = 0;
	string EntityType = "";

	DWORD RootComponent = 0;	
	INT MyTeamId = 9999;

	DWORD MeshAddv;
	DWORD BodyAddv;				
	DWORD BoneAddv;

	string GrenadeType = "";
	string ItemType = "";
	string VehicleType = "";
	string airdroptype = "";
	string deadboxtype = "";

	vector<AActor> AActorList = {};
	vector<BombAlert> BombAlertList = {};
	vector<Item> ItemList = {};
	vector<impItem> impItemList = {};
	vector<Vehicle> VehicleList = {};
	

	

	while (true)
	{
	
		g_pEsp->lootbox.clear();
		Sleep(30);
		UWorld = Utility::ReadMemoryEx<DWORD>(GWorld);
		PersistentLevel = Utility::ReadMemoryEx<DWORD>(UWorld + Offset::PersistentLevel);
		NetDriver = Utility::ReadMemoryEx<DWORD>(UWorld + Offset::NetDriver);
		ServerConnection = Utility::ReadMemoryEx<DWORD>(NetDriver + Offset::ServerConnection);
		PlayerController = Utility::ReadMemoryEx<DWORD>(ServerConnection + Offset::PlayerController);
		LocalPlayer = Utility::ReadMemoryEx<DWORD>(PlayerController + Offset::AcknowledgedPawn);
		EntityList = Utility::ReadMemoryEx<DWORD>(PersistentLevel + Offset::EntityList);
		EntityCount = Utility::ReadMemoryEx<INT>(PersistentLevel + Offset::EntityCount);

		for (int i = 0; i < EntityCount; ++i)
		{
			EntityAddress = Utility::ReadMemoryEx<DWORD>(EntityList + i * 4);
			EntityStruct = Utility::ReadMemoryEx<DWORD>(EntityAddress + 0x10);

			if (Emulator::IsGameLoop4 == TRUE)
			{
				EntityType = GetEntityType(GName, EntityStruct);
			}
			else if (Emulator::IsGameLoop7 == TRUE)
			{
				EntityType = GetEntityType2(GName, EntityStruct);
			}
			else if (Emulator::IsSmartGaGa4 == TRUE)
			{
				EntityType = GetEntityType2(GName, EntityStruct);
			}
			else if (Emulator::IsSmartGaGa7 == TRUE)
			{
				EntityType = GetEntityType2(GName, EntityStruct);
			}

			RootComponent = Utility::ReadMemoryEx<DWORD>(EntityAddress + Offset::RootComponent);

			if (IsPlayer(EntityType))
			{
				AActor AActor;
				AActor.TeamId = Utility::ReadMemoryEx<INT>(EntityAddress + Offset::TeamId);
				AActor.Address = EntityAddress;

				if (AActor.Address == LocalPlayer)
				{
					MyTeamId = AActor.TeamId;
					continue;
				}

				if (AActor.TeamId == MyTeamId) continue;

				AActor.Position = Utility::ReadMemoryEx<VECTOR3>(RootComponent + Offset::Position);
				AActor.IsBot = Utility::ReadMemoryEx<bool>(EntityAddress + Offset::IsBot);
				AActor.Name = GetPlayerName(Utility::ReadMemoryEx<DWORD>(EntityAddress + Offset::Name));
				AActor.Health = Utility::ReadMemoryEx<FLOAT>(EntityAddress + Offset::Health);
				AActor.HealthMax = Utility::ReadMemoryEx<FLOAT>(EntityAddress + Offset::HealthMax);
				AActor.Velocity = Utility::ReadMemoryEx<VECTOR3>(Utility::ReadMemoryEx<DWORD>(AActor.Address + Offset::CharacterMovement) + Offset::LastUpdateVelocity);
				AActor.TeamId = Utility::ReadMemoryEx<INT>(EntityAddress + Offset::TeamId);
				if (Setting::Bone || Setting::MagicBullet)
				{
					MeshAddv = Utility::ReadMemoryEx<DWORD>(AActor.Address + Offset::Mesh);
					BodyAddv = MeshAddv + Offset::BodyAddv;
					BoneAddv = Utility::ReadMemoryEx<DWORD>(MeshAddv + Offset::MinLOD) + 48;
					AActor.HeadPos = Algorithm::GetBoneWorldPosition(BodyAddv, BoneAddv + 5 * 48);
				}

				if (Setting::Bone)
				{
					AActor.ChestPos = Algorithm::GetBoneWorldPosition(BodyAddv, BoneAddv + 4 * 48);
					AActor.PelvisPos = Algorithm::GetBoneWorldPosition(BodyAddv, BoneAddv + 1 * 48);
					AActor.lSholderPos = Algorithm::GetBoneWorldPosition(BodyAddv, BoneAddv + 11 * 48);
					AActor.rSholderPos = Algorithm::GetBoneWorldPosition(BodyAddv, BoneAddv + 32 * 48);
					AActor.lElbowPos = Algorithm::GetBoneWorldPosition(BodyAddv, BoneAddv + 12 * 48);
					AActor.rElbowPos = Algorithm::GetBoneWorldPosition(BodyAddv, BoneAddv + 33 * 48);
					AActor.lWristPos = Algorithm::GetBoneWorldPosition(BodyAddv, BoneAddv + 63 * 48);
					AActor.rWristPos = Algorithm::GetBoneWorldPosition(BodyAddv, BoneAddv + 62 * 48);
					AActor.lThighPos = Algorithm::GetBoneWorldPosition(BodyAddv, BoneAddv + 52 * 48);
					AActor.rThighPos = Algorithm::GetBoneWorldPosition(BodyAddv, BoneAddv + 56 * 48);
					AActor.lKneePos = Algorithm::GetBoneWorldPosition(BodyAddv, BoneAddv + 53 * 48);
					AActor.rKneePos = Algorithm::GetBoneWorldPosition(BodyAddv, BoneAddv + 57 * 48);
					AActor.lAnklePos = Algorithm::GetBoneWorldPosition(BodyAddv, BoneAddv + 54 * 48);
					AActor.rAnklePos = Algorithm::GetBoneWorldPosition(BodyAddv, BoneAddv + 58 * 48);
				}

				AActorList.push_back(AActor);
			}
			

			if (Setting::Item)
			{
				ItemType = GetItemType(EntityType);
				if (ItemType.find("Unknown") == std::string::npos)
				{
					Item Item;
					Item.Name = ItemType;
					Item.Position = Utility::ReadMemoryEx<VECTOR3>(RootComponent + Offset::Position);

					ItemList.push_back(Item);
				}
			}

			if (Setting::Vehicle)
			{
				VehicleType = GetVehicleType(EntityType);
				if (VehicleType.find("Unknown") == std::string::npos)
				{
					Vehicle Vehicle;
					Vehicle.Name = VehicleType;
					Vehicle.Position = Utility::ReadMemoryEx<VECTOR3>(RootComponent + Offset::Position);

					DWORD veh = Utility::ReadMemoryEx<DWORD>(EntityAddress + Offset::VehicleCommon);
					float HP = Utility::ReadMemoryEx<float>(veh + Offset::HP);
					float HPMax = Utility::ReadMemoryEx<float>(veh + Offset::HPMax);
				//	int _HP = (int)(HP * 100 / HPMax);
					Vehicle.hp = (int)(HP * 100 / HPMax);
					Vehicle.driving = Utility::ReadMemoryEx<DWORD>(EntityAddress + 0x681);
					float Fuel = Utility::ReadMemoryEx<float>(veh + Offset::Fuel);
					float FuelMax = Utility::ReadMemoryEx<float>(veh + Offset::FuelMax);

					Vehicle.oil = (int)(Fuel * 100 / FuelMax);

					VehicleList.push_back(Vehicle);
				}
			}
			
		
			if (Setting::deadbox)
			{
				if (IsBox(EntityType))
				{
					
					g_pEsp->lootbox.push_back(g_pEsp->lbox);
					
					DWORD  count = Utility::ReadMemoryEx<DWORD>(EntityAddress + 0x644);
					if (count > 30)
						count = 30;
					g_pEsp->lbox.boxitem.clear();
					if (count > 0)

					{
						long itemBase = Utility::ReadMemoryEx<DWORD>(EntityAddress + 0x640);
					//	DWORD tem1 = Utility::ReadMemoryEx<DWORD>(EntityAddress + 0x630);
						long itemAddv;
						
						for (int t = 0; t < count; t++)
						{
							itemAddv = itemBase + t * 48;
						//	std::string bitm = Dead(Utility::ReadMemoryEx<DWORD>(itemAddv + 0x4), "");
							std::string bitm = GetBoxItems(Utility::ReadMemoryEx<DWORD>(itemAddv + 0x4));
							if (bitm.find("tatti") == std::string::npos)
							{
								Itemb temo;
								bitm.append("  :  ").append(std::to_string(Utility::ReadMemoryEx<DWORD>(itemAddv + 0x18)));
								g_pEsp->lbox.Position = Utility::ReadMemoryEx<VECTOR3>(Utility::ReadMemoryEx<DWORD>(EntityAddress + 332) + Offset::Position);
								temo.colorcod = Setting::colorcode;
								temo.Displayname = bitm;
								g_pEsp->lbox.boxitem.push_back(temo);
								//	bitm.clear();
							}
						}
					}
					

				}

			}


		}

		Data::AActorList.clear();
		Data::BombAlertList.clear();
		Data::ItemList.clear();
		Data::VehicleList.clear();
		Data::AActorList = AActorList;
		Data::BombAlertList = BombAlertList;
		Data::ItemList = ItemList;
		Data::VehicleList = VehicleList;
		g_pEsp->lootboxr = g_pEsp->lootbox;
		AActorList.clear();
		BombAlertList.clear();
		ItemList.clear();
		VehicleList.clear();
	}
}

