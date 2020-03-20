#pragma once
#include "LnkDllPort.h"


// Standard Windows file system attributes.
// Refer to winnt.h
//
//  FILE_ATTRIBUTE_READONLY,
//  FILE_ATTRIBUTE_HIDDEN,
//  FILE_ATTRIBUTE_SYSTEM,
//  Reserved1,
//  FILE_ATTRIBUTE_DIRECTORY,
//  FILE_ATTRIBUTE_ARCHIVE,
//  Reserved2,  //  FILE_ATTRIBUTE_DEVICE
//  FILE_ATTRIBUTE_NORMAL,
//  FILE_ATTRIBUTE_TEMPORARY,
//  FILE_ATTRIBUTE_SPARSE_//  FILE,
//  FILE_ATTRIBUTE_REPARSE_POINT,
//  FILE_ATTRIBUTE_COMPRESSED,
//  FILE_ATTRIBUTE_OFFLINE,
//  FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,
//  FILE_ATTRIBUTE_ENCRYPTED,
//  FILE_ATTRIBUTE_INTEGRITY_STREAM,
//  FILE_ATTRIBUTE_VIRTUAL,
//  FILE_ATTRIBUTE_NO_SCRUB_DATA,
//  FILE_ATTRIBUTE_EA,
//  FILE_ATTRIBUTE_PINNED,
//  FILE_ATTRIBUTE_UNPINNED,
//  FILE_ATTRIBUTE_RECALL_ON_OPEN,
//  FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS
//

#pragma pack(push, 1)

struct LnkDllPort LinkFileAttributes
{
  // The named flags mentioned by the LNK specification.
  //
  int readOnly : 1;              // The file or directory is read-only. For a file, if this bit is set, applications can read the file but cannot write to it or delete it. For a directory, if this bit is set, applications cannot delete the directory.
  int hidden : 1;                // The file or directory is hidden. If this bit is set, the file or folder is not included in an ordinary directory listing.
  int system : 1;                // The file or directory is part of the operating system or is used exclusively by the operating system.
  int reserved1 : 1;             // A bit that MUST be zero.
  int directory : 1;             // The link target is a directory instead of a file.
  int archive : 1;               // The file or directory is an archive file. Applications use this flag to mark files for backup or removal.
  int reserved2 : 1;             // A bit that MUST be zero.
  int normal : 1;                // The file or directory has no other flags set. If this bit is 1, all other bits in this structure MUST be clear.
  int temporary : 1;             // The file is being used for temporary storage.
  int sparseFile : 1;            // The file is a sparse file.
  int reparsePoint : 1;          // The file or directory has an associated reparse point.
  int compressed : 1;            // The file or directory is compressed. For a file, this means that all data in the file is compressed. For a directory, this means that compression is the default for newly created files and subdirectories.
  int offline : 1;               // The data of the file is not immediately available.
  int notContentIndexed : 1;     // The contents of the file need to be indexed.
  int encrypted : 1;             // The file or directory is encrypted. For a file, this means that all data in the file is encrypted. For a directory, this means that encryption is the default for newly created files and subdirectories.
  int integrity_stream : 1;      // The following flags aren't referred to in the LNK Spec.
  int virtual_file : 1;          // The .LNK format seems to ignore the rest of the file attribute range.
  int no_scrub_data : 1;         //
  int ea : 1;                    //
  int pinned : 1;                //
  int unpinned : 1;              //
  int recall_on_open : 1;        //
  int recall_on_data_access : 1; //
  int reserved3 : 9;             // MUST be zero. 
};

#pragma pack(pop)
