# Door_Security_System_AVR

Implemented via **AVR atmega32** MCU with **8 MHz** internal clock frequency.

>### Project Description

* **The door is unlocked using a password entered via a keypad:**
&emsp; <i>i. The door opens by rotating the DC motor Anti-clockwise for 15 seconds.<br>
&emsp;    ii. It is then held open for 3 seconds.<br>
&emsp;    iii. Then it is closed by rotating the DC motor clockwise for 15 seconds.<br> </i>
* **If the user entered a wrong password 3 consecutive times:** <br>
&emsp; <i>i. The keypad accepts no input for 1 minute.<br>
&emsp;    ii. A buzzer and a LED are triggered for 1 minute.<br>

>### Project Specifications

* **Human Machine Interface Micro-controller Unit:**    <br>
&emsp; <i>- Responsible for interaction with the user.  <br>
&emsp;    - System initialization: It prompts the user to enter a new password and confirm it.<br>
&emsp;    - It send the password input to the CONTROL ECU to be confirmed. <br>
&emsp;    - No input is accepted from keypad if the password is entered 3 consecutive times.<br></i>
* **Control Micro-controller Unit:**    <br>

>### Project Design

* **The Project follows modular design as shown in the diagram below:**

Layered Model            |  Layered Model Detailed
:-------------------------:|:-------------------------:
![Abstraction_Layers_](./imgs/Abstraction_Layers.png)  |  ![Abstraction_Layers_Detailed](./imgs/Abstraction_Layers_Detailed.png)

>### Project Simulation

![Project Simulation GIF](./imgs/sim.gif)

>### Hardware Implementation
* **youtube video emb.**
  
>### How to simulate

* Clone the project repo via `git clone https://github.com/0marAmr/Fan-Control-System`.
* Open command prompt in the project directory.
* Run `make compile` to compile project files.
* Open ***project_simulation*** file found in ***simulation*** directory.
* After simulating, run `make clean` to remove compilation files.
