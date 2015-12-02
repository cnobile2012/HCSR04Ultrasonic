Arduino library for the HCSR04 Ultrasonic Sensor
================================================

It features timing, metric conversion (centimeter and inch) and standard deviation methods. Some ideas were gleaned from a library created by ITead studio at iteadstudio.com.

Look at the code and examples to understand how the library works. This is a reworked version of the code I previously released and is documented at http://wiki.tetrasys-design.net/HCSR04Ultrasonic.

The standard deviation code can be removed from the compilation resulting in a significant savings of resources. To do this comment the following line in the header file.

`#define COMPILE_STD_DEV`

## NOTES

 1. Two of the examples were written before Arduino changed the file extention from .pde to .ino, but should not have issues when used in newer systems.
 2. The Makefile is not use, as of yet, to compile the library, but to create tar and zip files and clean up backup files.
