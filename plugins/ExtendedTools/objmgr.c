/*
 * Copyright (c) 2022 Winsider Seminars & Solutions, Inc.  All rights reserved.
 *
 * This file is part of System Informer.
 *
 * Authors:
 *
 *     dmex    2016-2022
 *
 */

#include "exttools.h"

static PH_STRINGREF RootDirectoryObject = PH_STRINGREF_INIT(L"\\"); // RtlNtPathSeperatorString

typedef struct _OBJECT_ENTRY
{
    PPH_STRING Name;
    PPH_STRING TypeName;
} OBJECT_ENTRY, *POBJECT_ENTRY;

typedef struct _OBJ_CONTEXT
{
    HWND ListViewHandle;
    HWND TreeViewHandle;
    PH_LAYOUT_MANAGER LayoutManager;

    HTREEITEM RootTreeObject;
    HTREEITEM SelectedTreeItem;

    HIMAGELIST TreeImageList;
    HIMAGELIST ListImageList;
} OBJ_CONTEXT, *POBJ_CONTEXT;

typedef struct _DIRECTORY_ENUM_CONTEXT
{
    HWND TreeViewHandle;
    HTREEITEM RootTreeItem;
    PH_STRINGREF DirectoryPath;
} DIRECTORY_ENUM_CONTEXT, *PDIRECTORY_ENUM_CONTEXT;

NTSTATUS EnumDirectoryObjects(
    _In_ HWND TreeViewHandle,
    _In_ HTREEITEM RootTreeItem,
    _In_ PH_STRINGREF DirectoryPath
    );

PPH_STRING GetSelectedTreeViewPath(
    _In_ POBJ_CONTEXT Context
    )
{
    HTREEITEM treeItem;
    PPH_STRING treePath = NULL;
    WCHAR buffer[_MAX_FNAME] = L"";

    treeItem = Context->SelectedTreeItem;

    while (treeItem != Context->RootTreeObject)
    {
        TVITEM tvItem;
        tvItem.mask = TVIF_PARAM | TVIF_HANDLE | TVIF_TEXT;
        tvItem.hItem = treeItem;
        tvItem.pszText = buffer;
        tvItem.cchTextMax = ARRAYSIZE(buffer);

        if (!TreeView_GetItem(Context->TreeViewHandle, &tvItem))
            break;

        if (treePath)
            treePath = PhaConcatStrings(3, buffer, RtlNtPathSeperatorString.Buffer, treePath->Buffer);
        else
            treePath = PhaCreateString(buffer);

        treeItem = TreeView_GetParent(Context->TreeViewHandle, treeItem);
    }

    return PhConcatStrings2(RtlNtPathSeperatorString.Buffer, PhGetStringOrEmpty(treePath));
}

HTREEITEM TreeViewAddItem(
    _In_ HWND TreeViewHandle,
    _In_ HTREEITEM Parent,
    _In_ BOOLEAN Expanded,
    _In_ PPH_STRINGREF Text
    )
{
    TV_INSERTSTRUCT insert;

    memset(&insert, 0, sizeof(TV_INSERTSTRUCT));

    insert.item.mask = TVIF_TEXT | TVIF_STATE;
    insert.hInsertAfter = TVI_LAST;
    insert.hParent = Parent;
    insert.item.pszText = Text->Buffer;

    if (Expanded)
    {
        insert.item.state = insert.item.stateMask = TVIS_EXPANDED;
    }

    return TreeView_InsertItem(TreeViewHandle, &insert);
}

HTREEITEM TreeViewFindItem(
    _In_ HWND TreeViewHandle,
    _In_ HTREEITEM ParentTreeItem,
    _In_ PWSTR Name
    )
{
    TVITEM treeItem;

    for (
        treeItem.hItem = TreeView_GetChild(TreeViewHandle, ParentTreeItem);
        treeItem.hItem;
        treeItem.hItem = TreeView_GetNextSibling(TreeViewHandle, treeItem.hItem)
        )
    {
        WCHAR itemText[_MAX_FNAME] = L"";

        treeItem.mask = TVIF_TEXT | TVIF_CHILDREN;
        treeItem.pszText = itemText;
        treeItem.cchTextMax = ARRAYSIZE(itemText);

        if (TreeView_GetItem(TreeViewHandle, &treeItem))
        {
            if (PhEqualStringZ(treeItem.pszText, Name, TRUE))
            {
                return treeItem.hItem;
            }

            if (treeItem.cChildren)
            {
                HTREEITEM treeChildren;

                if (treeChildren = TreeViewFindItem(TreeViewHandle, treeItem.hItem, Name))
                {
                    return treeChildren;
                }
            }
        }
    }

    return NULL;
}

VOID InitializeTreeImages(
    _In_ POBJ_CONTEXT Context
    )
{
    HICON icon;
    LONG dpiValue;

    dpiValue = PhGetWindowDpi(Context->TreeViewHandle);

    Context->TreeImageList = PhImageListCreate(
        PhGetDpi(24, dpiValue),
        PhGetDpi(24, dpiValue),
        ILC_MASK | ILC_COLOR32,
        1, 1
        );

    if (icon = PhLoadIcon(
        PluginInstance->DllBase,
        MAKEINTRESOURCE(IDI_FOLDER),
        PH_LOAD_ICON_SIZE_LARGE,
        PhGetDpi(16, dpiValue),
        PhGetDpi(16, dpiValue),
        dpiValue
        ))
    {
        PhImageListAddIcon(Context->TreeImageList, icon);
        DestroyIcon(icon);
    }
}

VOID InitializeListImages(
    _In_ POBJ_CONTEXT Context
    )
{
    HICON icon;
    LONG dpiValue;
    LONG size;

    dpiValue = PhGetWindowDpi(Context->TreeViewHandle);
    size = PhGetDpi(24, dpiValue);
    
    Context->ListImageList = PhImageListCreate(
        size,
        size,
        ILC_MASK | ILC_COLOR32,
        10, 10
        );

    icon = PhLoadIcon(PluginInstance->DllBase, MAKEINTRESOURCE(IDI_UNKNOWN), PH_LOAD_ICON_SIZE_LARGE, size, size, dpiValue);
    PhImageListAddIcon(Context->ListImageList, icon);
    DestroyIcon(icon);

    icon = PhLoadIcon(PluginInstance->DllBase, MAKEINTRESOURCE(IDI_MUTANT), PH_LOAD_ICON_SIZE_LARGE, size, size, dpiValue);
    PhImageListAddIcon(Context->ListImageList, icon);
    DestroyIcon(icon);

    icon = PhLoadIcon(PluginInstance->DllBase, MAKEINTRESOURCE(IDI_DRIVER), PH_LOAD_ICON_SIZE_LARGE, size, size, dpiValue);
    PhImageListAddIcon(Context->ListImageList, icon);
    DestroyIcon(icon);

    icon = PhLoadIcon(PluginInstance->DllBase, MAKEINTRESOURCE(IDI_SECTION), PH_LOAD_ICON_SIZE_LARGE, size, size, dpiValue);
    PhImageListAddIcon(Context->ListImageList, icon);
    DestroyIcon(icon);

    icon = PhLoadIcon(PluginInstance->DllBase, MAKEINTRESOURCE(IDI_LINK), PH_LOAD_ICON_SIZE_LARGE, size, size, dpiValue);
    PhImageListAddIcon(Context->ListImageList, icon);
    DestroyIcon(icon);

    icon = PhLoadIcon(PluginInstance->DllBase, MAKEINTRESOURCE(IDI_KEY), PH_LOAD_ICON_SIZE_LARGE, size, size, dpiValue);
    PhImageListAddIcon(Context->ListImageList, icon);
    DestroyIcon(icon);

    icon = PhLoadIcon(PluginInstance->DllBase, MAKEINTRESOURCE(IDI_PORT), PH_LOAD_ICON_SIZE_LARGE, size, size, dpiValue);
    PhImageListAddIcon(Context->ListImageList, icon);
    DestroyIcon(icon);

    icon = PhLoadIcon(PluginInstance->DllBase, MAKEINTRESOURCE(IDI_SESSION), PH_LOAD_ICON_SIZE_LARGE, size, size, dpiValue);
    PhImageListAddIcon(Context->ListImageList, icon);
    DestroyIcon(icon);

    icon = PhLoadIcon(PluginInstance->DllBase, MAKEINTRESOURCE(IDI_EVENT), PH_LOAD_ICON_SIZE_LARGE, size, size, dpiValue);
    PhImageListAddIcon(Context->ListImageList, icon);
    DestroyIcon(icon);

    icon = PhLoadIcon(PluginInstance->DllBase, MAKEINTRESOURCE(IDI_DEVICE), PH_LOAD_ICON_SIZE_LARGE, size, size, dpiValue);
    PhImageListAddIcon(Context->ListImageList, icon);
    DestroyIcon(icon);
}

static BOOLEAN NTAPI EnumDirectoryObjectsCallback(
    _In_ PPH_STRINGREF Name,
    _In_ PPH_STRINGREF TypeName,
    _In_ PVOID Context
    )
{
    PDIRECTORY_ENUM_CONTEXT context = (PDIRECTORY_ENUM_CONTEXT)Context;

    if (PhEqualStringZ(TypeName->Buffer, L"Directory", TRUE))
    {
        PPH_STRING childDirectoryPath;
        HTREEITEM currentItem;

        if (PhEqualStringRef(&context->DirectoryPath, &RootDirectoryObject, TRUE))
        {
            childDirectoryPath = PhConcatStringRef2(
                &context->DirectoryPath,
                Name
                );
        }
        else
        {
            childDirectoryPath = PhConcatStrings(
                3,
                context->DirectoryPath.Buffer,
                RtlNtPathSeperatorString.Buffer,
                Name->Buffer
                );
        }

        currentItem = TreeViewAddItem(
            context->TreeViewHandle,
            context->RootTreeItem,
            FALSE,
            Name
            );

        // Recursively enumerate all subdirectories.
        EnumDirectoryObjects(
            context->TreeViewHandle,
            currentItem,
            PhGetStringRef(childDirectoryPath)
            );

        PhDereferenceObject(childDirectoryPath);
    }

    return TRUE;
}

static BOOLEAN NTAPI EnumCurrentDirectoryObjectsCallback(
    _In_ PPH_STRINGREF Name,
    _In_ PPH_STRINGREF TypeName,
    _In_ PVOID Context
    )
{
    POBJ_CONTEXT context = (POBJ_CONTEXT)Context;

    if (PhEqualStringZ(TypeName->Buffer, L"Directory", TRUE))
    {
        if (!TreeViewFindItem(context->TreeViewHandle, context->SelectedTreeItem, Name->Buffer))
        {
            TreeViewAddItem(context->TreeViewHandle, context->SelectedTreeItem, TRUE, Name);
        }
    }
    else
    {
        INT index;
        INT imageIndex;
        POBJECT_ENTRY entry;

        entry = PhAllocateZero(sizeof(OBJECT_ENTRY));
        entry->Name = PhCreateString2(Name);
        entry->TypeName = PhCreateString2(TypeName);

        if (PhEqualStringRef2(TypeName, L"ALPC Port", TRUE))
        {
            imageIndex = 6;
        }
        else if (PhEqualStringRef2(TypeName, L"Device", TRUE))
        {
            imageIndex = 2;
        }
        else if (PhEqualStringRef2(TypeName, L"Driver", TRUE))
        {
            imageIndex = 9;
        }
        else if (PhEqualStringRef2(TypeName, L"Event", TRUE))
        {
            imageIndex = 8;
        }
        else if (PhEqualStringRef2(TypeName, L"Key", TRUE))
        {
            imageIndex = 5;
        }
        else if (PhEqualStringRef2(TypeName, L"Mutant", TRUE))
        {
            imageIndex = 1;
        }
        else if (PhEqualStringRef2(TypeName, L"Section", TRUE))
        {
            imageIndex = 3;
        }
        else if (PhEqualStringRef2(TypeName, L"Session", TRUE))
        {
            imageIndex = 7;
        }
        else if (PhEqualStringRef2(TypeName, L"SymbolicLink", TRUE))
        {
            imageIndex = 4;
        }
        else
        {
            imageIndex = 0;
        }

        index = PhAddListViewItem(
            context->ListViewHandle,
            MAXINT,
            Name->Buffer,
            entry
            );

        PhSetListViewItemImageIndex(
            context->ListViewHandle,
            index,
            imageIndex
            );

        PhSetListViewSubItem(
            context->ListViewHandle,
            index,
            1,
            TypeName->Buffer
            );
    }

    return TRUE;
}

NTSTATUS EnumDirectoryObjects(
    _In_ HWND TreeViewHandle,
    _In_ HTREEITEM RootTreeItem,
    _In_ PH_STRINGREF DirectoryPath
    )
{
    HANDLE directoryHandle;
    OBJECT_ATTRIBUTES objectAttributes;
    UNICODE_STRING name;

    PhStringRefToUnicodeString(&DirectoryPath, &name);

    InitializeObjectAttributes(
        &objectAttributes,
        &name,
        0,
        NULL,
        NULL
        );

    if (NT_SUCCESS(NtOpenDirectoryObject(
        &directoryHandle,
        DIRECTORY_QUERY,
        &objectAttributes
        )))
    {
        DIRECTORY_ENUM_CONTEXT enumContext;

        enumContext.TreeViewHandle = TreeViewHandle;
        enumContext.RootTreeItem = RootTreeItem;
        enumContext.DirectoryPath = DirectoryPath;

        PhEnumDirectoryObjects(
            directoryHandle,
            EnumDirectoryObjectsCallback,
            &enumContext
            );

        NtClose(directoryHandle);
    }

    TreeView_SortChildren(TreeViewHandle, RootTreeItem, TRUE);

    return STATUS_SUCCESS;
}

NTSTATUS EnumCurrentDirectoryObjects(
    _In_ POBJ_CONTEXT Context,
    _In_ PH_STRINGREF DirectoryPath
    )
{
    HANDLE directoryHandle;
    OBJECT_ATTRIBUTES objectAttributes;
    UNICODE_STRING name;

    PhStringRefToUnicodeString(&DirectoryPath, &name);

    InitializeObjectAttributes(
        &objectAttributes,
        &name,
        0,
        NULL,
        NULL
        );

    if (NT_SUCCESS(NtOpenDirectoryObject(
        &directoryHandle,
        DIRECTORY_QUERY,
        &objectAttributes
        )))
    {
        PhEnumDirectoryObjects(
            directoryHandle,
            EnumCurrentDirectoryObjectsCallback,
            Context
            );

        NtClose(directoryHandle);
    }

    ExtendedListView_SortItems(Context->ListViewHandle);

    return STATUS_SUCCESS;
}

INT NTAPI WinObjNameCompareFunction(
    _In_ PVOID Item1,
    _In_ PVOID Item2,
    _In_opt_ PVOID Context
    )
{
    POBJECT_ENTRY item1 = Item1;
    POBJECT_ENTRY item2 = Item2;

    return PhCompareStringZ(PhGetStringOrEmpty(item1->Name), PhGetStringOrEmpty(item2->Name), TRUE);
}

INT NTAPI WinObjTypeCompareFunction(
    _In_ PVOID Item1,
    _In_ PVOID Item2,
    _In_opt_ PVOID Context
    )
{
    POBJECT_ENTRY item1 = Item1;
    POBJECT_ENTRY item2 = Item2;

    return PhCompareStringZ(PhGetStringOrEmpty(item1->TypeName), PhGetStringOrEmpty(item2->TypeName), TRUE);
}

INT_PTR CALLBACK WinObjDlgProc(
    _In_ HWND hwndDlg,
    _In_ UINT uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
    )
{
    POBJ_CONTEXT context = NULL;

    if (uMsg == WM_INITDIALOG)
    {
        context = PhAllocateZero(sizeof(OBJ_CONTEXT));
        PhSetWindowContext(hwndDlg, PH_WINDOW_CONTEXT_DEFAULT, context);
    }
    else
    {
        context = PhGetWindowContext(hwndDlg, PH_WINDOW_CONTEXT_DEFAULT);
    }

    if (!context)
        return FALSE;

    switch (uMsg)
    {
    case WM_INITDIALOG:
        {
            PhCenterWindow(hwndDlg, PhMainWndHandle);

            context->TreeViewHandle = GetDlgItem(hwndDlg, IDC_OBJMGR_TREE);
            context->ListViewHandle = GetDlgItem(hwndDlg, IDC_OBJMGR_LIST);

            PhSetApplicationWindowIcon(hwndDlg);

            InitializeTreeImages(context);
            InitializeListImages(context);

            PhSetControlTheme(context->TreeViewHandle, L"explorer");
            TreeView_SetExtendedStyle(context->TreeViewHandle, TVS_EX_DOUBLEBUFFER, TVS_EX_DOUBLEBUFFER);
            TreeView_SetImageList(context->TreeViewHandle, context->TreeImageList, TVSIL_NORMAL);
            context->RootTreeObject = TreeViewAddItem(context->TreeViewHandle, TVI_ROOT, TRUE, &RootDirectoryObject);

            PhSetControlTheme(context->ListViewHandle, L"explorer");
            PhSetListViewStyle(context->ListViewHandle, FALSE, FALSE);
            PhSetExtendedListView(context->ListViewHandle);
            ListView_SetImageList(context->ListViewHandle, context->ListImageList, LVSIL_SMALL);

            PhAddListViewColumn(context->ListViewHandle, 0, 0, 0, LVCFMT_LEFT, 620, L"Name");
            PhAddListViewColumn(context->ListViewHandle, 1, 1, 1, LVCFMT_LEFT, 150, L"Type");

            ExtendedListView_SetSortFast(context->ListViewHandle, TRUE);
            ExtendedListView_SetCompareFunction(context->ListViewHandle, 0, WinObjNameCompareFunction);
            ExtendedListView_SetCompareFunction(context->ListViewHandle, 1, WinObjTypeCompareFunction);

            PhInitializeLayoutManager(&context->LayoutManager, hwndDlg);
            PhAddLayoutItem(&context->LayoutManager, context->TreeViewHandle, NULL, PH_ANCHOR_LEFT | PH_ANCHOR_TOP | PH_ANCHOR_BOTTOM);
            PhAddLayoutItem(&context->LayoutManager, context->ListViewHandle, NULL, PH_ANCHOR_ALL);
            PhLoadWindowPlacementFromSetting(SETTING_NAME_OBJMGR_WINDOW_POSITION, SETTING_NAME_OBJMGR_WINDOW_SIZE, hwndDlg);
            PhLoadListViewColumnsFromSetting(SETTING_NAME_OBJMGR_COLUMNS, context->ListViewHandle);

            EnumDirectoryObjects(
                context->TreeViewHandle,
                context->RootTreeObject,
                RootDirectoryObject
                );

            PhInitializeWindowTheme(hwndDlg, !!PhGetIntegerSetting(L"EnableThemeSupport"));

            SendMessage(hwndDlg, WM_NEXTDLGCTL, (WPARAM)context->TreeViewHandle, TRUE);
        }
        break;
    case WM_DESTROY:
        {
            if (context->TreeImageList)
                PhImageListDestroy(context->TreeImageList);
            if (context->ListImageList)
                PhImageListDestroy(context->ListImageList);

            PhSaveWindowPlacementToSetting(SETTING_NAME_OBJMGR_WINDOW_POSITION, SETTING_NAME_OBJMGR_WINDOW_SIZE, hwndDlg);
            PhSaveListViewColumnsToSetting(SETTING_NAME_OBJMGR_COLUMNS, context->ListViewHandle);
            PhDeleteLayoutManager(&context->LayoutManager);

            PhRemoveWindowContext(hwndDlg, PH_WINDOW_CONTEXT_DEFAULT);
            PhFree(context);
        }
        break;
    case WM_SIZE:
        PhLayoutManagerLayout(&context->LayoutManager);
        break;
    case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
            case IDCANCEL:
                EndDialog(hwndDlg, IDOK);
                break;
            }
        }
        break;
    case WM_NOTIFY:
        {
            LPNMHDR header = (LPNMHDR)lParam;

            switch (header->code)
            {
            case TVN_SELCHANGED:
                {
                    context->SelectedTreeItem = TreeView_GetSelection(context->TreeViewHandle);

                    ExtendedListView_SetRedraw(context->ListViewHandle, FALSE);
                    ListView_DeleteAllItems(context->ListViewHandle);

                    if (context->SelectedTreeItem == context->RootTreeObject)
                    {
                        EnumCurrentDirectoryObjects(context, RootDirectoryObject);
                    }
                    else
                    {
                        PPH_STRING selectedPath = GetSelectedTreeViewPath(context);

                        EnumCurrentDirectoryObjects(
                            context,
                            PhGetStringRef(selectedPath)
                            );

                        PhDereferenceObject(selectedPath);
                    }

                    ExtendedListView_SetRedraw(context->ListViewHandle, TRUE);
                }
                break;
            case NM_SETCURSOR:
                {
                    if (header->hwndFrom == context->TreeViewHandle)
                    {
                        HCURSOR cursor = (HCURSOR)LoadImage(
                            NULL,
                            IDC_ARROW,
                            IMAGE_CURSOR,
                            0,
                            0,
                            LR_SHARED
                            );

                        if (cursor != GetCursor())
                        {
                            SetCursor(cursor);
                        }

                        SetWindowLongPtr(hwndDlg, DWLP_MSGRESULT, TRUE);
                        return TRUE;
                    }
                }
                break;
            }
        }
        break;
    case WM_CTLCOLORBTN:
        return HANDLE_WM_CTLCOLORBTN(hwndDlg, wParam, lParam, PhWindowThemeControlColor);
    case WM_CTLCOLORDLG:
        return HANDLE_WM_CTLCOLORDLG(hwndDlg, wParam, lParam, PhWindowThemeControlColor);
    case WM_CTLCOLORSTATIC:
        return HANDLE_WM_CTLCOLORSTATIC(hwndDlg, wParam, lParam, PhWindowThemeControlColor);
    }

    return FALSE;
}
