
#if defined _jsonblu_function
	#endinput
#endif
#define _jsonblu_function

            // Macros
#if !defined MAX_JSON_KEY_LEN
    #define MAX_JSON_KEY_LEN 64
#endif

#if !defined MAX_JSON_LEN
    #define MAX_JSON_LEN 512
#endif

			// Includes
#include <YSI_Coding\y_va>



/*

                    .oooooo..o ooooooooooooo   .oooooo.     .oooooo.   oooo    oooo  .oooooo..o 
                    d8P'    `Y8 8'   888   `8  d8P'  `Y8b   d8P'  `Y8b  `888   .8P'  d8P'    `Y8 
                    Y88bo.           888      888      888 888           888  d8'    Y88bo.      
                    `"Y8888o.       888      888      888 888           88888[       `"Y8888o.  
                        `"Y88b      888      888      888 888           888`88b.         `"Y88b 
                    oo     .d8P      888      `88b    d88' `88b    ooo   888  `88b.  oo     .d8P 
                    8""88888P'      o888o      `Y8bood8P'   `Y8bood8P'  o888o  o888o 8""88888P'  

*/

static stock bool:IsNumeric(const szInput[]) {
	new iChar, i = 0;
	while ((iChar = szInput[i++])) if (!('0' <= iChar <= '9')) return false;
	return true;
}

static stock bool:IsFloat(const szInput[]) {
    
    new 
        iChar, 
        i = 0, 
        bool:hasPoint = false, 
        iDigits = 0
    ;

    if (szInput[0] == '-') i++;
    while ((iChar = szInput[i++])) {
        if (iChar == '.') {
            if (hasPoint) 
                return false;
            hasPoint = true;
        } 
        else if ('0' <= iChar <= '9') iDigits++;
        else return false; 
    }
    return (hasPoint && iDigits > 0);
}

static stock bool:SAFE_INDEX(const str[], idx)
    return (idx >= 0 && idx < strlen(str));



/*

                    .oooooo.    oooooooooooo ooooooooooooo  .oooooo..o 
                    d8P'  `Y8b   `888'     `8 8'   888   `8 d8P'    `Y8 
                    888            888              888      Y88bo.      
                    888            888oooo8         888       `"Y8888o.  
                    888     ooooo  888    "         888           `"Y88b 
                    `88.    .88'   888       o      888      oo     .d8P 
                    `Y8bood8P'   o888ooooood8     o888o     8""88888P'  

*/

static stock bool:JSON_GetValueRaw(const jsonStr[], const key[], value[], const size) {
    new keyWithQuotes[64];
    format(keyWithQuotes, sizeof(keyWithQuotes), "\"%s\":", key);

    new pos = strfind(jsonStr, keyWithQuotes, true);
    if (pos == -1) return false;

    pos += strlen(keyWithQuotes);
    
    while (SAFE_INDEX(jsonStr, pos) && (jsonStr[pos] <= ' ')) pos++;
    if (!SAFE_INDEX(jsonStr, pos)) return false;

    new bool:isString = (jsonStr[pos] == '\"');
    new startPos = isString ? (pos + 1) : pos;
    new endPos = startPos;

    if (isString) {
        while (SAFE_INDEX(jsonStr, endPos)) {
            if (jsonStr[endPos] == '\"' && jsonStr[endPos - 1] != '\\') break;
            endPos++;
        }
    } else {
        while (SAFE_INDEX(jsonStr, endPos) && jsonStr[endPos] != ',' && 
               jsonStr[endPos] != '}' && jsonStr[endPos] != ']') {
            endPos++;
        }
    }

    strmid(value, jsonStr, startPos, endPos, size);
    return true;
}

stock bool:JSON_HasKey(const jsonStr[], const key[]) {
    new keyWithQuotes[64];
    format(keyWithQuotes, sizeof(keyWithQuotes), "\"%s\":", key);

    new pos = strfind(jsonStr, keyWithQuotes, true);
    if (pos == -1) return false;
    return true;
}

stock bool:JSON_GetBool(const jsonStr[], const key[], &bool:value) {
    new tmp[10];
    if (!JSON_GetValueRaw(jsonStr, key, tmp, sizeof tmp)) return false;
    
    if (strcmp(tmp, "true", true) == 0) value = true;
    else if (strcmp(tmp, "false", true) == 0) value = false;
    else return false;
    return true;
}

stock bool:JSON_GetInt(const jsonStr[], const key[], &value) {
    new tmp[32];
    if (!JSON_GetValueRaw(jsonStr, key, tmp, sizeof tmp)) return false;
    value = strval(tmp);
    return true;
}

stock bool:JSON_GetFloat(const jsonStr[], const key[], &Float:value) {
    new tmp[32];
    if (!JSON_GetValueRaw(jsonStr, key, tmp, sizeof tmp)) return false;
    value = floatstr(tmp);
    return true;
}

stock bool:JSON_GetString(const jsonStr[], const key[], value[], const size = sizeof(value))
    return JSON_GetValueRaw(jsonStr, key, value, size);

stock JSON_GetArrayLen(const jsonStr[], const key[]) {
    new keyWithQuotes[64];
    format(keyWithQuotes, sizeof(keyWithQuotes), "\"%s\":", key);

    new pos = strfind(jsonStr, keyWithQuotes, true);
    if (pos == -1) return 0;

    pos += strlen(keyWithQuotes);
    
    while (jsonStr[pos] && jsonStr[pos] <= ' ') pos++;
    if (jsonStr[pos] != '[') return 0;
    
    pos++;
    
    new count = 0;
    while (jsonStr[pos] != '\0') {
        while (jsonStr[pos] && (jsonStr[pos] <= ' ' || jsonStr[pos] == ',')) pos++;
        if (jsonStr[pos] == ']' || jsonStr[pos] == '\0') break;

        count++;

        new depth = 0;
        new bool:inQuotes = false;

        while (jsonStr[pos] != '\0') {
            if (jsonStr[pos] == '\"' && (pos == 0 || jsonStr[pos-1] != '\\')) {
                inQuotes = !inQuotes;
            }

            if (!inQuotes) {
                if (jsonStr[pos] == '{' || jsonStr[pos] == '[') depth++;
                else if (jsonStr[pos] == '}' || jsonStr[pos] == ']') depth--;

                if (depth == 0 && (jsonStr[pos] == ',' || jsonStr[pos] == ']')) break;
                if (depth < 0) break; 
            }
            pos++;
        }
    }
    return count;
}

stock bool:JSON_GetArrayItem(const jsonStr[], const key[], index, value[], const size = sizeof(value)) {
    new keyWithQuotes[64];
    format(keyWithQuotes, sizeof(keyWithQuotes), "\"%s\":", key);

    new pos = strfind(jsonStr, keyWithQuotes, true);
    if (pos == -1) return false;

    pos += strlen(keyWithQuotes);
    while (SAFE_INDEX(jsonStr, pos) && jsonStr[pos] <= ' ') pos++;
    
    if (jsonStr[pos] != '[') return false;

    pos++;
    new count = 0;

    while (SAFE_INDEX(jsonStr, pos)) {
        while (SAFE_INDEX(jsonStr, pos) && (jsonStr[pos] <= ' ' || jsonStr[pos] == ',')) pos++;
        
        if (jsonStr[pos] == ']' || jsonStr[pos] == '\0') break;

        new start = pos;
        new depth = 0;
        new bool:inString = false;

        while (SAFE_INDEX(jsonStr, pos)) {
            if (jsonStr[pos] == '\"' && (pos == 0 || jsonStr[pos-1] != '\\')) {
                inString = !inString;
            }

            if (!inString) {
                if (jsonStr[pos] == '{' || jsonStr[pos] == '[') depth++;
                else if (jsonStr[pos] == '}' || jsonStr[pos] == ']') {
                    if (depth == 0 && jsonStr[pos] == ']') break; 
                    depth--;
                }
                
                if (depth == 0 && jsonStr[pos] == ',') break;
            }
            pos++;
        }

        if (count == index) {
            strmid(value, jsonStr, start, pos, size);
            
            if (value[0] == '\"') {
                new len = strlen(value);
                if (len > 1 && value[len-1] == '\"') value[len-1] = '\0';
                strdel(value, 0, 1);
            }
            for(new s = strlen(value) - 1; s >= 0 && value[s] <= ' '; s--) value[s] = '\0';
            
            return true;
        }
        
        count++;
        if (jsonStr[pos] == ']') break;
        pos++; 
    }
    return false;
}



/*

                    .oooooo.   ooooooooo.   oooooooooooo       .o.       ooooooooooooo ooooo   .oooooo.   ooooo      ooo 
                    d8P'  `Y8b  `888   `Y88. `888'     `8      .888.      8'   888   `8 `888'  d8P'  `Y8b  `888b.     `8' 
                    888           888   .d88'  888             .8"888.          888       888  888      888  8 `88b.    8  
                    888           888ooo88P'   888oooo8       .8' `888.         888       888  888      888  8   `88b.  8  
                    888           888`88b.     888    "      .88ooo8888.        888       888  888      888  8     `88b.8  
                    `88b    ooo   888  `88b.   888       o  .8'     `888.       888       888  `88b    d88'  8       `888  
                    `Y8bood8P'  o888o  o888o o888ooooood8 o88o     o8888o     o888o     o888o  `Y8bood8P'  o8o        `8  

*/

stock JSON_AddString(const title[], const value[]) {
    new buffer[MAX_JSON_LEN];
    format(buffer, sizeof(buffer), "\"%s\": \"%s\"", title, value);
    return buffer;
}

stock JSON_AddInt(const title[], value) {
    new buffer[MAX_JSON_LEN];
    format(buffer, sizeof(buffer), "\"%s\": %d", title, value);
    return buffer;
}

stock JSON_AddBool(const title[], bool:value) {
    new buffer[MAX_JSON_LEN];
    format(buffer, sizeof(buffer), "\"%s\": %s", title, value ? "true" : "false");
    return buffer;
}

stock JSON_AddFloat(const title[], Float:value) {
    new buffer[MAX_JSON_LEN];
    format(buffer, sizeof(buffer), "\"%s\": %.2f", title, value);
    return buffer;
}

stock JSON_AddObject(const title[], const value[]) {
    new buffer[MAX_JSON_LEN];
    format(buffer, sizeof(buffer), "\"%s\": {%s}", title, value);
    return buffer;
}

stock JSON_AddArrayString(const value[]) {
    new buffer[MAX_JSON_LEN];
    format(buffer, sizeof(buffer), "\"%s\"", value);
    return buffer;
}

stock JSON_AddArrayFloat(Float:value) {
    new buffer[MAX_JSON_LEN];
    format(buffer, sizeof(buffer), "%f", value);
    return buffer;
}

stock JSON_AddArrayBool(bool:value) {
    new buffer[MAX_JSON_LEN];
    format(buffer, sizeof(buffer), "%s", value ? "true" : "false");
    return buffer;
}

stock JSON_AddArrayInt(value) {
    new buffer[MAX_JSON_LEN];
    format(buffer, sizeof(buffer), "%d", value);
    return buffer;
}

stock JSON_AddArray(const title[], {Float, _}:...) {
    new 
        buffer[MAX_JSON_LEN],
        content[MAX_JSON_LEN],
        num = numargs()
    ;

    content[0] = '\0';

    for(new i = 1; i < num; i++) {
        new argStr[256];
        
        new j = 0;
        while(j < sizeof(argStr) - 1) {
            new chr = getarg(i, j);
            argStr[j] = chr;
            if(chr == 0) break;
            j++;
        }

        strcat(content, argStr);

        if(i < num - 1) 
            strcat(content, ", ");
    }

    format(buffer, sizeof(buffer), "\"%s\": [%s]", title, content);
    return buffer;
}

stock JSON_Object(...) {
    new 
        dest[MAX_JSON_LEN],
        maxlength = sizeof dest,
        num = numargs(),
        cur_len = 0
    ;

    if (cur_len < maxlength - 1)
        dest[cur_len++] = '{';

    for(new i = 0; i < num; i++) {
        if (i > 0 && cur_len < maxlength - 2) {
            dest[cur_len++] = ',';
            dest[cur_len++] = ' ';
        }

        new index = 0;
        while(cur_len < maxlength - 2) {
            new chari = getarg(i, index++);
            if (chari == 0) break; 
            
            dest[cur_len++] = chari;
        }
    }

    if (cur_len < maxlength - 1)
        dest[cur_len++] = '}';

    dest[cur_len] = '\0'; 
    return dest; 
}

stock JSON_Array(...) {
    new 
        dest[MAX_JSON_LEN],
        maxlength = sizeof dest,
        num = numargs(),
        cur_len = 0
    ;

    if (cur_len < maxlength - 1)
        dest[cur_len++] = '[';

    for(new i = 0; i < num; i++) {
        if (i > 0 && cur_len < maxlength - 2) {
            dest[cur_len++] = ',';
            dest[cur_len++] = ' ';
        }

        new index = 0;
        while(cur_len < maxlength - 2) {
            new chari = getarg(i, index++);
            if (chari == 0) break; 
            
            dest[cur_len++] = chari;
        }
    }

    if (cur_len < maxlength - 1)
        dest[cur_len++] = ']';

    dest[cur_len] = '\0'; 
    return dest; 
}
