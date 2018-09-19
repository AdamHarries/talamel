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

#[derive(Debug)]
pub enum StringError {
    StringAsStr,
    NulPtr,
    Utf8Err(Utf8Error),
    NullString(NulError),
}

type StringReadResult = Result<String, StringError>;
type MultiStringReadResult = Result<Vec<String>, StringError>;

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
            .and_then(|filename| CString::new(filename).map_err(FileError::NullPathString))?;

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
        if c_string_pointer.is_null() {
            return Err(StringError::NulPtr);
        }
        unsafe {
            let str_slice = CStr::from_ptr(c_string_pointer);
            // try and parse that ptr into a string
            let str_res: StringReadResult = str_slice
                .to_str()
                .map(|s| s.to_owned())
                .map_err(StringError::Utf8Err);
            // free the pointer - TODO: Make this optional!
            tml_free_str(c_string_pointer);
            // and return the owned string
            str_res
        }
    }

    pub fn read_property_values<S: Into<String>>(self: &Self, key: S) -> MultiStringReadResult {
        let cs_key = CString::new(key.into().as_str()).map_err(StringError::NullString)?;

        unsafe {
            // count the number of values for the key
            let pcnt = tml_count_property_values(self.file_handle, cs_key.as_ptr());

            let mut v: Vec<String> = Vec::with_capacity(pcnt as usize);
            for i in 0..pcnt {
                let comment = Self::read_and_parse(tml_read_property_value(
                    self.file_handle,
                    cs_key.as_ptr(),
                    i as u32,
                ));
                match comment {
                    Ok(s) => v.push(s),
                    Err(e) => return Err(e),
                }
            }
            return Ok(v);
        }
    }

    // TALAMEL_C_EXPORT unsigned int tml_count_properties(tml_TalamelFile* tf, const char* key);

    // TALAMEL_C_EXPORT char * tml_read_property(tml_TalamelFile* tf, const char* key, unsigned int ix);

    // TALAMEL_C_EXPORT char * tml_read_property_z(tml_TalamelFile* tf, const char* key);

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

    pub fn comments(self: &Self) -> MultiStringReadResult {
        unsafe {
            let comment_count = tml_count_comments(self.file_handle);

            let mut v: Vec<String> = Vec::with_capacity(comment_count as usize);
            for i in 0..comment_count {
                let comment = Self::read_and_parse(tml_get_comment(self.file_handle, i as u32));
                match comment {
                    Ok(s) => v.push(s),
                    Err(e) => return Err(e),
                }
            }
            return Ok(v);
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
