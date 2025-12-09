# ESP32 Brain EEG Sound Controller

This repository contains the code for the ESP32 Brain EEG Sound Controller, a project for the NYU Music Technology class MPATE-GE 2047 Advanced Computer Music taught by Mason Mann for the Fall 2025 semester.

At a high level, the project consists of an ESP32 development board and a MindFlex Brain EEG sensor. The MindFlex sensor transmits the intensities of various frequency bands to the ESP32 as a digital signal, to which the ESP32 will use to control a plethora of random processes from chaos generators to physics simulations, by which the output can be sent with some meaningful communication format (MIDI, OSC, HID, CV, etc.)

The current iteration of this framework will enable brain waves to control audio software via MIDI Control Changes through USB using tinyUSB.

The main contents of the file is under the file `ADV_COMP_MUSIC_BRAINWAVES.ino`. Other directories within this repository are test code used to interface with crucial partsof the project (interfacing with the MindFlex, sending MIDI via tinyUSB). An obsidian `.canvas` file has been provided to give a high-level software stack.

## MindFlex Brain EEG.
You can find more information about the MindFlex EEG sensor [here](https://en.wikipedia.org/wiki/Mindflex). They retail for less than $80 on [eBay](https://www.ebay.com/sch/i.html?_nkw=mindflex&_sacat=0&_from=R40&_trksid=p4624852.m570.l1313). 

The incredible folks at NYU's ITP program have provided a comprehensive [tutorial](https://frontiernerds.com/brain-hack) on how to get this up and running. In addition, they have also provided an [Arduino Library](https://github.com/kitschpatrol/Brain) to interface with the MindFlex controller. 

Find more about FrontierNerds [here](https://frontiernerds.com).

## MEAP: The ESP32 Development Board
[Mason Mann](https://masonmann.online)'s Advanced Computer Music class utilizes a custom-built ESP32 development board and a fork of the Mozzi library to create algorithmic music. Mason's ESP32 Audio Prototyping System (or MEAP) is the development board in question. 

Learn more about it [here](https://masonmann.online/electronics/meap/)
The Mozzi-based Audio Library can also be found [here](https://masonmann.online/electronics/meap/software/)

## The Computer Music System
At its current iteration, the MEAP is being used as a MIDI controller to
translate the magnitudes of the eight EEG frequency bands into MIDI-CC.

To get MIDI-CC out of the MEAP via a USB cable, the [Adafruit TinyUSB](https://github.com/adafruit/Adafruit_TinyUSB_Arduino)
was used. To accomodate for the slow sample rate from the MindFlex, a 
leaky-integrator is employed to interpolate between samples in real time.

A plugin host such as [VCV Rack](https://vcvrack.com) was used alongside
select VST plugins to create an ever-evolving soundscape:
- [Spectral Compressor](https://github.com/robbert-vdh/spectral-compressor)
- [Unfiltered Audio Sandman Pro](https://www.unfilteredaudio.com/collections/plug-ins/products/sandman-pro)
- [Freakshow Industries Dumpster Fire](https://freakshowindustries.com/dumpster-fire)
- [Freakshow Industries MISHBY](https://freakshowindustries.com/mishby)
- [Unfiltered Audio Silo](https://www.unfilteredaudio.com/collections/plug-ins/products/silo)

The project itself is finished, but components within the codebase will
continue to get updated (as they barely work). Any changes made to the 
existing embedded music system will be small quality-of-life things or
minor features.
