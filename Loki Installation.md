**Loki Server**
Publish Web API:
- If you have a Windows 10/11 you need to add the IIS feature, the servers ones have preinstalled:
https://answers.microsoft.com/en-us/windows/forum/all/how-to-enable-iis-on-windows-10-instantly/70a5a376-840e-4d79-b930-ccd7ec6f2844
- Open the VIsual Studio project (2022):
![[Pasted image 20240610213517.png]]
- Publish Project in a Local folder:
![[Pasted image 20240610213446.png]]
- Create a new Site in IIS:
 ![[Pasted image 20240610214524.png]]
Set a name, the file path and the port that you want deploy the API:
![[Pasted image 20240610214804.png]]
- Copy the content files in the publish/ path of the choosed publish path:
![[Pasted image 20240610215048.png]]
- Install dotnet if you don't have yet (7.0):
[.NET Downloads (Linux, macOS, and Windows) (microsoft.com)](https://dotnet.microsoft.com/en-us/download/dotnet)
### Steps to Install the .NET Core Hosting Bundle

1. **Download the Hosting Bundle:**
    
    - Go to the [.NET download page](https://dotnet.microsoft.com/download/dotnet-core).
    - Choose the latest stable version of the .NET Core Runtime that matches your application's target framework.
    - Download the "ASP.NET Core Runtime" or ".NET Core Hosting Bundle" installer.
2. **Run the Installer:**
    
    - Run the downloaded installer.
    - Follow the prompts to complete the installation.
    

- Check the website result:



**Angular App**
- Go to the angular application folder with cmd or linux terminal and run **ng build**:
![[Pasted image 20240610221254.png]]
- Copy the content of the **dist/loki-web-viewer/browser**/ folder to the server one, (the same of the API choosed in the Create Site step)
![[Pasted image 20240610221418.png]]
- Create a new Site (with a other port than the API):
![[Pasted image 20240617214435.png]]
- Check web result:
![[Pasted image 20240617214526.png]]
