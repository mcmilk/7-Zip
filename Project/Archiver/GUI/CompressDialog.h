// CompressDialog.h

#pragma once

#ifndef __COMPRESSDIALOG_H
#define __COMPRESSDIALOG_H

#include "../Common/ZipRegistry.h"
#include "../Common/ZipRegistryMain.h"
#include "../Resource/CompressDialog/resource.h"

#include "Windows/Control/Dialog.h"
#include "Windows/Control/Edit.h"
#include "Windows/Control/ComboBox.h"

namespace NCompressDialog
{
  namespace NUpdateMode
  {
    enum EEnum
    {
      kAdd,
      kUpdate,
      kFresh,
      kSynchronize,
    };
  }
  namespace NMethod
  {
    enum EEnum
    {
      kStore,
      kNormal,
      kMaximum,
    };
  }
  struct CInfo
  {
    NUpdateMode::EEnum UpdateMode;
    NMethod::EEnum Method;
    bool SolidModeIsAllowed;
    bool SolidMode;

    CSysString Options;

    bool SFXMode;
    
    CSysString ArchiveNameSrc; // in: Relative for ; out: abs
    CSysString ArchiveName; // in: Relative for ; out: abs
    CSysString CurrentDirPrefix;

    bool GetFullPathName(CSysString &aResult) const;

    int ArchiverInfoIndex;
  };
}

class CCompressDialog: public NWindows::NControl::CModalDialog
{
	NWindows::NControl::CComboBox	m_ArchivePath;
	NWindows::NControl::CComboBox	m_Format;
	NWindows::NControl::CComboBox	m_Method;
	NWindows::NControl::CComboBox	m_UpdateMode;
  NWindows::NControl::CDialogChildControl m_Options;

  NWindows::NControl::CEdit _passwordControl;


  NZipSettings::NCompression::CInfo m_RegistryInfo;

  int m_PrevFormat;
  void SetArchiveName(const CSysString &aName);
  int FindFormat(const CSysString &aName);
  void SaveOptions();
  void SetOptions();
  void UpdatePasswordControl();
public:
  CObjectVector<NZipRootRegistry::CArchiverInfo> m_ArchiverInfoList;

  NCompressDialog::CInfo m_Info;

  CSysString Password;

  INT_PTR Create(HWND aWndParent = 0)
    { return CModalDialog::Create(MAKEINTRESOURCE(IDD_DIALOG_COMPRESS ), aWndParent); }

protected:

  void CheckSolidEnable();
  void CheckSFXEnable();
  void CheckPasswordEnable();

	void OnButtonSetArchive();
  void OnButtonSFX();

  virtual bool OnInit();
  virtual bool OnCommand(int aCode, int anItemID, LPARAM lParam);
  virtual bool OnButtonClicked(int aButtonID, HWND aButtonHWND);
  virtual void OnOK();
  virtual void OnHelp();

  void OnSelChangeComboFormat();
};

#endif