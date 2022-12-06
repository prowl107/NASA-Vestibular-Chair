# Some Design Notes

## Use Case

### Actors
- Patient
- Proctor
- Web Interface
- Chair 
- Controller module

### Responsibilities:

#### Patient Responsibilities:
- Sit in the chair
- Indicate what direction they are spinning in
- Activate soft switch

#### Proctor Responsibilities: 
- Start test sequence
- Activate soft switch
- Activate mechanical/hard switch
- Control RPM via switch

#### Web Interface Responsibilities
- Display data 
- Accept commands from I/O

#### Chair Responsibilites
	Send data
		- Accelerometer
		- Tach

#### Controller Module
- Process commands
- Read data over serial
- Send sign

### Assumptions:
- Assuming that the controller module consists of the following components:
	- Micro controller
	- Motor controller
	- 1x soft switch
	- 1x hard switch
	- 7 seg displays

### Constraints:




## State Transitions (For the chair-controller interaction)

- Powering off
	- Entry: Kill voltage to system if powered
	- Leave state once mechanical switch is pressed/depressed
	- Can go back to powering on state, once kill switch is not enabled

- Powering on
	- Entry: System initialization
	- exit: nothing
	- Do: Cycle displays 
	- Leave state once system initialization completes

- Configuring sequence
	- Nested state
	- 1. Configuring RPM
		- Leave after center pushbutton confirmation
		- Stay in state if down button is pressed 
	- 2. Configuring Time/Duration
		- Leave after centeR pushbutton confirmation
		- Go back to previous state if down button is pressed

- Waiting for proctor confirmation
	- Enter if test sequence has been set and confirmed
	- DO: Wait
	- Exit: nothing
	- Only leave state once proctor gives pushbutton confirmation
	- Go back to configuring sequence if down button is pressed

- Executing test sequence
	- Do: Send signals to chair via motor controller
	- Only enter state if arming switch is triggered

- Slowing down
	- Enter this state if soft_kill switch is pressed at any point
	- Do: Quickly bring RPM down to 0
	- 