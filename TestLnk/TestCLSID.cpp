#include "pch.h"
#include <KnownFolders.h>
#include "CLSID_Name.hpp"


namespace TestCLSID
{
  TEST(CLSID_Names, getCLSID_Name)
  {
    CLSID_Name const& clsid_Docs = getCLSID_Name(FOLDERID_Documents);
    EXPECT_EQ(clsid_Docs.clsid, FOLDERID_Documents) << "FOLDERID_Documents not found.";
    EXPECT_EQ(clsid_Docs.name, std::string("Documents")) << "FOLDERID_Documents not found.";
  }

  TEST(CLSID_Names, getCLSID_Name_missing)
  {
    CLSID_Name const& clsid_Docs = getCLSID_Name(IID_IUnknown);
    EXPECT_EQ(clsid_Docs.clsid, gCLSID_Invalid) << "IID_IUnknown unexpectedly found.";
    EXPECT_EQ(clsid_Docs.name, std::string("Invalid")) << "IID_IUnknown description isn't Invalid as expected.";
  }

}

