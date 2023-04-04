
/*
 *   𝓐𝓡𝓢 𝓢𝓒𝓡𝓘𝓟𝓣𝓤𝓜
 *   🇧​​​​​🇾​​​​​ 🇬​​​​​🇺​​​​​🇮​​​​​🇱​​​​​🇱​​​​​🇦​​​​​🇺​​​​​🇲​​​​​🇪​​​​​🇵​​​​​🇱​​​​​🇦​​​​​🇳​​​​​🇹​​​​​🇪​​​​​.🇶​​​​​🇨​​​​​@🇬​​​​​🇲​​​​​🇦​​​​​🇮​​​​​🇱​​​​​.🇨​​​​​🇴​​​​​🇲​​​​​
*/

#ifndef __SERVICE_HANDLE_H__
#define __SERVICE_HANDLE_H__

#include <windows.h>
#include <Winsvc.h>


class ServiceHandle
{
   SC_HANDLE _handle = nullptr;

   void Close()
   {
      if (_handle != nullptr)
         ::CloseServiceHandle(_handle);
   }
   
public:
   ServiceHandle(SC_HANDLE const handle = nullptr) noexcept :_handle(handle) {}

   ServiceHandle(ServiceHandle&& other) noexcept : _handle(std::move(other._handle)) {}

   ServiceHandle& operator=(SC_HANDLE const handle)
   {
      if (_handle != handle)
      {
         Close();

         _handle = handle;
      }

      return *this;
   }

   ServiceHandle& operator=(ServiceHandle&& other)
   {
      if (this != &other)
      {
         _handle = std::move(other._handle);
         other._handle = nullptr;
      }

      return *this;
   }

   operator SC_HANDLE() const noexcept { return _handle; }

   explicit operator bool() const noexcept { return _handle != nullptr; }
   
   ~ServiceHandle() 
   {
      Close();
   }
};


#endif //__SERVICE_HANDLE_H__