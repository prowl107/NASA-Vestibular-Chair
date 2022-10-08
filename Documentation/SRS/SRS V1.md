# System Requirement Specification for NASA Vestibular Restoration Chair Project
**Team Members:**
- Brandon Boyle-Fagan
- Matthaeus Gebauer
- Miles Osborne 
- Dylan Prothro
- Noah Reid
- Kent Wilson

| Name                     | Date    | Reason          | Initials | Version |
| ------------------------ | ------- | --------------- | -------- | ------- |
| Initial Draft (V1 Draft) | 10/7/22 | Initial Release | MO       | 0.1     | 

## Table of Contents:

- [System Requirement Specification for NASA Vestibular Restoration Chair Project](#system-requirement-specification-for-nasa-vestibular-restoration-chair-project)
  - [Table of Contents:](#table-of-contents)
  - [1. Introduction](#1-introduction)
    - [1.1 Purpose](#11-purpose)
    - [1.2 Intended Audience and Reading Suggestions](#12-intended-audience-and-reading-suggestions)
    - [1.3 Product Scope](#13-product-scope)
    - [1.4 References](#14-references)
  - [2. Overall Description](#2-overall-description)
    - [2.1 Product Perspective](#21-product-perspective)
    - [2.2 Product Functions](#22-product-functions)
    - [2.3 User Classes and Characteristics](#23-user-classes-and-characteristics)
      - [2.3.1 Testee](#231-testee)
      - [2.3.2 Proctor/observer](#232-proctorobserver)
  - [2.4 Operating Environment](#24-operating-environment)
    - [2.4.1 Vestibular Chair:](#241-vestibular-chair)
    - [2.4.2 Web Interface:](#242-web-interface)
  - [2.5 Design and Implementation Constraints](#25-design-and-implementation-constraints)
    - [2.5.1 Design Constraints](#251-design-constraints)
    - [2.5.2 Implementation Constraints](#252-implementation-constraints)
  - [2.6 User Documentation](#26-user-documentation)
  - [2.7 Assumptions and Dependencies](#27-assumptions-and-dependencies)
  - [3 System Requirements](#3-system-requirements)
    - [3.1 Chair/Motor Control](#31-chairmotor-control)
    - [3.2 Data Acquisition](#32-data-acquisition)
    - [3.3 User Input/Feedback](#33-user-inputfeedback)
    - [3.4 Web Interface](#34-web-interface)
    - [3.5 Controller Requirements](#35-controller-requirements)
  - [4. Hardware Requirements](#4-hardware-requirements)
  - [5. Other Nonfunctional Requirements](#5-other-nonfunctional-requirements)
  - [6. Appendix](#6-appendix)


## 1. Introduction

This project is designed with the goal of restoring the NASA Vestibular Chair. The device was originally used in the late 1960s to conduct experiments and trials for astronauts and pilots to asses vestibulo-ocular reflex suppression. The purpose of the restoration is to conduct trials to study vestibular system illusions. 

The chair was actively involved in research at NASA and was recently acquired by Embry-Riddle Aeronautical University faculty. This involves restoring the basic functionality and then adding software functionality to improve the ease of use.

### 1.1 Purpose
The purpose of this document is to define all requirements and constraints of the NASA Vestibular Chair Restoration Project. This includes but is not limited to user interactiosn with the system, hardware constraints, and data acquisition. 

### 1.2 Intended Audience and Reading Suggestions
The intended users of this product include aerospace researchers, pilots, astronauts, astronaut candidates, and engineers. This document contains information on the design, requirements, and general functionality of the restore NASA vestibular chair. 

*Additional reading supporting documentation will be listed at a later date*

### 1.3 Product Scope
When the chair was given to researchers at Embry-Riddle Aeronautical University, its functionality was unknown. The last time the chair operated would have been at least 30 years ago. Furthermore, the original controller used has been lost and its whereabouts are unknown. However, thanks to external supporters, documentation from the manufacturers of the chair has been acquired. The goal of this project is to analyze the current state of the chair, determine if it is still functional, resolve any mechanical abnormalities, and expand on its basic functionality. 

The original purpose of this chair was to study vestibular illusions that occur from flight. Once the project has been completed, the chair will be used to conduct research trials into spatial disorientation. 

### 1.4 References
*No references at this time.*

## 2. Overall Description
Section 2 covers the purpose of the product in 2.1, the product functions in 2.2, associated
user information in 2.3, and the operating environment in 2.4. Sections 2.5, 2.6, and 2.7
contain additional information on the implementation of the functions outlined in section
2.2

### 2.1 Product Perspective
The vestibular much like it's original design, relies on an external controller to send commands to the chair. The restored designed is very similar with a host station displaying a web interface that with allow for commands to be send to a embedded target that will perform the requested action. 

### 2.2 Product Functions
- Actuate the analog motors to spin the user at a  specified rate
- Collect data from the on-board sensors and external peripherals
- Collect data from the user through the use of wireless peripheral
- Log all collected data in a human readable format
- Manage and start test sequences from the web interface

### 2.3 User Classes and Characteristics

#### 2.3.1 Testee
The Testee is the person who will sit in the chair and will complete the experiment by indicating their status throughout the duration of the test. The user will utilize a bi-directional control interface such as a joystick or omni-directional button to indicate whether they are spinning clockwise or counter clockwise and to what extent. Furthermore, the user may also indicate when they are not spinning by not orienting the control interface in either direction. 

#### 2.3.2 Proctor/observer
The proctor will be responsible for interacting with the web based interface and starting the test. They are responsible for configuring, selecting, and executing the test sequence. Furthermore, the proctor has the responsibility of activating the mechanical kill switch in the event of an emergencies where the current test needs to be cancelled immediately. 

## 2.4 Operating Environment
### 2.4.1 Vestibular Chair:
The vestibular chair itself is the dynamic aspect of the system. This item mostly makes up the physical and hardware items of the project including an external power supply, a brushless motor for actuation, and microcontroller or equivalent embedded target, and a dedicated servo motor controller. This aspect of the system has a hard limitation of 10v with generated rpm being equivalent to the supplied voltage. The controller module has not been confirmed yet but it is likely to be a microcontroller that is programmed via  embedded C.

### 2.4.2 Web Interface: 
The web interface is the medium where the proctor can control the chair for the testee. The purpose of this module is to give a front end interface for the proctor to configure and execute test sequences. The corresponding commands are generated and then sent to the controller module via a serial bus most likely via USB.  Specific details regarding its implementation, requirements, and any limitations are premature at this stage, and will be included in later revisions. 

## 2.5 Design and Implementation Constraints
### 2.5.1 Design Constraints
- The system cannot operate at greater than 10v. Beyond this range introduces bodily harm to the user or testee. 

### 2.5.2 Implementation Constraints
- Accelerometer data will be limited by the capability and resolution of the on-board accelerometer
- Tachometer data will be limited by the capability and resolution of the on-board tachometer
- Due to the chair spinning a wired bidirectional input for the user is not feasible
- Generated RPM is also dependent on the testee's weight and posture in the chair


## 2.6 User Documentation
*Nothing to list at this time*

## 2.7 Assumptions and Dependencies
*Nothing to list at this time*

## 3 System Requirements
Section 3 covers information regarding the usage and core functionality of the system. Sections 3.1 outlines the motor control requirements. Section 3.2 states the requirements for data acquisition. 3.3 outlines user input and feedback. Sections 3.4 and 3.5 outline the web interface and controller requirements respectfully. 
<br>
### 3.1 Chair/Motor Control
| REQ ID | Requirement                                                                                                               |
| ------ | ------------------------------------------------------------------------------------------------------------------------- |
| SYS-1  | The system shall allow the chair to spin upto a specified RPM determined by the proctor                                   |
| SYS-2  | After a specified duration set by the operator/proctor, the chair should enter a free coast state                         |
| SYS-3  | The system should allow the chair to spin in either clockwise or counterclockwise directions                              |
| SYS-4  | The system shall have a "soft" kill switch that will allow the chair to "free coast" down to 0 rpm                        |
| SYS-5  | The system shall have a mechanical kill switch that will bring the chair to stop faster than the soft switch              |
| SYS-6  | The system should adjust voltage to the motors in response to the user's weight in order to maintain the desired rate/RPM |
| SYS-7  | The system shall not exceed 3 RPM at any point during its operation                                                       |

### 3.2 Data Acquisition
| REQ ID | Requirement                                                                                                                                |
| ------ | ------------------------------------------------------------------------------------------------------------------------------------------ |
| SYS-8  | The system shall record chairs current position (steady state)                                                                             |
| SYS-9  | The system shall record the desired rpm set by the proctor                                                                                 |
| SYS-10 | The system shall record the rpm that the chair is currently spinning at                                                                    |
| SYS-11 | The system shall record the time elapsed during the current test sequence                                                                  |
| SYS-12 | When the user indicates via the bi-directional control method, the system shall record the total time the user indicates they are spinning |
| SYS-13 | When the user indicates via the bi-directional control method, the system shall record the direction they think they are spinning in       |
| SYS-14 | When the user indicates via the bi-directional control method, the system shall record timestamps of when the button is triggered          |
| SYS-15 | The system shall have the ability to export data in a human readable format                                                                |
| SYS-16 | The system shall record acceleration data                                                                                                  |
| SYS-17 | The system shall record tachometer data                                                                                                    |
| SYS-18 | The system shall record the current direction of the chair                                                                                 |
| SYS-19 | The system shall record all commands sent to the controller                                                                                |

### 3.3 User Input/Feedback
| REQ ID | Requirement                                                                                                          |
| ------ | -------------------------------------------------------------------------------------------------------------------- |
| SYS-20 | The system shall support suser input from a bi-directional control interface                                         |
| SYS-21 | The system shall allow for the user to indicate what direction they are spinning via a bi-directional control method |
| SYS-22 | The proctor shall be able to start a sequence or test until a unique button/switch is pressed                        |
| SYS-23 | The system shall record intermediate values for the direction indicated                                              |
| SYS-24 | The system must interface with a wireless bi-directional controller such as a joystick or button                     |

### 3.4 Web Interface
*The system shall have a web based interface that allows the proctor to control the following: *

| REQ ID | Requirement                                                                      |
| ------ | -------------------------------------------------------------------------------- |
| SYS-25 | The web interface shall allow the operator to add different test sequences       |
| SYS-26 | The web interface shall allow the proctor to input the user's weight             |
| SYS-27 | The web interface shall feature a unique button to select the test sequence      |
| SYS-28 | The web interface shall have a unique button to start the selected test sequence |
| SYS-29 | The web interface shall have a text box to enter the duration of the test        |
| SYS-30 | The web interface shall display stored test sequences from a dropdown menu       |

### 3.5 Controller Requirements

| REQ ID| Requirement                                                                                       |
| ------ | ------------------------------------------------------------------------------------------------- |
| SYS-31 | The controller module shall have wifi capabilities                                                |
| SYS-32 | The controller module shall support serial transmission of data to a receiver over USB            |
| SYS-33 | The controller module shall support serial receiving of data from a host                          |
| SYS-34 | The controller module shall be able to read analog values                                         |
| SYS-35 | The controller module shall have sufficient I/O (GPIO) to interface with a mechanical kill switch |
| SYS-36 | The controller module shall support the I2C protocol for communicating with external peripherals  |
|        |                                                                                                   |

## 4. Hardware Requirements
*Nothing listed at this time*

## 5. Other Nonfunctional Requirements
*Nothing listed at this time*

## 6. Appendix
*Nothing listed at this time*
