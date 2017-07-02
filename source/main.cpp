#include<Windows.h>
#include<stdio.h>
#include "res.h"
#include <cstdlib>
#include <math.h>

#pragma comment(lib,"winmm.lib")
#define Pi 3.141592
#define czas 35
//char *snotes = "D C C E F G A H C H A A A A G F E D F E E D C E D G C E E E E A H C H A H A G A E E E A H C H A H A G H C G F E";
//char *stime = "16 16 16 8 4 16 16 16 16 8 4 16 16 16 16 8 4 16 16 16 16 8 4 16 16 16 16 8 4 16 16 16 16 8 4 16 16 16 16 8 4 16 16 16 16 8 4 16 16 16 16 8 4 16 16 16";

char snotes[100] = { 'a','b','c','c','c','a','d','c','b','a','g','c','b','b','a','a','b','c','c','c','a','d','c','b','a','g','g','c','e','g','f','g','g','b','b','a','b','b','b','d','d','c' };

double stime[100] = { 24,24,24,4,24,24,24,24,
24,24,24,4,24,24,24,24,
24,24,24,4,24,24,24,24,
24,24,24,4,24,24,24,24 };
void muzyka(BYTE *pBufferForAudio, WAVEFORMATEX pcmWaveFormat, char *snotes, double *stime);
int Note(BYTE* pBufferForAudio, int iStart, int iDuration, float fNote, float fDiv);
CHAR szText[5];

float fH = 123.47;

/*float fc = 130.8;
float fd = 146.8;
float fe = 164.8;
float ff = 174.6;
float fg = 196;
float fa = 220;
float fh = 247;
float fis = 185.0;
*/
float fa1 = 440.0;
float fb1 = 493.88;
float fe1 = 329.6;
float ff1 = 349.2;
float fg1 = 392;
float fc2 = 523.25;
float fd2 = 587.32;
float fe2 = 659.25;
float ff2 = 698.45;

float fp = 0;

INT_PTR CALLBACK DialogProc(HWND hwndDig, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_COMMAND:
    switch (HIWORD(wParam))
    {
    case BN_CLICKED:
      switch (LOWORD(wParam))
      {
      case IDC_BUTTON2:
        //gra(hwndDig);
        return TRUE;
      }
      return FALSE;
    case EN_CHANGE:
      switch (LOWORD(wParam))
      {
      case IDC_EDIT1:
        //HWND hwndEditBox = GetDlgItem(hwndDig, IDC_EDIT1);
        //GetWindowText(hwndEditBox, szText, 3);
        return TRUE;
      }
      return FALSE;
    }
    return FALSE;
  case WM_CLOSE:
    DestroyWindow(hwndDig);
    PostQuitMessage(0);
    return TRUE;
  }
  return FALSE;
}

//utwor na conajmniej 35sek 

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, PSTR szCmdLIne, int iCmdShow)
{
  HWND hwndMainWindow = CreateDialog(hinstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  ShowWindow(hwndMainWindow, iCmdShow);
  MSG msg = {};

  WAVEFORMATEX pcmWaveFormat;
  pcmWaveFormat.wFormatTag = WAVE_FORMAT_PCM;
  pcmWaveFormat.nChannels = 1;
  pcmWaveFormat.nSamplesPerSec = 44100L;
  pcmWaveFormat.wBitsPerSample = 8;
  pcmWaveFormat.nAvgBytesPerSec = pcmWaveFormat.nSamplesPerSec*pcmWaveFormat.wBitsPerSample / 8;
  pcmWaveFormat.nBlockAlign = 1; //2 jak na 16 bitow
  pcmWaveFormat.cbSize = 0;

  MMRESULT mmResult = 0;
  HWAVEOUT hwo = 0;
  UINT devId;

  for (devId = 0; devId < waveOutGetNumDevs(); devId++)
  {
    mmResult = waveOutOpen(&hwo, devId, &pcmWaveFormat, 0, 0, CALLBACK_NULL);

    if (mmResult == MMSYSERR_NOERROR) break;
  }

  if (mmResult != MMSYSERR_NOERROR)//gdy sie nie udalo
  {
    MessageBox(hwndMainWindow, TEXT("Nie znaleziono karty"), TEXT("Error"), MB_OK);
    return mmResult;
  }

  BYTE *pBufferForAudio = new BYTE[pcmWaveFormat.nAvgBytesPerSec * czas];

  /* int i = 0;
    for (; i < 0.5* pcmWaveFormat.nAvgBytesPerSec; i++)
   {
   float t = i / (float)pcmWaveFormat.nSamplesPerSec;
   f = fC;
   pBufferForAudio[i] = 128 * sin(2 * Pi*f*t) + 128;
   }
   for (; i < 1.0 * pcmWaveFormat.nAvgBytesPerSec; i++)
   {
   float t = i / (float)pcmWaveFormat.nSamplesPerSec;
   f = fD;
   pBufferForAudio[i] = 128 * sin(2 * Pi*f*t) + 128;
   }
   for (; i < 1.5 * pcmWaveFormat.nAvgBytesPerSec; i++)
   {
   float t = i / (float)pcmWaveFormat.nSamplesPerSec;
   f = fE;
   pBufferForAudio[i] = 128 * sin(2 * Pi*f*t) + 128;
   }
   for (; i < 2.0 * pcmWaveFormat.nAvgBytesPerSec; i++)
   {
   float t = i / (float)pcmWaveFormat.nSamplesPerSec;
   f = fF;
   pBufferForAudio[i] = 128 * sin(2 * Pi*f*t) + 128;
   }
   for (; i < 2.5 * pcmWaveFormat.nAvgBytesPerSec; i++)
   {
   float t = i / (float)pcmWaveFormat.nSamplesPerSec;
   f = fG;
   pBufferForAudio[i] = 128 * sin(2 * Pi*f*t) + 128;
   }
   for (; i < 3.0 * pcmWaveFormat.nAvgBytesPerSec; i++)
   {
   float t = i / (float)pcmWaveFormat.nSamplesPerSec;
   f = fA;
   pBufferForAudio[i] = 128 * sin(2 * Pi*f*t) + 128;
   }
   for (; i < 3.5 * pcmWaveFormat.nAvgBytesPerSec; i++)
   {
   float t = i / (float)pcmWaveFormat.nSamplesPerSec;
   f = fH;
   pBufferForAudio[i] = 128 * sin(2 * Pi*f*t) + 128;
   }*/

  WAVEHDR whdr;
  ZeroMemory(&whdr, sizeof(WAVEHDR));
  whdr.lpData = reinterpret_cast<LPSTR>(pBufferForAudio);
  /*
  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
  TranslateMessage(&msg);
  DispatchMessage(&msg);
  }*/

  whdr.dwBufferLength = czas * pcmWaveFormat.nAvgBytesPerSec;
  {
    muzyka(pBufferForAudio, pcmWaveFormat, snotes, stime);
    mmResult = waveOutPrepareHeader(hwo, &whdr, sizeof(WAVEHDR));
    mmResult = waveOutWrite(hwo, &whdr, sizeof(WAVEHDR));
    while ((whdr.dwFlags & WHDR_DONE) != WHDR_DONE)Sleep(100);
    mmResult = waveOutUnprepareHeader(hwo, &whdr, sizeof(WAVEHDR));
  }

  mmResult = waveOutClose(hwo);
  delete[] pBufferForAudio;
  return 0;
}
void muzyka(BYTE *pBufferForAudio, WAVEFORMATEX pcmWaveFormat, char *snotes, double  *stime)
{
  float S = pcmWaveFormat.nAvgBytesPerSec;
  float nuta=0.0;
  float czasNuty;
  int k = 0;
  for (int j = 0; j < 100; j++)
  {
    czasNuty = (stime[j]) / 32.0;
    switch (snotes[j])
    {
    case 'a': nuta = fa1; break;
    case 'b': nuta = fb1; break;
    case 'c': nuta = fc2; break;
    case 'd': nuta = fd2; break;
    case 'e': nuta = fe2; break;
    case 'g': nuta = fg1; break;
    case 'f': nuta = ff2; break;
    }
    k = Note(pBufferForAudio, k, czasNuty*S, nuta, pcmWaveFormat.nSamplesPerSec);
  }
}

int Note(BYTE* pBufferForAudio, int iStart, int iDuration, float fNote, float fDiv)
{
  int i;
  for (i = iStart; i < iStart + iDuration - 200; i++)
  {
    float t = i / fDiv;
    pBufferForAudio[i] = 128 * sin(2 * Pi*fNote*t) + 128;
  }
  for (; i < iStart + iDuration; ++i)
  {
    float t = i / fDiv;
    pBufferForAudio[i] = 128;
  }
  return i;
}
