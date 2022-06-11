#pragma once

HWND FindMainWindowByPID(DWORD pid);

#define ErrorDialogT(title, msg) MessageBox(unityWnd, msg, title, MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
#define ErrorDialog(msg) ErrorDialogT("YaeAchievement", msg)
