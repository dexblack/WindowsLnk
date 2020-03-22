#pragma once
#include "LnkDllPort.h"

enum class LinkFlagBit : uint32_t
{
  eHasLinkTargetIDList,				
  eHasLinkInfo,						    
  eHasName,						        
  eHasRelativePath,						
  eHasWorkingDir,						  
  eHasArguments,						  
  eHasIconLocation,						
  eIsUnicode,						      
  eForceNoLinkInfo,						
  eHasExpString,						  
  eRunInSeparateProcess,			
  eUnused1,						        
  eHasDarwinID,						    
  eRunAsUser,						      
  eHasExpIcon,						    
  eNoPidlAlias,						    
  eUnused2,						        
  eRunWithShimLayer,					
  eForceNoLinkTrack,					
  eEnableTargetMetadata,			
  eDisableLinkPathTracking,		
  eDisableKnownFolderTracking,
  eDisableKnownFolderAlias,		
  eAllowLinkToLink,						
  eUnaliasOnSave,						  
  ePreferEnvironmentPath,			
  eKeepLocalIDListForUNCTarget
};

#pragma pack(push, 1)

struct LnkDllPort LinkFlags
{
  int hasLinkTargetIDList : 1;          // The shell link is saved with an item ID list(IDList).If this bit is set, a LinkTargetIDList structure(section 2.2) MUST follow the ShellLinkHeader.If this bit is not set, this structure MUST NOT be present.
  int hasLinkInfo : 1;                  // The shell link is saved with link information.If this bit is set, a LinkInfo structure(section 2.3) MUST be present.If this bit is not set, this structure MUST NOT be present.
  int hasName : 1;                      // The shell link is saved with a name string.If this bit is set, a NAME_STRING StringData structure(section 2.4) MUST be present.If this bit is not set, this structure MUST NOT be present.
  int hasRelativePath : 1;              // The shell link is saved with a relative path string.If this bit is set, a RELATIVE_PATH StringData structure(section 2.4) MUST be present.If this bit is not set, this structure MUST NOT be present.
  int hasWorkingDir : 1;                // The shell link is saved with a working directory string.If this bit is set, a WORKING_DIR StringData structure(section 2.4) MUST be present.If this bit is not set, this structure MUST NOT be present.
  int hasArguments : 1;                 // The shell link is saved with command line arguments.If this bit is set, a COMMAND_LINE_ARGUMENTS StringData structure(section 2.4) MUST be present.If this bit is not set, this structure MUST NOT be present.
  int hasIconLocation : 1;              // The shell link is saved with an icon location string.If this bit is set, an ICON_LOCATION StringData structure(section 2.4) MUST be present.If this bit is not set, this structure MUST NOT be present.
  int isUnicode : 1;                    // The shell link contains Unicode encoded strings.This bit SHOULD be set.If this bit is set, the StringData section contains Unicode - encoded strings; otherwise, it contains strings that are encoded using the system default code page.
  int forceNoLinkInfo : 1;              // The LinkInfo structure(section 2.3) is ignored.
  int hasExpString : 1;                 // The shell link is saved with an EnvironmentVariableDataBlock(section 2.5.4).
  int runInSeparateProcess : 1;         // The target is run in a separate virtual machine when launching a link target that is a 16 - bit application.
  int unused1 : 1;                      // A bit that is undefined and MUST be ignored.
  int hasDarwinID : 1;                  // The shell link is saved with a DarwinDataBlock(section 2.5.3).
  int runAsUser : 1;                    // The application is run as a different user when the target of the shell link is activated.
  int hasExpIcon : 1;                   // The shell link is saved with an IconEnvironmentDataBlock(section 2.5.5).
  int noPidlAlias : 1;                  // The file system location is represented in the shell namespace when the path to an item is parsed into an IDList.
  int unused2 : 1;                      // A bit that is undefined and MUST be ignored.
  int runWithShimLayer : 1;             // The shell link is saved with a ShimDataBlock(section 2.5.8).
  int forceNoLinkTrack : 1;             // The TrackerDataBlock(section 2.5.10) is ignored.
  int enableTargetMetadata : 1;         // The shell link attempts to collect target properties and store them in the PropertyStoreDataBlock(section 2.5.7) when the link target is set.
  int disableLinkPathTracking : 1;      // The EnvironmentVariableDataBlock is ignored.
  int disableKnownFolderTracking : 1;   // The SpecialFolderDataBlock(section 2.5.9) and the KnownFolderDataBlock(section 2.5.6) are ignored when loading the shell link.If this bit is set, these extra data blocks SHOULD NOT be saved when saving the shell link.
  int disableKnownFolderAlias : 1;      // If the link has a KnownFolderDataBlock(section 2.5.6), the unaliased form of the known folder IDList SHOULD be used when translating the target IDList at the time that the link is loaded.
  int allowLinkToLink : 1;              // Creating a link that references another link is enabled.Otherwise, specifying a link as the target IDList SHOULD NOT be allowed.
  int unaliasOnSave : 1;                // When saving a link for which the target IDList is under a known folder, either the unaliased form of that known folder or the target IDList SHOULD be used.
  int preferEnvironmentPath : 1;        // The target IDList SHOULD NOT be stored; instead, the path specified in the EnvironmentVariableDataBlock(section 2.5.4) SHOULD be used to refer to the target.
  int keepLocalIDListForUNCTarget : 1;  // When the target is a UNC name that refers to a location on a local machine, the local path IDList in the PropertyStoreDataBlock(section 2.5.7) SHOULD be stored, so it can be used when the link is loaded on the local machine.
};

#pragma pack(pop)
