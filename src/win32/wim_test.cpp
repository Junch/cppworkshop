#define _WIN32_WINNT 0x0400
#define _WIN32_DCOM

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <comdef.h>
#include <wbemidl.h>
#include <gtest/gtest.h>

#pragma comment(lib, "wbemuuid.lib")

void getCommandLine()
{
    HRESULT hr = 0;
    IWbemLocator         *WbemLocator  = NULL;
    IWbemServices        *WbemServices = NULL;
    IEnumWbemClassObject *EnumWbem  = NULL;

    //initializate the Windows security
    hr = CoInitializeEx(0, COINIT_MULTITHREADED);
    hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

    hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *) &WbemLocator);
    //connect to the WMI
    hr = WbemLocator->ConnectServer(L"ROOT\\CIMV2", NULL, NULL, NULL, 0, NULL, NULL, &WbemServices);
    //Run the WQL Query
    hr = WbemServices->ExecQuery(L"WQL", L"SELECT ProcessId,CommandLine FROM Win32_Process", WBEM_FLAG_FORWARD_ONLY, NULL, &EnumWbem);

    // Iterate over the enumerator
    if (EnumWbem != NULL) {
        IWbemClassObject *result = NULL;
        ULONG returnedCount = 0;

        while((hr = EnumWbem->Next(WBEM_INFINITE, 1, &result, &returnedCount)) == S_OK) {
            VARIANT ProcessId;
            VARIANT CommandLine;

            // access the properties
            hr = result->Get(L"ProcessId", 0, &ProcessId, 0, 0);
            hr = result->Get(L"CommandLine", 0, &CommandLine, 0, 0);
            if (!(CommandLine.vt==VT_NULL))
            wprintf(L"%u  %s \r\n", ProcessId.uintVal, CommandLine.bstrVal);

            result->Release();
        }
    }

    // Release the resources
    EnumWbem->Release();
    WbemServices->Release();
    WbemLocator->Release();

    CoUninitialize();
}

TEST(wim, test)
{
    getCommandLine();
}
