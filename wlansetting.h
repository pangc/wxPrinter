#ifndef WLANSETTING_H
#define WLANSETTING_H
#include <windows.h>
#include <wlanapi.h>
#include <wtypes.h>
class WlanSetting
{
public:
    WlanSetting();
    HANDLE hClient;
    GUID guidIntf;
    PWLAN_PROFILE_INFO_LIST pProfileList;
    PWLAN_PROFILE_INFO currInfo;
    BOOL connectToNetwork(GUID *pInterfaceGuid,LPCTSTR lpszSSID,BOOL bIsAdhoc,UINT uAuthAlgo, UINT uCipherAlgo,LPCTSTR lpszSecurityKey);
private:
    DWORD getProfileInto();
};

#endif // WLANSETTING_H
