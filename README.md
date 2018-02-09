# MOD003197
Fire simulation

Degree course module

Simulates a forest fire spread

## Features
* Compile-time subsystem selection
* Rule-based generation and simulation
* Cross-platform
* Abstract render pipeline
  * OpenGL renderer implemented as subsystem
  * Buffered renderer, using the Win32 API to allow for off-screen console window buffers, implemented as windows-only subsystem
* Provides a menu at the start, allowing the user to select the desired renderer
