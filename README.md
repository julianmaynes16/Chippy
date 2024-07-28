Chippy

Chippy is an emulator that plays games using the CHIP8 language, used on old microprocessors such as the COSMAC VIP. The emulator supports all system functions such as keyboard input, audio and visual output, and its staple 60Hz refresh rate. It also contains customizable settings such as adjustable cpu speed and custom volume control on startup.

Setup:

PLease clone this repository into an accessible place. From there, open the terminal and move into the Chippy directory using the commands:

cd XXXX

For example, I put my repo into Documents, so I would do the following:

cd OneDrive/Documents/Chippy

If you want to see the folders in your directory type this into the terminal:

dir

And if you want to go back, type this:

cd ..

Once you've made it into the Chippy directory, compile the program using the following command:

make 

Once this is done, add a ROM to the rom folder. CHIP8 ROM's can be found online very easily. Do note that the ROM must be in the .ch8 format, so if you run into a ROM not in that format, feel free to just rename the extension.  

Then run the program using the following command

start chippy.exe 2 5000

The first number is the cpu speed. Certain games favor certain cpu speeds, so try different numbers and see which ones you like. For Space Invaders I prefer a speed of 1. FYI Lower numbers are generally faster. The second number is the volume. Feel free to pick any number between 0 and 32,767, noting higher numbers mean a higher volume and that I am not responsible for speaker / headphone damages! The sound can be a little loud so if you don't want to change your system volume and dont want to bother with the audio mixer, just leave it at 1000 if you like to leave your audio on the lower side and increase from there. 

Controls:

Quit = Escape

............


