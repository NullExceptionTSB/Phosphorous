# PhosphorousRewrite
yes
i have been working on this for way too fucking long, the source code is going public 31.12.2019\
thank you to everyone that is mentioned in the explorer source code credits, but especially wipet and nikit\

as #1 states, this source code is a mess that has a lot of commented out and disabled features that were originally intended but i couldn't be assed to fix, namely "polymorphism"\

if you want to compile it, do so. just remember it will not work on default compiler settings. if you have enough skill to fix and compile this, then you have enough skill for me to trust you with it. parts of the source code are intentionally broken to fend off skids. also **DO NOT SHARE ANY BINARIES OF THIS REPO**, this is already covered in the license so just don't. if you want to fork it for whatever reason delete your binaries before committing\

**THIS REPO IS MALWARE !!! I AM NOT RESPONSIBLE FOR ANY DAMAGAE CAUSED BY IT. SEE LICENSE FOR MORE INFO**\\

Writeup:\\

Major payloads:\
24.12. 	: Destructive, Draws a pulsating St. Peter cross on the screen while running the windows equivalent to "sudo dd if=/dev/zero of=/dev/sda"\
24.5. 	: Destructive, Explorer overwrite, draws a scrolling gradient and scrolling text on the bottom of the screen. Also the only payload that applies the MBR overwrite\
26.11.	: Creates a thread that generates random beeps, and launches a progressive payload\\

Minor payloads:\
7.8.	: "Block Multiply" payload, basically a combination of a DVD screen saver and missingno\
8.10	: "RubikCascade" payload, basically splits the screen up into sections and does stff between them\
24.11	: "Block Wipe" payload, simmilar to "Block Multiply" under the hood but looks completely different. BitBlt spam.\\

Additional payloads:\
Adds registry keys that greatly restrict the user and their ability to remove Phosphorous\
Starts an AntiDebug thread that makes sure the process is critical and kills itself it if is being debugged\
Main executable is located inside an NTFS alternate data stream of a system file, and is launched by P15Dropper\


Disabled payloads:\
Dropper removing the main executable from itself after dropping it. Removed because for some reason my method of finding the file location did not work anymore.\
UAC bypass. Originally worked but likely got fixed since then as I couldnt get it to work after\
PEB corruption to make debug harder. Removed because it would cause CreateProcessW to not work\
Overwrites the loaded PE headers to make dumping harder. Removed because of a weird glitch with some functions\


Date reasoning:\
24.12.	: Christmas, obviously\
24.5.	: My birthday\
26.11.	: Nikitpad's birthday\
7.8.	: Midori's birthday\
8.10.	: Release date of Covenant's "Northern Light" album\
24.11.	: Siam's birthday\
