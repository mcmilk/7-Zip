// ExtractEngine.h

#include "StdAfx.h"

#include "Common/StringConvert.h"

#include "Windows/FileDir.h"
#include "Windows/FileFind.h"

#include "../Common/OpenEngine2.h"

#ifndef  NO_REGISTRY
#include "../Common/ZipRegistry.h"
#endif

#include "MyMessages.h"
#include "FormatUtils.h"

#include "ExtractDialog.h"
#include "ExtractCallback.h"

using namespace NWindows;

HRESULT ExtractArchive(HWND aParentWindow, const CSysString &aFileName)
{
  CComPtr<IArchiveHandler100> anArchiveHandler;
  NZipRootRegistry::CArchiverInfo anArchiverInfoResult;

  UString aDefaultName;
  HRESULT aResult = OpenArchive(aFileName, &anArchiveHandler, 
      anArchiverInfoResult, aDefaultName, NULL);
  if (aResult != S_OK)
    return aResult;

  #ifndef  NO_REGISTRY
  CZipRegistryManager aZipRegistryManager;
  #endif

  CExtractDialog aDialog;
  aDialog.Init(
      #ifndef  NO_REGISTRY
      &aZipRegistryManager, 
      #endif
      aFileName);
  aDialog.m_FilesMode = NExtractionDialog::NFilesMode::kAll;
  aDialog.m_EnableSelectedFilesButton = false;
  aDialog.m_EnableFilesButton = false;

  if(aDialog.Create(aParentWindow) != IDOK)
    return E_ABORT;

  CSysString aDirectoryPath = aDialog.m_DirectoryPath;
  if(!NFile::NDirectory::CreateComplexDirectory(aDirectoryPath))
  {
    MyMessageBox(MyFormat(IDS_CANNOT_CREATE_FOLDER, (LPCTSTR)aDirectoryPath));
    return E_FAIL;
  }
  
  CComObjectNoLock<CExtractCallBackImp> *anExtractCallBackSpec =
    new CComObjectNoLock<CExtractCallBackImp>;

  CComPtr<IExtractCallback2> anExtractCallBack(anExtractCallBackSpec);
  
  anExtractCallBackSpec->m_ParentWindow = 0;
  anExtractCallBackSpec->StartProgressDialog();

  // anExtractCallBackSpec->m_ProgressDialog.ShowWindow(SW_SHOWNORMAL);

  NExtractionDialog::CModeInfo anExtractModeInfo;
  aDialog.GetModeInfo(anExtractModeInfo);
  UStringVector aRemovePathParts;

  UString aPassword = GetUnicodeString((LPCTSTR)aDialog.m_Password);

  NFile::NFind::CFileInfo anArchiveFileInfo;
  if (!NFile::NFind::FindFile(aFileName, anArchiveFileInfo))
    throw "there is no archive file";

  anExtractCallBackSpec->Init(anArchiveHandler, anExtractModeInfo,
      !aPassword.IsEmpty(), aPassword);

  NExtractionMode::NPath::EEnum aPathMode;
  NExtractionMode::NOverwrite::EEnum anOverwriteMode;
  switch (anExtractModeInfo.OverwriteMode)
  {
    case NExtractionDialog::NOverwriteMode::kAskBefore:
      anOverwriteMode = NExtractionMode::NOverwrite::kAskBefore;
      break;
    case NExtractionDialog::NOverwriteMode::kWithoutPrompt:
      anOverwriteMode = NExtractionMode::NOverwrite::kWithoutPrompt;
      break;
    case NExtractionDialog::NOverwriteMode::kSkipExisting:
      anOverwriteMode = NExtractionMode::NOverwrite::kSkipExisting;
      break;
    case NExtractionDialog::NOverwriteMode::kAutoRename:
      anOverwriteMode = NExtractionMode::NOverwrite::kAutoRename;
      break;
    default:
      throw 12334454;
  }
  switch (anExtractModeInfo.PathMode)
  {
    case NExtractionDialog::NPathMode::kFullPathnames:
      aPathMode = NExtractionMode::NPath::kFullPathnames;
      break;
    case NExtractionDialog::NPathMode::kCurrentPathnames:
      aPathMode = NExtractionMode::NPath::kCurrentPathnames;
      break;
    case NExtractionDialog::NPathMode::kNoPathnames:
      aPathMode = NExtractionMode::NPath::kNoPathnames;
      break;
    default:
      throw 12334455;
  }

  return anArchiveHandler->Extract(
      aPathMode, anOverwriteMode, GetUnicodeString(aDirectoryPath), 
      BoolToMyBool(false), anExtractCallBack);
}
