# STM32F4 source digital synthesizer

## Digital synthesizer project by STM32F407VET6.
The purpose of this project is to make a sound just by creating a simple synthesizer controller.  
Hopefully there will be lots of innovative UI ideas for music.  
A binary file is prepared for the controller designer, so download it and write with STM32CubeProg etc.  
See [Wiki](https://github.com/k-omura/STM32F4-Digital-Synthesizer/wiki) or [Parameter.xlsx](https://github.com/k-omura/STM32F4-Digital-Synthesizer/blob/main/doc/Parameter.xlsx) and [STM32F407_Synthesizer.pdf](https://github.com/k-omura/STM32F4-Digital-Synthesizer/blob/main/doc/STM32F407_Synthesizer.pdf) in the doc directory for parameter and SPI communication methods.  

## Development environment
- [STM32F407VET6 Black](https://stm32-base.org/boards/STM32F407VET6-STM32-F4VE-V2.0)
- ILI9341 FSMC Display with touch panel for STM32F407VET6 Black. [for example this](https://www.ebay.com/itm/322979958874)

## Basic features
- Designed to be operated from the outside via SPI.
- Play 10 sounds (sine, square, saw, triangle x10) at the same time.
- It has a built-in low-pass filter that can manipulate the cutoff frequency and Q value.
- The sound wave type is output by the DAC.
- Tremolo, Vibrato, WOW
- ADSR

## Future features
- What you want.

## Known issue(s)
- SPI operation may stop.
- If the SPI speed is too fast (more than about 2MHz), the operation cannot keep up.

## Demo
- [Introduction of each function](https://youtu.be/cMSFZtEUxo4)
- Old Version
  - [Waveform creation, low-pass filter demo](https://youtu.be/SDA9uaBMBQ4)
  - [SPI control demo](https://youtu.be/EjWuWOQzq90)
  - [Auto performance (Super Mario)](https://youtu.be/yB0PNu2G10Q)

## Hot to use
Write the binary and connect your controller.  
See [here](https://github.com/k-omura/STM32F4-Digital-Synthesizer/wiki/How-to-Use) on the Wiki for details.  
I have prepared a [simple code](https://github.com/k-omura/STM32F4-Digital-Synthesizer/tree/main/sample_contoler) for Arduino nano, so please check it out.  
Set the SPI speed to about 1MHz.  

## How to edit this project with STM32CubeIDE
See [here](https://github.com/k-omura/STM32F4-Digital-Synthesizer/wiki/Create-Project) on the Wiki for details.  
Only the basic code is uploaded.  
This is because I think that it is smoother to start from the environment construction because I am using ARM CMSIS etc.  
Therefore, you need to prepare the ST and ARM libraries with confidence, but it is easy if you use the STM32Cube IDE.  
Create a project from the information in [32F407_FSMC.pdf](https://github.com/k-omura/STM32F4-Digital-Synthesizer/blob/main/doc/32F407_FSMC.pdf) and [32F407_FSMC.ioc](https://github.com/k-omura/STM32F4-Digital-Synthesizer/blob/main/32F407_FSMC.ioc) in the doc directory.  

More information will be added.
