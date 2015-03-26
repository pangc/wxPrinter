#include "wlansetting.h"
#include <QDebug>


WlanSetting::WlanSetting()
{
    pProfileList = NULL;
    currInfo = NULL;
    getProfileInto();
}

BOOL WlanSetting::connectToNetwork(GUID *pInterfaceGuid, LPCTSTR lpszSSID, BOOL bIsAdhoc, UINT uAuthAlgo, UINT uCipherAlgo, LPCTSTR lpszSecurityKey)
{
    return true;
}

DWORD WlanSetting::getProfileInto()
{
    char *ch;
    QString str;
    PWLAN_AVAILABLE_NETWORK_LIST pBssList = NULL;
    PWLAN_AVAILABLE_NETWORK pBssEntry = NULL;
    PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
    PWLAN_INTERFACE_INFO pIfInfo = NULL;
    WLAN_PROFILE_INFO pInfo;
    DWORD dwMaxClient = 1;
    DWORD dwCurVersion = 0;
    DWORD dwError = ERROR_SUCCESS;
    if ((dwError = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient)) != ERROR_SUCCESS)
    {
        return dwError;
    }
//
    if((dwError = WlanEnumInterfaces(hClient, NULL, &pIfList))!=ERROR_SUCCESS)
    {
        qDebug()<<"wlanEnumInterFaces";
        return dwError;
    }

//    if ((dwError = WlanGetProfileList(hClient,&guidIntf,NULL,&pProfileList))!= ERROR_SUCCESS)
//    {
//        qDebug()<<dwError <<ERROR_INVALID_PARAMETER;
//        return dwError;
//    }
    qDebug()<<pIfList->dwNumberOfItems;
    for(UINT i=0;i<pIfList->dwNumberOfItems;i++)
    {
         pIfInfo = (WLAN_INTERFACE_INFO *) &pIfList->InterfaceInfo[i];
         qDebug()<< pIfInfo->strInterfaceDescription;
         switch (pIfInfo->isState) {
                     case wlan_interface_state_not_ready:
                         qDebug()<<"Not ready\n";
                         break;
                     case wlan_interface_state_connected:
                         qDebug()<<"Connected\n";
                         break;
                     case wlan_interface_state_ad_hoc_network_formed:
                         qDebug()<<"First node in a ad hoc network\n";
                         break;
                     case wlan_interface_state_disconnecting:
                         qDebug()<<"Disconnecting\n";
                         break;
                     case wlan_interface_state_disconnected:
                         qDebug()<<"Not connected\n";
                         break;
                     case wlan_interface_state_associating:
                         qDebug()<<"Attempting to associate with a network\n";
                         break;
                     case wlan_interface_state_discovering:
                         qDebug()<<"Auto configuration is discovering settings for the network\n";
                         break;
                     case wlan_interface_state_authenticating:
                         qDebug()<<"In process of authenticating\n";
                         break;
                     default:
                         qDebug()<<"Unknown state %ld\n", pIfInfo->isState;
                         break;
         }
         dwError = WlanGetAvailableNetworkList(hClient,&pIfInfo->InterfaceGuid,0,NULL,&pBssList);
         if(dwError!=ERROR_SUCCESS){
             qDebug()<<"WlanGetAvailableNetworkList failed with error";
             return dwError;
         }
         else
         {
             for (UINT j = 0; j < pBssList->dwNumberOfItems; j++) {
                pBssEntry = (WLAN_AVAILABLE_NETWORK *) & pBssList->Network[j];
                qDebug()<<"  Profile Name:"<<pBssEntry->strProfileName <<"\n";
                qDebug()<<QString::fromWCharArray(pBssEntry->strProfileName);
                if (pBssEntry->dot11Ssid.uSSIDLength == 0)
                                     qDebug()<<"diulei\n";
                 else {
                    QString ssid;
                     ssid.append((char *) pBssEntry->dot11Ssid.ucSSID);
                     qDebug()<<j<<":"<<ssid;
                }
         }
         }
    }
    return dwError;
}
