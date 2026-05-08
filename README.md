# Pseudocode interpreter / translator
This is a pseudocode interpreter wich can be used to execute pseudocode.

It has three main capabilities:
+ Interpreter
+ C++ -> Pseudo
+ Pseudo -> C++

## For VS Code users
### launch.json
```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Debug (Makefile)",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/pseudo.exe",
      "args": [],
      "cwd": "${workspaceFolder}",
      "stopAtEntry": true,
      "externalConsole": false,
      "MIMode": "gdb",
      "preLaunchTask": "make debug",
      "setupCommands": [
        {
          "description": "Skip STL",
          "text": "skip -gfi C:/msys64/*"
        },
        {
          "description": "Enable pretty-printing for gdb",
          "text": "-enable-pretty-printing",
          "ignoreFailures": true
        }
      ]
    }
  ]
}
```

### tasks.json
```json
{
    "tasks": [
        {
            "label": "make debug",
            "type": "shell",
            "command": "make",
            "args": [
                "MODE=debug"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": [
                "$gcc"
            ]
        },
        {
            "label": "make clean",
            "type": "shell",
            "command": "make",
            "args": [
                "clean"
            ]
        }
    ],
    "version": "2.0.0"
}
```