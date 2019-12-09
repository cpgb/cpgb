#include "stdafx.h"

struct MyData
{
    int x;
    int y;
    HWND hFindit;
    DWORD SearchData;
};


static BOOL CALLBACK EnumChildWindowProc(HWND hwnd,LPARAM lParam)
{
    RECT rc;
    struct MyData* p = (struct MyData*)lParam;
    if(!IsWindowVisible(hwnd))return TRUE;
    GetWindowRect(hwnd,&rc);
    if( p->x >= rc.left  && p->y >= rc.top  &&
        p->x <= rc.right &&	p->y <= rc.bottom )
    {
        DWORD d =(DWORD)(rc.right - rc.left);
        if( d < p->SearchData)
        {
            p->hFindit = hwnd;
            p->SearchData = rc.right-rc.left;
        }
    }
    return TRUE;
}


static HWND SearchWindow(HWND hWnd, POINT pt)
{
    struct MyData md = {pt.x, pt.y, 0, -1};
    EnumChildWindows(hWnd, EnumChildWindowProc, (LPARAM)&md);
    if (md.hFindit == NULL || md.hFindit == hWnd)
        return hWnd;
    return SearchWindow(md.hFindit, pt);
}

HWND WINAPI get_window_from_point_ex(POINT pt)
{
    HWND hWnd;
    DWORD style;

    hWnd = WindowFromPoint(pt);
    if (hWnd)
    {
        POINT pt2, ptx;
        ptx.x = ptx.y = 0;

        ClientToScreen(hWnd, &ptx);
        pt2.x = pt.x - ptx.x;
        pt2.y = pt.y - ptx.y;
        HWND hs1 = ChildWindowFromPoint(hWnd, pt2);
        if (hs1)
            hWnd = hs1;
    }
    if (hWnd == NULL)
        return NULL;
    LPARAM lp = MAKELPARAM(pt.x, pt.y);
    ULONG_PTR result = 0;
    style = GetWindowLong(hWnd, GWL_STYLE);
    if ((style & WS_CHILDWINDOW) != 0 &&
        SendMessageTimeout(hWnd, WM_NCHITTEST, 0, lp, SMTO_ABORTIFHUNG, 1000, &result) &&
        result == HTTRANSPARENT)
    {
        hWnd = SearchWindow(GetParent(hWnd), pt);
    }
    return hWnd;
}

void draw_ract(HWND hWnd)
{
    HPEN pen = (HPEN)GetStockObject(WHITE_PEN);
    RECT rc;
    GetWindowRect(hWnd, &rc);
    int w = rc.right - rc.left - 1;
    int h = rc.bottom - rc.top - 1;
    HDC hDC = GetWindowDC(hWnd);
    SetROP2(hDC, R2_XORPEN);
    HPEN hOldPen = (HPEN)SelectObject(hDC, pen);
#define BB 4
    for (int i = 0; i < 4; ++i)
    {
        MoveToEx(hDC, 0, i, 0);
        LineTo(hDC, w + 1, i);
        MoveToEx(hDC, 0, h - i, 0);
        LineTo(hDC, w + 1, h - i);
        MoveToEx(hDC, i, BB, 0);
        LineTo(hDC, i, h - BB + 1);
        MoveToEx(hDC, w - i, BB, 0);
        LineTo(hDC, w - i, h - BB + 1);
    }
    SelectObject(hDC, hOldPen);
    ReleaseDC(hWnd, hDC);
}

