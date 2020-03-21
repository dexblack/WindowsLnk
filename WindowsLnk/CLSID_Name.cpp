#include "pch.h"

#include <assert.h>

#include "CLSID_Name.hpp"

// Makes CLSID_Name ctor possible.
CLSID makeCLSID(char const* cszCLSID)
{
  assert(nullptr != cszCLSID);
  std::string const sCLSID(cszCLSID);
#ifdef TESTLNK_WIDE_OLECHAR
  std::wstring osCLSID;
  std::copy(sCLSID.begin(), sCLSID.end(), std::back_inserter(osCLSID));
#else
  std::string const& osCLSID = sCLSID;
#endif

  CLSID clsid;
  static const std::string error("Invalid CLSID: " + std::string(sCLSID) + " - ");
  std::string reason;
  HRESULT hr = CLSIDFromString(osCLSID.c_str(), &clsid);
  switch (hr)
  {
    case NOERROR:
      break;
    case CO_E_CLASSSTRING:
      reason = "The class string was improperly formatted.";
      break;
    case REGDB_E_CLASSNOTREG:
      reason = "The CLSID corresponding to the class string was not found in the registry.";
      break;
    case REGDB_E_READREGDB:
      reason = "The registry could not be opened for reading.";
      break;
  }
  if (NOERROR != hr)
  {
    throw std::runtime_error(error + reason);
  }
  return clsid;
}

// Everything is const here.
CLSID_Name::CLSID_Name(
  char const* const cszCLSID,
  char const* const name1,
  char const* const name2
)
  : clsid(makeCLSID(cszCLSID))
  , name(name2 ? name2 : (name1 ? name1 : "null"))
{
  assert(nullptr != name1); // Better late than never.
}

CLSID_Name::CLSID_Name(
  CLSID const& cCLSID,
  char const* const name1,
  char const* const name2
)
  : clsid(cCLSID)
  , name(name2 ? name2 : (name1 ? name1 : "null"))
{}

#define CLSID_INST(name, clsid_string, name2) \
  CLSID_Name const CLSID_##name{ clsid_string, #name, name2 }

CLSID_INST(ThisPC, "{20D04FE0-3AEA-1069-A2D8-08002B30309D}", "This PC");
CLSID_INST(ControlPanel, "{5399E694-6CE5-4D6C-8FCE-1D8870FDCBA0}", "Control Panel");
CLSID_INST(ControlPanelAllTasks, "{ED7BA470-8E54-465E-825C-99712043E01C}", "Control Panel All Tasks");
CLSID_INST(ControlPanelCategoryView, "{26EE0668-A00A-44D7-9371-BEB064C98683}", "Control Panel Category View");
CLSID_INST(ControlPanelIconsView, "{21EC2020-3AEA-1069-A2DD-08002B30309D}", "Control Panel Icons View");
CLSID_INST(InternetExplorer, "{871C5380-42A0-1069-A2EA-08002B30309D}", "Internet Explorer");
CLSID_INST(NetworkWorkGroupPlaces, "{208D2C60-3AEA-1069-A2D7-08002B30309D}", "NetworkWork Group Places");
CLSID_INST(NetworkConnections, "{7007ACC7-3202-11D1-AAD2-00805FC1270E}", "Network Connections");
CLSID_INST(NetworkConnections2, "{992CFFA0-F557-101A-88EC-00DD010CCC48}", "Network Connections2");

//static const CLSID{ 0x, 0x, 0x, { 0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x } };

#define FOLDERID_1(name) CLSID_Name{ FOLDERID_##name, #name }
#define FOLDERID_2(name1, desc) CLSID_Name{ FOLDERID_##name1, #name1, desc }
#define CLSID_NAME(x) CLSID_##x

static std::vector<CLSID_Name> const clsid_names
{
  FOLDERID_2(AddNewPrograms, "Add New Programs"),
  FOLDERID_2(AdminTools, "Admin Tools"),
  FOLDERID_2(LocalAppDataLow, "Local AppData Low"),
  FOLDERID_2(ApplicationShortcuts, "Application Shortcuts"),
  FOLDERID_2(AppsFolder, "Applications Folder"),
  FOLDERID_2(AppUpdates, "Application Updates"),
  FOLDERID_2(CDBurning, "CD Burning"),
  FOLDERID_2(ChangeRemovePrograms, "Change Remove Programs"),
  FOLDERID_2(CommonAdminTools, "Common Admin Tools"),
  FOLDERID_2(CommonOEMLinks, "Common OEM Links"),
  FOLDERID_2(CommonPrograms, "Common Programs"),
  FOLDERID_2(CommonStartMenu, "Common Start Menu"),
  FOLDERID_2(CommonStartup, "Common Startup"),
  FOLDERID_2(CommonTemplates, "Common Templates"),
  FOLDERID_2(ComputerFolder, "Computer Folder"),
  FOLDERID_2(ConflictFolder, "Conflict Folder"),
  FOLDERID_2(ConnectionsFolder, "Connections Folder"),
  FOLDERID_2(Contacts, "Contacts"),
  FOLDERID_2(ControlPanelFolder, "Control Panel Folder"),
  FOLDERID_2(Cookies, "Cookies"),
  FOLDERID_2(Desktop, "Desktop"),
  FOLDERID_2(Documents, "Documents"),
  FOLDERID_2(DocumentsLibrary, "Documents Library"),
  FOLDERID_2(Downloads, "Downloads"),
  FOLDERID_2(Favorites, "Favorites"),
  FOLDERID_2(Fonts, "Fonts"),
  FOLDERID_2(Games, "Games"),
  FOLDERID_2(GameTasks, "Game Tasks"),
  FOLDERID_2(History, "History"),
  FOLDERID_2(HomeGroup, "Home Group"),
  FOLDERID_2(HomeGroupCurrentUser, "Home Group CurrentUser"),
  FOLDERID_2(ImplicitAppShortcuts, "Implicit Application Shortcuts"),
  FOLDERID_2(InternetCache, "Internet Cache"),
  FOLDERID_2(InternetFolder, "Internet Folder"),
  FOLDERID_2(Libraries, "Libraries"),
  FOLDERID_2(Links, "Links"),
  FOLDERID_2(LocalAppData, "AppData Local"),
  FOLDERID_2(LocalAppDataLow, "AppData Local Low"),
  FOLDERID_2(LocalizedResourcesDir, "Localized Resources Dir"),
  FOLDERID_2(Music, "Music"),
  FOLDERID_2(MusicLibrary, "Music Library"),
  FOLDERID_2(NetHood, "Network Shortcuts"),
  FOLDERID_2(NetworkFolder, "Network"),
  FOLDERID_2(OriginalImages, "Original Images"),
  FOLDERID_2(PhotoAlbums, "Photo Albums"),
  FOLDERID_2(Pictures, "Pictures"),
  FOLDERID_2(PicturesLibrary, "Pictures Library"),
  FOLDERID_2(Playlists, "Play Lists"),
  FOLDERID_2(PrintersFolder, "Printers"),
  FOLDERID_2(PrintHood, "Printer Shortcuts"),
  FOLDERID_2(Profile, "Profile"),
  FOLDERID_2(ProgramData, "Program Data"),
  FOLDERID_2(ProgramFiles, "Program Files"),
  FOLDERID_2(ProgramFilesCommon, "Program Files Common"),
  FOLDERID_2(ProgramFilesCommonX64, "Program Files Common X64"),
  FOLDERID_2(ProgramFilesCommonX86, "Program Files Common X86"),
  FOLDERID_2(ProgramFilesX64, "Program Files X64"),
  FOLDERID_2(ProgramFilesX86, "Program Files X86"),
  FOLDERID_2(Programs, "Programs"),
  FOLDERID_2(Public, "Public"),
  FOLDERID_2(PublicDesktop, "Public Desktop"),
  FOLDERID_2(PublicDocuments, "Public Documents"),
  FOLDERID_2(PublicDownloads, "Public Downloads"),
  FOLDERID_2(PublicGameTasks, "Public Game Tasks"),
  FOLDERID_2(PublicLibraries, "Public Libraries"),
  FOLDERID_2(PublicMusic, "Public Music"),
  FOLDERID_2(PublicPictures, "Public Pictures"),
  FOLDERID_2(PublicUserTiles, "Public Account Pictures"),
  FOLDERID_2(PublicRingtones, "Ringtones"),
  FOLDERID_2(PublicVideos, "Public Videos"),
  FOLDERID_2(QuickLaunch, "Quick Launch"),
  FOLDERID_2(Recent, "Recent"),
  //CLSID_NAME(RecordedTV),
  FOLDERID_2(RecordedTVLibrary, "Recorded TV Library"),
  FOLDERID_2(RecycleBinFolder, "Recycle Bin"),
  FOLDERID_2(ResourceDir, "Resources"),
  FOLDERID_2(Ringtones, "Ring Tones"),
  FOLDERID_2(RoamingAppData, "AppData Roaming"),
  FOLDERID_2(RoamingTiles, "Roaming Tiles"),
  FOLDERID_2(SampleMusic, "Sample Music"),
  FOLDERID_2(SamplePictures, "Sample Pictures"),
  FOLDERID_2(SamplePlaylists, "Sample Play Lists"),
  FOLDERID_2(SampleVideos, "Sample Videos"),
  FOLDERID_2(SavedGames, "Saved Games"),
  FOLDERID_2(SavedSearches, "Saved Searches"),
  FOLDERID_2(SEARCH_CSC, "Offline Files"),
  FOLDERID_2(SEARCH_MAPI, "Microsoft Office Outlook"),
  FOLDERID_2(SearchHome, "Search Results"),
  FOLDERID_2(SearchTemplates, "Search Templates"),
  FOLDERID_2(SendTo, "SendTo"),
  FOLDERID_2(SidebarDefaultParts, "Sidebar Default Parts"),
  FOLDERID_2(SidebarParts, "Sidebar Parts"),
  FOLDERID_2(StartMenu, "Start Menu"),
  FOLDERID_2(Startup, "Startup"),
  FOLDERID_2(SyncManagerFolder, "Sync Center"),
  FOLDERID_2(SyncResultsFolder, "Sync Results"),
  FOLDERID_2(SyncSetupFolder, "Sync Setup"),
  FOLDERID_2(System, "System32"),
  FOLDERID_2(SystemX86, "System32 x864"),
  FOLDERID_2(Templates, "Templates"),
  //CLSID_NAME(TreeProperties),
  FOLDERID_2(UserPinned, "User Pinned"),
  FOLDERID_2(UserProfiles, "User Profiles"),
  FOLDERID_2(UserProgramFiles, "User Program Files"),
  FOLDERID_2(UserProgramFilesCommon, "User Program Files Common"),
  FOLDERID_2(UsersFiles, "User's Files"),
  FOLDERID_2(UsersLibraries, "User's Libraries"),
  //CLSID_NAME(UsersLibrariesFolder),
  FOLDERID_2(PublicVideos, "Public Videos"),
  FOLDERID_2(RoamingTiles, "Roaming Tiles"),
  FOLDERID_2(Videos, "Videos"),
  FOLDERID_2(VideosLibrary, "Videos Library"),
  FOLDERID_2(Windows, "Windows"),
  FOLDERID_2(Games, "Games"),

  // Other CLSIDs
  CLSID_NAME(ThisPC),
  CLSID_NAME(ControlPanel),
  CLSID_NAME(ControlPanelAllTasks),
  CLSID_NAME(ControlPanelCategoryView),
  CLSID_NAME(ControlPanelIconsView),

  //CLSID_NAME(DeviceMetadataStore),
  //CLSID_NAME(InternetExplorer),
  CLSID_NAME(NetworkWorkGroupPlaces),
  CLSID_NAME(NetworkConnections),

  //CLSID_NAME(Printers_and_Faxes),
  //CLSID_NAME(Dial_up_Connection),
  //CLSID_NAME(Show_Desktop),
  //CLSID_NAME(Users),
  //CLSID_NAME(Window_Switcher),
  //CLSID_NAME(CD_Burner),
  //CLSID_NAME(CSC_CLS),
  //CLSID_NAME(Search),
  //CLSID_NAME(Help_and_Support),
  //CLSID_NAME(Windows_Security),
  //CLSID_NAME(Run),
  //CLSID_NAME(Email),
  //CLSID_NAME(Set_Program_Access),
  //CLSID_NAME(Start_Menu_Provider),
  //CLSID_NAME(Start_Menu),
  //CLSID_NAME(Search_Results),
};

LnkDllPort CLSID_Name const gCLSID_Invalid(CLSID_NULL, "Invalid");

// Makes this call possible CLSID_Names()[
//
CLSID_Name const& CLSID_Names::operator[](CLSID const& clsid) const
{
  // Linear search here. It's ok for now.
  // If it becomes an issue just sort and binary search or use a map.
  for (CLSID_Name const& clsid_name : clsid_names)
  {
    if (clsid_name.clsid == clsid)
      return clsid_name;
  }
  return gCLSID_Invalid;
}

// Singleton created to allow easy access
// to the static collection of CLSID_Name.
LnkDllPort CLSID_Names gCLSID_Names;

LnkDllPort CLSID_Name const& getCLSID_Name(char const* const cszCLSID)
{
  CLSID const clsid(makeCLSID(cszCLSID));
  return gCLSID_Names[clsid];
}

LnkDllPort CLSID_Name const& getCLSID_Name(CLSID const& clsid)
{
  return gCLSID_Names[clsid];
}
