#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

extern crate libc;

// // std library imports
use std::ffi::CStr;
use std::ffi::CString;
use std::ffi::NulError;
use std::os::raw::c_char;
use std::path::PathBuf;
use std::str::Utf8Error;

/* Various kinds of errors that we can get from interfacing with the C talmel interface */
// errors while openeing a file
#[derive(Debug)]
pub enum FileError {
    OpenFailure,
    PathAsString,
    NullPathString(NulError),
    InvalidTagFile,
}

type StringReadResult = Result<String, Utf8Error>;
type MultiStringReadResult = Result<Vec<String>, Utf8Error>;

/* Define a file interface */
#[derive(Debug)]
pub struct TalamelFile {
    file_handle: *mut tml_TalamelFile,
}

impl TalamelFile {
    /* Open a file with tag information */
    pub fn new<P: Into<PathBuf>>(filename: P) -> Result<TalamelFile, FileError> {
        // get the filename as a string, then a c string
        let cs_filename = filename
            .into()
            .to_str()
            .ok_or(FileError::PathAsString)
            .and_then(|filename| {
                CString::new(filename).map_err(|err| FileError::NullPathString(err))
            })?;

        unsafe {
            // try to open the file using the ffi
            let file_ptr = tml_open_file(cs_filename.as_ptr());
            // Todo: Should the struct member be a reference instead?
            if file_ptr.is_null() {
                return Err(FileError::OpenFailure);
            } else {
                // Check to see if the file pointer is valid
                if file_ptr.is_null() {
                    return Err(FileError::InvalidTagFile);
                }
                return Ok(TalamelFile {
                    file_handle: file_ptr,
                });
            }
        }
    }

    fn read_and_parse(c_string_pointer: *mut c_char) -> StringReadResult {
        unsafe {
            let str_slice = CStr::from_ptr(c_string_pointer);
            // try and parse that ptr into a string
            let str_res: StringReadResult = str_slice.to_str().map(|s| s.to_owned());
            // free the pointer - TODO: Make this optional!
            tml_free_str(c_string_pointer);
            // and return the owned string
            str_res
        }
    }

    pub fn title(self: &Self) -> StringReadResult {
        unsafe { Self::read_and_parse(tml_read_title(self.file_handle)) }
    }

    pub fn artist(self: &Self) -> StringReadResult {
        unsafe { Self::read_and_parse(tml_read_artist(self.file_handle)) }
    }

    pub fn bpm(self: &Self) -> Option<u32> {
        unsafe {
            match tml_read_bpm(self.file_handle) {
                0 => None,
                v => Some(v),
            }
        }
    }

    // pub fn bpm(self: &Self) -> Option<u32> {
    //     unsafe {
    //         match taglib_tag_bpm(self.tag) {
    //             0 => None,
    //             v => Some(v),
    //         }
    //     }
    // }
}

impl Drop for TalamelFile {
    fn drop(&mut self) {
        // free the taglib file!
        unsafe {
            tml_free_file(self.file_handle);
        }
    }
}
