/*
Author: David Pozos Cerón
Date: 2018/05/01
Licence: MIT.
*/

#include "bgddl.h"
#include "bgdrtm.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
//Necesaria para el uso de funciones de cadena (string_get(), string_new(), etc)
#include "xstrings.h"

//Definimos un tipo string
typedef char* string;

//áéíóúÁÉÍÓÚÇçÑñ¡¿
//Función encargada de cambiar los carácteres de la codificacion de Bennu al español.
static void check_string_chars_for_clip(char *miString)
{
    int tamano, indice;
    char valor;

    tamano = strlen(miString);
    for (indice = 0; indice < tamano; indice++)
    {
        valor = miString[indice];
        switch(valor)
        {
            case '¤':
            {
                miString[indice] = 'ñ';
                break;
            }
            case '¥':
            {
                miString[indice] = 'Ñ';
                break;
            }
            case '‡':
            {
                miString[indice] = 'ç';
                break;
            }
            case '€':
            {
                miString[indice] = 'Ç';
                break;
            }
            case '­':
            {
                miString[indice] = '¡';
                break;
            }
            case '¨':
            {
                miString[indice] = '¿';
                break;
            }
            case ' ':
            {
                miString[indice] = 'á';
                break;
            }
            case 'µ':
            {
                miString[indice] = 'Á';
                break;
            }
            case '‚':
            {
                miString[indice] = 'é';
                break;
            }
            case '':
            {
                miString[indice] = 'É';
                break;
            }
            case '¡':
            {
                miString[indice] = 'í';
                break;
            }
            case 'Ö':
            {
                miString[indice] = 'Í';
                break;
            }
            case '¢':
            {
                miString[indice] = 'ó';
                break;
            }
            case 'à':
            {
                miString[indice] = 'Ó';
                break;
            }
            case '£':
            {
                miString[indice] = 'ú';
                break;
            }
            case 'é':
            {
                miString[indice] = 'Ú';
                break;
            }
        }
    }
}

//Funcion encargada de cambiar los carácteres del español a codificacion de Bennu.
static void check_string_chars(char *miString)
{
    int tamano, indice;
    char valor;

    tamano = strlen(miString);
    for (indice = 0; indice < tamano; indice++)
    {
        valor = miString[indice];
        switch(valor)
        {
            case 'ñ':
            {
                miString[indice] = 164;
                break;
            }
            case 'Ñ':
            {
                miString[indice] = 164;
                break;
            }
            case 'ç':
            {
                miString[indice] = 135;
                break;
            }
            case 'Ç':
            {
                miString[indice] = 128;
                break;
            }
            case '¡':
            {
                miString[indice] = 173;
                break;
            }
            case '¿':
            {
                miString[indice] = 168;
                break;
            }
            case 'á':
            {
                miString[indice] = 160;
                break;
            }
            case 'Á':
            {
                miString[indice] = 181;
                break;
            }
            case 'é':
            {
                miString[indice] = 130;
                break;
            }
            case 'É':
            {
                miString[indice] = 144;
                break;
            }
            case 'í':
            {
                miString[indice] = 161;
                break;
            }
            case 'Í':
            {
                miString[indice] = 214;
                break;
            }
            case 'ó':
            {
                miString[indice] = 162;
                break;
            }
            case 'Ó':
            {
                miString[indice] = 224;
                break;
            }
            case 'ú':
            {
                miString[indice] = 163;
                break;
            }
            case 'Ú':
            {
                miString[indice] = 233;
                break;
            }
        }
    }
}

//Retorna el contenido del clipboard en caso de ser tipo string, sino retorna una frase de error.
static int * modclip_read_text(INSTANCE * my, int * params)
{
    HANDLE clip;
    int format = 0;
    int texto = 0;
    string text, text1;
    char *error = "Error, clipboard data type isn't a string or it's empty.";

    //Abrimos el clipboard
    if (OpenClipboard(NULL)) {
        //Validamos que tengamos un valor de tipo texto a leer.
        do
        {
            format = EnumClipboardFormats(format);
            if (format == CF_TEXT)
            {
                //¡Texto encontrado!
                texto = CF_TEXT;
            }
        }
        while (format != 0);

        //evaluamos si encontramos un texto en el paso anterior
        if (texto == CF_TEXT)
        {
            clip = GetClipboardData(CF_TEXT);
            CloseClipboard();
            if (clip == NULL)
            {
                text1 = string_new(error);
                return text1;
            }
            else
            {
                text = (char*)clip;
                check_string_chars(text);
                text1 = string_new(text);
                return text1;
            }
        }
        else
        {
            CloseClipboard();
            text1 = string_new(error);
            return text1;
        }
    }
    else
    {
        text1 = string_new(error);
        return text1;
    }
}

static int * modclip_write_text(INSTANCE * my, int * params)
{
        char *myString, *myString2;
        myString = string_get(params[0]);
        const size_t len = strlen(myString)+1;
        myString2 = (char*) malloc((len+1) * sizeof(char));
        strcpy(myString2, myString);
        check_string_chars_for_clip(myString2);

        HANDLE hMem;
        hMem = GlobalAlloc(GMEM_MOVEABLE, len);
        memcpy(GlobalLock(hMem), myString2, len);
        GlobalUnlock(hMem);
        // Set clipboard data
        if (OpenClipboard(NULL))
        {
            EmptyClipboard();
        }
        else
        {
            string_discard(params[0]);
            return -1;
        }
        if (SetClipboardData(CF_TEXT, hMem))
        {
            CloseClipboard();
            string_discard(params[0]);
            return 1;
        }
        else
        {
            string_discard(params[0]);
            return -1;
        }
}

DLSYSFUNCS __bgdexport( mod_clipboard, functions_exports) [] = {
        {"CLIPBOARD_READ", "", TYPE_STRING, modclip_read_text},
        {"CLIPBOARD_WRITE", "S", TYPE_INT, modclip_write_text},
        {0, 0, 0, 0}
};
