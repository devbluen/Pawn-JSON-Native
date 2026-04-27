# ♟️ Pawn JSON Native (Plugin-less)
A lightweight and efficient library to generate and parse JSON data directly in Pawn. This include is designed for developers who need JSON support in SA-MP/open.mp without relying on external plugins (.dll or .so)

# 🪶 How to use examples
## 🍪 Creation
```pawn
new payload[MAX_JSON_LEN];
payload = JSON_Object(
    JSON_AddItem("string", "%s", "string"),
    JSON_AddItem("floating", "%f", 210.12),
    JSON_AddItem("int", "%d", 69),
    JSON_Array("myarray", 
        JSON_AddItemArray("%s", "option1"),
        JSON_AddItemArray("%s", "option2"),
        JSON_AddItemArray("%s", "option3"),
        JSON_AddItemArray("%s", "option4")
    )
);
```
### Result of Payload Variable
```json
{
  "string": "string", 
  "floating": 210.119995, 
  "int": 69, 
  "myarray": [
    "option1", 
    "option2", 
    "option3", 
    "option4"
  ]
}
```

## 📖 Reading
```pawn
new 
    Float:floatingBuff,
    intBuff,
    arrayLength,
    stringBuff[32]
;

JSON_GetString(payload, "string", stringBuff);
JSON_GetFloat(payload, "floating", floatingBuff);
JSON_GetInt(payload, "int", intBuff);
arrayLength = JSON_GetArrayLen(payload, "myarray");

Results:
stringBuff -> string
floatingBuff -> 210.119995
intBuff -> 69
arrayLength -> myArray: 4

for(new i = 0; i < arrayLength; i++) {
    new stringArray[32];
    JSON_GetArrayItem(payload, "myarray", i, stringArray);

    Results:
      - option1
      - option2
      - option3
      - option4
}
```

# 🚀 Natives
```pawn
// Creations
JSON_Object(...);
JSON_Array(const title[], ...);
JSON_AddItem(const title[], const fmt[], {Float, _}:...);
JSON_AddItemArray(const fmt[], {Float, _}:...);

// Readings
bool:JSON_GetInt(const jsonStr[], const key[], &value);
bool:JSON_GetBool(const jsonStr[], const key[], &bool:value);
bool:JSON_GetFloat(const jsonStr[], const key[], &Float:value);
bool:JSON_GetString(const jsonStr[], const key[], value[], const size = sizeof(value));
bool:JSON_HasKey(const jsonStr[], const key[]);
JSON_GetArrayLen(const jsonStr[], const key[]);
bool:JSON_GetArrayItem(const jsonStr[], const key[], index, value[], const size = sizeof(value));
```

# 🤍 Thanks
- Microsoft Copilot and Gemini (Helped with array creation logic and testing)
