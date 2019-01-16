/*
Author: David Pozos Cerón
Date: 2018/05/01
Licence: MIT.
*/

import "mod_key"
import "mod_video"
import "mod_text"
import "mod_map"
import "mod_string"

import "mod_clipboard"


Process Main()
PRIVATE
	string str, str2, str3;
	int fileFNT;
END
Begin
	str3 = "Hello World";
    fileFNT = load_fnt("FNT\Blanca_18.fnt");  //You need to load a FNT font file with codification CP850 to represent correctly the spanish characters.
	set_mode(800,600,32, mode_window);
	write_string(fileFNT,20,200,0,&str);
	write_string(fileFNT,20,250,0,&str2);

	while(!(key(_ESC)))
		if (key(_down))
            str = CLIPBOARD_READ();
			str2 = "Copy string  from clipboard.";
		END
		if (key(_up)) 
			CLIPBOARD_WRITE(str3);
   			str2 = "Copy string 'Hello world' (str3) to clipboard.";
		END
		FRAME;
	END
END