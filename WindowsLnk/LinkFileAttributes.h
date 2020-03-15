#pragma once

enum class LinkFileAttributeBit
{
  eFILE_ATTRIBUTE_READONLY, // The file or directory is read-only. For a file, if this bit is set, applications can read the file but cannot write to it or delete it. For a directory, if this bit is set, applications cannot delete the directory.
  eFILE_ATTRIBUTE_HIDDEN, // The file or directory is hidden. If this bit is set, the file or folder is not included in an ordinary directory listing.
  eFILE_ATTRIBUTE_SYSTEM, // The file or directory is part of the operating system or is used exclusively by the operating system.
  Reserved1, // A bit that MUST be zero.
  eFILE_ATTRIBUTE_DIRECTORY, // The link target is a directory instead of a file.
  eFILE_ATTRIBUTE_ARCHIVE, // The file or directory is an archive file. Applications use this flag to mark files for backup or removal.
  Reserved2, // A bit that MUST be zero.
  eFILE_ATTRIBUTE_NORMAL, // The file or directory has no other flags set. If this bit is 1, all other bits in this structure MUST be clear.
  eFILE_ATTRIBUTE_TEMPORARY, // The file is being used for temporary storage.
  eFILE_ATTRIBUTE_SPARSE_FILE, // The file is a sparse file.
  eFILE_ATTRIBUTE_REPARSE_POINT, // The file or directory has an associated reparse point.
  eFILE_ATTRIBUTE_COMPRESSED, // The file or directory is compressed. For a file, this means that all data in the file is compressed. For a directory, this means that compression is the default for newly created files and subdirectories.
  eFILE_ATTRIBUTE_OFFLINE, // The data of the file is not immediately available.
  eFILE_ATTRIBUTE_NOT_CONTENT_INDEXED, // The contents of the file need to be indexed.
  eFILE_ATTRIBUTE_ENCRYPTED, // The file or directory is encrypted. For a file, this means that all data in the file is encrypted. For a directory, this means that encryption is the default for newly created files and subdirectories.
};

struct LinkFileAttributes
{
  int readOnly : 1;
  int hidden : 1;
  int system : 1;
  int reserved1 : 1;
  int directory : 1;
  int archive : 1;
  int reserved2 : 1;
  int normal : 1;
  int temporary : 1;
  int sparseFile : 1;
  int reparsePoint : 1;
  int compressed : 1;
  int offline : 1;
  int notContentIndexed : 1;
  int encrypted : 1;
  int reserved3 : 17;
};
