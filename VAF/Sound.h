#include <iostream>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <sstream>

#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

#define _CRT_SECURE_NO_WARNINGS
static int *sesptr = new int(0);
static bool *muteptr = new bool(false);
#define GET_MUTE 0
#define GET_VOLUME 1
#define GET_VOLUMEMUTE 2

#define SET_MUTE_AUTO 4
#define SET_VOLUME 5
#define SET_MUTE_WANT 6
//static int* parametertoSETVOLUME = new int(1);//change that's before than setvolume
namespace SOUND
{

	inline void Center(byte type)
	{
		try
		{
			IMMDeviceEnumerator *enumerator = NULL;
			IMMDevice *device = NULL;
			IAudioEndpointVolume *endpointVolume = NULL;
			CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (void**)&enumerator);
			enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
			device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);

			if (type == GET_VOLUME)//1
			{
				float currentVolume = 0;
				endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
				int ses = (currentVolume + 0.00000039) * 100;//0.259999961
				endpointVolume->Release();
				device->Release();
				enumerator->Release();
				*sesptr = ses;				
				return; 
			}
			if (type == GET_VOLUMEMUTE)//2
			{
				float currentVolume = 0;
				endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
				int ses = (currentVolume + 0.00000039) * 100;//0.259999961
				BOOL mute;
				endpointVolume->GetMute(&mute);
				endpointVolume->Release();
				device->Release();
				enumerator->Release();
				*sesptr = ses;
				*muteptr = mute;
				return;
			}
			if (type == SET_MUTE_AUTO)//4
			{
				BOOL mute;
				endpointVolume->GetMute(&mute);
				endpointVolume->SetMute(!mute, 0);
				endpointVolume->Release();
				device->Release();
				enumerator->Release();
				return;
			}
			if (type == GET_MUTE)//0
			{
				BOOL mute;
				endpointVolume->GetMute(&mute);
				endpointVolume->Release();
				device->Release();
				enumerator->Release();
				*muteptr = mute;
				return;
			}
			if (type == SET_VOLUME)//5
			{
				float newVolume = static_cast<float>(*sesptr) / 100;//1 ile 0 arasinda tutmak icin 100'e bolduk
				endpointVolume->SetMasterVolumeLevelScalar(newVolume, NULL);
				endpointVolume->SetMute(false, 0);//eger volume yapilacaksa otomatik muteden cikarilmali
				endpointVolume->Release();
				device->Release();
				enumerator->Release();
				return;
			}
			if (type == SET_MUTE_WANT)//6
			{
				endpointVolume->SetMute(*muteptr, 0);
				endpointVolume->Release();
				device->Release();
				enumerator->Release();
				return;
			}
		}
		catch (std::exception H){
			std::stringstream sx;
			sx << H.what();
			MessageBoxA(0, sx.str().c_str(), "CENTER!", MB_OK);
		}
	}

	HWND hwndpencere, hwndlabel, AktifWindowtoback = 0;// lblSes, hProgress;
	const HBRUSH boya_black = RGB(0, 0, 0), boya_mavi = CreateSolidBrush(RGB(0, 116, 255)), boya_gri = CreateSolidBrush(RGB(106, 106, 106)), boya_yesil = CreateSolidBrush(RGB(82, 180, 0));
	HDC  hdcpencere = 0;

	static int *ekrandabekletileceksure = new int(3);//default olarak 3
	static bool *formgozukuyor = new bool(true);
	static bool *enablemouseclicks4show = new bool(false);
	inline int GetVolume()
	{
		Center(GET_VOLUME);
		return *sesptr;
	}
	inline int GetMute()
	{
		Center(GET_MUTE);
		return *muteptr;
	}

	inline void SetVolume(int ses)//
	{
		ses = ses > 100 ? 100 : ses;
		*sesptr = ses < 0 ? 0 : ses;
		Center(SET_VOLUME);//Do not forget set *sesptr before call Center(SET_VOLUME);
	}
	inline void SetMute(bool mute)
	{
		*muteptr = mute;
		Center(SET_MUTE_WANT);
	}
	inline void ChangeMute()
	{
		Center(SET_MUTE_AUTO);
	}
	inline void GetVolumeAndMute() {
		Center(GET_VOLUMEMUTE);
	}

	char out[5];
	const int labelX = 23, labelY = 94;
	const int labelX_MUTE = 13, labelY_MUTE = 94;
	int *oncekises = new int(0);
	bool *oncekimute = new bool(false);
	inline void  SetVisual(bool muteli, int value)
	{
		RECT rcProgressbar = { 23, 20, 37, 90 }; //rcProgressbar = {top=20 bottom=90 left=23 right=37}
		::FillRect(hdcpencere, &rcProgressbar, boya_gri);//gri (gray) 
		rcProgressbar.top = (value*-0.7 + 90);//ax+b
		if (!muteli)//{  Beyaz cubuk
			::FillRect(hdcpencere, &rcProgressbar, boya_mavi);//mavi (blue)
		else
			::FillRect(hdcpencere, &rcProgressbar, boya_yesil);//yesil (green)

		if (!muteli)
		{
			sprintf(out, "%d", value);
			if (value > 9){
				SetWindowPos(hwndlabel, 0, 20, 94, 30, 17, 0);//bunlarida yap
			}
			else{
				SetWindowPos(hwndlabel, 0, 22, 94, 30, 17, 0);//bunlarida
			}
		}
		else{
			sprintf(out, "%s", "MUTE");
			SetWindowPos(hwndlabel, 0, 12, 94, 43, 17, 0);
		}
		if (value != *oncekises || muteli != *oncekimute){
			*oncekises = value;
			*oncekimute = muteli;
			SendMessage(hwndlabel, WM_SETTEXT, 0, (LPARAM)TEXT(out));
			InvalidateRect(hwndlabel, NULL, 0);
		}
	}

	void TimeDifference()
	{
		while (true)
		{
			if (*ekrandabekletileceksure < 1)//Fazla performans tutketmesin diye
				Sleep(200);

			do
			{
				Sleep(1000);
			} while (*ekrandabekletileceksure -= 1 >= 0);

			while (GetAsyncKeyState(VK_LBUTTON))//Eger hala sol click ile ses seciliyorsa		
				Sleep(100);

			*formgozukuyor = false;
			ShowWindow(hwndpencere, SW_HIDE);

			SetForegroundWindow(AktifWindowtoback);
			System::GC::Collect();
			System::GC::WaitForPendingFinalizers();			
		}
	}

	inline void eControl()
	{
		static int* volumecompare = new int(0);
		static bool* mutecompare = new bool(false);
		GetVolumeAndMute();
		*volumecompare = *sesptr;
		*mutecompare = *muteptr;
		
		static bool triggered = false;
		while (true)
		{
			GetVolumeAndMute();
		
			if ((*muteptr != *mutecompare) ||(*sesptr != *volumecompare))
				triggered = true;


			if (*enablemouseclicks4show)
				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 && GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
					if (IsWindowVisible(hwndpencere))//eger gosteriliyorsa kapanmasi icin;
					{
						ShowWindow(hwndpencere, SW_HIDE);
						*formgozukuyor = false;
						SetForegroundWindow(AktifWindowtoback);
						Sleep(300);
						continue;
					}
					triggered = true;
				}



			*mutecompare = *muteptr;//inf->getmute;
			*volumecompare = *sesptr;

			if (triggered)
			{
				triggered = false;
				*ekrandabekletileceksure = 3;
				*formgozukuyor = true;
				SetVisual(*muteptr, *sesptr);
				ShowWindow(hwndpencere, SW_SHOW);
				Sleep(10);
				SetVisual(*muteptr, *sesptr);
			}
			Sleep(100);
		}
	}
	const enum MOUSEEVENTS{
		ON_FORM,
		ON_PROGRESSBAR,
		ON_LABEL
	};
	inline bool mouse(MOUSEEVENTS whatwant);
	const RECT Progressbar = RECT{ 23, 20, 37, 91 }, Window = RECT{ 60, 120, 75, 100 };//Right = loc X, Bottom = Loc Y, Left = Size X, Top = Size Y	
	static HWND beforewindow = 0;
	static bool *mut = new bool(0); static int  *volume = new int(0);
	inline void  eRefresh()	{// 1 Kere calisacak, threadlari baslatiyor.
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)TimeDifference, 0, 0, 0);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)eControl, 0, 0, 0);
		while (true)
		{
			try
			{
				if (*formgozukuyor)
				{
					do
					{
						GetVolumeAndMute();
						SetVisual(*muteptr, *sesptr);

						if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
						{
							if (mouse(MOUSEEVENTS::ON_FORM) && !mouse(MOUSEEVENTS::ON_LABEL))//YOU CAN USE THAT :  (mouse(MOUSEEVENTS::ON_PROGRESSBAR)) 
							{
								*ekrandabekletileceksure = 3;
								do
								{
									POINT pt;
									GetCursorPos(&pt);
									int y = 100 - (1.42*(pt.y - Window.bottom - Progressbar.top));
									y = y > 100 ? 100 : y;
									y = y < 0 ? 0 : y;
									*muteptr = false;
									*sesptr = y;
									SetVolume(*sesptr);
									SetVisual(*muteptr, *sesptr);
									Sleep(5);
								} while ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) > 0);//While dongusunu kurmamin sebebi: eger ekranın en altina yada en ustune mouseyi getirdiginde volume de maximum yada minimum olması icin (X positionu yok sayiyorum)
							}
							else if (mouse(MOUSEEVENTS::ON_LABEL))//set right&left at the same time to see window
							{
								*enablemouseclicks4show = !*enablemouseclicks4show;
								Sleep(150);
							}
						}

						if (mouse(MOUSEEVENTS::ON_FORM))//bunu yapmazsam MOUSE TEKERLEK MESAJI GONDERILEMIYOR (THAT'S FOR WM_MOUSEWHEEL MESSAGE)
							SetForegroundWindow(hwndpencere);
						else
						{
							beforewindow = GetAncestor(GetForegroundWindow(), 2);
							if (beforewindow != hwndpencere)
								AktifWindowtoback = beforewindow;
							SetForegroundWindow(AktifWindowtoback);
							Sleep(200);
						}

						Sleep(20);
					} while (*formgozukuyor);
				}
				Sleep(200);
			}
			catch (std::exception exx)
			{
				std::stringstream ss;
				ss << exx.what();
				MessageBoxA(0, ss.str().c_str(), "eREFRESH", 0);

			}
		}
	}
	POINT *mousept = new POINT();
	inline bool mouse(MOUSEEVENTS whatwant)//Right = loc X, Bottom = Loc Y, Left = Size X, Top = Size Y
	{
		GetCursorPos(mousept);
		if (whatwant == MOUSEEVENTS::ON_FORM)
		{
			if ((mousept->x > Window.right && mousept->x < Window.right + Window.left - 1) &&
				(mousept->y > Window.bottom && mousept->y < Window.bottom + Window.top - 1))
				return true;
			else
				return false;
		}
		else if (whatwant == MOUSEEVENTS::ON_PROGRESSBAR)//progressbarin ustune tiklandiginda da calissin diye ozel degerler eklendi
		{
			if ((mousept->x + 5 > Window.right + Progressbar.left && mousept->x < Window.right + Window.left - Progressbar.left + 1) &&
				(mousept->y + 10 > Window.bottom + Progressbar.top && mousept->y < Window.bottom + Window.top - Progressbar.top - 7))
				return true;
			else
				return false;
		}
		else if (whatwant == MOUSEEVENTS::ON_LABEL)
		{
			if ((mousept->x + 12 > Window.right + labelX && mousept->x < Window.right + labelX + 25) &&//25 = DEGERIN SIZESI
				mousept->y > Window.bottom + labelY && mousept->y < Window.bottom + labelY + 15)
				return true;
			else
				return false;
		}
	}
};
