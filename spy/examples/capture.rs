extern crate spy;

use spy::windef::POINT;

fn main() {
    let hwnd = spy::get_window_from_point_ex(POINT {
        x: 100,
        y: 100,
    });
    spy::draw_react(hwnd);
}
