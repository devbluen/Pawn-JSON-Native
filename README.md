# 🫧 Pawn JSON Native (Plugin-less)
A lightweight and efficient library to generate and parse JSON data directly in Pawn. This include is designed for developers who need JSON support in SA-MP/open.mp without relying on external plugins (.dll or .so)

# 🪶 How to use examples
## 📝 Creation
```pawn
new payload[MAX_JSON_LEN];
payload = JSON_Object(
    JSON_AddString("string", "string"),
    JSON_AddFloat("floating", 210.12),
    JSON_AddInt("int", 69),
    JSON_AddArray("myarray", 
        JSON_AddArrayString("option1"),
        JSON_AddArrayString("option2"),
        JSON_AddArrayString("option3"),
        JSON_AddArrayString("option4")
    ),
    JSON_AddArray("myobject", 
        JSON_Object(
            JSON_AddInt("slot", 0), 
            JSON_AddInt("item", 22)
        ),
        JSON_Object(
            JSON_AddInt("slot", 1), 
            JSON_AddInt("item", 233)
        )
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
  ],
  "myobject": [
    {
      "slot": 0,
      "item": 22
    },
    {
      "slot": 1,
      "item": 233
    }
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

printf("stringBuff: %s", stringBuff); // Result -> string
printf("floating: %f", floatingBuff); // Result -> 210.110000
printf("intBuff: %d", intBuff); // Result -> 69
printf("arrayLength: %d", arrayLength); // Result -> 4

for(new i = 0; i < arrayLength; i++) {
    new stringArray[32];
    JSON_GetArrayItem(payload, "myarray", i, stringArray);

    printf("stringArray: %s", stringArray); 
    // Result - 1 -> option1
    // Result - 2 -> option2
    // Result - 3 -> option3
    // Result - 4 -> option4
}

arrayLength = JSON_GetArrayLen(payload, "myobject");
for(new i = 0; i < arrayLength; i++) {
    new stringObject[32];
    JSON_GetArrayItem(payload, "myobject", i, stringObject);

    new slotBuff, itemBuff;
    JSON_GetInt(stringObject, "slot", slotBuff);
    JSON_GetInt(stringObject, "item", itemBuff);

    printf("stringObject (%i): Slot: %d | Item: %d", i, slotBuff, itemBuff); 
    // Result - 1 -> Slot: 0 | Item: 22
    // Result - 2 -> Slot: 1 | Item: 233
}
```

# 🍪 Dependencies
- YSI-Includes: https://github.com/pawn-lang/YSI-Includes/releases

# 🚀 Natives
```pawn
// Creations
JSON_Object(...);
JSON_Array(...);
JSON_AddString(const title[], const value[]);
JSON_AddInt(const title[], value);
JSON_AddBool(const title[], bool:value);
JSON_AddFloat(const title[], Float:value);
JSON_AddObject(const title[], const value[]);
JSON_AddArray(const title[], {Float, _}:...);
JSON_AddArrayString(const value[]);
JSON_AddArrayFloat(Float:value);
JSON_AddArrayBool(bool:value);
JSON_AddArrayInt(value);

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
