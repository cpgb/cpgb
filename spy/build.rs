extern crate cc;

use std::env;

fn main() {
    let mut build = cc::Build::new();
    build.file("capture.c");
    let target = env::var("TARGET").unwrap();
    if !target.contains("windows") {
        panic!("unsupported target");
    }
    build.compile("capture");
//    println!("cargo:rustc-link-lib=static={}", "capture");
}
