// NativeTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "ServiceController.h"
#include "ServiceEnumerator.h"

#include <iostream>

#define SCC_CONTROL_STOP                     SERVICE_CONTROL_STOP
#define SCC_CONTROL_PAUSE                    SERVICE_CONTROL_PAUSE
#define SCC_CONTROL_CONTINUE                 SERVICE_CONTROL_CONTINUE
#define SCC_CONTROL_INTERROGATE              SERVICE_CONTROL_INTERROGATE
#define SCC_CONTROL_SHUTDOWN                 SERVICE_CONTROL_SHUTDOWN
#define SCC_CONTROL_PARAMCHANGE              SERVICE_CONTROL_PARAMCHANGE
#define SCC_CONTROL_NETBINDADD               SERVICE_CONTROL_NETBINDADD
#define SCC_CONTROL_NETBINDREMOVE            SERVICE_CONTROL_NETBINDREMOVE
#define SCC_CONTROL_NETBINDENABLE            SERVICE_CONTROL_NETBINDENABLE
#define SCC_CONTROL_NETBINDDISABLE           SERVICE_CONTROL_NETBINDDISABLE
#define SCC_CONTROL_DEVICEEVENT              SERVICE_CONTROL_DEVICEEVENT
#define SCC_CONTROL_HARDWAREPROFILECHANGE    SERVICE_CONTROL_HARDWAREPROFILECHANGE
#define SCC_CONTROL_POWEREVENT               SERVICE_CONTROL_POWEREVENT
#define SCC_CONTROL_SESSIONCHANGE            SERVICE_CONTROL_SESSIONCHANGE
#define SCC_CONTROL_PRESHUTDOWN              SERVICE_CONTROL_PRESHUTDOWN
#define SCC_CONTROL_TIMECHANGE               SERVICE_CONTROL_TIMECHANGE
#define SCC_CONTROL_TRIGGEREVENT             SERVICE_CONTROL_TRIGGEREVENT
#define SCC_CONTROL_LOWRESOURCES             SERVICE_CONTROL_LOWRESOURCES
#define SCC_CONTROL_SYSTEMLOWRESOURCES       SERVICE_CONTROL_SYSTEMLOWRESOURCES

#define SCC_CONTROL_USER_COMMAND_01			 0x00000081
#define SCC_CONTROL_USER_COMMAND_02			 0x00000082
#define SCC_CONTROL_USER_COMMAND_03			 0x00000083
#define SCC_CONTROL_USER_COMMAND_04			 0x00000084
#define SCC_CONTROL_USER_COMMAND_05			 0x00000085
#define SCC_CONTROL_USER_COMMAND_06			 0x00000086
#define SCC_CONTROL_USER_COMMAND_07			 0x00000087
#define SCC_CONTROL_USER_COMMAND_08			 0x00000088


int send(int cmd);
int main()
{

    send(SCC_CONTROL_USER_COMMAND_03);
    Sleep(2000);
    return 0;
  /* auto services = ServiceEnumerator::EnumerateServices();
   for (auto const & s : services)
   {
      std::wcout << "Name:    " << s.ServiceName << std::endl
                 << "Display: " << s.DisplayName << std::endl
                 << "Status:  " << ServiceStatusToString(static_cast<ServiceStatus>(s.Status.dwCurrentState)) << std::endl
                 << "--------------------------" << std::endl;
   }

   // open the service
   auto service = ServiceController{ L"LanmanWorkstation" };

   auto print_status = [&service]() {
      std::wcout << "Status:              " << ServiceStatusToString(service.GetStatus()) << std::endl; 
      
   };

   auto print_config = [](ServiceConfig const config) {
      std::wcout << "---------------------" << std::endl;
      std::wcout << "Start name:          " << config.GetStartName() << std::endl;
      std::wcout << "Display name:        " << config.GetDisplayName() << std::endl;
      std::wcout << "Description:         " << config.GetDescription() << std::endl;
      std::wcout << "Type:                " << ServiceTypeToString(config.GetType()) << std::endl;
      std::wcout << "Start type:          " << ServiceStartTypeToString(config.GetStartType()) << std::endl;
      std::wcout << "Error control:       " << ServiceErrorControlToString(config.GetErrorControl()) << std::endl;
      std::wcout << "Binary path:         " << config.GetBinaryPathName() << std::endl;
      std::wcout << "Load ordering group: " << config.GetLoadOrderingGroup() << std::endl;
      std::wcout << "Tag ID:              " << config.GetTagId() << std::endl;
      std::wcout << "Dependencies:        ";
      for (auto const & d : config.GetDependencies()) std::wcout << d << ", ";
      std::wcout << std::endl;
      std::wcout << "---------------------" << std::endl;
   };

   // read the service configuration, temporary change its description and then restore the old one
   {
      auto config = service.GetServiceConfig();

      print_config(config);

      auto oldDescription = config.GetDescription();

      auto newDescription = _T("This is a sample description.");

      config.ChangeDescription(newDescription);

      config.Refresh();

      print_config(config);

      config.ChangeDescription(oldDescription);

      config.Refresh();

      print_config(config);
   }

   // check the service status
   print_status();

   // start the service if the service is currently stopped
   if (service.GetStatus() == ServiceStatus::Stopped)
   {
      service.Start();

      print_status();

      service.WaitForStatus(ServiceStatus::Running);

      print_status();
   }

   // if the service and running and it supports pause and continue then first pause and then resume the service
   if (service.GetStatus() == ServiceStatus::Running && service.CanPauseContinue())
   {
      service.Pause();

      print_status();

      service.WaitForStatus(ServiceStatus::Paused);

      print_status();

      service.Continue();

      print_status();

      service.WaitForStatus(ServiceStatus::Running);

      print_status();
   }
   */
   return 0;
}


int send(int cmd)
{
    SC_HANDLE managerHandle;
    SC_HANDLE serviceHandle;

    SERVICE_STATUS   controlParms;
    DWORD retStatus;

    managerHandle = OpenSCManager(NULL, NULL, GENERIC_READ);
    if (NULL != managerHandle)
    {
        serviceHandle = OpenService(managerHandle, L"_dev_service_10", SERVICE_ALL_ACCESS);

        if (NULL != serviceHandle)
        {
            cout << "connected to Service _dev_service_10" << endl;
            BOOL success = ::ControlService(serviceHandle, cmd, &controlParms);
            if (!success)
            {
                DWORD le = ::GetLastError();
                cout << "ControlService failed " << le << endl;
                switch (le)
                {
                case ERROR_ACCESS_DENIED:
                case ERROR_DEPENDENT_SERVICES_RUNNING:
                case ERROR_INVALID_HANDLE:
                case ERROR_INVALID_PARAMETER:
                case ERROR_INVALID_SERVICE_CONTROL:
                case ERROR_SERVICE_CANNOT_ACCEPT_CTRL:
                case ERROR_SERVICE_REQUEST_TIMEOUT:
                case ERROR_SHUTDOWN_IN_PROGRESS:
                    return FALSE;

                case ERROR_SERVICE_NOT_ACTIVE:
                default:
                    break;
                }
            }
            CloseServiceHandle(serviceHandle);
        }
        else
        {
            cout << "could not connect to Service" << endl;
        }

        CloseServiceHandle(managerHandle);
    }
    else
    {
        cout << "could not open service manager" << endl;
    }
    return 0;
}
