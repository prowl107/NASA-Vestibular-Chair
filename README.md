# NASA-Vestibular-Chair
This project is designed with the goal of restoring the NASA Vestibular Chair. The chair was actively involved in research at NASA and was recently acquired by Embry-Riddle Aeronautical University faculty. This involves restoring the basic functionality and then adding software functionality to improve the ease of use.

## Vision Statement
Our goal for the end of the spring semester (2023) is to accurately introduce a web-based interface into the external controller along with defining more accurate data acquisition over wireless connection. This consists of encoding a Raspberry Pi to allow for communication of testing procedure specifications to the microcontroller opposed to having manual input. The data acquisition will be achieved through the use of Bluetooth modules present in the microcontroller to communicate data to a host console wirelessly. If time permits, we would begin refining and updating the current design of the external controller for better usability and aesthetic. 

## Installation
```bash
git clone git@github.com:prowl107/NASA-Vestibular-Chair.git
cd NASA-Vestibular-Chair
git config user.name "User"
git config user.email "admin@example.com"
```
Alternatively, you can use a git GUI client to manage your copy of the project. 

## Usage
Open Visual Studio Code (or your text editor/IDE of choice) and then open the PlatformIO menu.
Select "Open Project" and navigate to where the repo is located on your device.
Open the PlatformIO project with the NASA-Vestibular-Chair root folder selected.
Have fun :)

## Hardware 
### Version 1 - Controller Module
| Category  | Component  |
|---|---|
| MCU  |  [Arduino Mega 2560 Rev3](https://store.arduino.cc/products/arduino-mega-2560-rev3) |
| Motor Controller  | [Pololu Simple Motor Controller G2 24v12](https://www.pololu.com/product/1365)  |
| Display  | [Standard 20x4 LCD w/ I2C](https://www.amazon.com/SunFounder-Serial-Module-Arduino-Mega2560/dp/B01GPUMP9C) |
| I/O  | SPST Momentary switch; 250k potentiometer  |
| Power Supply | USB 5v |
| Communication Interface (Receiver) | [433Mhz Receiver](https://www.amazon.com/HiLetgo-Wireless-Transmitter-Receiver-Raspberry/dp/B01DKC2EY4/ref=sr_1_3?keywords=433mhz+transmitter+and+receiver&qid=1675643344&sr=8-3) |

**NOTE:** Initially, the plan was to utilize a [Nucleo-WB55RG](https://www.st.com/en/evaluation-tools/nucleo-wb55rg.html) Development board. Due to time constraints and availability, the Arduino Mega (and Nano) were chosen due to their extensive library/peripheral support and quick prototyping capabilitiy.

### Version 1 - Patient Remote

| Category  |  Component |
|---|---|
| MCU  | [Arduno Nano R3](https://store.arduino.cc/products/arduino-nano)  |
| I/O  |  3x SPST Momentary switch |
| Power Supply   | 5v li-ion battery  |
| Communication Interface (Transmitter)  |  [433Mhz Receiver](https://www.amazon.com/HiLetgo-Wireless-Transmitter-Receiver-Raspberry/dp/B01DKC2EY4/ref=sr_1_3?keywords=433mhz+transmitter+and+receiver&qid=1675643344&sr=8-3) |

## Presentations
- [Sprint 1 Demo Presentation](https://docs.google.com/presentation/d/1b5IdnyLSNwiTLVy-gZ9E79ZbYqnyJbTD/edit?usp=sharing&ouid=115407868332019012098&rtpof=true&sd=true)
- [Sprint 2 Demo Presentation](https://docs.google.com/presentation/d/1l_CmvSCrBEOVg_VuYpDZWNs5qSBOpJvT/edit?usp=sharing&ouid=115407868332019012098&rtpof=true&sd=true)
- [Sprint 3 (End of Fall 2022) Demo Presentation](https://docs.google.com/presentation/d/1xmUZu0XUEXimdjX_mnKVwzS-KKVoy4oE/edit?usp=sharing&ouid=115407868332019012098&rtpof=true&sd=true)
- [Sprint 4 (2023) Demo Presentation](https://docs.google.com/presentation/d/1zxAWRcQLVy_wj2OVS8DIGSSmD8TUqGN_/edit?usp=sharing&ouid=115407868332019012098&rtpof=true&sd=true)
- [Sprint 5 (2023) Demo Presentation](https://docs.google.com/presentation/d/1c0bh65YAwqH6N8ktK_QlXqYoBvHQKnEx/edit?usp=sharing&ouid=115407868332019012098&rtpof=true&sd=true)
