**Arduino Snake4Ever**  

## Overview

**Arduino Snake4Ever** is a retro Snake game for Arduino that you can run on an OLED display. This project is perfect for classic game enthusiasts and Arduino fans who want to try programming a Snake game using various peripherals and technologies.  

The project includes:  
- Graphics rendered on an OLED display.  
- Control via buttons.  
- Three-channel sound support.  
- Implemented basic artificial intelligence (demo mode).  

## Demo Mode

The project has an integrated demo mode in which the snake moves randomly across the screen. However, when the user presses a button, the demo mode ends, and the standard game begins.  

---

## Hardware Requirements

- **Arduino** (tested on NANO, UNO)  
- **OLED display** (SSD1306, I2C, 128x64 pixels)  
- **Buttons** for control (up, down, left, right)  
- **Speaker** (for MIDI music output)  
- Wiring and accessories for connection  

---

## Libraries Used

This project uses the following libraries:  
- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
- [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library)
- [Playtune](https://github.com/LenShustek/arduino-playtune) (for MIDI music)
  
You can install these libraries via the Library Manager in Arduino IDE, or manually.  

---

## Pin Configuration

| Component         | Pin      |  
|-------------------|----------|  
| Left button       | 2        |  
| Right button      | 3        |  
| Up button         | 4        |  
| Down button       | 5        |  
| OLED display      | I2C (A4, A5 on UNO) |  
| Speaker           | Connected as needed |  

---

## How to Play the Game

1. After starting, the demo mode will display where the snake moves randomly.  
2. Pressing any button will interrupt the demo and start the game.  
3. Move the snake using the buttons (left, right, up, down).  
4. Your task as the programmer is to add fruit to the code, allowing the snake to grow. **This is your challenge!**

---

## Tasks for Contributors

In the current version, the snake does not grow. Your task as a contributor is to:  
- Add a function to generate fruit in the code.  
- Implement the mechanism for the snake to grow after eating the fruit.  

---

## Challenge:
 - Once you finish your modification, send a Pull Request and add your feature. The project is waiting for your creativity!  

---

## Special Thanks

A special thanks to my amazing girlfriend for her support during three fun days while I was creating this project! 🤓  

---

## Author

Tomas Mark 2024 - tomas@digitalspace.name / https://tomas.digitalspace.name

---

## License

This project is licensed under The Unlicense. Use the source code however you like.  