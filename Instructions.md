# Setup Guide

[Video walkthrough](https://youtu.be/1Ug1PUPSozg) if you prefer watching.

## Dependencies

1. **MemProcFS**: Download from [releases](https://github.com/ufrisk/MemProcFS/releases) and extract the win_x64 files to your build directory.

2. **FTD3xx DLLs**: Get them [here](https://ftdichip.com/wp-content/uploads/2024/01/FTD3XXLibrary_v1.3.0.9.zip), extract, and copy the DLLs from `FTD3XXLibrary/x64/DLL/` to your build directory.

Your build folder should look like this after:
```
Build/Release/DMADeceit2/
├── DMADeceit2.exe
├── vmm.dll
├── leechcore.dll  
├── FTD3XX.dll
└── ... (other files)
```

## Python Setup

Install [Python 3.8](https://www.python.org/ftp/python/3.8.0/python-3.8.0-amd64.exe) and add it to your PATH:

1. Open Environment Variables (Win+R → `sysdm.cpl` → Advanced → Environment Variables)
2. Edit the `Path` variable and add these two paths (replace `YourUsername` with your actual username):
   ```
   C:\Users\YourUsername\AppData\Local\Programs\Python\Python38
   C:\Users\YourUsername\AppData\Local\Programs\Python\Python38\Scripts
   ```

That's it! The project should build and run now.
