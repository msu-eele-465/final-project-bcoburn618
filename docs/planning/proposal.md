# Final project proposal

- [x] I have reviewed the project guidelines.
- [x] I will be working alone on this project.
- [x] No significant portion of this project will be (or has been) used in other course work.

## Embedded System Description

In this final project, I will be designing a digital dial board
which will be displayed on an 64x32 RGB screen.  The dial board can be set to an ET using the 4x4 keypad and the color can be changed using the IR remote.  The LCD screen will be used to display the selected dial in which would be seen within the car.

RGB Screen: The RGB screen will display decimal values from 0-9 such as 7.90 or 6.69.  Information for what number is to be be displayed will be sent over I2C from the main controller to the slave controlling the display.

LCD Screen:  The LCD screen will be used to show the driver of the car the dial currently displayed on the RBG board.  A 2310 will receive data over I2C and display the current dial in.

RGB LED:  This LED will flash green to indicate that a successful IR signal was received to alert the user to successful transmission.

IR Receiver/Remote: The color of the screen can be changed corresponding to an input from an IR remote.  Each number on the IR remote will correspond to a certain color that will be defined and known to the user. 

Push Button:  A push button will be wired to the controller to manually clear the output on the RGB screen from outside the car and will simply connect to the an I/O pin.

## Hardware Setup

Hardware Required:
- 64x32 6mm pitch RGB display
- 1838B IR receiver and Controller (38MHz)
- LCD Screen
- Voltage Inverter for contrast
- Potentiometer for adjust contrast
- MSP4302310
- MSP4302355
- RGB controll module (Unknown at this time)
- Push button
- RGB status LED for IR
- Status LED for 2310 and 2355 


![Circuit Diagram](Final_Project_Proposal_Circuit_Diagram.jpg)
## Software overview

Discuss, at a high level, a concept of how your code will work. Include a *high-level* flowchart. This is a high-level concept that should concisely communicate the project's concept.

## Testing Procedure

Briefly describe how you will test and verify that your project is a success. Think about how you will *demo* the final project. If you need extra equipment for the demo, be sure that you can either bring the equipment or convincingly simulate it. For example, if you want to build a system that uses CAN bus to interface with your car, you won't be able to bring your car into Cobleigh for the demo...


## Prescaler

Desired Prescaler level: 

- [ ] 100%
- [ ] 95% 
- [ ] 90% 
- [ ] 85% 
- [ ] 80% 
- [ ] 75% 

### Prescalar requirements 

**Outline how you meet the requirements for your desired prescalar level**

**The inputs to the system will be:**
1.  Short description of input #1
2.  

**The outputs of the system will be:**
1.   Short description of output #1
2. 

**The project objective is**

{text – 1 to 2 sentences}

**The new hardware or software modules are:**
1. Short description of new hardware or software module
2. 


The Master will be responsible for:

{text – 1 to 2 sentences}

The Slave(s) will be responsible for:

{text – 1 to 2 sentences}



### Argument for Desired Prescaler

Consider the guidelines presented in the lecture notes and convince the reviewers that this proposal meets the minimum requirements for your desired prescale level.
