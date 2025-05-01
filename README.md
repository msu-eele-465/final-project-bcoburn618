# EELE 465 Final Project
## RGB Digital Dial Board

### Main Controller
This program allows the user to control a 64x32 matrix RGB display.  Utilizing the 4x4 keypad
the user can select between four diffent functionalites.  Pressing A, the user is prompted by 
a message on the LCD screen to enter a dial in.  A three digit number can be entered by pressing digits 0-9
on the keypad.  The system then prompts the user to either send the dial (via I2C) or choose a different dial.
If the user chooses to send the dial, the data will be set to I2C to the matrix controller to be displayed.
If the user selects C, they will then be prompted by the LCD screen to pick a color for the RGB display.  A
new color can be selected by pressing 0-6.  This data will then be passed to the matrix controller for it to
be processed.  The color of the screen at the current moment will be displayed in the lower right corner of the
LCD screen.  The user can then press D which will toggle the state of the RGB display by sending a flag over
I2C.  The state of the board is also shown on the LCD screen and can be seen in the lower left corner.  
The status of the RGB display can also be controlled by pressing an external button.

### RGB Matrix Controller
This program allows for the RGB matrix display to be driven and controlled.
The programs functionality is dictated by data received over I2C sent from
the main controller.  If the the program receives a packet that is of length 
three, it will take the data received and pass it into an array known as dial in.
This dial is then passed into a function that will display the ET and will remain doing
so until a new dial is received or the board is turned off.  If the program receives
a packet that is two bytes long with the first element being 0xC, the second element
will contain a digit that corresponds to a color.  This digit will then be decoded corresponding
to a color.  If the first element of the I2C transaction is 0xD, the the display state will be toggled.
