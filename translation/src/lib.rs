use std::ffi::*;

#[unsafe(no_mangle)]
pub extern "C" fn translate_text(text: *const c_char) -> *mut c_char {
    let text = unsafe { CStr::from_ptr(text).to_str().unwrap() };
    let text = {
        use tokio::runtime::Runtime;
        let rt = Runtime::new().expect("couldn't create a tokio runtime :(");
        rt.block_on(rust_translate::translate(text, "zh", "en"))
    };
    return CString::new(text.unwrap()).unwrap().into_raw();
}
