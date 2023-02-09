#pragma once

using std::string;

VOID DisableVMProtect();
bool IsLittleEndian();
HWND FindMainWindowByPID(DWORD pid);
string ToString(Il2CppString* str, UINT codePage = CP_ACP);

#define cstring_new(str) il2cpp_string_new(str)
#define string_new(str) cstring_new((str).c_str())

#define ErrorDialogT(title, msg) MessageBox(unityWnd, msg, title, MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
#define ErrorDialog(msg) ErrorDialogT("YaeAchievement", msg)
#define Win32ErrorDialog(code) ErrorDialogT("YaeAchievement", ("CRITICAL ERROR!\nError code: " + std::to_string(GetLastError()) + "-"#code"\n\nPlease take the screenshot and contact developer by GitHub Issue to solve this problem\nNOT MIHOYO/COGNOSPHERE CUSTOMER SERVICE!").c_str())

template<class T>
static T ReadMapped(void* data, int offset, bool littleEndian = false) {
	char* cData = (char*)data;
	T result = {};
	if (IsLittleEndian() != littleEndian) {
		for (int i = 0; i < sizeof(T); i++)
			((char*)&result)[i] = cData[offset + sizeof(T) - i - 1];
		return result;
	}
	memcpy(&result, cData + offset, sizeof(result));
	return result;
}
