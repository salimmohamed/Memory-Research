# Deceit 2 DMA Memory Reader

A C++ project for reading game memory using Direct Memory Access (DMA) techniques. Originally built for educational purposes to understand memory analysis and system architecture.

## What's in here

```
DMADeceit2/     # Main C++ source
├── Config/     # Configuration handling
├── ESP/        # ESP rendering code  
├── Graphics/   # UI components and drawing
├── Memory/     # Memory reading utilities
├── SDK/        # Game-specific structures
└── Security/   # Anti-detection stuff

Include/        # External headers (json, leechcore, etc.)
Lib/           # Static libraries
Build/         # Compiled output
```

See [TODO.md](TODO.md) for what's planned next.

## Building

Requires Visual Studio 2019+ with C++17 support. The project uses:
- PCILeech for DMA access
- DirectX for rendering
- JSON for config files

Just open `DMADeceit2.sln` and build. Make sure you have the required hardware for DMA (FPGA board or similar).

## ⚠️ Important

This is for educational purposes only. Don't use this on games you don't own or in ways that violate terms of service. The techniques here are useful for understanding memory analysis and system security, but use them responsibly.

## Credits

Forked from [ProbablyXS/deceit_2_dma](https://github.com/ProbablyXS/deceit_2_dma). Uses:

- [PCILeech](https://github.com/ufrisk/pcileech) - The DMA magic happens here
- [MemProcFS](https://github.com/ufrisk/MemProcFS) - Memory process file system  
- [UnnamedZ03/DBD-external-base](https://github.com/UnnamedZ03/DBD-external-base) - External memory reading patterns
- [DMALibrary](https://github.com/Metick/DMALibrary) - DMA implementation helpers

Thanks to the original authors for the solid foundation.