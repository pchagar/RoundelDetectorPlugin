#pragma once

extern "C" {
	__declspec(dllexport) int detectLogo(unsigned char*, int, int, int*);
	__declspec(dllexport) int loadCascade(const char*);
	__declspec(dllexport) int getExePath(unsigned char*);
}
