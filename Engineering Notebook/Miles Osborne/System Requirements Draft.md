# System Requirements Draft

# Control
- The system shall allow the chair to spin upto a specified RPM
- After a specified duration set by the operator/proctor, the chair should enter a free coast state
- The system should allow the chair to spin in either clockwise or counterclockwise directions
- The system shall have a "soft" kill switch that will allow the chair to "free coast" down to 0 rpm
- The system shall have a "mechanical/hard" kill switch that will bring the chair to stop faster than the soft switch
	- By how much faster?
- The system should adjust `<parameter>` in response to the user's weight in order to maintain the desired RPM
	- Not sure what parameter needs to be adjusted yet (voltage, current, etc...)

- **HOW DOES THE RPM GET SET**?
- The system shall not exceed `<parameter>` RPM at any point during its operation

- Do not exceed 10v power consumption
- DO not exceed 4a power consumption
# Web Interface
The system shall have a web interface that allows the user/proctor to control the following

- The web interface shall allow the operator to add different test sequences 
	- **NOTE:** We don't know what constitutes a test sequence entirely yet
- The web interface shall allow the proctor to input the user's weight
- The web interface shall feature a unique button to select the test sequence
- The web interface shall have a unique button to start the selected test sequence
- The web interface shall have a text box to enter the duration of the test 
- The web interface shall display stored test sequences from a dropdown menu


# Data Acquisition
- The system shall record chair steady state (current position) of the chair
- The system shall record the rpm set by the proctor
- The system shall record the rpm that the chair is currently spinning at
	- NOTE: We will need an encoder for this
- The system shall record the time elapsed during the current test
- When the user indicates they are spinning, the system shall record the total time the user thinks they are spinning 
- When the user thinks they are spinning, the system shall record the direction they think they are spinning in 
- When the user thinks they are spinning, the system shall record the timestamps they think they are spinning for
- The system shall have the ability to export data in a human readable formate
	- Something like excel, csv, json, etc.
- The system shall have the ability to obtain positional data from an external accelerometer

# User Input/Feedback
- The system shall support user input from a bi-directional control interface
	- Button, joystick, etc.
- The system shall allow for the user to indicate what direction they are spinning in via a bi-directional control method
- The proctor cannot start a sequence or test until a separate button/switch is pressed
- The system shall record intermediate values for the direction indicated
	- x (slow, medium, fast) amount in left direction
	- x (slow, medium, fast) amount in right direction
- The user remote is wireless
- Controller has wireless recieve and transmit functionality

