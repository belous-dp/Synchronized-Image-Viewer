# Synchronized-Image-Viewer
An utility for collaborative viewing photos from different devices. Features synchronized cursor. Works over TCP. Built with Qt&C++.

## Demo
[Screencast from 24-06-24 06:06:56.webm](https://github.com/belous-dp/Synchronized-Image-Viewer/assets/46475907/db36ea1f-a696-4562-afad-2280ed116d16)


## Installation
Follow [this instruction](https://github.com/belous-dp/Mandelbrot-Set?tab=readme-ov-file#installation) but instad of cloning Mandelbrot-Set `git clone https://github.com/belous-dp/Synchronized-Image-Viewer.git`

## Usage
1. Run the application, choose server mode
2. Run another instance of the app (on another device or in a new instance of the terminal or by choosing an appropriate pre-saved CMake configuration)
3. Copy host info from server instance of the app
4. Paste to the client and connect
5. Open a picture from local files
6. Open the same picture on another device
7. Drag the cursor somewhere; its position on another device will be updated

> Devices must be in the same local network or the server must have a public ip.

## Roadmap/TODO
* an ability to iterate over all photos in a directory
* sharing photos from the host to the client over the network
* zooming and resizing
* seamless reconnect in case of sudden client disconnect (now app restart required)
* android app
* pre-built packages for Linux&Windows 
