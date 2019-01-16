# libmod_clipboard
Library to manage the clipboard on Windows for BennuGD.


This is a dll to access to the content of the clipboard and copy it over a string or  the other way, 
copying a string content over the clipboard.


This library have 2 functions:


-- str = CLIPBOARD_READ(); --> Return the content of the clipboard over a string or a error message (now in english!)
in the cases of empty or non valid content (eg: a file).

-- CLIPBOARD_WRITE(str);      --> Copy the content of a string over the clipboard.


Caution, if you want to see the correct visualization of spanish characters is necesary a font file with CP850 codification. 
This library does a conversion "on the fly" of the special characters for spanish like (áéíóú,...), 
but this should not affect to english strings, but carefull if you are working in other languages.


I attach the .dll and a little example program inside the zip file. 

It example need a FNT file with CP850 to work.
