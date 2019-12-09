
use crate::windef::{HWND, POINT};

#[link(name = "capture", kind = "static")]
extern "C" {
    pub fn get_window_from_point_ex(pt: POINT) -> HWND;
    pub fn draw_ract(hwnd: HWND);
}

