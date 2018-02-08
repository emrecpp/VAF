#include <iostream>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#pragma comment(lib, "Ole32.lib")


class SOUND
{
public:
	struct INFO
	{
		bool getmute;
		int value;
		INFO(int val, bool getmut)
		{
			value = val;
			getmute = getmut;
		}
	};
	inline INFO  ControlOfVolumes(int setvalue, bool setmute)
	{
		HRESULT hr = NULL;
		IAudioEndpointVolume *endpointVolume;
		float newVolume = -1;
		if (setvalue != -1) {
			newVolume = (setvalue + 0.1) / 100;//intten floata cevirmek icin ekledik
			newVolume -= 0.1 / 100;//eski hali icin cikardik
		}
		hr = NULL;

		CoInitialize(NULL);
		IMMDeviceEnumerator *deviceEnumerator = NULL;
		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
		IMMDevice *defaultDevice = NULL;

		hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
		deviceEnumerator->Release();
		deviceEnumerator = NULL;
		endpointVolume = NULL;
		hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
		defaultDevice->Release();
		defaultDevice = NULL;

		if (setmute && setvalue == -1)//mute degisterimek icinse (-1, true)
		{

			BOOL mute;
			endpointVolume->GetMute(&mute);
			endpointVolume->SetMute(!mute, 0);

			endpointVolume->Release();
			//CoUninitialize();
			return  INFO(-1, false);//NULL
		}
		else if (setvalue == -2)//Change mute 2 :what do you want to do
		{
			endpointVolume->SetMute(setmute, 0);

			endpointVolume->Release();
			//CoUninitialize();
			return  INFO(-1, false);//NULL			
		}
		else if (setvalue == -1 && !setmute)//not set (get volume) (-1,false)
		{
			float currentVolume = 0;
			hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
			int ses = (currentVolume + 0.00000039) * 100;//0.259999961
			BOOL mute;
			endpointVolume->GetMute(&mute);
			endpointVolume->Release();
			//CoUninitialize();

			return   INFO(ses, mute);
		}
		else if (setvalue > -1)//set volume
		{
			hr = endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL);
			endpointVolume->SetMute(false, 0);//eger volume yapilacaksa otomatik muteden cikarilmali
			endpointVolume->Release();
			//	CoUninitialize();
			return INFO(-1, false);//NULL
		}
	}

	class SpecialSizes{//you can change that as you like
	public:
		const char* Label_FontName = "Segoe UI Semibold";
		const int Label_Size = 18;
		const RECT Progressbar = RECT{ 23, 20, 37, 91 }, Window = RECT{ 60, 120, 75, 100 };//Right = loc X, Bottom = Loc Y, Left = Size X, Top = Size Y		
	};
	HWND hwndpencere, AktifWindowtoback = 0;// lblSes, hProgress;
	const HBRUSH boya_black = RGB(0, 0, 0), boya_mavi = CreateSolidBrush(RGB(0, 116, 255)), boya_gri = CreateSolidBrush(RGB(106, 106, 106)), boya_yesil = CreateSolidBrush(RGB(82,180,0));
	HDC device_context, memdevice;
	PAINTSTRUCT ps;
	HFONT labelFont;

	int *ekrandabekletileceksure = new int(3);//default olarak 3
	bool *formgozukuyor = new bool(true);
	bool *enablemouseclicks4show = new bool(false);
	inline int GetVolume()
	{
		INFO  inf = ControlOfVolumes(-1, false);
		return inf.value;
	}
	inline int GetMute()
	{
		INFO  inf = ControlOfVolumes(-1, false);
		return inf.getmute;
	}

	inline void SetVolume(int ses)//
	{
		ses = ses > 100 ? 100 : ses;
		ses = ses < 0 ? 0 : ses;
		ControlOfVolumes(ses, false);
	}
	inline void SetMute(bool mute)
	{
		ControlOfVolumes(-2, mute);//-2 is special for set mute what do you want to do
	}
	inline void ChangeMute()
	{
		ControlOfVolumes(-1, true);
	}
	inline void GetVolumeAndMute(bool *mut, int *vol) {
		INFO  inf = ControlOfVolumes(-1, false);
		*mut = inf.getmute;
		*vol = inf.value;
	}
	
	char out[5];
	const int labelX = 23, labelY = 94;
	const int labelX_MUTE = 13, labelY_MUTE = 94;
	inline void SetVisual(bool muteli, int value)
	{
		BitBlt(device_context, 0, 0, 60, 130, memdevice, 0, 0, SRCCOPY);

		//SendMessage(hProgress, (WM_USER + 2), value, 0);//pbm_setpos   		//		SendMessage(lblSes, WM_SETTEXT, NULL, (LPARAM)out);
		
		RECT rcProgressbar = { 23, 20, 37, 90 }; //rcProgressbar = {top=20 bottom=90 left=23 right=37}
		//Progressbar
		::FillRect(device_context, &rcProgressbar, boya_gri);//gri (gray) 
		rcProgressbar.top = ((value*-0.7) + 90);
		if (!muteli)//{  Beyaz cubuk
		::FillRect(device_context, &rcProgressbar, boya_mavi);//mavi (blue)
		/*RECT rcProgressbarDOTS = { 23, 74, 37, 75 };	
		rcProgressbarDOTS.top = rcProgressbar.top - 12;
		rcProgressbarDOTS.bottom = rcProgressbar.top;
		::FillRect(device_context, &rcProgressbarDOTS, RGB(0,0,0));*/
		//}
		else
			::FillRect(device_context, &rcProgressbar, boya_yesil);//yesil (green)

		//Right = loc X, Bottom = Loc Y, Left = Size X, Top = Size Y

		//Label
		if (*this->enablemouseclicks4show)//left & right  click as the same time for show form, label's color
			SetTextColor(device_context, RGB(255, 255, 0));//sari (yellow)
		else
			SetTextColor(device_context, RGB(255, 255, 255));

		if (!muteli){
			sprintf(out, "%d", value);
			TextOut(device_context, labelX, labelY, out, strlen(out));
		}
		else{
			sprintf(out, "%s", "MUTE");
			TextOut(device_context, labelX_MUTE, labelY_MUTE, out, strlen(out));
		}
	
		
		EndPaint(hwndpencere, &ps);
	}
};
SOUND *vaf = new SOUND();
SOUND::SpecialSizes *sizes = new SOUND::SpecialSizes();
void TimeDifference()
{
	while (true)
	{
		if (*vaf->ekrandabekletileceksure < 1)//Fazla performans tutketmesin diye
			Sleep(200);

		do
		{
			Sleep(1000);
		} while (*vaf->ekrandabekletileceksure -= 1 >= 0);

		while (GetAsyncKeyState(VK_LBUTTON))//Eger hala sol click ile ses seciliyorsa		
			Sleep(100);

		ShowWindow(vaf->hwndpencere, SW_HIDE);
		*vaf->formgozukuyor = false;
		SetForegroundWindow(vaf->AktifWindowtoback);
	}
}

inline void Control()
{
	bool mute2 = false;//karsilastirma icin
	SOUND::INFO inf2 = vaf->ControlOfVolumes(-1, false);
	int ses2 = inf2.value;
	bool triggered = false;
	int *nowses = new int(0);
	while (true)
	{
		SOUND::INFO inf = vaf->ControlOfVolumes(-1, false);
		*nowses = inf.value;

		if (inf.getmute != mute2)
			triggered = true;

		if (*nowses != ses2)
			triggered = true;


		if (*vaf->enablemouseclicks4show){
			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 && GetAsyncKeyState(VK_RBUTTON) & 0x8000) {

				if (IsWindowVisible(vaf->hwndpencere))//eger gosteriliyorsa kapanmasi icin;
				{
					ShowWindow(vaf->hwndpencere, SW_HIDE);
					*vaf->formgozukuyor = false;
					SetForegroundWindow(vaf->AktifWindowtoback);
					Sleep(120);
					continue;
				}
				triggered = true;
			}
		}



		mute2 = inf.getmute;
		ses2 = *nowses;

		if (triggered)
		{
			triggered = false;
			if (GetForegroundWindow() != vaf->hwndpencere)
				vaf->AktifWindowtoback = GetForegroundWindow();//Aktif windowu aliyorum ki form gizlenince tekrar o formu aktif edilsin						

			*vaf->ekrandabekletileceksure = 3;
			ShowWindow(vaf->hwndpencere, SW_SHOW);
			*vaf->formgozukuyor = true;
			vaf->SetVisual(mute2, ses2);
		}
		Sleep(100);
	}
}
enum MOUSEEVENTS{
	ON_FORM,
	ON_PROGRESSBAR,
	ON_LABEL
};
inline bool mouse(MOUSEEVENTS whatwant);
void Refresh()	{// 1 Kere calisacak, threadlari baslatiyor.
	vaf->device_context = GetDC(vaf->hwndpencere);
	vaf->memdevice = CreateCompatibleDC(vaf->device_context);
	vaf->labelFont = CreateFont(sizes->Label_Size, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, sizes->Label_FontName);
	SetBkMode(vaf->device_context, TRANSPARENT);
	SetTextColor(vaf->device_context, RGB(255, 255, 255));
	SelectObject(vaf->device_context, vaf->labelFont);

	HBITMAP hBmp = CreateCompatibleBitmap(vaf->device_context, 60, 130);//MUTE yazsý arkada kaliyor
	SelectObject(vaf->memdevice, hBmp);//Bu olmazsa labelin ustune yaziyor

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)TimeDifference, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Control, 0, 0, 0);

	bool mut = 0; int volume = 0;
	while (true)
	{
		if (*vaf->formgozukuyor)
		{
			do
			{
				vaf->GetVolumeAndMute(&mut, &volume);
				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)//Left click to events
				{
					if (mouse(MOUSEEVENTS::ON_PROGRESSBAR))//set volume
					{
						do
						{
							POINT pt;
							GetCursorPos(&pt);
							int y = 100 - (1.42*(pt.y - sizes->Window.bottom - sizes->Progressbar.top));
							y = y > 100 ? 100 : y;
							y = y < 0 ? 0 : y;
							mut = false;
							volume = y;
							vaf->SetVolume(volume);
							Sleep(5);
							vaf->SetVisual(mut, volume);
						} while ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) > 0);//While dongusunu kurmamin sebebi: eger ekranýn en altina yada en ustune mouseyi getirdiginde volume de maximum yada minimum olmasý icin (X positionu yok sayiyorum)
					}
					else if (mouse(MOUSEEVENTS::ON_LABEL))//set right&left at the same time to see window
					{
						*vaf->enablemouseclicks4show = !*vaf->enablemouseclicks4show;
						Sleep(150);
					}
				}
				else if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) && (mouse(MOUSEEVENTS::ON_LABEL)))//Label'a Sag click ile cikis
					SendMessage(vaf->hwndpencere, WM_CLOSE, 0, 0);
							
				vaf->SetVisual(mut, volume);
				if (!mouse(MOUSEEVENTS::ON_FORM))//bunu yapmazsam MOUSE TEKERLEK MESAJI GONDERILEMIYOR (THAT'S FOR WM_MOUSEWHEEL MESSAGE
					SetForegroundWindow(vaf->AktifWindowtoback);//Eger internette geziniyorsaniz mouse asagi kaymasi icin, focus olmadan formun gitmesini beklemek zorunda kalmamak icin
				else
					SetForegroundWindow(vaf->hwndpencere);
				Sleep(5);
			} while (*vaf->formgozukuyor);
		}
		Sleep(200);
	}
}

inline bool mouse(MOUSEEVENTS whatwant)//Right = loc X, Bottom = Loc Y, Left = Size X, Top = Size Y
{
	POINT mouse;
	GetCursorPos(&mouse);
	if (whatwant == MOUSEEVENTS::ON_FORM)
	{
		if ((mouse.x > sizes->Window.right && mouse.x < sizes->Window.right + sizes->Window.left - 1) &&
			(mouse.y > sizes->Window.bottom && mouse.y < sizes->Window.bottom + sizes->Window.top - 1))
			return true;
		else
			return false;
	}
	else if (whatwant == MOUSEEVENTS::ON_PROGRESSBAR)//progressbarin ustune tiklandiginda da calissin diye ozel degerler eklendi
	{
		if ((mouse.x + 5 > sizes->Window.right + sizes->Progressbar.left && mouse.x < sizes->Window.right + sizes->Window.left - sizes->Progressbar.left + 1) &&
			(mouse.y + 10 > sizes->Window.bottom + sizes->Progressbar.top && mouse.y < sizes->Window.bottom + sizes->Window.top - sizes->Progressbar.top - 7))
			return true;
		else
			return false;
	}
	else if (whatwant == MOUSEEVENTS::ON_LABEL)
	{
		if ((mouse.x + 12 > sizes->Window.right + vaf->labelX && mouse.x < sizes->Window.right + vaf->labelX + 25) &&//25 = DEGERIN SIZESI
			mouse.y > sizes->Window.bottom + vaf->labelY && mouse.y < sizes->Window.bottom + vaf->labelY + 15)
			return true;
		else
			return false;
	}
}