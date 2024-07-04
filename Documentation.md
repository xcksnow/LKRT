**Create Agent:**
Click this button:
![[Pasted image 20240626210405.png]]
Enter the URL of the server to reach from the agent: 
![[Pasted image 20240626210440.png]]
Go to the server path where you have your web api running, see inside the LokiServer folder if you find a Agents folder, inside you will find something like this:
![[Pasted image 20240627231758.png]]
The 192.168.1.114 it's the url filled in the previous popup, inside you have two EXE's, we need both:
![[Pasted image 20240627231904.png]]
The number of the final of the filename can be different in some cases.
Now execute the LokiAgent.exe in the victim machine with the other exe in the same directory (the second one, the one with name not need to be executed):
When this is done the agent it's added:
![[Pasted image 20240630225257.png]]
To see the victim device information click on **Info**:
![[Pasted image 20240701222926.png]]
**File Explorer:**
Enter in the File Explorer button and you will see this:
![[Pasted image 20240703231731.png]]
Here in the pointed input write and search the path to see:
![[Pasted image 20240703232021.png]]
You can read the files clicking in the icon:
![[Pasted image 20240704212138.png]]
Click on folder enter inside the directory.

**Upload File:**
Fill the popup and wait for the file added in **C:\Users\Public\Documents** of the victim machine:
![[Pasted image 20240704212335.png]]
The file need to be in the server not in the computer executing the web client if they dont are the server.

**File Download**
Fill the popup:
![[Pasted image 20240704212434.png]]
And go to the server folder path and look for a folder named Files, inside you will find the downloaded file:
![[Pasted image 20240704212531.png]]

**Hide File:**
FIll the popup:
![[Pasted image 20240704213819.png]]
Before:
![[Pasted image 20240704213832.png]]
After:
![[Pasted image 20240704214722.png]]
**Unhide File:**
Fill the popup:
![[Pasted image 20240704214919.png]]
Result:

**Timestomping:**
Inside the file explorer click TimeStompiing:
![[Pasted image 20240703232730.png]]
Original TImeStamp:
![[Pasted image 20240703232509.png]]
Destination:
![[Pasted image 20240703232712.png]]
**Upload FIle**
Click upload file button on file explorer:
![[Pasted image 20240703233130.png]]

**DLL Injector:**
![[Pasted image 20240703214945.png]]
Fill the popup:
![[Pasted image 20240703215004.png]]
Result:
![[Pasted image 20240703215041.png]]
User Rootkit:
Fill the popup and have fun :)
![[Pasted image 20240703220156.png]]
Here you have this tool alone:
https://github.com/S12cybersecurity/S12URootkit

**Scheduled Task:**
Fill the popup:
![[Pasted image 20240703220530.png]]
Result:
![[Pasted image 20240703220654.png]]
**Keylogger:**
![[Pasted image 20240703220813.png]]
Now only wait for results, close and reopen to see if you haven'r results yet:
![[Pasted image 20240704220545.png]]
**RDP Stealer:**
Click on RDP Stealer button and wait for the captured results:
![[Pasted image 20240703221346.png]]

**Security Detector:**
Click on button:
![[Pasted image 20240703222307.png]]

**Persistence:**
Click on the popup, and fill it:
![[Pasted image 20240703222759.png]]
Result:
![[Pasted image 20240703222828.png]]

**PPID Spoofing**
Fill the popup and the new process with the choosed parent will be executed:
![[Pasted image 20240703224607.png]]

**Install EXE As Service**
[!] You need to execute as admin this one
Fill the popup and check the result:
![[Pasted image 20240703224744.png]]

**Windows Defender Killer**
![[Pasted image 20240703225239.png]]
Confirm the action and when the victim pc its rebooted...
![[Pasted image 20240703225513.png]]

**Unhook NTDLL**
![[Pasted image 20240703225623.png]]
And done...