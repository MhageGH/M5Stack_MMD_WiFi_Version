#pragma once

#define DIRECTINPUT_VERSION 0x0800
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_DELETE(p) { if(p) { delete (p); (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#include <d3dx9.h>
#include <dinput.h>
#include <mmsystem.h>
#include <Dsound.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "user32.lib")
using namespace std;
