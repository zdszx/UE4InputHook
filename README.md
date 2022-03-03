# UEInput

## **1.What**
Using @MicrosoftDetour to hook the UE4 input flow, as can be seen below, all InputKey is blocked or replaced:

<img width="500" alt="asdasd" src="https://user-images.githubusercontent.com/43289834/153576189-eaf1aaec-7c5e-486b-af3b-3c374f221ad2.PNG">

## **1.1 The basic idea of this project**

![image](https://user-images.githubusercontent.com/43289834/155071798-c8c586ff-650e-41c0-be65-13248d2e7db2.png)

The main idea of dll is to find the similar structure in UnrealEngine and hook the DispatchMessage relating function. 
```
void mainloop() {
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    
    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
    
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            // run game code
        }
    }
}
```
## **1.2 Detour** 

Most commonly used to intercept Win32 API calls within an application.
The Detours library enables interception of function calls. Interception code is applied dynamically at runtime. Detours replaces the first few instructions of the target function with an unconditional jump to the user-provided detour function.

## **1.3 UE InputFlow**

![v2-33ff9ab5a1de90625354c591b67a1fad_1440w](https://user-images.githubusercontent.com/43289834/155072114-967a26e0-d4fc-4157-b50e-1e9a3771a6ec.png)




## **2.How to use it**
Put the Plugin into your UE4 project directory , and then change the directory name according to dll file location in the 
UE4InputHook/Plugins/WindowsMessageHandler/Source/WindowsMessageHandlerExample/Private/WindowsMessageHandlerExampleModule.cpp

## **3.To Do** 
   1.is some C# hook decision?
   2.to replace the message completely

