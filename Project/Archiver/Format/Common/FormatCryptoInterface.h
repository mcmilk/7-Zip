// FormatCryptoInterface.h

#pragma once

#ifndef __FORMATCRYPTOINTERFACE_H
#define __FORMATCRYPTOINTERFACE_H

#include "Common/Types.h"

/*
// {23170F69-40C1-278A-0000-000200260000}
DEFINE_GUID(IID_ICryptoGetPassword, 
0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x02, 0x00, 0x26, 0x00, 0x00);
MIDL_INTERFACE("23170F69-40C1-278A-0000-000200260000")
ICryptoGetPassword: public IUnknown
{
  STDMETHOD(CryptoGetPassword)(const BYTE **aData, UINT32 *aSize) PURE;
};
*/

// {23170F69-40C1-278A-0000-000200270000}
DEFINE_GUID(IID_ICryptoGetTextPassword, 
0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x02, 0x00, 0x27, 0x00, 0x00);
MIDL_INTERFACE("23170F69-40C1-278A-0000-000200270000")
ICryptoGetTextPassword: public IUnknown
{
  STDMETHOD(CryptoGetTextPassword)(BSTR *aPassword) PURE;
};

#endif
