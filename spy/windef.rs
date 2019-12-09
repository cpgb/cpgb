use std::os::raw::c_long;

DECLARE_HANDLE!{HWND, HWND__}

STRUCT!{struct POINT {
    x: c_long,
    y: c_long,
}}
