#include <Windows.h>
#include <tlhelp32.h>
#include "framework.h"
#include "middle.h"
#include "xor.hpp"

#pragma comment(lib, "ntdll.lib")

extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN OldValue);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask, PULONG_PTR Parameters, ULONG ValidResponseOptions, PULONG Response);

void bsod()
{
    BOOLEAN bl;
    ULONG Response;
    RtlAdjustPrivilege(19, TRUE, FALSE, &bl); // Enable SeShutdownPrivilege
    NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response); // Shutdown
}

void DebuggerPresent()
{
    if (IsDebuggerPresent())
    {
        bsod();
    }
}

DWORD_PTR FindProcessId2(const std::string& processName)
{
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE)
        return 0;

    Process32First(processesSnapshot, &processInfo);
    if (!processName.compare(processInfo.szExeFile))
    {
        CloseHandle(processesSnapshot);
        return processInfo.th32ProcessID;
    }

    while (Process32Next(processesSnapshot, &processInfo))
    {
        if (!processName.compare(processInfo.szExeFile))
        {
            CloseHandle(processesSnapshot);
            return processInfo.th32ProcessID;
        }
    }

    CloseHandle(processesSnapshot);
    return 0;
}

void ScanProccessListForBlacklistedProcess()
{
    if (FindProcessId2(_xor_("ollydbg.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("ProcessHacker.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("tcpview.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("autoruns.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("autorunsc.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("filemon.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("procmon.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("regmon.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("procexp.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("idaq.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("idaq64.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("ImmunityDebugger.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("Wireshark.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("dumpcap.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("HookExplorer.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("ImportREC.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("PETools.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("LordPE.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("dumpcap.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("SysInspector.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("proc_analyzer.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("sysAnalyzer.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("sniff_hit.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("windbg.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("joeboxcontrol.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("Fiddler.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("joeboxserver.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("ida64.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("ida.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("Vmtoolsd.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("Vmwaretrat.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("Vmwareuser.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("Vmacthlp.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("vboxservice.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("vboxtray.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("ReClass.NET.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("x64dbg.exe")) != 0)
    {
        bsod();
    }
    else if (FindProcessId2(_xor_("OLLYDBG.exe")) != 0)
    {
        bsod();
    }
}

void ScanBlacklistedWindows()
{
    if (FindWindow(NULL, _xor_("The Wireshark Network Analyzer").c_str()))
    {
        bsod();
    }

    if (FindWindow(NULL, _xor_("Progress Telerik Fiddler Web Debugger").c_str()))
    {
        bsod();
    }

    if (FindWindow(NULL, _xor_("x64dbg").c_str()))
    {
        bsod();
    }
}

void AntiDebug()
{
    DebuggerPresent();
    ScanBlacklistedWindows();
    ScanProccessListForBlacklistedProcess();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    while (1)
    {
      //  AntiDebug();
        Sleep(2);
    }

    return 0;
}