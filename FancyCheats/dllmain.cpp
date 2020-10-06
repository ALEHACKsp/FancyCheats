#include "main.h"

#define OFFSET_UWORLD 0x7085D20 //48 8b 0d ? ? ? ? 48 85 c9 74 30 e8 ? ? ? ? 48 8b f8
#define location_Offs 0x705FC90 //F3 44 0F 11 1D ? ? ? ?

bool ShowMenu = false;
bool visuals = false;
bool skeleton = false;
bool aimbot = false;
bool box = false;
bool line = false;
bool drawfov = false;
bool playeresp = false;
bool dist = false;
bool filled = true;
bool fovchanger = false;

static int camfov = 50;
static int test1337 = 0;
static float test13372 = 1.19f;

static int VisDist = 350;
static int AimDist = 350;

void writefloat(unsigned long long int Address, float stuff)
{
	info_t Input_Output_Data;

	Input_Output_Data.pid = processID;

	Input_Output_Data.address = (void*)Address;

	Input_Output_Data.value = &stuff;

	Input_Output_Data.size = sizeof(float);

	unsigned long int Readed_Bytes_Amount;

	DeviceIoControl(DriverHandle, ctl_write, &Input_Output_Data, sizeof Input_Output_Data, &Input_Output_Data, sizeof Input_Output_Data, &Readed_Bytes_Amount, nullptr);
}


DWORD Menukey(LPVOID in)
{
	while (1)
	{
		if (GetAsyncKeyState(VK_INSERT) & 1) {
			ShowMenu = !ShowMenu;
		}
		Sleep(5);
	}
}

DWORD init(LPVOID)
{
	DriverHandle = CreateFileW(_xor_(L"\\\\.\\niggers62432").c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (DriverHandle == INVALID_HANDLE_VALUE)
	{
		Sleep(4000);
		exit(0);
	}

	info_t Input_Output_Data1;
	unsigned long int Readed_Bytes_Amount1;
	DeviceIoControl(DriverHandle, ctl_clear, &Input_Output_Data1, sizeof Input_Output_Data1, &Input_Output_Data1, sizeof Input_Output_Data1, &Readed_Bytes_Amount1, nullptr);

	while (hwnd == NULL)
	{
		hwnd = FindWindowA(0, _xor_("Fortnite  ").c_str());
		Sleep(10);
	}
	GetWindowThreadProcessId(hwnd, &processID);

	info_t Input_Output_Data;
	Input_Output_Data.pid = processID;
	unsigned long int Readed_Bytes_Amount;

	DeviceIoControl(DriverHandle, ctl_base, &Input_Output_Data, sizeof Input_Output_Data, &Input_Output_Data, sizeof Input_Output_Data, &Readed_Bytes_Amount, nullptr);

	base_address = (unsigned long long int)Input_Output_Data.data;
	//std::printf(_xor_("Process base address: %p.\n").c_str(), (void*)base_address);

	CreateThread(NULL, NULL, Menukey, NULL, NULL, NULL);

	while (TRUE)
	{
		SetupWindow();
		DirectXInit(MyWnd);
		MainLoop();
		Sleep(6);
	}
}

FTransform GetBoneIndex(DWORD_PTR mesh, int index)
{
	DWORD_PTR bonearray = read<DWORD_PTR>(DriverHandle, processID, mesh + 0x410);

	return read<FTransform>(DriverHandle, processID, bonearray + (index * 0x30));
}

Vector3 GetBoneWithRotation(DWORD_PTR mesh, int id)
{
	FTransform bone = GetBoneIndex(mesh, id);
	FTransform ComponentToWorld = read<FTransform>(DriverHandle, processID, mesh + 0x1C0);

	D3DMATRIX Matrix;
	Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}

Vector3 ProjectWorldToScreen(Vector3 WorldLocation, Vector3 camrot)
{
	Vector3 Screenlocation = Vector3(0, 0, 0);
	Vector3 Rotation = camrot; // FRotator

	D3DMATRIX tempMatrix = Matrix(Rotation);

	Vector3 vAxisX, vAxisY, vAxisZ;

	vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	Vector3 camloc = read<Vector3>(DriverHandle, processID, base_address + location_Offs);

	Vector3 vDelta = WorldLocation - camloc;
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	uint64_t zoomBase = read<uint64_t>(DriverHandle, processID, Localplayer + 0xb0);
	float zoom = read<float>(DriverHandle, processID, zoomBase + 0x500);

	if (fovchanger)
	{
		if (camfov == 50)
		{
			test1337 = 30;
			test13372 = 1.19f;
		}
		else if (camfov == 60)
		{
			test1337 = 23;
			test13372 = 1.277f;
		}
		else if (camfov == 70)
		{
			test1337 = 20;
			test13372 = 1.277f;
		}
		else if (camfov == 80)
		{
			test1337 = 17;
			test13372 = 1.277f;
		}
		else if (camfov == 90)
		{
			test1337 = 14;
			test13372 = 1.277f;
		}
		else if (camfov == 100)
		{
			test1337 = 11;
			test13372 = 1.277f;
		}
		else if (camfov == 110)
		{
			test1337 = 9;
			test13372 = 1.19f;
		}
		else if (camfov = 120)
		{
			test1337 = 6;
			test13372 = 1.277f;
		}
	}
	else
	{
		camfov = 80;
		test1337 = 0;
		test13372 = 1.19f;
	}
	float FovAngle = camfov + test1337 / (zoom / test13372);

	float ScreenCenterX = Width / 2.0f;
	float ScreenCenterY = Height / 2.0f;

	Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
	Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

	return Screenlocation;
}

Vector3 Camera(unsigned __int64 RootComponent)
{
	Vector3 Camera;

	auto pitch = read<uintptr_t>(DriverHandle, processID, Localplayer + 0xb0);
	Camera.x = read<float>(DriverHandle, processID, RootComponent + 0x12C);
	Camera.y = read<float>(DriverHandle, processID, pitch + 0x678);

	float test = asin(Camera.y);
	float degrees = test * (180.0 / M_PI);

	Camera.y = degrees;

	if (Camera.x < 0)
		Camera.x = 360 + Camera.x;

	return Camera;
}

float AimSpeed = 1.5f;
float AimSmooth = 1.0f;

bool GetAimKey()
{
	return (GetAsyncKeyState(aimkey));
}

void Aimbot(float x, float y)
{
	float ScreenCenterX = (Width / 2);
	float ScreenCenterY = (Height / 2);

	float TargetX = 0;
	float TargetY = 0;

	if (x != 0)
	{
		if (x > ScreenCenterX)
		{
			TargetX = -(ScreenCenterX - x);
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
		}

		if (x < ScreenCenterX)
		{
			TargetX = x - ScreenCenterX;
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX < 0) TargetX = 0;
		}
	}

	if (y != 0)
	{
		if (y > ScreenCenterY)
		{
			TargetY = -(ScreenCenterY - y);
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
		}

		if (y < ScreenCenterY)
		{
			TargetY = y - ScreenCenterY;
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY < 0) TargetY = 0;
		}
	}
	if (modepos == 0)
	{
		mouse_event(MOUSEEVENTF_MOVE, floor(TargetX / AimSmooth), floor(TargetY / AimSmooth), NULL, NULL);
	}
	else if (modepos == 1)
	{
		INPUT  input;
		input.type = INPUT_MOUSE;
		input.mi.mouseData = 0;
		input.mi.time = 0;
		input.mi.dx = floor(TargetX / AimSmooth);
		input.mi.dy = floor(TargetY / AimSmooth);
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		input.mi.dwExtraInfo = 0;
		SendInput(1, &input, sizeof(input));
	}
	return;
}

double GetCrossDistance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

bool GetClosestPlayerToCrossHair(Vector3 Pos, float& max, float aimfov, DWORD_PTR entity)
{
	if (!GetAimKey() || !isaimbotting)
	{
		if (entity)
		{
			float Dist = GetCrossDistance(Pos.x, Pos.y, ScreenCenterX, ScreenCenterY);
			if (Dist < max)
			{
				max = Dist;
				entityx = entity;
				AimFOV = aimfov;
				//return true;
			}
		}
	}
	return false;
}

void AimAt(DWORD_PTR entity, Vector3 Localcam)
{
	uint64_t currentactormesh = read<uint64_t>(DriverHandle, processID, entity + 0x278);

	auto rootHead = GetBoneWithRotation(currentactormesh, hitbox);
	Vector3 rootHeadOut = ProjectWorldToScreen(rootHead, Vector3(Localcam.y, Localcam.x, Localcam.z));
	if (rootHeadOut.y != 0 || rootHeadOut.y != 0)
	{

		if ((GetCrossDistance(rootHeadOut.x, rootHeadOut.y, ScreenCenterX, ScreenCenterY) <= AimFOV * 8) || isaimbotting)
		{
			Aimbot(rootHeadOut.x, rootHeadOut.y);
		}
	}
}

void Aimbot(Vector3 Localcam)
{
	if (entityx != 0)
	{

		if (GetAimKey())
		{
			AimAt(entityx, Localcam);
		}
		else
		{
			isaimbotting = false;
		}
	}
}

void AIms(DWORD_PTR entity, Vector3 Localcam)
{
	float max = 100.f;

	uint64_t currentactormesh = read<uint64_t>(DriverHandle, processID, entity + 0x278);

	Vector3 rootHead = GetBoneWithRotation(currentactormesh, hitbox);
	Vector3 rootHeadOut = ProjectWorldToScreen(rootHead, Vector3(Localcam.y, Localcam.x, Localcam.z));

	if (GetClosestPlayerToCrossHair(rootHeadOut, max, AimFOV, entity))
		entityx = entity;
}

std::string string_To_UTF8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}


void DrawNewText(int x, int y, RGBA* color, const char* str)
{
	ImFont a;
	std::string utf_8_1 = std::string(str);
	std::string utf_8_2 = string_To_UTF8(utf_8_1);
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), utf_8_2.c_str());
}

void DrawCircle(int x, int y, int radius, RGBA* color, int segments)
{
	ImGui::GetOverlayDrawList()->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), segments);
}

void DrawLine(int x1, int y1, int x2, int y2, RGBA* color, int thickness)
{
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), thickness);
}

void DrawESP() {
	HWND test = FindWindowA(0, _xor_("Fortnite  ").c_str());

	if (test == NULL)
	{
		ExitProcess(0);
	}

	if (hitboxpos == 0)
	{
		hitbox = BONE_HEAD;//head
	}
	else if (hitboxpos == 1)
	{
		hitbox = BONE_NECK;//neck
	}
	else if (hitboxpos == 2)
	{
		hitbox = BONE_TORSO;//body
	}
	else if (hitboxpos == 3)
	{
		hitbox = 0;//root
	}
	else if (hitboxpos == 4)
	{
		hitbox = BONE_PELVIS_1;//pelvis
	}

	if (aimkeypos == 0)
	{
		aimkey = 0x01;//left mouse button
	}
	else if (aimkeypos == 1)
	{
		aimkey = 0x02;//right mouse button
	}
	else if (aimkeypos == 2)
	{
		aimkey = 0x04;//middle mouse button
	}
	else if (aimkeypos == 3)
	{
		aimkey = 0x05;//x1 mouse button
	}
	else if (aimkeypos == 4)
	{
		aimkey = 0x06;//x2 mouse button
	}
	else if (aimkeypos == 5)
	{
		aimkey = 0x03;//control break processing
	}
	else if (aimkeypos == 6)
	{
		aimkey = 0x08;//backspace
	}
	else if (aimkeypos == 7)
	{
		aimkey = 0x09;//tab
	}
	else if (aimkeypos == 8)
	{
		aimkey = 0x0c;//clear
	}
	else if (aimkeypos == 9)
	{
		aimkey == 0x0D;//enter
	}
	else if (aimkeypos == 10)
	{
		aimkey = 0x10;//shift
	}
	else if (aimkeypos == 11)
	{
		aimkey = 0x11;//ctrl
	}
	else if (aimkeypos == 12)
	{
		aimkey == 0x12;//alt
	}
	else if (aimkeypos == 13)
	{
		aimkey == 0x14;//caps lock
	}
	else if (aimkeypos == 14)
	{
		aimkey == 0x1B;//esc
	}
	else if (aimkeypos == 15)
	{
		aimkey == 0x20;//space
	}
	else if (aimkeypos == 16)
	{
		aimkey == 0x30;//0
	}
	else if (aimkeypos == 17)
	{
		aimkey == 0x31;//1
	}
	else if (aimkeypos == 18)
	{
		aimkey == 0x32;//2
	}
	else if (aimkeypos == 19)
	{
		aimkey == 0x33;//3
	}
	else if (aimkeypos == 20)
	{
		aimkey == 0x34;//4
	}
	else if (aimkeypos == 21)
	{
		aimkey == 0x35;//5
	}
	else if (aimkeypos == 22)
	{
		aimkey == 0x36;//6
	}
	else if (aimkeypos == 23)
	{
		aimkey == 0x37;//7
	}
	else if (aimkeypos == 24)
	{
		aimkey == 0x38;//8
	}
	else if (aimkeypos == 25)
	{
		aimkey == 0x39;//9
	}
	else if (aimkeypos == 26)
	{
		aimkey == 0x41;//a
	}
	else if (aimkeypos == 27)
	{
		aimkey == 0x42;//b
	}
	else if (aimkeypos == 28)
	{
		aimkey == 0x43;//c
	}
	else if (aimkeypos == 29)
	{
		aimkey == 0x44;//d
	}
	else if (aimkeypos == 30)
	{
		aimkey == 0x45;//e
	}
	else if (aimkeypos == 31)
	{
		aimkey == 0x46;//f
	}
	else if (aimkeypos == 32)
	{
		aimkey == 0x47;//g
	}
	else if (aimkeypos == 33)
	{
		aimkey == 0x48;//h
	}
	else if (aimkeypos == 34)
	{
		aimkey == 0x49;//i
	}
	else if (aimkeypos == 35)
	{
		aimkey == 0x4A;//j
	}
	else if (aimkeypos == 36)
	{
		aimkey == 0x4B;//k
	}
	else if (aimkeypos == 37)
	{
		aimkey == 0x4C;//L
	}
	else if (aimkeypos == 38)
	{
		aimkey == 0x4D;//m
	}
	else if (aimkeypos == 39)
	{
		aimkey == 0x4E;//n
	}
	else if (aimkeypos == 40)
	{
		aimkey == 0x4F;//o
	}
	else if (aimkeypos == 41)
	{
		aimkey == 0x50;//p
	}
	else if (aimkeypos == 42)
	{
		aimkey == 0x51;//q
	}
	else if (aimkeypos == 43)
	{
		aimkey == 0x52;//r
	}
	else if (aimkeypos == 44)
	{
		aimkey == 0x53;//s
	}
	else if (aimkeypos == 45)
	{
		aimkey == 0x54;//t
	}
	else if (aimkeypos == 46)
	{
		aimkey == 0x55;//u
	}
	else if (aimkeypos == 47)
	{
		aimkey == 0x56;//v
	}
	else if (aimkeypos == 48)
	{
		aimkey == 0x57;//w
	}
	else if (aimkeypos == 49)
	{
		aimkey == 0x58;//x
	}
	else if (aimkeypos == 50)
	{
		aimkey == 0x59;//y
	}
	else if (aimkeypos == 51)
	{
		aimkey == 0x5A;//z
	}
	else if (aimkeypos == 52)
	{
		aimkey == 0x60;//numpad 0
	}
	else if (aimkeypos == 53)
	{
		aimkey == 0x61;//numpad 1
	}
	else if (aimkeypos == 54)
	{
		aimkey == 0x62;//numpad 2
	}
	else if (aimkeypos == 55)
	{
		aimkey == 0x63;//numpad 3
	}
	else if (aimkeypos == 56)
	{
		aimkey == 0x64;//numpad 4
	}
	else if (aimkeypos == 57)
	{
		aimkey == 0x65;//numpad 5
	}
	else if (aimkeypos == 58)
	{
		aimkey == 0x66;//numpad 6
	}
	else if (aimkeypos == 59)
	{
		aimkey == 0x67;//numpad 7
	}
	else if (aimkeypos == 60)
	{
		aimkey == 0x68;//numpad 8
	}
	else if (aimkeypos == 61)
	{
		aimkey == 0x69;//numpad 9
	}
	else if (aimkeypos == 62)
	{
		aimkey == 0x6A;//multiply
	}

	float radiusx = AimFOV * (ScreenCenterX / 100);
	float radiusy = AimFOV * (ScreenCenterY / 100);

	float calcradius = (radiusx + radiusy) / 2;
	if (drawfov)
	{
		ImGui::GetOverlayDrawList()->AddCircle(ImVec2(ScreenCenterX, ScreenCenterY), calcradius, 0xFFFFFFFF, 25);
	}

	Uworld = read<DWORD_PTR>(DriverHandle, processID, base_address + OFFSET_UWORLD);
	DWORD_PTR Gameinstance = read<DWORD_PTR>(DriverHandle, processID, Uworld + 0x170);

	if (Gameinstance == (DWORD_PTR)nullptr)
		return;

	DWORD_PTR LocalPlayers = read<DWORD_PTR>(DriverHandle, processID, Gameinstance + 0x38);

	if (LocalPlayers == (DWORD_PTR)nullptr)
		return;

	Localplayer = read<DWORD_PTR>(DriverHandle, processID, LocalPlayers);

	if (Localplayer == (DWORD_PTR)nullptr)
		return;

	PlayerController = read<DWORD_PTR>(DriverHandle, processID, Localplayer + 0x30);

	if (PlayerController == (DWORD_PTR)nullptr)
		return;

	LocalPawn = read<DWORD_PTR>(DriverHandle, processID, PlayerController + 0x298);

	if (LocalPawn == (DWORD_PTR)nullptr)
		return;

	Rootcomp = read<DWORD_PTR>(DriverHandle, processID, LocalPawn + 0x130);

	if (Rootcomp == (DWORD_PTR)nullptr)
		return;

	if (LocalPawn != 0) {
		localplayerID = read<int>(DriverHandle, processID, LocalPawn + 0x18);
	}

	Ulevel = read<DWORD_PTR>(DriverHandle, processID, Uworld + 0x30);

	if (Ulevel == (DWORD_PTR)nullptr)
		return;

	DWORD ActorCount = read<DWORD>(DriverHandle, processID, Ulevel + 0xA0);

	DWORD_PTR AActors = read<DWORD_PTR>(DriverHandle, processID, Ulevel + 0x98);

	DWORD64 PlayerState = read<DWORD64>(DriverHandle, processID, LocalPawn + 0x238);

	if (AActors == (DWORD_PTR)nullptr)
		return;

	Vector3 Localcam = Camera(Rootcomp);

	int revise = 0;
	int radius = 0;
	float LowestDistance = FLT_MAX;
	DWORD64 toApuntar;

	if (fovchanger)
	{
		if (camfov)
		{
			writefloat(Localplayer + 0x420, camfov);
		}
	}

	for (int i = 0; i < ActorCount; i++)
	{
		if (ActorCount > 1)
		{
			uint64_t CurrentActor = read<uint64_t>(DriverHandle, processID, AActors + i * 0x8);

			if (CurrentActor == (uint64_t)nullptr || CurrentActor == -1 || CurrentActor == NULL)
				continue;

			int curactorid = read<int>(DriverHandle, processID, CurrentActor + 0x18);

			if (curactorid == localplayerID || curactorid == BOT_ACTORID)
			{
				uint64_t CurrentActorRootComponent = read<uint64_t>(DriverHandle, processID, CurrentActor + 0x130);

				if (CurrentActorRootComponent == (uint64_t)nullptr || CurrentActorRootComponent == -1 || CurrentActorRootComponent == NULL)
					continue;

				uint64_t currentactormesh = read<uint64_t>(DriverHandle, processID, CurrentActor + 0x278);

				if (currentactormesh == (uint64_t)nullptr || currentactormesh == -1 || currentactormesh == NULL)
					continue;

				Vector3 BoxHeadPos = GetBoneWithRotation(currentactormesh, 66);
				Vector3 Headpos = GetBoneWithRotation(currentactormesh, 66);
				Vector3 rootOut = GetBoneWithRotation(currentactormesh, 0);
				Vector3 Localcam = Camera(Rootcomp);
				Vector3 Localpos = read<Vector3>(DriverHandle, processID, Rootcomp + 0x11C);

				int MyTeamId = read<int>(DriverHandle, processID, PlayerState + 0xE38);
				DWORD64 otherPlayerState = read<uint64_t>(DriverHandle, processID, CurrentActor + 0x238);
				int ActorTeamId = read<int>(DriverHandle, processID, otherPlayerState + 0xE38);

				Vector3 vHeadBone = GetBoneWithRotation(currentactormesh, 96);
				Vector3 vHip = GetBoneWithRotation(currentactormesh, 2);
				Vector3 vNeck = GetBoneWithRotation(currentactormesh, 65);
				Vector3 vUpperArmLeft = GetBoneWithRotation(currentactormesh, 34);
				Vector3 vUpperArmRight = GetBoneWithRotation(currentactormesh, 91);
				Vector3 vLeftHand = GetBoneWithRotation(currentactormesh, 35);
				Vector3 vRightHand = GetBoneWithRotation(currentactormesh, 63);
				Vector3 vLeftHand1 = GetBoneWithRotation(currentactormesh, 33);
				Vector3 vRightHand1 = GetBoneWithRotation(currentactormesh, 60);
				Vector3 vRightThigh = GetBoneWithRotation(currentactormesh, 74);
				Vector3 vLeftThigh = GetBoneWithRotation(currentactormesh, 67);
				Vector3 vRightCalf = GetBoneWithRotation(currentactormesh, 75);
				Vector3 vLeftCalf = GetBoneWithRotation(currentactormesh, 68);
				Vector3 vLeftFoot = GetBoneWithRotation(currentactormesh, 69);
				Vector3 vRightFoot = GetBoneWithRotation(currentactormesh, 76);

				Vector3 vHeadBoneOut = ProjectWorldToScreen(vHeadBone, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vHipOut = ProjectWorldToScreen(vHip, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vNeckOut = ProjectWorldToScreen(vNeck, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vUpperArmLeftOut = ProjectWorldToScreen(vUpperArmLeft, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vUpperArmRightOut = ProjectWorldToScreen(vUpperArmRight, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vLeftHandOut = ProjectWorldToScreen(vLeftHand, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vRightHandOut = ProjectWorldToScreen(vRightHand, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vLeftHandOut1 = ProjectWorldToScreen(vLeftHand1, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vRightHandOut1 = ProjectWorldToScreen(vRightHand1, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vRightThighOut = ProjectWorldToScreen(vRightThigh, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vLeftThighOut = ProjectWorldToScreen(vLeftThigh, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vRightCalfOut = ProjectWorldToScreen(vRightCalf, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vLeftCalfOut = ProjectWorldToScreen(vLeftCalf, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vLeftFootOut = ProjectWorldToScreen(vLeftFoot, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vRightFootOut = ProjectWorldToScreen(vRightFoot, Vector3(Localcam.y, Localcam.x, Localcam.z));

				Vector3 bone0 = GetBoneWithRotation(currentactormesh, 0);
				Vector3 bottom = ProjectWorldToScreen(bone0, Vector3(Localcam.y, Localcam.x, Localcam.z));

				float distance = Localpos.Distance(Headpos) / 100.f;

				if (distance < 1.5f)
					continue;

				if (MyTeamId != ActorTeamId)
				{
					int ScreenX = GetSystemMetrics(SM_CXSCREEN) / 2;
					int ScreenY = GetSystemMetrics(SM_CYSCREEN);

					int ScreenXx = GetSystemMetrics(SM_CXSCREEN) / 2;
					int ScreenYy = GetSystemMetrics(SM_CYSCREEN) / 20000;
					int ScreenYyy = GetSystemMetrics(SM_CYSCREEN) / 2;

					Vector3 HeadposW2s = ProjectWorldToScreen(Headpos, Vector3(Localcam.y, Localcam.x, Localcam.z));
					Vector3 Out = ProjectWorldToScreen(rootOut, Vector3(Localcam.y, Localcam.x, Localcam.z));
					Vector3 BoxHead = ProjectWorldToScreen(BoxHeadPos, Vector3(Localcam.y, Localcam.x, Localcam.z));

					if (line)
					{
						if (visuals)
						{
							if (distance < VisDist)
							{
								if (linemodepos == 0)
								{
									DrawLine(ScreenX, ScreenY, bottom.x, bottom.y, &Col.NiggaGreen, 1.5);//bottom
								}
								else if (linemodepos == 1)
								{
									DrawLine(ScreenXx, ScreenYy, HeadposW2s.x, HeadposW2s.y, &Col.NiggaGreen, 1.5);//top
								}
								else if (linemodepos == 2)
								{
									DrawLine(ScreenX, ScreenYyy, vHipOut.x, vHipOut.y, &Col.NiggaGreen, 1.5);//center
								}
							}
						}
					}
					if (box)
					{
						float boxsize = (float)(Out.y - BoxHead.y);
						float boxwidth = boxsize / 2;
						float dwpleftx = (float)Out.x - boxwidth / 2;
						float dwplefty = (float)Out.y;

						if (visuals)
						{
							if (distance < VisDist)
							{
								if (boxmodepos == 0)
								{
									ImGui::GetOverlayDrawList()->AddRect(ImVec2(dwpleftx, dwplefty), ImVec2(BoxHead.x + boxwidth, BoxHead.y), IM_COL32(128, 224, 0, 200));
								}
								else if (boxmodepos == 1)
								{
									ImGui::GetOverlayDrawList()->AddRect(ImVec2(dwpleftx, dwplefty), ImVec2(BoxHead.x + boxwidth, BoxHead.y), IM_COL32(128, 224, 0, 200));
									ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(dwpleftx, dwplefty), ImVec2(BoxHead.x + boxwidth, BoxHead.y), IM_COL32(0, 0, 0, 160));
								}
							}
						}
					}
					if (playeresp)
					{
						if (visuals)
						{
							if (distance < VisDist)
							{
								char cName[64];
								if (ActorTeamId == -1)
								{
									sprintf_s(cName, _xor_("Bot [%.fm]").c_str(), distance);
								}
								else
								{
									sprintf_s(cName, _xor_("Player [%.fm]").c_str(), distance);
								}
								revise = strlen(cName) * 7 + 28;
								int textX = (int)(Out.x - (revise / 2) + (Rect.w / 2));

								DrawNewText(textX, Out.y + Out.z, &Col.white_, cName);
							}
						}
					}
					if (skeleton)
					{
						if (visuals)
						{
							if (distance < VisDist)
							{
								if (distance > 0)
									radius = 50 / distance;

								DrawCircle(HeadposW2s.x, HeadposW2s.y - radius / 2, radius, &Col.NiggaGreen, radius * 2);

								DrawLine(vHipOut.x, vHipOut.y, vNeckOut.x, vNeckOut.y, &Col.NiggaGreen, 1.5);

								DrawLine(vUpperArmLeftOut.x, vUpperArmLeftOut.y, vNeckOut.x, vNeckOut.y, &Col.NiggaGreen, 1.5);
								DrawLine(vUpperArmRightOut.x, vUpperArmRightOut.y, vNeckOut.x, vNeckOut.y, &Col.NiggaGreen, 1.5);

								DrawLine(vLeftHandOut.x, vLeftHandOut.y, vUpperArmLeftOut.x, vUpperArmLeftOut.y, &Col.NiggaGreen, 1.5);
								DrawLine(vRightHandOut.x, vRightHandOut.y, vUpperArmRightOut.x, vUpperArmRightOut.y, &Col.NiggaGreen, 1.5);

								DrawLine(vLeftHandOut.x, vLeftHandOut.y, vLeftHandOut1.x, vLeftHandOut1.y, &Col.NiggaGreen, 1.5);
								DrawLine(vRightHandOut.x, vRightHandOut.y, vRightHandOut1.x, vRightHandOut1.y, &Col.NiggaGreen, 1.5);

								DrawLine(vLeftThighOut.x, vLeftThighOut.y, vHipOut.x, vHipOut.y, &Col.NiggaGreen, 1.5);
								DrawLine(vRightThighOut.x, vRightThighOut.y, vHipOut.x, vHipOut.y, &Col.NiggaGreen, 1.5);

								DrawLine(vLeftCalfOut.x, vLeftCalfOut.y, vLeftThighOut.x, vLeftThighOut.y, &Col.NiggaGreen, 1.5);
								DrawLine(vRightCalfOut.x, vRightCalfOut.y, vRightThighOut.x, vRightThighOut.y, &Col.NiggaGreen, 1.5);

								DrawLine(vLeftFootOut.x, vLeftFootOut.y, vLeftCalfOut.x, vLeftCalfOut.y, &Col.NiggaGreen, 1.5);
								DrawLine(vRightFootOut.x, vRightFootOut.y, vRightCalfOut.x, vRightCalfOut.y, &Col.NiggaGreen, 1.5);
							}
						}
					}
					if (aimbot)
					{
						if (distance > 2.f)
						{
							if (distance > 2.f && distance < AimDist)
							{
								AIms(CurrentActor, Localcam);
							}
						}
					}
				}
			}
		}
	}
	Aimbot(Localcam);
}

HRESULT DirectXInit(HWND hWnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(3);

	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hWnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = Width;
	p_Params.BackBufferHeight = Height;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device)))
	{
		p_Object->Release();
		exit(4);
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(p_Device);

	ImGui::GetStyle().FrameRounding = 1.5f;
	ImGui::GetStyle().GrabRounding = 1.5f;
	ImGui::GetStyle().WindowRounding = 1.0f;
	ImGui::GetStyle().WindowBorderSize = 2.5f;
	ImGui::GetStyle().ChildBorderSize = 2.5f;
	ImGui::GetStyle().PopupBorderSize = 2.5f;
	ImGui::GetStyle().WindowTitleAlign.x = 0.50f;

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 0.80f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.5f, 0.1f, 0.1f, 1.0f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.75f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.10f, 0.10f, 0.10f, 0.77f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.10f, 0.10f, 0.10f, 0.80f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.5f, 0.1f, 0.1f, 0.9f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.5f, 0.1f, 0.1f, 0.9f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.5f, 0.1f, 0.1f, 0.9f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.5f, 0.1f, 0.1f, 1.0f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.4f, 0.1f, 0.1f, 0.9f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.5f, 0.1f, 0.1f, 0.9f);
	colors[ImGuiCol_Button] = ImVec4(0.5f, 0.1f, 0.1f, 0.85f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.4f, 0.1f, 0.1f, 0.85f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.4f, 0.1f, 0.1f, 0.85f);
	colors[ImGuiCol_Header] = ImVec4(0.5f, 0.1f, 0.1f, 0.85f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.4f, 0.1f, 0.1f, 0.85f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.4f, 0.1f, 0.1f, 0.85f);
	colors[ImGuiCol_Separator] = ImVec4(0.5f, 0.1f, 0.1f, 0.85f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.4f, 0.1f, 0.1f, 0.85f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.4f, 0.1f, 0.1f, 0.85f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.5f, 0.1f, 0.1f, 0.85f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.4f, 0.1f, 0.1f, 0.85f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.4f, 0.1f, 0.1f, 0.85f);
	colors[ImGuiCol_Tab] = ImVec4(0.5f, 0.1f, 0.1f, 0.85f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.4f, 0.1f, 0.1f, 0.85f);
	colors[ImGuiCol_TabActive] = ImVec4(0.4f, 0.1f, 0.1f, 0.85f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	io.Fonts->AddFontFromFileTTF(_xor_("C:\\Windows\\Fonts\\Arial.ttf").c_str(), 14.0f, nullptr, io.Fonts->GetGlyphRangesDefault());

	p_Object->Release();
	return S_OK;
}

void SetupWindow()
{
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SetWindowToTarget, 0, 0, 0);

	WNDCLASSEX wClass =
	{
		sizeof(WNDCLASSEX),
		0,
		WinProc,
		0,
		0,
		nullptr,
		LoadIcon(nullptr, IDI_APPLICATION),
		LoadCursor(nullptr, IDC_ARROW),
		nullptr,
		nullptr,
		M_Name,
		LoadIcon(nullptr, IDI_APPLICATION)
	};

	if (!RegisterClassEx(&wClass))
		exit(1);

	if (hwnd)
	{
		GetClientRect(hwnd, &GameRect);
		POINT xy;
		ClientToScreen(hwnd, &xy);
		GameRect.left = xy.x;
		GameRect.top = xy.y;

		Width = GameRect.right;
		Height = GameRect.bottom;
	}
	else
		exit(2);

	MyWnd = CreateWindowEx(NULL, M_Name, M_Name, WS_POPUP | WS_VISIBLE, GameRect.left, GameRect.top, Width, Height, NULL, NULL, 0, NULL);
	DwmExtendFrameIntoClientArea(MyWnd, &Margin);
	SetWindowLong(MyWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);

	ShowWindow(MyWnd, SW_SHOW);
	UpdateWindow(MyWnd);
}

void render() {
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (ShowMenu)
	{
		if (ImGui::Begin(_xor_("Winkieswant"/*"Fancycheats.pw | Built 23 December, 2019"*/).c_str(), 0, ImVec2(572, 300), -1.f, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse)) {
			static short tab = 0;

			if (ImGui::Button(_xor_("Visuals").c_str(), ImVec2(180, 25)))
				tab = 0;

			ImGui::SameLine();

			if (ImGui::Button(_xor_("Aimbot").c_str(), ImVec2(180, 25)))
				tab = 1;

			ImGui::SameLine();

			if (ImGui::Button(_xor_("Misc").c_str(), ImVec2(180, 25)))
				tab = 2;

			if (tab == 0) {
				ImGui::Checkbox(_xor_("Enable Visuals").c_str(), &visuals);
				ImGui::Checkbox(_xor_("Player ESP").c_str(), &playeresp);
				ImGui::Checkbox(_xor_("Skeleton ESP").c_str(), &skeleton);
				ImGui::Checkbox(_xor_("Snapline ESP").c_str(), &line);
				ImGui::Checkbox(_xor_("Box Esp").c_str(), &box);
				ImGui::Combo(_xor_("Snapline ESP Mode").c_str(), &linemodepos, linemodes, sizeof(linemodes) / sizeof(*linemodes));
				ImGui::Combo(_xor_("Box ESP Mode").c_str(), &boxmodepos, boxmodes, sizeof(boxmodes) / sizeof(*boxmodes));
				ImGui::SliderInt(_xor_("Max Visuals Distance").c_str(), &VisDist, 50, 500);
			}

			if (tab == 1) {
				ImGui::Checkbox(_xor_("Enable Aimbot").c_str(), &aimbot);
				ImGui::Checkbox(_xor_("Draw Aimbot FOV").c_str(), &drawfov);
				ImGui::Combo(_xor_("Aimbot Mode").c_str(), &modepos, modes, sizeof(modes) / sizeof(*modes));
				ImGui::Combo(_xor_("Aimbot Hitbox").c_str(), &hitboxpos, hitboxes, sizeof(hitboxes) / sizeof(*hitboxes));
				ImGui::Combo(_xor_("Aimbot Key").c_str(), &aimkeypos, aimkeys, sizeof(aimkeys) / sizeof(*aimkeys));
				ImGui::SliderFloat(_xor_("Aimbot FOV").c_str(), &AimFOV, 5.f, 150.f);
				ImGui::SliderFloat(_xor_("Aimbot Smooth").c_str(), &AimSmooth, 1.f, 50.f);
				ImGui::SliderInt(_xor_("Max Aimbot Distance").c_str(), &AimDist, 50, 500);
			}
			if (tab == 2)
			{
				ImGui::Checkbox(_xor_("Enable FOV Changer").c_str(), &fovchanger);
				if (fovchanger)
				{
					if (ImGui::Button(_xor_("<").c_str()))
					{
						camfov -= 10;
						if (camfov == 40)
						{
							camfov += 10;
						}
					}
					ImGui::SameLine();
					ImGui::Text(_xor_("%i").c_str(), camfov);
					ImGui::SameLine();
					if (ImGui::Button(_xor_(">").c_str()))
					{
						camfov += 10;
						if (camfov == 130)
						{
							camfov -= 10;
						}
					}
				}
			}
		}
		ImGui::End();
	}

	DrawESP();

	ImGui::EndFrame();
	p_Device->SetRenderState(D3DRS_ZENABLE, false);
	p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	p_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	p_Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	if (p_Device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		p_Device->EndScene();
	}
	HRESULT result = p_Device->Present(NULL, NULL, NULL, NULL);

	if (result == D3DERR_DEVICELOST && p_Device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		p_Device->Reset(&p_Params);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

WPARAM MainLoop()
{
	static RECT old_rc;
	ZeroMemory(&Message, sizeof(MSG));

	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, MyWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		HWND hwnd_active = GetForegroundWindow();
		if (GetAsyncKeyState(0x23) & 1)
			exit(8);

		if (hwnd_active == hwnd) {
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(MyWnd, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		RECT rc;
		POINT xy;

		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(hwnd, &rc);
		ClientToScreen(hwnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = hwnd;
		io.DeltaTime = 1.0f / 60.0f;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;

		if (GetAsyncKeyState(VK_LBUTTON)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
			io.MouseDown[0] = false;

		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
		{

			old_rc = rc;

			Width = rc.right;
			Height = rc.bottom;

			p_Params.BackBufferWidth = Width;
			p_Params.BackBufferHeight = Height;
			SetWindowPos(MyWnd, (HWND)0, xy.x, xy.y, Width, Height, SWP_NOREDRAW);
			p_Device->Reset(&p_Params);
		}
		render();
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanuoD3D();
	DestroyWindow(MyWnd);

	return Message.wParam;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
		return true;

	switch (Message)
	{
	case WM_DESTROY:
		CleanuoD3D();
		PostQuitMessage(0);
		exit(4);
		break;
	case WM_SIZE:
		if (p_Device != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			p_Params.BackBufferWidth = LOWORD(lParam);
			p_Params.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = p_Device->Reset(&p_Params);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

void CleanuoD3D()
{
	if (p_Device != NULL)
	{
		p_Device->EndScene();
		p_Device->Release();
	}
	if (p_Object != NULL)
	{
		p_Object->Release();
	}
}

int isTopwin()
{
	HWND hWnd = GetForegroundWindow();

	if (hWnd == hwnd)
		return TopWindowGame;

	if (hWnd == MyWnd)
		return TopWindowMvoe;

	return 0;
}

void SetWindowToTarget()
{
	while (true)
	{
		if (hwnd)
		{
			ZeroMemory(&GameRect, sizeof(GameRect));
			GetWindowRect(hwnd, &GameRect);
			Width = GameRect.right - GameRect.left;
			Height = GameRect.bottom - GameRect.top;
			DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);

			if (dwStyle & WS_BORDER)
			{
				GameRect.top += 32;
				Height -= 39;
			}
			ScreenCenterX = Width / 2;
			ScreenCenterY = Height / 2;
			MoveWindow(MyWnd, GameRect.left, GameRect.top, Width, Height, true);
		}
		else
		{
			MessageBox(0, _xor_(L"esa").c_str(), _xor_(L"hyv").c_str(), MB_OK | MB_ICONERROR);
			exit(0);
		}
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//AllocConsole();
		//freopen("CONOUT$", "w", stdout);
		CreateThread(0, 0, &init, 0, 0, 0);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

