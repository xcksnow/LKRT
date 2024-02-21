#pragma once
#include <Windows.h>
#include <ntstatus.h>  
#include <iostream>


using namespace std;


class TimeStompingClass {
public:

    typedef LONG NTSTATUS;

    typedef struct _FILE_BASIC_INFORMATION {
        LARGE_INTEGER CreationTime;
        LARGE_INTEGER LastAccessTime;
        LARGE_INTEGER LastWriteTime;
        LARGE_INTEGER ChangeTime;
        ULONG FileAttributes;
    } FILE_BASIC_INFORMATION, * PFILE_BASIC_INFORMATION;



    typedef enum _FILE_INFORMATION_CLASS {

        FileDirectoryInformation = 1,

        FileFullDirectoryInformation,                   // 2

        FileBothDirectoryInformation,                   // 3

        FileBasicInformation,                           // 4

        FileStandardInformation,                        // 5

        FileInternalInformation,                        // 6

        FileEaInformation,                              // 7

        FileAccessInformation,                          // 8

        FileNameInformation,                            // 9

        FileRenameInformation,                          // 10

        FileLinkInformation,                            // 11

        FileNamesInformation,                           // 12

        FileDispositionInformation,                     // 13

        FilePositionInformation,                        // 14

        FileFullEaInformation,                          // 15

        FileModeInformation,                            // 16

        FileAlignmentInformation,                       // 17

        FileAllInformation,                             // 18

        FileAllocationInformation,                      // 19

        FileEndOfFileInformation,                       // 20

        FileAlternateNameInformation,                   // 21

        FileStreamInformation,                          // 22

        FilePipeInformation,                            // 23

        FilePipeLocalInformation,                       // 24

        FilePipeRemoteInformation,                      // 25

        FileMailslotQueryInformation,                   // 26

        FileMailslotSetInformation,                     // 27

        FileCompressionInformation,                     // 28

        FileObjectIdInformation,                        // 29

        FileCompletionInformation,                      // 30

        FileMoveClusterInformation,                     // 31

        FileQuotaInformation,                           // 32

        FileReparsePointInformation,                    // 33

        FileNetworkOpenInformation,                     // 34

        FileAttributeTagInformation,                    // 35

        FileTrackingInformation,                        // 36

        FileIdBothDirectoryInformation,                 // 37

        FileIdFullDirectoryInformation,                 // 38

        FileValidDataLengthInformation,                 // 39

        FileShortNameInformation,                       // 40

        FileIoCompletionNotificationInformation,        // 41

        FileIoStatusBlockRangeInformation,              // 42

        FileIoPriorityHintInformation,                  // 43

        FileSfioReserveInformation,                     // 44

        FileSfioVolumeInformation,                      // 45

        FileHardLinkInformation,                        // 46

        FileProcessIdsUsingFileInformation,             // 47

        FileNormalizedNameInformation,                  // 48

        FileNetworkPhysicalNameInformation,             // 49

        FileIdGlobalTxDirectoryInformation,             // 50

        FileIsRemoteDeviceInformation,                  // 51

        FileUnusedInformation,                          // 52

        FileNumaNodeInformation,                        // 53

        FileStandardLinkInformation,                    // 54

        FileRemoteProtocolInformation,                  // 55



        //

        //  These are special versions of these operations (defined earlier)

        //  which can be used by kernel mode drivers only to bypass security

        //  access checks for Rename and HardLink operations.  These operations

        //  are only recognized by the IOManager, a file system should never

        //  receive these.

        //

        FileRenameInformationBypassAccessCheck,         // 56

        FileLinkInformationBypassAccessCheck,           // 57

        //

        // End of special information classes reserved for IOManager.

        //



        FileVolumeNameInformation,                      // 58

        FileIdInformation,                              // 59

        FileIdExtdDirectoryInformation,                 // 60

        FileReplaceCompletionInformation,               // 61

        FileHardLinkFullIdInformation,                  // 62

        FileIdExtdBothDirectoryInformation,             // 63

        FileDispositionInformationEx,                   // 64

        FileRenameInformationEx,                        // 65

        FileRenameInformationExBypassAccessCheck,       // 66

        FileDesiredStorageClassInformation,             // 67

        FileStatInformation,                            // 68

        FileMemoryPartitionInformation,                 // 69

        FileStatLxInformation,                          // 70

        FileCaseSensitiveInformation,                   // 71

        FileLinkInformationEx,                          // 72

        FileLinkInformationExBypassAccessCheck,         // 73

        FileStorageReserveIdInformation,                // 74

        FileCaseSensitiveInformationForceAccessCheck,   // 75

        FileKnownFolderInformation,   					// 76

        FileMaximumInformation

    } FILE_INFORMATION_CLASS, * PFILE_INFORMATION_CLASS;



    typedef struct _IO_STATUS_BLOCK {
        union {
            NTSTATUS Status;
            PVOID    Pointer;
        };
        ULONG_PTR Information;
    } IO_STATUS_BLOCK, * PIO_STATUS_BLOCK;

    typedef NTSTATUS(WINAPI* NtQueryInformationFile_t)(HANDLE, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS);
    typedef NTSTATUS(WINAPI* NtSetInformationFile_t)(HANDLE, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS);

    LPCWSTR ConvertToLPCWSTR(const char* charString) {
        int size = MultiByteToWideChar(CP_ACP, 0, charString, -1, NULL, 0);
        wchar_t* wideString = new wchar_t[size];
        MultiByteToWideChar(CP_ACP, 0, charString, -1, wideString, size);

        return wideString;
    }

    bool copyStamps(char* srcfile, char* dstfile) {
        FILE_BASIC_INFORMATION dst_fbi, src_fbi;
        IO_STATUS_BLOCK ioStat;

        // resolve Nt API calls
        NtQueryInformationFile_t pNtQueryInformationFile = (NtQueryInformationFile_t)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtQueryInformationFile");
        if (pNtQueryInformationFile == NULL) {
            return false;
        }
        NtSetInformationFile_t pNtSetInformationFile = (NtSetInformationFile_t)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtSetInformationFile");
        if (pNtSetInformationFile == NULL) {
            return false;
        }

        LPCWSTR src = ConvertToLPCWSTR(srcfile);
        LPCWSTR dst = ConvertToLPCWSTR(dstfile);

        // open destination and source files for information exchange
        HANDLE fileSrc = CreateFile(src, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
        if (fileSrc == INVALID_HANDLE_VALUE) {
            return false;
        }
        HANDLE fileDst = CreateFile(dst, GENERIC_READ | GENERIC_WRITE | FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, 0, NULL);
        if (fileDst == INVALID_HANDLE_VALUE) {
            CloseHandle(fileSrc);
            return false;
        }

        // obtain the source and destination file information
        if (pNtQueryInformationFile(fileSrc, &ioStat, &src_fbi, sizeof(FILE_BASIC_INFORMATION), FileBasicInformation) < 0) {
            CloseHandle(fileSrc);
            CloseHandle(fileDst);
            return false;
        }
        if (pNtQueryInformationFile(fileDst, &ioStat, &dst_fbi, sizeof(FILE_BASIC_INFORMATION), FileBasicInformation) < 0) {
            CloseHandle(fileSrc);
            CloseHandle(fileDst);
            return false;
        }

        // set new timestamp in destination file info block
        dst_fbi.LastWriteTime = src_fbi.LastWriteTime;
        dst_fbi.LastAccessTime = src_fbi.LastAccessTime;
        dst_fbi.ChangeTime = src_fbi.ChangeTime;
        dst_fbi.CreationTime = src_fbi.CreationTime;

        if (pNtSetInformationFile(fileDst, &ioStat, &dst_fbi, sizeof(FILE_BASIC_INFORMATION), FileBasicInformation) < 0) {
            CloseHandle(fileSrc);
            CloseHandle(fileDst);
            return false;
        }

        CloseHandle(fileSrc);
        CloseHandle(fileDst);

        return true;
    }

    
 

private:
    static string XOR(string data, string key) {
        string output = data;
        for (int i = 0; i < data.size(); i++) {
            output[i] = data[i] ^ key[i % key.size()];
        }
        return output;
    }


    


};