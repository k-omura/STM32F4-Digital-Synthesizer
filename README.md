# Open source digital synthesizer

## Digital synthesizer project by STM32F407VET6.
The purpose of this project is to make a sound just by creating a simple synthesizer controller.  
Hopefully there will be lots of innovative UI ideas for music.  
A binary file is prepared for the controller designer, so download it and write with STM32CubeProg etc.  
See [Wiki](https://github.com/k-omura/Open-Digital-Synthesizer/wiki) or [Parameter.xlsx](https://github.com/k-omura/Open-Digital-Synthesizer/blob/main/doc/Parameter.xlsx) and [STM32F407_Synthesizer.pdf](https://github.com/k-omura/Open-Digital-Synthesizer/blob/main/doc/STM32F407_Synthesizer.pdf) in the doc directory for parameter and SPI communication methods.  

## Development environment
- [STM32F407VED6 Black](https://stm32-base.org/boards/STM32F407VET6-STM32-F4VE-V2.0)
- ILI9341 FSMC Display with touch panel for STM32F407VED6 Black. [for example this](https://ebay.us/9OyquX)

## Basic features
- Designed to be operated from the outside via SPI.
- Play 10 sounds (sine, square, saw, triangle x10) at the same time.
- It has a built-in low-pass filter that can manipulate the cutoff frequency and Q value.
- Waveforms can be displayed on the LCD.
- Easy operation is possible with the touch panel.
- The sound wave type is output by the DAC.

## Future features
- ADSR.
- Master volume
- What you want.

## Demo
- [Waveform creation, low-pass filter demo](https://youtu.be/SDA9uaBMBQ4)
- [SPI control demo](https://youtu.be/EjWuWOQzq90)
- [Auto performance](https://youtu.be/yB0PNu2G10Q)

## How to edit this project with STM32CubeIDE.
Only the basic code is uploaded.  
This is because I think that it is smoother to start from the environment construction because I am using ARM CMSIS etc.  
Therefore, you need to prepare the ST and ARM libraries with confidence, but it is easy if you use the STM32Cube IDE.  
Create a project from the information in [32F407_FSMC.pdf](https://github.com/k-omura/Open-Digital-Synthesizer/blob/main/doc/32F407_FSMC.pdf) and [32F407_FSMC.ioc](https://github.com/k-omura/Open-Digital-Synthesizer/blob/main/32F407_FSMC.ioc) in the doc directory.  

More information will be added.
