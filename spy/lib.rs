#[macro_use]
mod macros;
pub mod windef;
mod raw;

use crate::windef::{HWND, POINT};

pub fn get_window_from_point_ex(pt: POINT) -> HWND {
    unsafe { raw::get_window_from_point_ex(pt) }
}

pub fn draw_react(hwnd: HWND) {
    unsafe { raw::draw_ract(hwnd); }
}
