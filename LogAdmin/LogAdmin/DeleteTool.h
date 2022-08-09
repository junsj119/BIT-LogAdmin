#pragma once

void SetListView(CListCtrl* ListView);
CString FolderChoice(HWND hWnd, CString strInitPath);
BOOL DeleteDirectoryFile(CString RootDir);

void CheckDisk(CEdit* CFree, CEdit* CPull, CStatic* CDrive, CEdit* DFree, CEdit* DPull, CStatic* DDrive);
void CheckFolder(HWND hwnd, CString Dir, CListCtrl* ListView, CEdit* DirEdit);